#pragma once

#include "pch.h"
#include "DeviceResources.h"
#include "GameBaseObjectsStack.h"

class GamePlanetHelper
{
public:
	GamePlanetHelper();
	GamePlanetHelper(DX::DeviceResources *resources);
	~GamePlanetHelper()
	{
		delete m_Tile1;
		delete m_Tile2;
	}

	void Init(DX::DeviceResources *resources);

	int CreatePlanetTile(std::string nameNormalFlat, std::string nameHeightFlat,
		std::string nameNormalSlope, std::string nameHeightSlope,
		float balanceFlat, float balanceSlope, int width, int height);
	void DeletePlanetTile(int id);
	GamePlanetTile* GetPlanetTile(int id);
	
	PlanetAssetsStacks *GetPlanetAssetsStacks() { return &m_planetAssetsStacks; }

	VereTextureFloat *GetTile1() { return m_Tile1; }
	VereTextureFloat *GetTile2() { return m_Tile2; }
private:
	DX::DeviceResources *m_resources;

	PlanetAssetsStacks m_planetAssetsStacks;

	VereTextureFloat *m_Tile1;
	VereTextureFloat *m_Tile2;
};

__declspec(selectany) GamePlanetHelper *GamePlanetHelperHandle = 0;
