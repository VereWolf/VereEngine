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
<<<<<<< HEAD
=======
#include "GenerateHeightAndNormalMapWithNoise.h"

struct PlanetLODInitMeessage
{
	PlanetData * master;
	int side;
	int level;
	XMINT2 coord;
	btVector3 position;
	btScalar scaling;
	int levelFromLastLoadData;
	XMINT2 coordFromLastLoadData;
	btScalar scalingFromLastLoadData;
	int levelFromLoadTile;
	XMINT2 coordFromLoadTile;
	btScalar scalingFromLoadTile;
	bool isMap;
	bool isMap1;
	bool isMap2;
	XMINT2 idHeightMapBig;
	XMINT2 idNormalMapBig;
	XMINT2 idEnviromentMapBig;
	XMINT2 idTreesMapBig;
	XMINT2 idRiverWidth1MapBig;
	XMINT2 idRiverWidth2MapBig;
	XMINT2 idRiverLength1MapBig;
	XMINT2 idRiverLength2MapBig;
	XMINT2 idRiverType1MapBig;
	XMINT2 idRiverType2MapBig;
	XMINT2 idRiverHeightMapBig;
	int idBillboardTreesVertex;
	std::vector<int> *billboardTreesIndex;
	btTransform billboardMatrix;
	int idBillboardTrees;

};
>>>>>>> master

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
				if (m_idHeightMapBig.x >= 0) GameStreamingDataHandle->ReleaseBlockInFLOATDepository(m_data->GetIDHeightMapBig(), m_idHeightMapBig.x);
				if (m_idHeightMapBig.y >= 0)GameRenderDeviceHandle->DeleteTexture(m_idHeightMapBig.y);
				if (m_idNormalMapBig.x >= 0) GameStreamingDataHandle->ReleaseBlockInBYTE4Depository(m_data->GetIDNormalMapBig(), m_idNormalMapBig.x);
				if (m_idNormalMapBig.y >= 0)GameRenderDeviceHandle->DeleteTexture(m_idNormalMapBig.y);
				if (m_idRiverWidth1MapBig.y >= 0)GameRenderDeviceHandle->DeleteTexture(m_idRiverWidth1MapBig.y);
				if (m_idRiverWidth2MapBig.y >= 0)GameRenderDeviceHandle->DeleteTexture(m_idRiverWidth2MapBig.y);
				if (m_idRiverLength1MapBig.y >= 0)GameRenderDeviceHandle->DeleteTexture(m_idRiverLength1MapBig.y);
				if (m_idRiverLength2MapBig.y >= 0)GameRenderDeviceHandle->DeleteTexture(m_idRiverLength2MapBig.y);
				if (m_idRiverType1MapBig.y >= 0)GameRenderDeviceHandle->DeleteTexture(m_idRiverType1MapBig.y);
				if (m_idRiverType2MapBig.y >= 0)GameRenderDeviceHandle->DeleteTexture(m_idRiverType2MapBig.y);
				if (m_idRiverHeightMapBig.y >= 0)GameRenderDeviceHandle->DeleteTexture(m_idRiverHeightMapBig.y);
			}

			if (m_level == 0)
			{
				if (m_idEnviromentMapBig.x >= 0) GameStreamingDataHandle->ReleaseBlockInBYTE4Depository(m_data->GetIDEnviromentMapBig(), m_idEnviromentMapBig.x);
				if (m_idEnviromentMapBig.y >= 0)GameRenderDeviceHandle->DeleteTexture(m_idEnviromentMapBig.y);
				if (m_idTreesMapBig.x >= 0) GameStreamingDataHandle->ReleaseBlockInBYTE4Depository(m_data->GetIDTreesMapBig(), m_idTreesMapBig.x);
				if (m_idTreesMapBig.y >= 0)GameRenderDeviceHandle->DeleteTexture(m_idTreesMapBig.y);
			}
		}

