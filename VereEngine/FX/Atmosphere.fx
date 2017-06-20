#include "LightHelper.fx"
cbuffer cbPerFrame
{
	float3 gEyePosW;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gViewProj;
	float gSpacing;
	float3 gCentrePos;
	float gFogStart;
	float gFogRange;
};

float c = 0.1f;

struct VertexIn
{
	float3		PosL:		POSITION;
	float3		Tex:		TEXCOORD;
};

struct VertexOut
{
	float4		PosH:		SV_POSITION;
	float3		PosW:		POSITION;
	float3		Tex:		TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.Tex = vin.Tex;

	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;

	float3 P = abs(vout.PosW - gCentrePos);

	float R = P.x;

	if (P.y > R)
	{
		R = P.y;
	}

	if (P.z > R)
	{
		R = P.z;
	}

	vout.PosW = R * normalize(vout.PosW - gCentrePos) + gCentrePos;

	vout.PosH = mul(float4(vout.PosW, 1.0f), gViewProj);

	vout.PosH.z = log(c * vout.PosH.w + 1.0f) / log(c * 1000000000000.0f + 1.0f) * vout.PosH.w;

	return vout;
}

float4 PS(VertexOut  pin) : SV_Target
{
	float4 color = float4(0.1f, 0.3f, 1.0f, 0.2f);
	/*if (pin.PosH.w > gFogStart)
	{
		float F = 0.25f * ((pin.PosH.w - gFogStart) / gFogRange);
		F = clamp(F, 0.0f, 0.25f);
		color.w = F;
	}*/
	return color;
}

technique11 LightTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}