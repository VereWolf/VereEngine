#include "LightHelper.fx"

cbuffer cbPerFrame
{
	float3 gEyePosW;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gViewProj;
	float gFarZ;
	float gFarModifier;
	float3 gFogColor;
	float gFogStart;
	float gFogRange;
	float gStartOfLOD;
	float gStartOfLODOfTrees;
	int2 gCoord;
	float gSpacing;
	float3 gSunVector;
	float4 gSkyColor;
	float gLight;
};

float c = 0.1f;
float4 org = float4(0.83f, 0.51f, 0.22f, 1.0f);

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

struct VertexIn
{
	float3		PosL:		POSITION;
	float2		TexTess:		TEXCOORD0;
};

struct VertexOut
{
	float3		PosL:		POSITION;
	float2		TexTess:		TEXCOORD0;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.TexTess = vin.TexTess;

	vout.PosL = vin.PosL;

	return vout;
}

struct PatchTess
{
	float EdgeTess[4]   : SV_TessFactor;
	float InsideTess[2] : SV_InsideTessFactor;
};

PatchTess ConstantHS(InputPatch<VertexOut, 4> patch, uint patchID : SV_PrimitiveID)
{
	PatchTess pt;

	pt.EdgeTess[0] = 1.0f;
	pt.EdgeTess[1] = 1.0f;
	pt.EdgeTess[2] = 1.0f;
	pt.EdgeTess[3] = 1.0f;

	pt.InsideTess[0] = 1.0f;
	pt.InsideTess[1] = 1.0f;

	return pt;
}struct HullOut
{
	float3 PosL     : POSITION;
	float2 TexTess:		TEXCOORD0;
};

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOut HS(InputPatch<VertexOut, 4> p,
	uint i : SV_OutputControlPointID,
	uint patchId : SV_PrimitiveID)
{
	HullOut hout;

	hout.PosL = p[i].PosL;
	hout.TexTess = p[i].TexTess;

	return hout;
}

struct DomainOut
{
	float4 PosH     : SV_POSITION;
	float3 PosW     :POSITION;
	float2	TexTess	:TEXCOORD0;
};

[domain("quad")]
DomainOut DS(PatchTess patchTess,
	float2 uv : SV_DomainLocation,
	const OutputPatch<HullOut, 4> quad)
{
	DomainOut dout;

	dout.PosW = lerp(
		lerp(quad[0].PosL, quad[1].PosL, uv.x),
		lerp(quad[2].PosL, quad[3].PosL, uv.x),
		uv.y);

	dout.TexTess = lerp(
		lerp(quad[0].TexTess, quad[1].TexTess, uv.x),
		lerp(quad[2].TexTess, quad[3].TexTess, uv.x),
		uv.y);

	float3 normal = float3(0.0f, 1.0f, 0.0f);

	dout.PosW = mul(float4(dout.PosW, 1.0f), gWorld).xyz;

	float3 N = 2.0f * (gNormalMap.SampleLevel(samNormalMap, float2((dout.TexTess.x + gCoord.x) * gSpacing,(dout.TexTess.y + gCoord.y) * gSpacing), 0).xyz - 0.5f);
	float D = (1.0f - dot(N, float3(0.0f, 1.0f, 0.0f)) + 0.05f) / 1.05f;
	dout.PosW.y -= gHeightMap.SampleLevel(samHeightMap, float2((dout.TexTess.x + gCoord.x) * gSpacing,(dout.TexTess.y + gCoord.y) * gSpacing), 0).x;
	/*float3 NS = gNormalTile_1.SampleLevel(samNormalMap, float2((dout.TexTess.x + gCoord.x) * gSpacing, (dout.TexTess.y + gCoord.y) * gSpacing), 0).xyz;
	NS = 2.0f * NS - 1.0f;
	NS = D * NS + (1.0f - D) * float3(0.0f, 1.0f, 0.0f);
	NS = normalize(NormalSampleToWorldSpace(NS, N, float3(1.0f, 0.0f, 0.0f)));*/
	/*float3 B;
	B.y = D * gHeightTile_1.SampleLevel(samHeightMap, float2((dout.TexTess.x + gCoord.x) * gSpacing, (dout.TexTess.y + gCoord.y) * gSpacing), 0).x * 64.0f;
	B.x = -clamp(2.0f * (NS.x - 0.5f), 0.0f, 1.0f) * B.y;
	B.z = -clamp(2.0f * (NS.z - 0.5f), 0.0f, 1.0f) * B.y;
	dout.PosW -= B;*/

	dout.PosH = mul(float4(dout.PosW, 1.0f), gViewProj);
	dout.PosH.z = log(gFarModifier * dout.PosH.w + 1.0f) / log(gFarModifier * gFarZ + 1.0f) * dout.PosH.w;

	return dout;
}

