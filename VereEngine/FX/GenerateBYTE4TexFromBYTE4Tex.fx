
cbuffer cbUpdateSettings
{
	float2 gStartPos;
	float2 gStepSize;
};

SamplerState samInputMap
{
	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};

Texture2D gInput;
RWTexture2D<float4> gOutput;

[numthreads(16, 16, 1)]
void GenBYTE4CS(int3 groupThreadID : SV_GroupThreadID,
	int3 dispatchThreadID : SV_DispatchThreadID)
{
	gOutput[dispatchThreadID.xy].xyzw = gInput.SampleLevel(samInputMap, float2(gStartPos.x + (float)dispatchThreadID.x * gStepSize.x, gStartPos.y + (float)dispatchThreadID.y * gStepSize.y), 0).xyzw;
}


technique11 LightTech
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, GenBYTE4CS()));
	}
}