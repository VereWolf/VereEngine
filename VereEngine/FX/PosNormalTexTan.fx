#include "LightHelper.fx"

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

/*Texture2D gDiffuseMap;
Texture2D gSpecularMap;
Texture2D gNormalMap;*/

/*SamplerState samState
{
	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};*/

struct VertexIn
{
	float3		PosL:		POSITION;
	float3		Tan:		TANGENT;
	float3		Normal:		NORMAL;
	float2		Tex:		TEXCOORD;
};

struct VertexOut
{
	float4		PosH:		SV_POSITION;
	float3		PosW:		POSITION;
	float3		Tan:		TANGENT;
	float3		Normal:		NORMAL;
	float2		Tex:		TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	//vin.Normal = 2.0f * vin.Normal - 1.0f;
	vout.Tan = mul(float4(vin.Tan, 1.0f), gWorld).xyz;
	vout.Normal = mul(float4(vin.Normal, 1.0f), gWorldN).xyz;
	//vout.Normal = vin.Normal;

	/*vout.Tan = vin.Tan;
	vout.Normal = vin.Normal;*/
	vout.Tex = vin.Tex;

	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	//vout.PosW = vin.PosL;
	vout.PosH = mul(float4(vout.PosW, 1.0f), gViewProj);
	//vout.PosH = float4(vout.PosW, vout.PosW.z);
	vout.PosH.z = log(gFarModifier * vout.PosH.w + 1.0f) / log(gFarModifier * gFarZ + 1.0f) * vout.PosH.w;
	//vout.PosH.z = log(0.1f * vout.PosH.w + 1.0f) / log(0.1f * 100000000000.0f + 1.0f) * vout.PosH.w;

	return vout;
}

float4 PS(VertexOut  pin) : SV_Target
{
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	DirectionalLight light;
	light.ambient = float4(0.4f, 0.4f, 0.4f, 1.0f);
	light.diffuse = float4(0.5f, 0.5f, 0.5f, 1.0f);
	light.specular = float4(0.2f, 0.2f, 0.2f, 1.0f);
	light.direction = normalize(float3(0.8247f, 0.7685f, 0.8247f));
	light.power = 1.0f;
	Material mat = gMaterial;
	
	ComputeDirectionalLight(mat, light,
		pin.Normal,
		gEyePosW,
		ambient,
		diffuse,
		spec);

	clip(mat.ambient.a - 0.1f);

	ambient = ambient + diffuse + spec;

	return ambient;
}

technique11 LightTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}