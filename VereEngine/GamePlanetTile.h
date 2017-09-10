#pragma once
#include "pch.h"

class GamePlanetTile: public GameBaseObject
{
public:
	GamePlanetTile() {}
	~GamePlanetTile() {}

	XMFLOAT4 GetNormal(XMFLOAT2 coord, float dot);
	float GetHeight(XMFLOAT2 coord, float dot);

	void BuildPlanetTile(std::string nameNormalFlat, std::string nameHeightFlat,
		std::string nameNormalSlope, std::string nameHeightSlope,
		float balanceFlat, float balanceSlope, int width, int height);

	GamePlanetTile *CreteNewGamePlanetTileFrom(float lvl, XMFLOAT2 offset, float seam, float height, float width);

	VereTextureFloat4 *GetTileNormalForFlat() { return m_tileNormalForFlat; }
	void SetTileNormalForFlat(VereTextureFloat4 *T) { m_tileNormalForFlat = T; }
	VereTextureFloat *GetTileHeightForFlat() { return m_tileHeightForFlat; }
	void SetTileHeightForFlat(VereTextureFloat *T) { m_tileHeightForFlat = T; }
	VereTextureFloat4 *GetTileNormalForSlope() { return m_tileNormalForSlope; }
	void SetTileNormalForSlope(VereTextureFloat4 *T) { m_tileNormalForSlope = T; }
	VereTextureFloat *GetTileHeightForSlope() { return m_tileHeightForSlope; }
	void SetTileHeightForSlope(VereTextureFloat *T) { m_tileHeightForSlope = T; }

	float GetTileBalancForFlat() { return m_tileBalancForFlat; }
	void SetTileBalancForFlat(float B) { m_tileBalancForFlat = B; }
	float GetTileBalancForSlope() { return m_tileBalancForSlope; }
	void SetTileBalancForSlope(float B) { m_tileBalancForSlope = B; }

private:
	VereTextureFloat4 *m_tileNormalForFlat;
	VereTextureFloat *m_tileHeightForFlat;
	float m_tileBalancForFlat;
	VereTextureFloat4 *m_tileNormalForSlope;
	VereTextureFloat *m_tileHeightForSlope;
	float m_tileBalancForSlope;
};
