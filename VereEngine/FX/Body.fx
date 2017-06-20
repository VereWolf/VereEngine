#include "LightHelper.fx"

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldN;
	float4x4 gViewProj;
	//float4x4 gWorldInvTranspose;
	float3 gEyePosW;
	//float3 gDirLight;
	Material gMaterial;
	//bool gIsDiffuseMap;
	//bool gIsSpecularMap;
	//bool gIsNormalMap;
	float gTS;
};

//Texture2D gDiffuseMap;
//Texture2D gSpecularMap;
//Texture2D gNormalMap;

SamplerState samState
{
	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};

float c = 0.1f;

struct VertexIn
{
	float3		PosL:		POSITION0;
	float3		PosL1:		POSITION1;
	float3		PosL2:		POSITION2;
	float3		Tan:		TANGENT0;
	float3		Tan1:		TANGENT1;
	float3		Tan2:		TANGENT2;
	float3		Normal:		NORMAL0;
	float3		Normal1:	NORMAL1;
	float3		Normal2:	NORMAL2;
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
	vin.PosL = vin.PosL + gTS * vin.PosL1;
	vin.Normal = vin.Normal + gTS * vin.Normal1;
	vout.Tan = mul(float4(vin.Tan, 1.0f), gWorld).xyz;
	vout.Normal = mul(float4(vin.Normal, 1.0f), gWorldN).xyz;
	//vout.Normal = vin.Normal;

	/*vout.Tan = vin.Tan;
	vout.Normal = vin.Normal;*/
	vout.Tex = vin.Tex;

	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.PosH = mul(float4(vout.PosW, 1.0f), gViewProj);
	vout.PosH.z = log(c * vout.PosH.w + 1.0f) / log(c * 20000.0f + 1.0f) * vout.PosH.w;

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

	/*if (gIsDiffuseMap)
	{
		mat.ambient = gDiffuseMap.Sample(samState, float2(pin.Tex.x, -pin.Tex.y));
	}*/
	//else
	{
		mat.ambient = float4(0.8f, 0.8f, 0.8f, 1.0f);
	}

	mat.diffuse = float4(0.8f, 0.8f, 0.8f, 1.0f);

	/*if (gIsSpecularMap)
	{
		mat.specular = gSpecularMap.Sample(samState, float2(pin.Tex.x, -pin.Tex.y));
	}*/
	//else
	{
		mat.specular = float4(0.8f, 0.8f, 0.8f, 1.0f);
	}

	/*if (gIsNormalMap)
	{
		ComputeDirectionalLight(mat, light,
			NormalSampleToWorldSpace(gNormalMap.Sample(samState, float2(pin.Tex.x, -pin.Tex.y)), pin.Normal, pin.Tan),
			gEyePosW,
			ambient,
			diffuse,
			spec);
	}*/
	//else
	{
		ComputeDirectionalLight(mat, light,
			pin.Normal,
			normalize(gEyePosW),
			ambient,
			diffuse,
			spec);
	}

	//clip(mat.ambient.a - 0.1f);

	ambient = ambient + mat.ambient * diffuse + spec;

	//return float4(NormalSampleToWorldSpace(gNormalMap.Sample(samState, float2(pin.Tex.x, -pin.Tex.y)), pin.Normal, pin.Tan), 1.0f);

	return ambient;

	//return float4(pin.Normal, 1.0f);
}

technique11 LightTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}