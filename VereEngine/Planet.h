#ifndef TERRAIN_PLANET_H
#define TERRAIN_PLANET_H

#include "pch.h"
#include "PlanetLOD.h"
#include "Atmosphere.h"
#include "PlanetData.h"
#include "GameObject.h"

class TerrainPlanet: public TerrainPlanetData
{
public:
	TerrainPlanet();
	TerrainPlanet(TerrainPlanet & other);

	void Init();
	void Render(btTransform camOffset, XMMATRIX camView, XMMATRIX camProj,
		float camFarZ, btScalar heightFar, btScalar aspect,
		float camFarRangeMod, float camModifier);

	void BuildPlanet(int cellSize, int maxLevel, int maxRenderLevel);

	GameObject* Clone() { return new TerrainPlanet(*this); }
	TerrainPlanetLOD *GetPlanetLOD(int i) { return &m_PlanetLOD[i]; }
private:
	TerrainPlanetLOD m_PlanetLOD[6];

};

#endif // !TERRAIN_PLANET_H

