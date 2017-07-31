#pragma once

#include "pch.h"
#include "GameComponent.h"
#include "PlanetData.h"
#include "TerrainLOD.h"
#include "WaterLOD.h"
#include "Atmosphere.h"
#include "Clouds.h"

class PlanetLOD: public GameComponent
{
public:
	PlanetLOD()
	{
		m_idHeightMap = -1;
		m_idNormalMap = -1;
		m_isCreateNewLevelInProcess = false;
		m_isNewLevel = false;
	}

	PlanetLOD(const PlanetLOD & n)
	{
		m_idHeightMap = -1;
		m_idNormalMap = -1;
		m_isCreateNewLevelInProcess = false;
		m_isNewLevel = false;
	}

	~PlanetLOD()
	{
		if (m_level == 0)
		{
			delete m_hMap;
			delete m_nMap;
		}

		if (m_heightMap != NULL) SafeDelete(m_heightMap);
		if (m_normalMap != NULL) SafeDelete(m_normalMap);

		if (m_idHeightMap >= 0)	GameRenderDeviceHandle->DeleteTexture(m_idHeightMap);
		if (m_idNormalMap >= 0) GameRenderDeviceHandle->DeleteTexture(m_idNormalMap);

		GameRenderDeviceHandle->DeleteModel(m_modelID);
	}

	void Init(PlanetData * master, int side, int level, XMINT2 coord, btVector3 position, btScalar scaling, VereTextureFloat *hMap, VereTextureBYTE4 *nMap);
	void Render();

	void DrawTerrain();
	void DrawWater();
	void DrawAtmosphere();
	void DrawClouds();

	bool ComponentProccess();

	void CreateNewLevelOfLoD();

	static bool m_onlyRenderText;
private:
	btVector3  m_position;
	btVector3  m_angle;
	int m_side;
	int m_level;
	XMINT2 m_coord;
	btScalar m_scaling;
	//btVector3  m_normal[9];
	int m_modelID;

	VereTextureFloat *m_hMap;
	VereTextureBYTE4 *m_nMap;
	VereTextureFloat *m_heightMap;
	VereTextureBYTE4 *m_normalMap;

	int m_idHeightMap;
	int m_idNormalMap;

	btVector3 m_OffsetCubeT;
	btVector3 m_CentreT;
	btVector3 m_OffsetCubeW;
	btVector3 m_CentreW;
	btVector3 m_OffsetCubeA;
	btVector3 m_CentreA;
	btVector3 m_OffsetCubeC;
	btVector3 m_CentreC;

	PlanetLOD *m_blocks[4];

	PlanetData *m_data;

	bool m_isCreateNewLevelInProcess;
	bool m_isNewLevel;
};