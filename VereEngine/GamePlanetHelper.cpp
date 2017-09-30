#include "pch.h"
#include "DeviceResources.h"
#include "GameBaseObjectsStack.h"
#include "GamePlanetHelper.h"

GamePlanetHelper::GamePlanetHelper()
{
	GamePlanetHelperHandle = this;
}
GamePlanetHelper::GamePlanetHelper(DX::DeviceResources *resources)
{
	GamePlanetHelperHandle = this;

	GamePlanetHelper::Init(resources);
}

void GamePlanetHelper::Init(DX::DeviceResources *resources)
{
	m_resources = resources;

	GetPlanetAssetsStacks()->m_gamePlanetTiles.Init(resources);

	int idTile1 = GameStreamingDataHandle->LoadData("planet/Tiles/Tile_1.raw");
	int idTile2 = GameStreamingDataHandle->LoadData("planet/Tiles/Tile_2.raw");

	m_Tile1 = new VereTextureFloat;
	m_Tile2 = new VereTextureFloat;

	m_Tile1->Init(GameStreamingDataHandle->GetStreamingData(idTile1), 512, 512);
	m_Tile2->Init(GameStreamingDataHandle->GetStreamingData(idTile2), 512, 512);

	GameStreamingDataHandle->DeleteStreamingData(idTile1);
	GameStreamingDataHandle->DeleteStreamingData(idTile2);



	int idTreesTiles = GameStreamingDataHandle->LoadData("planet/Tiles/Trees_tiles.raw");
	int idTreesIndex = GameStreamingDataHandle->LoadData("planet/Tiles/Trees_index.raw");

	m_TreesTiles.resize(GameStreamingDataHandle->GetSizeOfStreamingData(idTreesTiles) / sizeof(float));
	m_TreesIndex.resize(GameStreamingDataHandle->GetSizeOfStreamingData(idTreesIndex) / sizeof(int));

	memcpy(&m_TreesTiles[0], GameStreamingDataHandle->GetStreamingData(idTreesTiles), GameStreamingDataHandle->GetSizeOfStreamingData(idTreesTiles));
	memcpy(&m_TreesIndex[0], GameStreamingDataHandle->GetStreamingData(idTreesIndex), GameStreamingDataHandle->GetSizeOfStreamingData(idTreesIndex));

	GameStreamingDataHandle->DeleteStreamingData(idTreesTiles);
	GameStreamingDataHandle->DeleteStreamingData(idTreesIndex);
}

int GamePlanetHelper::CreatePlanetTile(std::string nameNormalFlat, std::string nameHeightFlat,
	std::string nameNormalSlope, std::string nameHeightSlope,
	float balanceFlat, float balanceSlope, int width, int height)
{
	GamePlanetTile* planetTile = new GamePlanetTile;
	planetTile->BuildPlanetTile(nameNormalFlat, nameHeightFlat,
		nameNormalSlope, nameHeightSlope,
		balanceFlat, balanceSlope, width, height);

	return m_planetAssetsStacks.m_gamePlanetTiles.CreateGameObject(planetTile);
}

void GamePlanetHelper::DeletePlanetTile(int id)
{
	m_planetAssetsStacks.m_gamePlanetTiles.DeleteGameObject(id);
}

GamePlanetTile* GamePlanetHelper::GetPlanetTile(int id)
{
	return ((GamePlanetTile*)m_planetAssetsStacks.m_gamePlanetTiles.GetGameObject(id));
}