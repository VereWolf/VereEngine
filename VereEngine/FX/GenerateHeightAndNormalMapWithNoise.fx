
cbuffer cbUpdateSettings
{
	float2 gStartPos;
	float2 gStepSize;
	float gSpacingMap;
	float gSpacingWorld;
	float gRangeNoise;
	bool gIsMap1;
	bool gIsMap2;
};

SamplerState samInputMap
{
	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};

Texture2D gInputHeight;
Texture2D gInputNormal;
Texture2D gTile;
Texture2D gInputRiverWidth1;
Texture2D gInputRiverWidth2;
Texture2D gInputRiverLength1;
Texture2D gInputRiverLength2;
Texture2D gInputRiverType1;
Texture2D gInputRiverType2;
Texture2D gInputRiverHeight;
RWTexture2D<float> gOutputHeight;
RWTexture2D<float4> gOutputNormal;

[numthreads(16, 16, 1)]
void GenHeightNormalWithNoiseCS(int3 groupThreadID : SV_GroupThreadID,
	int3 dispatchThreadID : SV_DispatchThreadID)
{
	float2 rCoord = float2(gStartPos.x + (float)dispatchThreadID.x * gStepSize.x, gStartPos.y + (float)dispatchThreadID.y * gStepSize.y);

	float2 lCoord = float2((float)dispatchThreadID.x / ((float)gOutputHeight.Length.x - 1.0f), (float)dispatchThreadID.y / ((float)gOutputHeight.Length.y - 1.0f));

	float H = gInputHeight.SampleLevel(samInputMap, float2(rCoord.x, rCoord.y), 0).r;

	float3 MNorm = 2.0f * gInputNormal.SampleLevel(samInputMap, float2(rCoord.x, rCoord.y), 0).rgb - 1.0f;

	gOutputHeight[dispatchThreadID.xy].r = H;
	gOutputNormal[dispatchThreadID.xy].rgba = float4(0.5f * MNorm + 0.5f, 0.0f);

	float RW1[5];
	RW1[0] = 0.0f;

	float RH1 = 0.0f;

	float RH = 0.0f;

	float MNorm1 = float3(0.0f, 0.0f, 0.0f);

	float MRI = 1.0f;

	if (gIsMap1 || gIsMap2)
	{
		RH = gInputRiverHeight.SampleLevel(samInputMap, float2(rCoord.x, rCoord.y), 0).r;
	}

	float3x3 TNCR = float3x3(float3(1.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f), float3(0.0f, 0.0f, 1.0f));

	float MRH2 = -2.0f;

	if (gIsMap1)
	{
		RW1[0] = gInputRiverWidth1.SampleLevel(samInputMap, float2(rCoord.x, rCoord.y), 0).r;
		RW1[1] = gInputRiverWidth1.SampleLevel(samInputMap, float2(rCoord.x + gStepSize.x, rCoord.y), 0).r;
		RW1[2] = gInputRiverWidth1.SampleLevel(samInputMap, float2(rCoord.x - gStepSize.x, rCoord.y), 0).r;
		RW1[3] = gInputRiverWidth1.SampleLevel(samInputMap, float2(rCoord.x, rCoord.y + gStepSize.y), 0).r;
		RW1[4] = gInputRiverWidth1.SampleLevel(samInputMap, float2(rCoord.x, rCoord.y - gStepSize.y), 0).r;
		float RL1 = gInputRiverLength1.SampleLevel(samInputMap, float2(rCoord.x, rCoord.y), 0).r;
		float RT1 = gInputRiverType1.SampleLevel(samInputMap, float2(rCoord.x, rCoord.y), 0).r;

		if (RW1[0] > -100.0f)
		{
			float RH2[5];

			float RI = 1.0f - abs(1.5f * RW1[0]) / (RT1);
			float RI2 = (RH - H) * (1.0f + clamp(RI, -1.0f, 0.0f));

			MRI = abs(clamp(RI, -1.0f, 0.0f));

			RH2[0] = pow(0.5f * cos(3.1416f * clamp(RI, 0.0, 1.0f)) - 0.5f, 2.0f);

			if (RW1[1] > -100.0f)
			{
				RI = 1.0f - abs(1.5f * RW1[1]) / (RT1);
				RI2 = (RH - H) * (1.0f + clamp(RI, -1.0f, 0.0f));

				RH2[1] = pow(0.5f * cos(3.1416f * clamp(RI, 0.0, 1.0f)) - 0.5f, 2.0f);
			}
			else
			{
				RH2[1] = float3(0.0f, 1.0f, 0.0f);
			}


			if (RW1[2] > -100.0f)
			{
				RI = 1.0f - abs(1.5f * RW1[2]) / (RT1);
				RI2 = (RH - H) * (1.0f + clamp(RI, -1.0f, 0.0f));

				RH2[2] = pow(0.5f * cos(3.1416f * clamp(RI, 0.0, 1.0f)) - 0.5f, 2.0f);
			}
			else
			{
				RH2[2] = float3(0.0f, 1.0f, 0.0f);
			}


			if (RW1[3] > -100.0f)
			{
				RI = 1.0f - abs(1.5f * RW1[3]) / (RT1);
				RI2 = (RH - H) * (1.0f + clamp(RI, -1.0f, 0.0f));

				RH2[3] = pow(0.5f * cos(3.1416f * clamp(RI, 0.0, 1.0f)) - 0.5f, 2.0f);
			}
			else
			{
				RH2[3] = float3(0.0f, 1.0f, 0.0f);
			}


			if (RW1[4] > -100.0f)
			{
				RI = 1.0f - abs(1.5f * RW1[4]) / (RT1);
				RI2 = (RH - H) * (1.0f + clamp(RI, -1.0f, 0.0f));

				RH2[4] = pow(0.5f * cos(3.1416f * clamp(RI, 0.0, 1.0f)) - 0.5f, 2.0f);
			}
			else
			{
				RH2[4] = float3(0.0f, 1.0f, 0.0f);
			}

			RH2[0] = RH2[0] * MRH2 + RI2;
			RH2[1] = RH2[1] * MRH2 + RI2;
			RH2[2] = RH2[2] * MRH2 + RI2;
			RH2[3] = RH2[3] * MRH2 + RI2;
			RH2[4] = RH2[4] * MRH2 + RI2;

			RH2[1] = RH2[0] - RH2[1];
			RH2[2] = RH2[0] - RH2[2];
			RH2[3] = RH2[0] - RH2[3];
			RH2[4] = RH2[0] - RH2[4];

			float3 N = normalize(float3(RH2[1] - RH2[2], gSpacingWorld, RH2[3] - RH2[4]));

			float3 MTan = cross(MNorm, float3(0.0f, 0.0f, 1.0f));
			float3 MCot = cross(float3(1.0f, 0.0f, 0.0f), MNorm);

			float3x3 TNCR = float3x3(MTan, MNorm, MCot);

			RH1 = RH2[0];

			//RH1 = -450.0f;
		}
	}

	if (gIsMap2)
	{
		RW1[0] = gInputRiverWidth2.SampleLevel(samInputMap, float2(rCoord.x, rCoord.y), 0).r;
		RW1[1] = gInputRiverWidth2.SampleLevel(samInputMap, float2(rCoord.x + gStepSize.x, rCoord.y), 0).r;
		RW1[2] = gInputRiverWidth2.SampleLevel(samInputMap, float2(rCoord.x - gStepSize.x, rCoord.y), 0).r;
		RW1[3] = gInputRiverWidth2.SampleLevel(samInputMap, float2(rCoord.x, rCoord.y + gStepSize.y), 0).r;
		RW1[4] = gInputRiverWidth2.SampleLevel(samInputMap, float2(rCoord.x, rCoord.y - gStepSize.y), 0).r;
		float RL1 = gInputRiverLength2.SampleLevel(samInputMap, float2(rCoord.x, rCoord.y), 0).r;
		float RT1 = gInputRiverType2.SampleLevel(samInputMap, float2(rCoord.x, rCoord.y), 0).r;

		if (RW1[0] > -100.0f)
		{
			float RH2[5];

			float RI = 1.0f - abs(1.5f * RW1[0]) / (RT1);
			float RI2 = (RH - H) * (1.0f + clamp(RI, -1.0f, 0.0f));

			RH2[0] = pow(0.5f * cos(3.1416f * clamp(RI, 0.0, 1.0f)) - 0.5f, 2.0f);

			if (RH2[0] < RH1)
			{
				MRI = abs(clamp(RI, -1.0f, 0.0f));

				if (RW1[1] > -100.0f)
				{
					RI = 1.0f - abs(1.5f * RW1[1]) / (RT1);
					RI2 = (RH - H) * (1.0f + clamp(RI, -1.0f, 0.0f));

					RH2[1] = pow(0.5f * cos(3.1416f * clamp(RI, 0.0, 1.0f)) - 0.5f, 2.0f);
				}
				else
				{
					RH2[1] = float3(0.0f, 1.0f, 0.0f);
				}


				if (RW1[2] > -100.0f)
				{
					RI = 1.0f - abs(1.5f * RW1[2]) / (RT1);
					RI2 = (RH - H) * (1.0f + clamp(RI, -1.0f, 0.0f));

					RH2[2] = pow(0.5f * cos(3.1416f * clamp(RI, 0.0, 1.0f)) - 0.5f, 2.0f);
				}
				else
				{
					RH2[2] = float3(0.0f, 1.0f, 0.0f);
				}


				if (RW1[3] > -100.0f)
				{
					RI = 1.0f - abs(1.5f * RW1[3]) / (RT1);
					RI2 = (RH - H) * (1.0f + clamp(RI, -1.0f, 0.0f));

					RH2[3] = pow(0.5f * cos(3.1416f * clamp(RI, 0.0, 1.0f)) - 0.5f, 2.0f);
				}
				else
				{
					RH2[3] = float3(0.0f, 1.0f, 0.0f);
				}


				if (RW1[4] > -100.0f)
				{
					RI = 1.0f - abs(1.5f * RW1[4]) / (RT1);
					RI2 = (RH - H) * (1.0f + clamp(RI, -1.0f, 0.0f));

					RH2[4] = pow(0.5f * cos(3.1416f * clamp(RI, 0.0, 1.0f)) - 0.5f, 2.0f);
				}
				else
				{
					RH2[4] = float3(0.0f, 1.0f, 0.0f);
				}

				RH2[0] = RH2[0] * MRH2 + RI2;
				RH2[1] = RH2[1] * MRH2 + RI2;
				RH2[2] = RH2[2] * MRH2 + RI2;
				RH2[3] = RH2[3] * MRH2 + RI2;
				RH2[4] = RH2[4] * MRH2 + RI2;

				RH2[1] = RH2[0] - RH2[1];
				RH2[2] = RH2[0] - RH2[2];
				RH2[3] = RH2[0] - RH2[3];
				RH2[4] = RH2[0] - RH2[4];

				float3 N = normalize(float3(RH2[1] - RH2[2], gSpacingWorld, RH2[3] - RH2[4]));

				float3 MTan = cross(MNorm, float3(0.0f, 0.0f, 1.0f));
				float3 MCot = cross(float3(1.0f, 0.0f, 0.0f), MNorm);

				TNCR = float3x3(MTan, MNorm, MCot);

				RH1 = RH2[0];

				RH1 = -450.0f;
			}
		}
	}

	if (lCoord.x != 0 && lCoord.x <= 63.0f && lCoord.y != 0 && lCoord.y <= 63)
	{
		float3 MTan = cross(MNorm, float3(0.0f, 0.0f, 1.0f));
		float3 MCot = cross(float3(1.0f, 0.0f, 0.0f), MNorm);

		float D = 10000.0f * pow((1.0f - 0.9f * dot(MNorm, float3(0.0f, 1.0f, 0.0f))), 2.0f);

		float random[5];

		rCoord = float2(gSpacingMap * dispatchThreadID.x, gSpacingMap * dispatchThreadID.y);

		random[0] = 0.5f * 2.0 * gRangeNoise * (gTile.SampleLevel(samInputMap, float2(rCoord.x + 0.31f, rCoord.y - 0.17f), 0).r - 0.5f)
			+ 0.25f * 2.0 * gRangeNoise * (gTile.SampleLevel(samInputMap, float2(2.0f * rCoord.x - 0.29f, 2.0f * rCoord.y + 0.19f), 0).r - 0.5f)
			+ 0.125f * 2.0 * gRangeNoise * (gTile.SampleLevel(samInputMap, float2(4.0f * rCoord.x - 0.36f, 4.0f * rCoord.y - 0.15f), 0).r - 0.5f)
			+ 0.0625f * 2.0 * gRangeNoise * (gTile.SampleLevel(samInputMap, float2(8.0f * (rCoord.x - 0.49f), 8.0f * (rCoord.y + 0.31f)), 0).r - 0.5f);

		rCoord = float2(gSpacingMap * (dispatchThreadID.x + 1), gSpacingMap * dispatchThreadID.y);

		random[1] = 0.5f * 2.0 * gRangeNoise * (gTile.SampleLevel(samInputMap, float2(rCoord.x + 0.31f, rCoord.y - 0.17f), 0).r - 0.5f)
			+ 0.25f * 2.0 * gRangeNoise * (gTile.SampleLevel(samInputMap, float2(2.0f * rCoord.x - 0.29f, 2.0f * rCoord.y + 0.19f), 0).r - 0.5f)
			+ 0.125f * 2.0 * gRangeNoise * (gTile.SampleLevel(samInputMap, float2(4.0f * rCoord.x - 0.36f, 4.0f * rCoord.y - 0.15f), 0).r - 0.5f)
			+ 0.0625f * 2.0 * gRangeNoise * (gTile.SampleLevel(samInputMap, float2(8.0f * (rCoord.x - 0.49f), 8.0f * (rCoord.y + 0.31f)), 0).r - 0.5f);

		rCoord = float2(gSpacingMap * (dispatchThreadID.x - 1), gSpacingMap * dispatchThreadID.y);

		random[2] = 0.5f * 2.0 * gRangeNoise * (gTile.SampleLevel(samInputMap, float2(rCoord.x + 0.31f, rCoord.y - 0.17f), 0).r - 0.5f)
			+ 0.25f * 2.0 * gRangeNoise * (gTile.SampleLevel(samInputMap, float2(2.0f * rCoord.x - 0.29f, 2.0f * rCoord.y + 0.19f), 0).r - 0.5f)
			+ 0.125f * 2.0 * gRangeNoise * (gTile.SampleLevel(samInputMap, float2(4.0f * rCoord.x - 0.36f, 4.0f * rCoord.y - 0.15f), 0).r - 0.5f)
			+ 0.0625f * 2.0 * gRangeNoise * (gTile.SampleLevel(samInputMap, float2(8.0f * (rCoord.x - 0.49f), 8.0f * (rCoord.y + 0.31f)), 0).r - 0.5f);

		rCoord = float2(gSpacingMap * dispatchThreadID.x, gSpacingMap * (dispatchThreadID.y + 1));

		random[3] = 0.5f * 2.0 * gRangeNoise * (gTile.SampleLevel(samInputMap, float2(rCoord.x + 0.31f, rCoord.y - 0.17f), 0).r - 0.5f)
			+ 0.25f * 2.0 * gRangeNoise * (gTile.SampleLevel(samInputMap, float2(2.0f * rCoord.x - 0.29f, 2.0f * rCoord.y + 0.19f), 0).r - 0.5f)
			+ 0.125f * 2.0 * gRangeNoise * (gTile.SampleLevel(samInputMap, float2(4.0f * rCoord.x - 0.36f, 4.0f * rCoord.y - 0.15f), 0).r - 0.5f)
			+ 0.0625f * 2.0 * gRangeNoise * (gTile.SampleLevel(samInputMap, float2(8.0f * (rCoord.x - 0.49f), 8.0f * (rCoord.y + 0.31f)), 0).r - 0.5f);

		rCoord = float2(gSpacingMap * dispatchThreadID.x, gSpacingMap * (dispatchThreadID.y - 1));

		random[4] = 0.5f * 2.0 * gRangeNoise * (gTile.SampleLevel(samInputMap, float2(rCoord.x + 0.31f, rCoord.y - 0.17f), 0).r - 0.5f)
			+ 0.25f * 2.0 * gRangeNoise * (gTile.SampleLevel(samInputMap, float2(2.0f * rCoord.x - 0.29f, 2.0f * rCoord.y + 0.19f), 0).r - 0.5f)
			+ 0.125f * 2.0 * gRangeNoise * (gTile.SampleLevel(samInputMap, float2(4.0f * rCoord.x - 0.36f, 4.0f * rCoord.y - 0.15f), 0).r - 0.5f)
			+ 0.0625f * 2.0 * gRangeNoise * (gTile.SampleLevel(samInputMap, float2(8.0f * (rCoord.x - 0.49f), 8.0f * (rCoord.y + 0.31f)), 0).r - 0.5f);

		rCoord = float2(gSpacingMap * dispatchThreadID.x, gSpacingMap * dispatchThreadID.y);

		float HR[4];
		HR[0] = random[0] - random[1];
		HR[1] = random[0] - random[2];
		HR[2] = random[0] - random[3];
		HR[3] = random[0] - random[4];

		float3 N = normalize(float3(HR[0] - HR[1], gSpacingWorld, HR[2] - HR[3]));

		float3x3 TNC = float3x3(MTan, MNorm, MCot);
		gOutputHeight[dispatchThreadID.xy].r += MRI * D * random[0] + RH1;

		gOutputNormal[dispatchThreadID.xy].rgba = float4(0.5f * mul(float3(normalize(MNorm + MRI * D * (mul(N, TNC) - MNorm))), TNCR) + 0.5f, 1.0f);
	}
}


technique11 LightTech
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, GenHeightNormalWithNoiseCS()));
	}
}