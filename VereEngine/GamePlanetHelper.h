#pragma once

#include "pch.h"
#include "DeviceResources.h"
#include "GameBaseObjectsStack.h"

class GamePlanetHelper
{
public:
	GamePlanetHelper();
	GamePlanetHelper(DX::DeviceResources *resources);

	void Init(DX::DeviceResources *resources);

	int CreatePlanetTile(std::string nameNormalFlat, std::string nameHeightFlat,
		std::string nameNormalSlope, std::string nameHeightSlope,
		float balanceFlat, float balanceSlope, int width, int height);
	void DeletePlanetTile(int id);
	GamePlanetTile* GetPlanetTile(int id);
	
	PlanetAssetsStacks *GetPlanetAssetsStacks() { return &m_planetAssetsStacks; }
private:
	DX::DeviceResources *m_resources;

	PlanetAssetsStacks m_planetAssetsStacks;
};

__declspec(selectany) GamePlanetHelper *GamePlanetHelperHandle = 0;