<<<<<<< HEAD
		/*if (m_idHeightMapBig >= 0) GameStreamingDataHandle->ReleaseBlockInFLOATDepository(m_data->GetIDHeightMapSmall(), m_idHeightMapSmall);
		if (m_idNormalMapBig >= 0) GameStreamingDataHandle->ReleaseBlockInBYTE4Depository(m_data->GetIDNormalMapSmall(), m_idNormalMapSmall);
		if (m_idNormalMapBig >= 0) GameStreamingDataHandle->ReleaseBlockInBYTE4Depository(m_data->GetIDNormalMapSmall(), m_idNormalMapSmall);
		if (m_idEnviromentMapBig >= 0) GameStreamingDataHandle->ReleaseBlockInBYTE4Depository(m_data->GetIDEnviromentMapSmall(), m_idEnviromentMapSmall);
		if (m_idTreesMapBig >= 0) GameStreamingDataHandle->ReleaseBlockInBYTE4Depository(m_data->GetIDTreesMapSmall(), m_idTreesMapSmall);*/

=======
>>>>>>> master
		if (m_idHeightMapInput >= 0)	GameRenderDeviceHandle->DeleteTexture(m_idHeightMapInput);
		if (m_idNormalMapInput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idNormalMapInput);
		if (m_idAngleMapInput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idAngleMapInput);
		if (m_idEnviromentMapInput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idEnviromentMapInput);
		if (m_idTreesMapInput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idTreesMapInput);
<<<<<<< HEAD
=======
		if (m_idRiverWidth1MapInput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idRiverWidth1MapInput);
		if (m_idRiverWidth2MapInput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idRiverWidth2MapInput);
		if (m_idRiverLength1MapInput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idRiverLength1MapInput);
		if (m_idRiverLength2MapInput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idRiverLength2MapInput);
		if (m_idRiverType1MapInput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idRiverType1MapInput);
		if (m_idRiverType2MapInput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idRiverType2MapInput);
		if (m_idRiverHeightMapInput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idRiverHeightMapInput);
>>>>>>> master

		if (m_idHeightMapOutput >= 0)	GameRenderDeviceHandle->DeleteTexture(m_idHeightMapOutput);
		if (m_idNormalMapOutput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idNormalMapOutput);
		if (m_idAngleMapOutput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idAngleMapOutput);
		if (m_idEnviromentMapOutput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idEnviromentMapOutput);
		if (m_idTreesMapOutput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idTreesMapOutput);
<<<<<<< HEAD

		/*if (m_idHeightMapSRT >= 0)	GameRenderDeviceHandle->DeleteTexture(m_idHeightMapSRT);
		if (m_idNormalMapSRT >= 0) GameRenderDeviceHandle->DeleteTexture(m_idNormalMapSRT);
		if (m_idEnviromentMapSRT >= 0) GameRenderDeviceHandle->DeleteTexture(m_idEnviromentMapSRT);
		if (m_idTreesMapSRT >= 0) GameRenderDeviceHandle->DeleteTexture(m_idTreesMapSRT);*/
=======
		if (m_idRiverWidth1MapOutput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idRiverWidth1MapOutput);
		if (m_idRiverLength1MapOutput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idRiverLength1MapOutput);
		if (m_idRiverType1MapOutput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idRiverType1MapOutput);
		if (m_idRiverHeightMapOutput >= 0) GameRenderDeviceHandle->DeleteTexture(m_idRiverHeightMapOutput);
>>>>>>> master

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

<<<<<<< HEAD
	void Init(PlanetData * master, int side, int level, XMINT2 coord, btVector3 position, btScalar scaling,
		int levelFromLastLoadData, XMINT2 coordFromLastLoadData, btScalar scalingFromLastLoadData,
		int levelFromLoadTile, XMINT2 coordFromLoadTile, btScalar scalingFromLoadTile, bool isMap,
		int idHeightMapBig, int idNormalMapBig, int idEnviromentMapBig, int idTreesMapBig,
		int idBillboardTreesVertex, std::vector<int> *billboardTreesIndex, btTransform billboardMatrix, int idBillboardTrees);
=======
	void Init(PlanetLODInitMeessage message);
>>>>>>> master
	void Render();

	void DrawTerrain();
	void DrawWater();
	void DrawAtmosphere();
	void DrawClouds();
	void DrawTreesBillboard();

	void DestroyNextBlocks();

	bool ComponentProccess();

	void CreateNewLevelOfLoD();

