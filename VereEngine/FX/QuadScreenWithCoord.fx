#include "LightHelper.fx"
cbuffer cbPerFrame
{
}

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gViewProj;
	float4x4 gProj;
	float4x4 gView;
	float gSize;
};

float c = 0.1f;

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
	vout.PosW = mul(float4(0.0f, 0.0f, 0.0f, 1.0f), gWorld);
	vout.PosH = mul(float4(vout.PosW, 1.0f), gView);
	vout.PosH = mul(vout.PosH + float4((vin.PosL * gSize), 0.0f), gProj);
	vout.PosH.zw = float2(0.0f, abs(vout.PosH.w));

	return vout;
}

float4 PS(VertexOut  pin) : SV_Target
{
	return float4(pin.TexTess,1.0f, 1.0f);
}

technique11 LightTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}