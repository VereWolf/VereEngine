#include "LightHelper.fx"

cbuffer cbPerObject
{
	float4x4 gView;
}

Texture2D gTargetMap;
Texture2D gDepthMap;

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
	float		Depth :		DEPTH;
};

PixelOut PS(VertexOut  pin) : SV_Target
{
	PixelOut pout;
	pout.Color = gTargetMap.SampleLevel(samState, float2(pin.TexTess.x, pin.TexTess.y), 0).rgba;
	pout.Color.a = 0.0f;
	pout.Depth = gDepthMap.SampleLevel(samState, float2(pin.TexTess.x, pin.TexTess.y), 0).r;
	return pout;
}

technique11 LightTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}