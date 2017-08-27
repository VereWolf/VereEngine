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
		m_isCreateNewLevelInProcess = 0;
		m_isNewLevel = false;
		m_blocks[0] = NULL;
		m_blocks[1] = NULL;
		m_blocks[2] = NULL;
		m_blocks[3] = NULL;
	}

	PlanetLOD(const PlanetLOD & n)
	{
		m_idHeightMap = -1;
		m_idNormalMap = -1;
		m_isCreateNewLevelInProcess = 0;
		m_isNewLevel = false;
		m_blocks[0] = NULL;
		m_blocks[1] = NULL;
		m_blocks[2] = NULL;
		m_blocks[3] = NULL;
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

	void Init(PlanetData * master, int side, int level, XMINT2 coord, btVector3 position, btScalar scaling,
		int levelFromLastLoadData, XMINT2 coordFromLastLoadData, btScalar scalingFromLastLoadData,
		int levelFromLoadTile, XMINT2 coordFromLoadTile, btScalar scalingFromLoadTile,
		VereTextureFloat *hMap, VereTextureBYTE4 *nMap, VereTextureBYTE4 *eMap, VereTextureBYTE4 *tMap);
	void Render();

	void DrawTerrain();
	void DrawWater();
	void DrawAtmosphere();
	void DrawClouds();

	void DestroyNextBlocks();

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
	int m_levelFromLastLoadData;
	XMINT2 m_coordFromLastLoadData;
	btScalar m_scalingFromLastLoadData;
	int m_levelFromLoadTile;
	XMINT2 m_coordFromLoadTile;
	btScalar m_scalingFromLoadTile;
	//btVector3  m_normal[9];
	int m_modelID;

	VereTextureFloat *m_hMap;
	VereTextureBYTE4 *m_nMap;
	VereTextureBYTE4 *m_eMap;
	VereTextureBYTE4 *m_tMap;

	VereTextureFloat *m_heightMap;
	VereTextureBYTE4 *m_normalMap;
	VereTextureBYTE4 *m_enviromentMap;
	VereTextureBYTE4 *m_treesMap;

	GamePlanetTile *m_planetTile;

	int m_idHeightMap;
	int m_idNormalMap;
	int m_idEnviromentMap;
	int m_idTreesMap;

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

	int m_isCreateNewLevelInProcess;
	bool m_isNewLevel;
};