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


	srand(514731);
	float noise[4356];

	for (int y=1; y < 65; ++y)
	{
		for (int x=1; x < 65; ++x)
		{
			noise[y * 66 + x] = (float)(rand() % 501) * 0.002f;
		}
	}

<<<<<<< HEAD
	GameStreamingDataHandle->DeleteStreamingData(idTile1);
	GameStreamingDataHandle->DeleteStreamingData(idTile2);


=======
	for (int x = 1; x < 65; ++x)
	{
		noise[x] = noise[4224 + x];
		noise[4290 + x] = noise[66 + x];
	}

	for (int y = 1; y < 65; ++y)
	{
		noise[y * 66] = noise[y * 66 + 64];
		noise[y * 66 + 65] = noise[y * 66 + 1];
	}

	noise[0] = noise[4288];
	noise[65] = noise[4225];
	noise[4290] = noise[130];
	noise[4355] = noise[67];

	m_TileNoiseSRV = GameRenderDeviceHandle->CreateTexture(&noise[0], 66, 66, DXGI_FORMAT_R32_FLOAT, 1);
>>>>>>> master

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