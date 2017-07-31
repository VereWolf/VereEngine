#include "LightHelper.fx"
#include "AtmosphereHelper.fx"

cbuffer cbPerObject
{
	float4x4 gView;
	float gDepth;
	float gSize;
	float gWaterRatio;
	float gCloudsRatio;
	float gFarZ;
	float gFarModifier;
}

Texture2D gMainDepthMap;
Texture2D gMainTargetMap;
Texture2D gWaterTopDepthMap;
Texture2D gWaterTopTargetMap;
Texture2D gWaterBottomDepthMap;
Texture2D gWaterBottomTargetMap;
Texture2D gCloudsDepthMap;
Texture2D gCloudsTargetMap;
Texture2D gAtmosphereDepthMap;
Texture2D gAtmosphereTargetMap;

SamplerState samState
{
	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct VertexIn
{
	float3		PosL:		POSITION;
	float2		TexTess:	TEXCOORD;
};

struct VertexOut
{
	float4		PosH:		SV_POSITION;
	float3		PosW:		POSITION;
	float2		TexTess:	TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.TexTess = vin.TexTess;
	vout.PosW = vin.PosL;
	vout.PosH = mul(float4(vout.PosW, 1.0f), gView);

	return vout;
}

struct PixelOut
{
	float4		Color:		COLOR;
	float		Depth : DEPTH;
};

PixelOut CalcTransparent(float d1, float d2, float4 color1, float4 color2, float alpha)
{
	PixelOut p;

	if (d1 < d2 || d2 < 0.0f)
	{
		p.Depth = d1;
		p.Color = color1;
		p.Color.a = 0.0f;
	}
	else
	{
		p.Depth = d2;
		float3 C1 = color1.rgb;
		float3 C2 = color2.rgb;
		p.Color = float4(alpha * C2 + (1.0f - alpha) * C1, 0.0f);
	}

	return p;
}

PixelOut PS(VertexOut  pin) : SV_Target
{
	PixelOut pout1;
PixelOut pout2;
float alpha = 0.0f;

pout1.Depth = gMainDepthMap.SampleLevel(samState, float2(pin.TexTess.x, pin.TexTess.y), 0).r;
pout1.Color = gMainTargetMap.SampleLevel(samState, float2(pin.TexTess.x, pin.TexTess.y), 0).rgba;

pout2.Depth = gAtmosphereDepthMap.SampleLevel(samState, float2(pin.TexTess.x, pin.TexTess.y), 0).r;
pout2.Color = gAtmosphereTargetMap.SampleLevel(samState, float2(pin.TexTess.x, pin.TexTess.y), 0).rgba;

alpha = pout2.Color.a;

pout1 = CalcTransparent(pout1.Depth, pout2.Depth, pout1.Color, pout2.Color, alpha);

pout2.Depth = gWaterBottomDepthMap.SampleLevel(samState, float2(pin.TexTess.x, pin.TexTess.y), 0).r;
pout2.Color = gWaterBottomTargetMap.SampleLevel(samState, float2(pin.TexTess.x, pin.TexTess.y), 0).rgba;

alpha = 1.0f - pout2.Color.a + 0.4f;

pout1 = CalcTransparent(pout1.Depth, pout2.Depth, pout1.Color, pout2.Color, alpha);

pout2.Depth = gWaterTopDepthMap.SampleLevel(samState, float2(pin.TexTess.x, pin.TexTess.y), 0).r;
pout2.Color = gWaterTopTargetMap.SampleLevel(samState, float2(pin.TexTess.x, pin.TexTess.y), 0).rgba;

alpha = 0.4f;

pout1 = CalcTransparent(pout1.Depth, pout2.Depth, pout1.Color, pout2.Color, alpha);

/*pout2.Depth = gCloudsDepthMap.SampleLevel(samState, float2(pin.TexTess.x, pin.TexTess.y), 0).r;
pout2.Color = gCloudsTargetMap.SampleLevel(samState, float2(pin.TexTess.x, pin.TexTess.y), 0).rgba;

alpha = 0.05f;

pout1 = CalcTransparent(pout1.Depth, pout2.Depth, pout1.Color, pout2.Color, alpha);*/

return pout1;
}

technique11 LightTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}