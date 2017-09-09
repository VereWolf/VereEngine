#pragma once

#include "pch.h"
#include "PlanetLOD.h"
#include "Atmosphere.h"
#include "PlanetData.h"
#include "GameObject.h"

class QuadScreenWCAMessage : public RenderMessage
{
public:
	void Use();

	ID3D11ShaderResourceView * m_MainTargetSRV;
	ID3D11ShaderResourceView * m_MainDepthSRV;
	ID3D11ShaderResourceView * m_WaterTopTargetSRV;
	ID3D11ShaderResourceView * m_WaterTopDepthSRV;
	ID3D11ShaderResourceView * m_WaterBottomTargetSRV;
	ID3D11ShaderResourceView * m_WaterBottomDepthSRV;
	ID3D11ShaderResourceView * m_CloudsTargetSRV;
	ID3D11ShaderResourceView * m_CloudsDepthSRV;
	ID3D11ShaderResourceView * m_AtmosphereTargetSRV;
	ID3D11ShaderResourceView * m_AtmosphereDepthSRV;
	float m_Depth;
	float m_Size;
	float m_WaterRatio;
	float m_CloudsRatio;
};

class Planet: public PlanetData
{
public:
	Planet();
	Planet(Planet & other);

	void Init();
	void Render();

	void DrawWCA();
	void DrawPlanet();

	void BuildPlanet(std::string planetPath, int cellSize, int maxLevel, int loadDataMaxLvl, int loadTilesLvl, int loadDataPer,
		XMFLOAT3 fogColor, XMFLOAT3 waterColor, float waterDeep,
		int sizeOfBigTile, int levelOfSmallBlock, int levelOfBigBlock);

	GameObject* Clone() { return new Planet(*this); }
	PlanetLOD *GetPlanetLOD(int i) { return &m_PlanetLOD[i]; }
private:
	int m_interator;

	/*m_CP and PP are for find out of chage velocity in relative to main camera*/

	btVector3 m_CP;
	btVector3 m_PP;

	PlanetLOD m_PlanetLOD[6];
	int m_RenderIdWCAQuadScreen;
};

