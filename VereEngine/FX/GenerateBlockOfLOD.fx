cbuffer cbUpdateSettings
{
	bool gIsLoad;
	float gOffset;
	float gScaling;
};

SamplerState samInputMap
{
	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};

Texture2D gInputH;
Texture2D gInputN;
Texture2D gInputE;
Texture2D gInputT;
Texture2D gTile;
RWTexture2D<float> gOutputH;
RWTexture2D<float4> gOutputN;
RWTexture2D<float> gOutputA;
RWTexture2D<float4> gOutputE;
RWTexture2D<float4> gOutputT;


//#define CONST 256
#define PI 3.14159265359f

[numthreads(16, 16, 1)]
void GenBlockOfLODCS(int3 groupThreadID : SV_GroupThreadID,
	int3 dispatchThreadID : SV_DispatchThreadID)
{
	float H = gInputH[dispatchThreadID.xy].x;
	float3 N = 2.0f * gInputN[dispatchThreadID.xy].xyz - 1.0f;
	float4 E = gInputE[dispatchThreadID.xy].xyzw;
	float4 T = gInputT[dispatchThreadID.xy].xyzw;
	float2 FE = float2(4.0f * E.x - (int)(4.0f * E.x), 4.0f * E.y - (int)(4.0f * E.y));
	float3 Z = float3(0.0f, 0.0f, 0.0f);
	float3 UP = float3(0.0f, 1.0f, 0.0f);

	float rad = 180.0f * acos(dot(UP, N)) / PI;

	float2 rCoord = float2(gScaling * (dispatchThreadID.x + gOffset), gScaling * (dispatchThreadID.y + gOffset));

	float3 random;

	random.x = gTile.SampleLevel(samInputMap, float2(rCoord.x + 0.31f, rCoord.y - 0.17f), 0)
		+ 0.5f * gTile.SampleLevel(samInputMap, float2(2.0f * rCoord.x - 0.29f, 2.0f * rCoord.y + 0.19f), 0)
			+ 0.25f * gTile.SampleLevel(samInputMap, float2(4.0f * rCoord.x - 0.36f, 4.0f * rCoord.y - 0.15f), 0)
				+ 0.125f * gTile.SampleLevel(samInputMap, float2(8.0f * (rCoord.x - 0.49f), 8.0f * (rCoord.y + 0.31f)), 0);

	random.y = gTile.SampleLevel(samInputMap, float2(rCoord.x - 0.36f, rCoord.y - 0.35f), 0)
		+ 0.5f * gTile.SampleLevel(samInputMap, float2(2.0f * rCoord.x + 0.47f, 2.0f * rCoord.y - 0.11f), 0)
			+ 0.25f * gTile.SampleLevel(samInputMap, float2(4.0f * rCoord.x - 0.03f, 4.0f * rCoord.y - 0.47f), 0)
				+ 0.125f * gTile.SampleLevel(samInputMap, float2(8.0f * rCoord.x - 0.14f, 8.0f * rCoord.y + 0.48f), 0);

	random.z = gTile.SampleLevel(samInputMap, float2(rCoord.x + 0.21f, rCoord.y - 0.36f), 0)
		+ 0.5f * gTile.SampleLevel(samInputMap, float2(2.0f * rCoord.x - 0.37f, 2.0f * rCoord.y - 0.28f), 0)
			+ 0.25f * gTile.SampleLevel(samInputMap, float2(4.0f * rCoord.x + 0.25f, 4.0f * rCoord.y - 0.13f), 0)
				+ 0.125f * gTile.SampleLevel(samInputMap, float2(8.0f * rCoord.x - 0.12f, 8.0f * rCoord.y - 0.29f), 0);

	random *= 0.53333f;

	float4 env;

	if (rad < 25)
	{
		E.z = 0.0f;
		E.w = 1.0f;
	}
	else if (rad < 35)
	{
		E.z = 0.1f;
		E.w = 0.5f + 0.05f * (35.0f - rad);
	}
	else if (rad < 45)
	{
		E.z = 0.2f;
		E.w = 0.5f + 0.05f * (-35.0f + rad);
	}
	else
	{
		E.z = 0.3f;
		E.w = 0.0f;
	}

	if (gIsLoad)
	{
		if (FE.x >= random.x)
		{
			E.x = 0.25f * ((int)(4 * E.x + 1.0f));
		}
		else
		{
			E.x = 0.25f * ((int)(4 * E.x));
		}

		if (FE.y >= random.y)
		{
			E.x = 0.25f * ((int)(4 * E.y + 1.0f));
		}
		else
		{
			E.x = 0.25f * ((int)(4 * E.y));
		}

		if (T.x >= random.z)
		{
			T.x = 0.25f * ((int)(4 * T.x + 1.0f));
		}
		else
		{
			T.x = 0.25f * ((int)(4 * T.x));
		}
	}

	gOutputH[dispatchThreadID.xy] = H;
	gOutputN[dispatchThreadID.xy] = float4(0.5f * N + 0.5f, 0.0f);
	gOutputA[dispatchThreadID.xy] = rad;
	gOutputE[dispatchThreadID.xy] = E;
	gOutputT[dispatchThreadID.xy] = T;
}


technique11 LightTech
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, GenBlockOfLODCS()));
	}
}