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

	XMFLOAT2 GetTreesTiles(int i) { return XMFLOAT2(m_TreesTiles[2 * i], m_TreesTiles[2 * i + 1]); }
	void SetTreesTilesSize(int s) { m_TreesTiles.resize(2 * s); }
	size_t GetTreesTilesSize() { return m_TreesTiles.size() / 2; }
	int GetTreesIndex(int i) { return m_TreesIndex[i]; }
	void SetTreesIndexSize(int s) { m_TreesIndex.resize(s); }
	size_t GetTreesIndexSize() { return m_TreesIndex.size(); }
private:
	DX::DeviceResources *m_resources;

	PlanetAssetsStacks m_planetAssetsStacks;

	VereTextureFloat *m_Tile1;
	VereTextureFloat *m_Tile2;

	std::vector<float> m_TreesTiles;
	std::vector<int> m_TreesIndex;
};

__declspec(selectany) GamePlanetHelper *GamePlanetHelperHandle = 0;
