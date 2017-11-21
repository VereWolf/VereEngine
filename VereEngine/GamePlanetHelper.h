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
		GameRenderDeviceHandle->DeleteTexture(m_TileNoiseSRV);
	}

	void Init(DX::DeviceResources *resources);

	int CreatePlanetTile(std::string nameNormalFlat, std::string nameHeightFlat,
		std::string nameNormalSlope, std::string nameHeightSlope,
		float balanceFlat, float balanceSlope, int width, int height);
	void DeletePlanetTile(int id);
	GamePlanetTile* GetPlanetTile(int id);
	
	PlanetAssetsStacks *GetPlanetAssetsStacks() { return &m_planetAssetsStacks; }

	ID3D11ShaderResourceView *GetTileNoiseSRV() { return GameRenderDeviceHandle->GetTexture(m_TileNoiseSRV); }

	XMFLOAT2 GetTreesTiles(int i) { return XMFLOAT2(m_TreesTiles[2 * i], m_TreesTiles[2 * i + 1]); }
	void SetTreesTilesSize(int s) { m_TreesTiles.resize(2 * s); }
	size_t GetTreesTilesSize() { return m_TreesTiles.size() / 2; }
	int GetTreesIndex(int i) { return m_TreesIndex[i]; }
	void SetTreesIndexSize(int s) { m_TreesIndex.resize(s); }
	size_t GetTreesIndexSize() { return m_TreesIndex.size(); }
private:
	DX::DeviceResources *m_resources;

	PlanetAssetsStacks m_planetAssetsStacks;

	int m_TileNoiseSRV;

	std::vector<float> m_TreesTiles;
	std::vector<int> m_TreesIndex;
};

__declspec(selectany) GamePlanetHelper *GamePlanetHelperHandle = 0;
