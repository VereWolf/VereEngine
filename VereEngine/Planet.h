#pragma once

#include "pch.h"
#include "PlanetLOD.h"
#include "Atmosphere.h"
#include "PlanetData.h"
#include "GameObject.h"

class Planet: public PlanetData
{
public:
	Planet();
	Planet(Planet & other);

	void Init();
	void Render(btTransform camOffset, XMMATRIX camView, XMMATRIX camProj,
		float camFarZ, btScalar heightFar, btScalar aspect,
		float camFarRangeMod, float camModifier);

	void BuildPlanet(int cellSize, int maxLevel, int maxRenderLevel, UINT loadDataAfterAgain, UINT maxLevelOfStreaming);

	GameObject* Clone() { return new Planet(*this); }
	PlanetLOD *GetPlanetLOD(int i) { return &m_PlanetLOD[i]; }
private:
	PlanetLOD m_PlanetLOD[6];

};