float4 PS(DomainOut  pin) : SV_Target
{
	float3 color = float3(1.0f, 1.0f, 1.0f);

	float3 treesColor = float3(0.3f, 1.0f, 0.3f);

	float distance = length(float2(pin.PosW.x, pin.PosW.z));

	/*if (distance < gStartOfLOD)
	{
		clip(-0.05);
	}*/
	
	/*if (distance > gFogStart)
	{
		color = saturate(lerp(color, gFogColor, (distance - gFogStart) / gFogRange));
		if (distance > gFogStart + gFogRange)
		{
			clip(-0.05);
		}
	}*/

	float3 N = 2.0f * (gNormalMap.SampleLevel(samNormalMap, float2((pin.TexTess.x + gCoord.x) * gSpacing,(pin.TexTess.y + gCoord.y) * gSpacing), 0).xyz - 0.5f);
	float angl = dot(float3(0.0f, 1.0f, 0.0f), N);
	
	float T = gTreesMap.SampleLevel(samTreesMap, float2((pin.TexTess.x + gCoord.x) * gSpacing, (pin.TexTess.y + gCoord.y) * gSpacing), 0);

	/*if (T > 0.5f && angl > 0.7f && gStartOfLODOfTrees <= distance)
	{
		color = treesColor;
	}*/

	if (distance > gFogStart)
	{
		float3 fogColor = SkyLight(-normalize(pin.PosW), gSunVector, gSkyColor, gLight, org).xyz;
		color = saturate(lerp(color, fogColor, (distance - gFogStart) / gFogRange));
		if (distance > gFogStart + gFogRange)
		{
			clip(-0.05);
		}
	}

	float D = (1.0f - dot(N, float3(0.0f, 1.0f, 0.0f)) + 0.05f) / 1.05f;
	//float4 NP = gNormalTile_1.SampleLevel(samNormalMap, float2((pin.TexTess.x + gCoord.x) * gSpacing, (pin.TexTess.y + gCoord.y) * gSpacing), 0).xyzw;
	//float3 NS = normalize(float3(NP.x, NP.w, NP.z));
	
	//NS = 2.0f * NS - 1.0f;
	//NS = D * NS + (1.0f - D) * float3(0.0f, 1.0f, 0.0f);
	//NS = float3(1.0f, 0.0f, 0.0f);
	//N = normalize(NormalSampleToWorldSpace(NS, N, float3(1.0f, 0.0f, 0.0f)));

	float3 L = gSunVector;

	color = (0.6f + dot(N, L)) * color / 1.6f;

	/*D = dot(NS, float3(0.0f, 1.0f, 0.0f));
	D = D * D;*/

	return float4(color*gLight, 1.0f);
	//return  float4(1.0f, 1.0f, 1.0f, 1.0f);
	//return float4(float3(D, D, D), 1.0f);
	//return float4(float2((pin.TexTess.x + gCoord.x) * gSpacing, (pin.TexTess.y + gCoord.y) * gSpacing), 0.0f, 1.0f);
}

technique11 LightTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetHullShader(CompileShader(hs_5_0, HS()));
		SetDomainShader(CompileShader(ds_5_0, DS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}