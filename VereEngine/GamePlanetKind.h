#pragma once

#include "GameBaseObject.h"

struct BlendEnviroment
{
	float blend11;
	int type11;
	float blend12;
	int type12;
	float blend21;
	int type21;
	float blend22;
	int type22;
};

class GamePlanetKind: public GameBaseObject
{
public:
	GamePlanetKind() {}
	~GamePlanetKind() {}

	void BuildPlanetKind(std::vector<int> *enviroments, int widthOfEnviroments);

	BlendEnviroment GetBlend(XMFLOAT2 range);
private:
	std::vector<int> m_Enviroments;

	int m_widthOfEnviroments;
};
