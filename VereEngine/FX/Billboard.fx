#include "LightHelper.fx"
cbuffer cbPerFrame
{
	float gRenderStart;
	float gRenderEnd;
	float3 gFogColor;
	float gFogStart;
	float gFogRange;

}

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gViewProj;
	int2 gCoord;
	float gSpacing;
};

SamplerState samHeightMap
{
	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

SamplerState samNormalMap
{
	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

SamplerState samTreesMap
{
	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

Texture2D gHeightMap;
Texture2D gNormalMap;
Texture2D gTreesMap;

//Texture2D gHeightTile_1;
//Texture2D gNormalTile_1;

float c = 0.1f;

struct VertexIn
{
	float3		PosW:		POSITION;
	float2		SizeW:		SIZE;
	float2		TexTess:	TEXCOORD0;
};

struct VertexOut
{
	float3		CenterW:	POSITION;
	float2		SizeW:		SIZE;
	float2		TexTess:	TEXCOORD0;
};

struct GeoOut
{
	float4		PosH:		SV_POSITION;
	float3		PosW:		POSITION;
	float3		NormalW:	NORMAL;
	uint		PrimID:		SV_PrimitiveID;
	float2		TexTess:	TEXCOORD0;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vout.CenterW = mul(float4(vin.PosW, 1.0f), gWorld).xyz;
	vout.SizeW = vin.SizeW;
	vout.TexTess = vin.TexTess;

	return vout;
}

[maxvertexcount(4)]
void GS(point VertexOut gin[1],
	uint primID :  SV_PrimitiveID,
	inout TriangleStream<GeoOut> triStream)
{
	float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 look = -gin[0].CenterW;
	look.y = 0.0f;
	look = normalize(look);
	float3 right = cross(up, look);

	float halfWidth = 0.5f * gin[0].SizeW.x;

	float3 N = 2.0f * (gNormalMap.SampleLevel(samNormalMap, float2((gin[0].TexTess.x + gCoord.x) * gSpacing, (gin[0].TexTess.y + gCoord.y) * gSpacing), 0).xyz - 0.5f);
	float D = (1.0f - dot(N, float3(0.0f, 1.0f, 0.0f)) + 0.05f) / 1.05f;
	/*float3 NS = gNormalTile_1.SampleLevel(samNormalMap, float2((gin[0].TexTess.x + gCoord.x) * gSpacing, (gin[0].TexTess.y + gCoord.y) * gSpacing), 0).xyz;
	NS = 2.0f * NS - 1.0f;
	NS = D * NS + (1.0f - D) * float3(0.0f, 1.0f, 0.0f);
	NS = normalize(NormalSampleToWorldSpace(NS, N, float3(1.0f, 0.0f, 0.0f)));*/
	//float B;
	//B = D * gHeightTile_1.SampleLevel(samHeightMap, float2((gin[0].TexTess.x + gCoord.x) * gSpacing, (gin[0].TexTess.y + gCoord.y) * gSpacing), 0).x * 64.0f;

	gin[0].CenterW.y -= gHeightMap.SampleLevel(samHeightMap, float2((gin[0].TexTess.x + gCoord.x) * gSpacing, (gin[0].TexTess.y + gCoord.y) * gSpacing), 0);

	float4 v[4];
	v[0] = float4(gin[0].CenterW + halfWidth * right, 1.0f);
	v[1] = float4(gin[0].CenterW + halfWidth * right - gin[0].SizeW.y * up, 1.0f);
	v[2] = float4(gin[0].CenterW - halfWidth * right, 1.0f);
	v[3] = float4(gin[0].CenterW - halfWidth * right - gin[0].SizeW.y * up, 1.0f);

	GeoOut gout;
	[unroll]
	for (int i = 0; i < 4; ++i)
	{
		gout.PosH = mul(v[i], gViewProj);
		gout.PosH.z = log(c * gout.PosH.w + 1.0f) / log(c * 20000.0f + 1.0f) * gout.PosH.w;
		gout.PosW = v[i].xyz;
		gout.NormalW = look;
		gout.TexTess = gin[0].TexTess;
		gout.PrimID = primID;

		triStream.Append(gout);
	}
}
float4 PS(GeoOut  pin) : SV_Target
{
	float3 color = float3(0.3f, 1.0f, 0.3f);

	float distance = length(float2(pin.PosW.x, pin.PosW.z));

	float3 N = 2.0f * (gNormalMap.SampleLevel(samNormalMap, float2((pin.TexTess.x + gCoord.x) * gSpacing, (pin.TexTess.y + gCoord.y) * gSpacing), 0) - 0.5f);
	float Ang = dot(float3(0.0f, 1.0f, 0.0f), N);

	float T = gTreesMap.SampleLevel(samTreesMap, float2((pin.TexTess.x + gCoord.x) * gSpacing, (pin.TexTess.y + gCoord.y) * gSpacing), 0);

	if (T < 0.5f || Ang < 0.7f /*|| distance < gRenderStart*/ || distance > gRenderEnd)
	{
		clip(-0.05);
	}

	if (distance > gFogStart)
	{
		color = saturate(lerp(color, gFogColor, (distance - gFogStart) / gFogRange));
		if (distance > gFogStart + gFogRange)
		{
			color = saturate(gFogColor);
		}
	}

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