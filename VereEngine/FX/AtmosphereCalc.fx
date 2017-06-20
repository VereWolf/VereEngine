float AtmosphereCalc(float3 pos, float height, float3 normal)
{

	float W = clamp(length(pos), 0.0f, height) * (2.0f - dot(normal, float3(0.0f, 1.0f, 0.0f)));

	return W;
}