#include "LightHelper.fx"
#include "AtmosphereHelper.fx"

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldN;
	float4x4 gViewProj;
	float3 gEyePosW;
	float gFarZ;
	float gFarRangeMod;
	float gFarModifier;
	Material gMaterial;
};

float c = 0.1f;

struct VertexIn
{
	float3		PosW:		POSITION0;
	float3		DirW:		POSITION1;
	float2		SizeW:		SIZE;
};

struct VertexOut
{
	float3		CenterW:	POSITION0;
	float3		DirW:		POSITION1;
	float2		SizeW:		SIZE;
};

struct GeoOut
{
	float4		PosH:		SV_POSITION;
	float3		PosW:		POSITION;
	float3		NormalW:	NORMAL;
	uint		PrimID:		SV_PrimitiveID;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vout.CenterW = mul(float4(vin.PosW, 1.0f), gWorld).xyz;
	vout.DirW = mul(float4(vin.DirW, 1.0f), gWorldN).xyz;
	vout.SizeW = vin.SizeW;

	return vout;
}

[maxvertexcount(4)]
void GS(point VertexOut gin[1],
	uint primID :  SV_PrimitiveID,
	inout TriangleStream<GeoOut> triStream)
{
	float3 look = -gin[0].CenterW;
	look = normalize(look);
	float3 right = cross(gin[0].DirW, look);

	float halfWidth = 0.5f * gin[0].SizeW.x;

	float4 v[4];
	v[0] = float4(gin[0].CenterW + halfWidth * right, 1.0f);
	v[1] = float4(gin[0].CenterW + halfWidth * right + gin[0].SizeW.y * gin[0].DirW, 1.0f);
	v[2] = float4(gin[0].CenterW - halfWidth * right, 1.0f);
	v[3] = float4(gin[0].CenterW - halfWidth * right + gin[0].SizeW.y * gin[0].DirW, 1.0f);

	GeoOut gout;
	[unroll]
	for (int i = 0; i < 4; ++i)
	{
		gout.PosH = mul(v[i], gViewProj);
		gout.PosH.z = log(gFarModifier * gout.PosH.w + 1.0f) / log(gFarModifier * gFarZ + 1.0f) * gout.PosH.w;
		gout.PosW = v[i].xyz;
		gout.NormalW = look;
		gout.PrimID = primID;

		triStream.Append(gout);
	}
}
float4 PS(GeoOut  pin) : SV_Target
{
	float3 color = float3(0.3f, 1.0f, 0.3f);

	float distance = length(float2(pin.PosW.x, pin.PosW.z));

	return float4(color, 1.0f);
}

technique11 LightTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}