float3 GetCoordForCube(float3 dir, float3x3 sideMatrixInverse, float3 up, float3 right, float3 front)
{
	float3 U = up;
	float3 R = right;
	float3 F = front;

	dir = normalize(mul(dir, sideMatrixInverse));

	if (dir.y <= 0.0f)
	{
		return float3(0.0f, 0.0f, -1.0f);
	}

	dir.x = 0.5f * dir.x / dir.y;
	dir.z = 0.5f * dir.z / dir.y;

	return float3(dir.x, dir.y, 0.0f);
}