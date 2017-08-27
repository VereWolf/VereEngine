#include "pch.h"
#include "GamePlanetTile.h"

XMFLOAT4 GamePlanetTile::GetNormal(XMFLOAT2 coord, float dot)
{
	float BF = m_tileBalancForFlat;
	XMFLOAT4 NF = m_tileNormalForFlat->GetVariable(coord.x, coord.y);
	NF = XMFLOAT4(BF * NF.x, (1.0f - BF) + BF * NF.y, BF * NF.z, 0.0f);

	float BS = m_tileBalancForSlope;
	XMFLOAT4 NS = m_tileNormalForSlope->GetVariable(coord.x, coord.y);
	NS = XMFLOAT4(BS * NS.x, (1.0f - BS) + BS * NS.y, BS * NS.z, 0.0f);

	return XMFLOAT4(dot * NS.x + (1.0f - dot) * NF.x,
		dot * NS.y + (1.0f - dot) * NF.y,
		dot * NS.z + (1.0f - dot) * NF.z, 0);
}

float GamePlanetTile::GetHeight(XMFLOAT2 coord, float dot)
{
	float HF = m_tileBalancForFlat * m_tileHeightForFlat->GetVariable(coord.x, coord.y);
	float HS = m_tileBalancForSlope * m_tileHeightForSlope->GetVariable(coord.x, coord.y);

	return (dot * HS + (1.0f - dot) * HF);
}

void GamePlanetTile::BuildPlanetTile(std::string nameNormalFlat, std::string nameHeightFlat,
	std::string nameNormalSlope, std::string nameHeightSlope,
	float balanceFlat, float balanceSlope, int width, int height)
{
	int id = GameStreamingDataHandle->LoadData(nameNormalFlat);
	m_tileNormalForFlat = new VereTextureFloat4;
	m_tileNormalForFlat->Init(GameStreamingDataHandle->GetStreamingData(id), width, height);
	GameStreamingDataHandle->DeleteStreamingData(id);

	id = GameStreamingDataHandle->LoadData(nameHeightFlat);
	m_tileHeightForFlat = new VereTextureFloat;
	m_tileHeightForFlat->Init(GameStreamingDataHandle->GetStreamingData(id), width, height);
	GameStreamingDataHandle->DeleteStreamingData(id);

	id = GameStreamingDataHandle->LoadData(nameNormalSlope);
	m_tileNormalForSlope = new VereTextureFloat4;
	m_tileNormalForSlope->Init(GameStreamingDataHandle->GetStreamingData(id), width, height);
	GameStreamingDataHandle->DeleteStreamingData(id);

	id = GameStreamingDataHandle->LoadData(nameHeightSlope);
	m_tileHeightForSlope = new VereTextureFloat;
	m_tileHeightForSlope->Init(GameStreamingDataHandle->GetStreamingData(id), width, height);
	GameStreamingDataHandle->DeleteStreamingData(id);

	m_tileBalancForFlat = balanceFlat;
	m_tileBalancForSlope = balanceSlope;
}

GamePlanetTile *GamePlanetTile::CreteNewGamePlanetTileFrom(float lvl, XMFLOAT2 offset, float seam, float height, float width)
{
	GamePlanetTile *PT = new GamePlanetTile;

	PT->SetTileNormalForFlat(m_tileNormalForFlat->CreateNewTextureFrom(lvl, offset, seam, height, width));
	PT->SetTileHeightForFlat(m_tileHeightForFlat->CreateNewTextureFrom(lvl, offset, seam, height, width));
	PT->SetTileNormalForSlope(m_tileNormalForSlope->CreateNewTextureFrom(lvl, offset, seam, height, width));
	PT->SetTileHeightForSlope(m_tileHeightForSlope->CreateNewTextureFrom(lvl, offset, seam, height, width));
	PT->SetTileBalancForFlat(m_tileBalancForFlat);
	PT->SetTileBalancForSlope(m_tileBalancForSlope);

	return PT;
}