#include "LightHelper.fx"

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gViewProj;
	float4 gSkyColor;
	float3 gSunVector;
	float gLight;
};

float c = 0.1f;

float4 org = float4(0.83f, 0.51f, 0.22f, 1.0f);

struct VertexIn
{
	float3		PosL:		POSITION;
	float2		Tex:		TEXCOORD;
};

struct VertexOut
{
	float4		PosH:		SV_POSITION;
	float3		PosW:		POSITION;
	float3		NormalW:	NORMAL;
	float2		Tex:		TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.Tex = vin.Tex;
	vout.NormalW = normalize(vin.PosL);

	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.PosH = mul(float4(vout.PosW, 1.0f), gViewProj);
	vout.PosH.z = log(c * vout.PosH.w + 1.0f) / log(c * 20000.0f + 1.0f) * vout.PosH.w;

	return vout;
}

float4 PS(VertexOut  pin) : SV_Target
{
	return SkyLight(pin.NormalW, gSunVector, gSkyColor, gLight,org);
}

technique11 LightTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}