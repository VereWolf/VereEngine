#include "LightHelper.fx"
#include "AtmosphereHelper.fx"
#include "CoordHelper.fx"

cbuffer cbPerFrame
{
	float3 gEyePosW;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float3x3 gWorldN;
	float4x4 gViewProj;
	float gSpacing;
	float gRadiusOfTerrain;
	float gRadiusOfAtmosphere;
	float gRadiusOfWater;
	float3 gOffset;
	float3 gTang;
	float gLevel;
	float gFarZ;
	float gFarModifier;
	float3 gCenterOfPlanet;
	float3 gDirectOfPlanet;
	float3 gFogAColor;
	float3 gFogWColor;
	float gFogWRange;
	float3x3 gInverseSide;
	float gSide;
	bool gIsMap;
};

float c = 0.1f;

SamplerState samHeightMap
{
	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};

SamplerState samNormalMap
{
	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};

Texture2D gHeightMap;
Texture2D gNormalMap;
Texture2D gEnviromentMap;
Texture2D gTreesMap;

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
}

struct HullOut
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
	float3 PosW     :POSITION01;
	float3 PosV     :POSITION02;
	float3 NormalW	:NORMAL01;
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

	float CH = dout.PosW.y * gSpacing;

	dout.PosW = mul(float4(dout.PosW, 1.0f), gWorld).xyz;

	float H = -2300.0f;

	if (gIsMap == true)
	{
		H = gHeightMap.SampleLevel(samHeightMap, float2(dout.TexTess.x, dout.TexTess.y), 0).x + CH;
	}

	float3 N = normalize(dout.PosW - gCenterOfPlanet);
	float3 B = normalize(cross(mul(gTang, gWorldN), N));
	float3 T = normalize(cross(N, B));

	float3 normalT = float3(0.0f, 1.0f, 0.0f);

	if (gIsMap == true)
	{
		normalT = 2.0f * gNormalMap.SampleLevel(samNormalMap, float2(dout.TexTess.x, dout.TexTess.y), 0).xyz - 1.0f;
	}

	float3x3 TBN = float3x3(T, N, B);

	dout.NormalW = mul(normalT, TBN);

	dout.PosW = (H + gRadiusOfTerrain) * N + gCenterOfPlanet;

	dout.PosV = mul(float4(dout.PosW, 1.0f), gViewProj).xyz;
	dout.PosH = mul(float4(dout.PosW, 1.0f), gViewProj);

	dout.PosH.z = log(gFarModifier * dout.PosH.w + 1.0f) / log(gFarModifier * gFarZ + 1.0f) * dout.PosH.w;

	return dout;
}

float4 PS(DomainOut  pin) : SV_Target
{
	float3 C1 = float3(0.478f, 0.463f, 0.439f);
	float3 C2;
	float3 C3 = float3(0.15f, 0.508f, 0.281);
	float3 color = float3(1.0f, 1.0f, 1.0f);

	float4 E = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 T = float4(0.0f, 0.0f, 0.0f, 0.0f);

	if (gIsMap == true)
	{
		E = gEnviromentMap.SampleLevel(samNormalMap, float2(pin.TexTess.x, pin.TexTess.y), 0).xyzw;
		T = gTreesMap.SampleLevel(samNormalMap, float2(pin.TexTess.x, pin.TexTess.y), 0).xyzw;

		int warm = (E.x + 0.125f) * 4.0f;
		int water = (E.y + 0.125f) * 4.0f;

		if (warm == 0)
		{
			C2 = float3(0.9f, 0.9f, 0.93f);
		}
		else if (warm == 1)
		{
			C2 = float3(0.558f, 0.631f, 0.274f);
		}
		else if (warm == 2)
		{
			if (water == 3 || water == 4)
			{
				C2 = float3(0.518f, 0.663f, 0.217f);
			}
			else if (water == 2)
			{
				C2 = float3(0.518f, 0.631f, 0.204f);
			}
			else if (water == 1)
			{
				C2 = float3(0.563f, 0.621f, 0.227f);
			}
			else
			{
				C2 = float3(0.731f, 0.558f, 0.324f);
			}
		}
		else if (warm == 3)
		{
			if (water == 3 || water == 4)
			{
				C2 = float3(0.340f, 0.645f, 0.304f);
			}
			else if (water == 2)
			{
				C2 = float3(0.508f, 0.631f, 0.304f);
			}
			else if (water == 1)
			{
				C2 = float3(0.573f, 0.611f, 0.304f);
			}
			else
			{
				C2 = float3(0.731f, 0.558f, 0.251f);
			}
		}
		else
		{
			if (water == 4 || water == 3)
			{
				C2 = float3(0.231f, 0.658f, 0.304f);
			}
			else if (water == 2)
			{
				C2 = float3(0.558f, 0.603f, 0.304f);
			}
			/*else if (water == 1)
			{
				C2 = float3(0.573f, 0.631f, 0.304f);
			}*/
			else
			{
				C2 = float3(0.731f, 0.558f, 0.204f);
			}
		}
	}

	float3 normalT = float3(0.0f, 1.0f, 0.0f);

	if (gIsMap == true)
	{
		normalT = 2.0f * gNormalMap.SampleLevel(samNormalMap, float2(pin.TexTess.x, pin.TexTess.y), 0).xyz - 1.0f;

		C2 = C2 + T.x * (C3 - C2);
		color = C1 + E.w * (C2 - C1);
	}

	float3 N = pin.NormalW;

	color = (0.6f + 0.5f * (dot(N, float3(0.0f, 1.0f, 0.0f)) + 1.0f)) * color / 1.6f;

	float d = clamp(CalcWater(gCenterOfPlanet, pin.PosW, float3(0.0f, 0.0f, 0.0f), gRadiusOfWater, gFogWRange), 0.0f, 1.0f);

	color = d * gFogWColor + (1.0f - d) * color;

	/*d = CalcFog(gCenterOfPlanet, pin.PosW, float3(0.0f, 0.0f, 0.0f), gRadiusOfAtmosphere);

	color = d * gFogAColor + (1.0f - d) * color;*/

	return float4(color, 1.0f);
	//return float4(E.zw, 0.0f, 1.0f);
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