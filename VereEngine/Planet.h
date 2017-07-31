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
	ID3D11ShaderResourceView * m_CoordTargetSRV;
	ID3D11ShaderResourceView * m_CoordDepthSRV;
	ID3D11ShaderResourceView * m_WaterTargetSRV;
	ID3D11ShaderResourceView * m_WaterDepthSRV;
	ID3D11ShaderResourceView * m_CloudsTargetSRV;
	ID3D11ShaderResourceView * m_CloudsDepthSRV;
	ID3D11ShaderResourceView * m_AtmosphereTargetSRV;
	ID3D11ShaderResourceView * m_AtmosphereDepthSRV;
	ID3D11ShaderResourceView * m_SphereDepthPatternMap;
	float m_Depth;
	float m_Size;
	float m_WaterRatio;
	float m_CloudsRatio;
};

class QuadScreenWithCoordMessage : public RenderMessage
{
public:
	void Use();

	float m_size;
};

class Planet: public PlanetData
{
public:
	Planet();
	Planet(Planet & other);

	void Init();
	void Render(btTransform camOffset, XMMATRIX camView, XMMATRIX camProj,
		float camFarZ, btScalar heightFar, btScalar aspect,
		float camFarRangeMod, float camModifier);

	void DrawQuadWithCoord();
	void DrawWCA();
	void DrawPlanet();

	void BuildPlanet(int cellSize, int maxLevel, int maxRenderLevel, UINT loadDataAfterAgain, UINT maxLevelOfStreaming);

	GameObject* Clone() { return new Planet(*this); }
	PlanetLOD *GetPlanetLOD(int i) { return &m_PlanetLOD[i]; }
private:
	PlanetLOD m_PlanetLOD[6];
	int m_RenderIdWCAQuadScreen;
	int m_RenderIdQuadScreenWithCoord;
};

