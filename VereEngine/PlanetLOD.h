#pragma once

#include "pch.h"
#include "GameComponent.h"
#include "PlanetData.h"
#include "TerrainLOD.h"
#include "WaterLOD.h"
#include "Atmosphere.h"
#include "TreesLOD.h"
#include "Clouds.h"
#include "GenerateBlockOfLOD.h"

class PlanetLOD: public GameComponent
{
public:
	PlanetLOD();
	PlanetLOD(const PlanetLOD & n);

	~PlanetLOD()
	{
		m_data->SetCurrentNumBlockSmall(m_data->GetCurerentNumBlockSmall() - m_valueOfLODSmall);
		m_data->SetCurrentNumBlockBig(m_data->GetCurerentNumBlockBig() - m_valueOfLODBig);
		m_data->SetCurrentNumBlockSmall(m_data->GetCurerentNumBlockSmall() - m_valueOfLODSmallForNext);
		m_data->SetCurrentNumBlockBig(m_data->GetCurerentNumBlockBig() - m_valueOfLODBigForNext);

		if (m_isMap)
		{
			if ((m_level % m_data->GetLoadDataPer()) == 0 && m_level < m_data->GetLoadDataMaxLvl())
			{
				if (m_idHeightMapBig >= 0) GameStreamingDataHandle->ReleaseBlockInFLOATDepository(m_data->GetIDHeightMapBig(), m_idHeightMapBig);
				if (m_idNormalMapBig >= 0) GameStreamingDataHandle->ReleaseBlockInBYTE4Depository(m_data->GetIDNormalMapBig(), m_idNormalMapBig);
				if (m_idTreesMapBig >= 0) GameStreamingDataHandle->ReleaseBlockInBYTE4Depository(m_data->GetIDTreesMapBig(), m_idTreesMapBig);
			}

			if (m_level == 0)
			{
				if (m_idEnviromentMapBig >= 0) GameStreamingDataHandle->ReleaseBlockInBYTE4Depository(m_data->GetIDEnviromentMapBig(), m_idEnviromentMapBig);
			}
		}

		/*if (m_idHeightMapBig >= 0) GameStreamingDataHandle->ReleaseBlockInFLOATDepository(m_data->GetIDHeightMapSmall(), m_idHeightMapSmall);
		if (m_idNormalMapBig >= 0) GameStreamingDataHandle->ReleaseBlockInBYTE4Depository(m_data->GetIDNormalMapSmall(), m_idNormalMapSmall);
		if (m_idNormalMapBig >= 0) GameStreamingDataHandle->ReleaseBlockInBYTE4Depository(m_data->GetIDNormalMapSmall(), m_idNormalMapSmall);
		if (m_idEnviromentMapBig >= 0) GameStreamingDataHandle->ReleaseBlockInBYTE4Depository(m_data->GetIDEnviromentMapSmall(), m_idEnviromentMapSmall);
		if (m_idTreesMapBig >= 0) GameStreamingDataHandle->ReleaseBlockInBYTE4Depository(m_data->GetIDTreesMapSmall(), m_idTreesMapSmall);*/

		if (m_idHeightMapInput >= 0)	GameRenderDeviceHandle->DeleteTexture(m_idHeightMapInput);
		if (m_idNormalMapInput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idNormalMapInput);
		if (m_idAngleMapInput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idAngleMapInput);
		if (m_idEnviromentMapInput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idEnviromentMapInput);
		if (m_idTreesMapInput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idTreesMapInput);

		if (m_idHeightMapOutput >= 0)	GameRenderDeviceHandle->DeleteTexture(m_idHeightMapOutput);
		if (m_idNormalMapOutput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idNormalMapOutput);
		if (m_idAngleMapOutput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idAngleMapOutput);
		if (m_idEnviromentMapOutput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idEnviromentMapOutput);
		if (m_idTreesMapOutput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idTreesMapOutput);

		/*if (m_idHeightMapSRT >= 0)	GameRenderDeviceHandle->DeleteTexture(m_idHeightMapSRT);
		if (m_idNormalMapSRT >= 0) GameRenderDeviceHandle->DeleteTexture(m_idNormalMapSRT);
		if (m_idEnviromentMapSRT >= 0) GameRenderDeviceHandle->DeleteTexture(m_idEnviromentMapSRT);
		if (m_idTreesMapSRT >= 0) GameRenderDeviceHandle->DeleteTexture(m_idTreesMapSRT);*/

		GameRenderDeviceHandle->DeleteModel(m_modelID);

		if (m_idBillboardTrees >= 0 && m_level == m_data->GetGenerateTreesLvl())
		{
			GameRenderDeviceHandle->DeleteVertex(GameRenderDeviceHandle->GetModel(m_idBillboardTrees)->idVertex);
			GameRenderDeviceHandle->DeleteEffect(GameRenderDeviceHandle->GetModel(m_idBillboardTrees)->idEffect);
			GameRenderDeviceHandle->DeleteInputLayouts(GameRenderDeviceHandle->GetModel(m_idBillboardTrees)->idInputLayouts);
			GameRenderDeviceHandle->DeleteMeshBuffer(GameRenderDeviceHandle->GetModel(m_idBillboardTrees)->idMeshBuffer);
			GameRenderDeviceHandle->DeleteModel(m_idBillboardTrees);
		}
	}

