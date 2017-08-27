#include "pch.h"
#include "GamePlanetKind.h"

void GamePlanetKind::BuildPlanetKind(std::vector<int> *enviroments, int widthOfEnviroments)
{
	m_Enviroments = *enviroments;

	m_widthOfEnviroments - widthOfEnviroments;
}

BlendEnviroment GamePlanetKind::GetBlend(XMFLOAT2 range)
{
	BlendEnviroment BE;

	float W = m_widthOfEnviroments;
	float H = m_Enviroments.size() / W;

	XMINT4 T;
	T.x = range.x * (W - 1.0f);
	T.y = range.y * (H - 1.0f);

	XMFLOAT4 B;

	if (T.x == 1.0f)
	{
		B.x = 1.0f;
		B.z = 0.0f;
		T.z = T.x - 1;
	}
	else
	{
		B.z = range.x * (W - 1.0f) - T.x;
		B.x = 1.0f - B.z;
		T.z = T.x + 1;
	}

	if (T.y == 1.0f)
	{
		B.y = 1.0f;
		B.w = 0.0f;
		T.w = T.y - 1;
	}
	else
	{
		B.w = range.y * (H - 1.0f) - T.y;
		B.y = 1.0f - B.w;
		T.w = T.y + 1;
	}

	BE.blend11 = B.x *B.z;
	BE.blend12 = B.y *B.z;
	BE.blend21 = B.x *B.w;
	BE.blend22 = B.y *B.w;
	BE.type11 = B.z * ((int)W) + B.x;
	BE.type11 = B.z * ((int)W) + B.y;
	BE.type11 = B.w * ((int)W) + B.x;
	BE.type11 = B.w * ((int)W) + B.y;

	return BE;
}