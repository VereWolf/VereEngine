
float CalcAlphaForAtmosphereDome(float depth, float range)
{
	return clamp(depth / range, 0.0f, 1.0f);
}

float CalcFog(float3 centerOfPlanet, float3 posOfObject, float3 posOfCamera, float radiusOfAtmosphere)
{
	float3 O = posOfObject - centerOfPlanet;
	float3 C = posOfCamera - centerOfPlanet;
	float LO = length(O);
	float LC = length(C);
	float L = length(O - C);
	float RR = 1.0f / radiusOfAtmosphere;

	if (LO > radiusOfAtmosphere)
	{
		return 0.0f;
	}
	else if (LC > radiusOfAtmosphere)
	{
		return (1.0f - abs(dot(normalize(O), -normalize(centerOfPlanet))));
	}

	return ((0.5f * (LO + LC)) * RR) * (L * RR);
}

float CalcWater(float3 centerOfPlanet, float3 posOfObject, float3 posOfCamera, float radiusOfWater, float maxDeep)
{
	float3 O = posOfObject - centerOfPlanet;
	float3 C = posOfCamera - centerOfPlanet;
	float LO = radiusOfWater - length(O);
	float LC = radiusOfWater - length(C);
	float L = length(O - C);
	float RR = 1.0f / radiusOfWater;
	float MD = radiusOfWater / maxDeep;

	if (LO < 0.0f)
	{
		return 0.0f;
	}
	else if (LC < 0.0f)
	{
		float D = clamp(1.0f - MD + MD * (1.0f - abs(dot(normalize(O), -normalize(centerOfPlanet)))), 0.0f, 1.0f);
		return clamp(D + (1.0f - D) * (MD * LO * RR), 0.0f, 1.0f);
	}

	return clamp(MD * L * RR, 0.0f, 1.0f);
}