	void Init(PlanetData * master, int side, int level, XMINT2 coord, btVector3 position, btScalar scaling,
		int levelFromLastLoadData, XMINT2 coordFromLastLoadData, btScalar scalingFromLastLoadData,
		int levelFromLoadTile, XMINT2 coordFromLoadTile, btScalar scalingFromLoadTile, bool isMap,
		int idHeightMapBig, int idNormalMapBig, int idEnviromentMapBig, int idTreesMapBig,
		int idBillboardTreesVertex, std::vector<int> *billboardTreesIndex, btTransform billboardMatrix, int idBillboardTrees);
	void Render();

	void DrawTerrain();
	void DrawWater();
	void DrawAtmosphere();
	void DrawClouds();
	void DrawTreesBillboard();

	void DestroyNextBlocks();

	bool ComponentProccess();

	void CreateNewLevelOfLoD();

	int GetIDHeightMapBig() { return m_idHeightMapBig; }
	int GetIDNormalMapBig() { return m_idNormalMapBig; }
	int GetIDEnviromentMapBig() { return m_idEnviromentMapBig; }
	int GetIDTreesMapBig() { return m_idTreesMapBig; }

	/*int GetIDHeightMapSmall() { return m_idHeightMapSmall; }
	int GetIDNormalMapSmall() { return m_idNormalMapSmall; }
	int GetIDEnviromentMapSmall() { return m_idEnviromentMapSmall; }
	int GetIDTreesMapSmall() { return m_idTreesMapSmall; }*/

	int GetValueOfLODBig() { return m_valueOfLODBig; }
	void SetValueOfLODBig(int d) { m_valueOfLODBig = d; }
	int GetValueOfLODSmall() { return m_valueOfLODSmall; }
	void SetValueOfLODSmall(int d) { m_valueOfLODSmall = d; }
	int GetValueOfLODBigForNext() { return m_valueOfLODBigForNext; }
	void SetValueOfLODBigForNext(int d) { m_valueOfLODBigForNext = d; }
	int GetValueOfLODSmallForNext() { return m_valueOfLODSmallForNext; }
	void SetValueOfLODSmallForNext(int d) { m_valueOfLODSmallForNext = d; }

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

	GamePlanetTile *m_planetTile;

	bool m_isMap;

	int m_idHeightMapBig;
	int m_idNormalMapBig;
	int m_idEnviromentMapBig;
	int m_idTreesMapBig;

	//int m_idHeightMapSmall;
	//int m_idNormalMapSmall;
	//int m_idEnviromentMapSmall;
	//int m_idTreesMapSmall;

	//int m_idHeightMapSRT;
	//int m_idNormalMapSRT;
	//int m_idEnviromentMapSRT;
	//int m_idTreesMapSRT;

	int m_idHeightMapInput;
	int m_idNormalMapInput;
	int m_idAngleMapInput;
	int m_idEnviromentMapInput;
	int m_idTreesMapInput;

	int m_idHeightMapOutput;
	int m_idNormalMapOutput;
	int m_idAngleMapOutput;
	int m_idEnviromentMapOutput;
	int m_idTreesMapOutput;

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

	//Helper proccess values

	std::stringstream m_strH;
	std::stringstream m_strN;
	std::stringstream m_strE;
	std::stringstream m_strT;

	btVector3 m_MaxT;
	btVector3 m_MinT;
	btVector3 m_MaxW;
	btVector3 m_MinW;
	btVector3 m_MaxA;
	btVector3 m_MinA;
	btVector3 m_MaxC;
	btVector3 m_MinC;

	int m_idH;
	void *m_VdH;
	int m_VsH;

	int m_idN;
	void *m_VdN;
	int m_VsN;

	int m_idT;
	void *m_VdT;
	int m_VsT;

	std::vector<Vertex::Billboard> m_treesVertices;
	std::vector<int> m_treesIndices;

	int m_index;
	btVector3 m_posCounter;
	std::vector<btVector3> m_posArray;
	btVector3 m_treesOffset;

	int m_valueOfLODBig;
	int m_valueOfLODSmall;

	int m_valueOfLODBigForNext;
	int m_valueOfLODSmallForNext;

	int m_idBillboardTreesVertex;
	std::vector<int> *m_billboardTreesIndex;
	btTransform m_billBoardMatrix;
	int m_idBillboardTrees;
};