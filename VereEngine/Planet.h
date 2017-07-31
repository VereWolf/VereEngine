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

	void BuildPlanet(int cellSize, int maxLevel, int maxRenderLevel, UINT loadDataAfterAgain, UINT maxLevelOfStreaming,
		XMFLOAT3 fogColor, XMFLOAT3 waterColor, float waterDeep);

	GameObject* Clone() { return new Planet(*this); }
	PlanetLOD *GetPlanetLOD(int i) { return &m_PlanetLOD[i]; }
private:
	PlanetLOD m_PlanetLOD[6];
	int m_RenderIdWCAQuadScreen;
};