<<<<<<< HEAD
	int GetIDHeightMapBig() { return m_idHeightMapBig; }
	int GetIDNormalMapBig() { return m_idNormalMapBig; }
	int GetIDEnviromentMapBig() { return m_idEnviromentMapBig; }
	int GetIDTreesMapBig() { return m_idTreesMapBig; }

	/*int GetIDHeightMapSmall() { return m_idHeightMapSmall; }
	int GetIDNormalMapSmall() { return m_idNormalMapSmall; }
	int GetIDEnviromentMapSmall() { return m_idEnviromentMapSmall; }
	int GetIDTreesMapSmall() { return m_idTreesMapSmall; }*/
=======
	XMINT2 GetIDHeightMapBig() { return m_idHeightMapBig; }
	XMINT2 GetIDNormalMapBig() { return m_idNormalMapBig; }
	XMINT2 GetIDEnviromentMapBig() { return m_idEnviromentMapBig; }
	XMINT2 GetIDTreesMapBig() { return m_idTreesMapBig; }
	XMINT2 GetIDRiverWidth1MapBig() { return m_idRiverWidth1MapBig; }
	XMINT2 GetIDRiverWidth2MapBig() { return m_idRiverWidth2MapBig; }
	XMINT2 GetIDRiverLength1MapBig() { return m_idRiverLength1MapBig; }
	XMINT2 GetIDRiverLength2MapBig() { return m_idRiverLength2MapBig; }
	XMINT2 GetIDRiverType1MapBig() { return m_idRiverType1MapBig; }
	XMINT2 GetIDRiverType2MapBig() { return m_idRiverType2MapBig; }
	XMINT2 GetIDRiverHeightMapBig() { return m_idRiverHeightMapBig; }
>>>>>>> master

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
	int m_modelID;

	GamePlanetTile *m_planetTile;

	bool m_isMap;
<<<<<<< HEAD

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
=======
	bool m_isMap1;
	bool m_isMap2;

	XMINT2 m_idHeightMapBig;
	XMINT2 m_idNormalMapBig;
	XMINT2 m_idEnviromentMapBig;
	XMINT2 m_idTreesMapBig;
	XMINT2 m_idRiverWidth1MapBig;
	XMINT2 m_idRiverWidth2MapBig;
	XMINT2 m_idRiverLength1MapBig;
	XMINT2 m_idRiverLength2MapBig;
	XMINT2 m_idRiverType1MapBig;
	XMINT2 m_idRiverType2MapBig;
	XMINT2 m_idRiverHeightMapBig;
>>>>>>> master

	int m_idHeightMapInput;
	int m_idNormalMapInput;
	int m_idAngleMapInput;
	int m_idEnviromentMapInput;
	int m_idTreesMapInput;
<<<<<<< HEAD
=======
	int m_idRiverWidth1MapInput;
	int m_idRiverWidth2MapInput;
	int m_idRiverLength1MapInput;
	int m_idRiverLength2MapInput;
	int m_idRiverType1MapInput;
	int m_idRiverType2MapInput;
	int m_idRiverHeightMapInput;
>>>>>>> master

	int m_idHeightMapOutput;
	int m_idNormalMapOutput;
	int m_idAngleMapOutput;
	int m_idEnviromentMapOutput;
	int m_idTreesMapOutput;
<<<<<<< HEAD
=======
	int m_idRiverWidth1MapOutput;
	int m_idRiverWidth2MapOutput;
	int m_idRiverLength1MapOutput;
	int m_idRiverLength2MapOutput;
	int m_idRiverType1MapOutput;
	int m_idRiverType2MapOutput;
	int m_idRiverHeightMapOutput;
>>>>>>> master

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
	std::stringstream m_strRW1;
	std::stringstream m_strRW2;
	std::stringstream m_strRL1;
	std::stringstream m_strRL2;
	std::stringstream m_strRT1;
	std::stringstream m_strRT2;
	std::stringstream m_strRH;

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
<<<<<<< HEAD
=======

	btVector3 m_dir[4];
>>>>>>> master
};