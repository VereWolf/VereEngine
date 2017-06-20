#include "AtmosphereCalc.fx"
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
	float gRadius;
	float3 gCentrePos;
	float3 gOffset;
	float3 gTang;
	float gLevel;
	float gFarZ;
	float gFarModifier;
	float3 gCenterOfPlanet;
	float gRadiusOfPlanet;
	float gFogStart;
	float gFogRange;
	float3 gFogColor;
};

float c = 0.1f;

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

//Texture2D gHeightMap;
//Texture2D gNormalMap;

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
	float3 BinormW	:NORMAL02;
	float3 TangW	:NORMAL03;
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

	dout.PosW = mul(float4(dout.PosW, 1.0f), gWorld).xyz;

	float H = 0.0f;// 2.0f * gHeightMap.SampleLevel(samHeightMap, float2(gLevel * (dout.TexTess.x + gOffset.x), gLevel * (dout.TexTess.y + gOffset.y)), 0).x;

	float3 N = normalize(dout.PosW - gCentrePos);
	float3 B = normalize(cross(gTang, N));
	float3 T = cross(N, B);

	/*if (H >= 0)
	{
		float3 normalT = float3(0.0f, 1.0f, 0.0f);//gNormalMap.SampleLevel(samNormalMap, float2(gLevel * (dout.TexTess.x + gOffset.x), gLevel * (dout.TexTess.y + gOffset.y)), 0).xyz;

		float3x3 TBN = float3x3(T, N, B);

		dout.NormalW = mul(normalT, TBN);
		dout.BinormW = normalize(cross(gTang, dout.NormalW));
		dout.TangW = cross(dout.NormalW, dout.BinormW);

		dout.PosW = (H + gRadius) * N + gCentrePos;
	}
	else
	{
		dout.PosW = (gRadius) * N + gCentrePos;
	}*/
	dout.NormalW = N;
	dout.PosW = (gRadius)* N + gCentrePos;

	dout.PosV = mul(float4(dout.PosW, 1.0f), gViewProj).xyz;
	dout.PosH = mul(float4(dout.PosW, 1.0f), gViewProj);

	dout.PosH.z = log(gFarModifier * dout.PosH.w + 1.0f) / log(gFarModifier * gFarZ + 1.0f) * dout.PosH.w;

	return dout;
}

float4 PS(DomainOut  pin) : SV_Target
{
	float H = 0.0f;// 2.0f * gHeightMap.SampleLevel(samHeightMap, float2(gLevel * (pin.TexTess.x + gOffset.x), gLevel * (pin.TexTess.y + gOffset.y)), 0).x;

	float3 color = float3(1.0f, 1.0f, 1.0f);

	float3 N = pin.NormalW;

	/*if (H < 0.0f)
	{
		color = float3(0.2f, 0.35f, 0.9f);
		N = float3(0.0f, 1.0f, 0.0f);
	}*/

	float d = 0.5f * AtmosphereCalc(pin.PosV, gFogRange + gFogStart, pin.NormalW);

	/*if (d > gFogStart)
	{
		d = clamp((d - gFogStart) / gFogRange, 0.0f, 1.0f);

		color = ((1.0f - d) * color + d * gFogColor);
	}*/

	color = (0.6f + 0.5f * (dot(N, float3(0.0f, 1.0f, 0.0f)) + 1.0f)) * color / 1.6f;

	//color = 0.5f * (dot(N, float3(0.0f, 1.0f, 0.0f)) + 1.0f);

	//color = float3(1.0f, 0.0f, 0.0f);

	return float4(color, 1.0f);
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