#include "pch.h"
#include "PlanetLOD.h"
#include "PlanetData.h"
#include "GameComponent.h"
#include "RenderDevice.h"

bool PlanetLOD::m_onlyRenderText = false;

<<<<<<< HEAD
void PlanetLOD::Init(PlanetData * data, int side, int level, XMINT2 coord, btVector3 position, btScalar scaling, VereTextureFloat *hMap = NULL, VereTextureBYTE4 *nMap = NULL)
=======
PlanetLOD::PlanetLOD()
{
	m_idHeightMapSRT = -1;
	m_idNormalMapSRT = -1;
	m_idEnviromentMapSRT = -1;
	m_idTreesMapSRT = -1;

	m_idHeightMapBig = -1;
	m_idNormalMapBig = -1;
	m_idEnviromentMapBig = -1;
	m_idTreesMapBig = -1;

	m_idHeightMapSmall = -1;
	m_idNormalMapSmall = -1;
	m_idEnviromentMapSmall = -1;
	m_idTreesMapSmall = -1;

	m_isCreateNewLevelInProcess = 0;
	m_isNewLevel = false;
	m_isMap = false;
	m_blocks[0] = NULL;
	m_blocks[1] = NULL;
	m_blocks[2] = NULL;
	m_blocks[3] = NULL;

	m_valueOfLODBig = 0;
	m_valueOfLODSmall = 0;
	m_valueOfLODBigForNext = 0;
	m_valueOfLODSmallForNext = 0;
}

PlanetLOD::PlanetLOD(const PlanetLOD & n)
{
	m_idHeightMapSRT = -1;
	m_idNormalMapSRT = -1;
	m_idEnviromentMapSRT = -1;
	m_idTreesMapSRT = -1;

	m_idHeightMapBig = -1;
	m_idNormalMapBig = -1;
	m_idEnviromentMapBig = -1;
	m_idTreesMapBig = -1;

	m_idHeightMapSmall = -1;
	m_idNormalMapSmall = -1;
	m_idEnviromentMapSmall = -1;
	m_idTreesMapSmall = -1;

	m_isCreateNewLevelInProcess = 0;
	m_isNewLevel = false;
	m_isMap = false;
	m_blocks[0] = NULL;
	m_blocks[1] = NULL;
	m_blocks[2] = NULL;
	m_blocks[3] = NULL;

	m_valueOfLODBig = 0;
	m_valueOfLODSmall = 0;
	m_valueOfLODBigForNext = 0;
	m_valueOfLODSmallForNext = 0;
}

void PlanetLOD::Init(PlanetData * data, int side, int level, XMINT2 coord, btVector3 position, btScalar scaling,
	int levelFromLastLoadData, XMINT2 coordFromLastLoadData, btScalar scalingFromLastLoadData,
	int levelFromLoadTile, XMINT2 coordFromLoadTile, btScalar scalingFromLoadTile, bool isMap,
	int idHeightMapBig, int idNormalMapBig, int idEnviromentMapBig, int idTreesMapBig)
>>>>>>> VereEngine-Planet
{
	m_data = data;
	m_side = side;
	m_level = level;
	m_coord = coord;
	m_scaling = scaling;
	m_position = position;
	m_isMap = isMap;
	m_levelFromLastLoadData = levelFromLastLoadData;
	m_coordFromLastLoadData = coordFromLastLoadData;
	m_scalingFromLastLoadData = scalingFromLastLoadData;
	m_levelFromLoadTile = levelFromLoadTile;
	m_coordFromLoadTile = coordFromLoadTile;
	m_scalingFromLoadTile = scalingFromLoadTile;
	btScalar S = m_data->GetScaling().getRow(0).getX();
	btScalar S2 = S * scaling;
	int PIR = m_data->GetNumPointInRowInCell();
	int size = PIR * PIR;
	float PIRR = 1.0f / ((float)PIR + 2.0f);

	SetId(m_data->m_planetElementID->TakeElement());

	if (GetId() < 0)
	{
		return;
	}

<<<<<<< HEAD
	while (GetId() > m_data->m_planetElements.size() && GetId() != -1) m_data->m_planetElements.resize(m_data->m_planetElements.size() + 1024);
=======
>>>>>>> VereEngine-Planet
	m_data->m_planetElements[GetId()] = this;

	if (!((level % m_data->GetLoadDataPer()) == 0 && m_level < m_data->GetLoadDataMaxLvl()))
	{
		m_idHeightMapBig = idHeightMapBig;
		m_idNormalMapBig = idNormalMapBig;
		m_idTreesMapBig = idTreesMapBig;
	}

<<<<<<< HEAD
	m_heightMap = NULL;
	m_normalMap = NULL;
=======
	if (level != 0)
	{
		m_idEnviromentMapBig = idEnviromentMapBig;
	}
>>>>>>> VereEngine-Planet

	if (GameRenderDeviceHandle->GetModel(data->GetRenderId()))
	{
		Model *model = new Model;
		memcpy(model, GameRenderDeviceHandle->GetModel(data->GetRenderId()), sizeof(Model));
		m_modelID = GameRenderDeviceHandle->CreateModel(model);
	}
}

void PlanetLOD::Render()
{
	if (GetId() < 0)
	{
		return;
	}

	btMatrix3x3 I = btMatrix3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);
	btScalar S = m_scaling;
	btScalar S2 = m_data->GetScaling().getRow(0).getX();
	btScalar S3 = S * S2;

	btVector3 dir = -(m_data->GetWorldPosition() - GameObjectStackHandle->GetMainCamera()->GetWorldPosition());
	dir = m_data->GetWorldTransform().getBasis().inverse() * dir;

	btTransform AM = btTransform(m_data->GetBlockAnglMatrix(m_side), btVector3(0.0, 0.0, 0.0));
	btTransform GT = m_data->GetWorldTransform();
	{
		btTransform M = RenderMessage::m_CameraOffset * GT;

		btVector3 coord = (PlanetCordinateMat::GetCoordForCube(dir,
			btTransform(m_data->GetBlockAnglMatrix(m_side))) + btVector3(0.5, 0.5, 0.0)) / S;

		btScalar distance = -1.0f;

		if (coord.getZ() == 0.0)
		{
			distance = btVector3(coord.getX() - m_coord.x, coord.getY() - m_coord.y, 0.0).length();
		}

<<<<<<< HEAD
		if (m_isCreateNewLevelInProcess == true)
		{
			if (m_isNewLevel == false && m_blocks[0]->GetProccessed() == 2 && m_blocks[1]->GetProccessed() == 2
				&& m_blocks[2]->GetProccessed() == 2 && m_blocks[3]->GetProccessed() == 2)
			{
				m_isCreateNewLevelInProcess = false;
				m_isNewLevel = true;
			}
		}

		if (m_isNewLevel == false && distance >= 0.0 && distance < 1.5 && m_level < m_data->GetMaxLevel() && m_isCreateNewLevelInProcess == false)
		{
			m_data->m_planetElementsInProcess.GiveElement(GetId());
			SetProccessed(1);
		}
		else if (m_isNewLevel == true && (distance > 1.55 || distance == -1))
		{
			m_blocks[0]->Destroy();
			m_blocks[0] = NULL;
			m_blocks[1]->Destroy();
			m_blocks[1] = NULL;
			m_blocks[2]->Destroy();
			m_blocks[2] = NULL;
			m_blocks[3]->Destroy();
			m_blocks[3] = NULL;

			m_isNewLevel = false;
		}
=======
		if (m_isCreateNewLevelInProcess == 2)
		{
			if (m_isNewLevel == false && m_blocks[0] != NULL && m_blocks[1] != NULL && m_blocks[2] != NULL && m_blocks[3] != NULL)
			{
				if (m_blocks[0]->GetProccessed() == 2 && m_blocks[1]->GetProccessed() == 2
					&& m_blocks[2]->GetProccessed() == 2 && m_blocks[3]->GetProccessed() == 2)
				{
					m_isCreateNewLevelInProcess = 3;
					m_isNewLevel = true;
				}
			}
		}

		bool CE = true;
		bool CEB = false;

		if (!((m_data->GetMaxNumBlockSmall() - 4) > m_data->GetCurerentNumBlockSmall())) CE = false;

		if (((m_level + 1) % m_data->GetLoadDataPer()) == 0 && (m_level + 1) < m_data->GetLoadDataMaxLvl())
		{
			CEB = true;
			if (!((m_data->GetMaxNumBlockBig() - 4) > m_data->GetCurerentNumBlockBig())) CE = false;
		}

		if (CE == true && m_isNewLevel == false && distance >= 0.0 && distance < 2.0 && m_level < m_data->GetCurrentMaxLevel() && m_isCreateNewLevelInProcess == 0)
		{
			m_data->m_planetElementsInProcess.GiveElement(GetId());
			m_isCreateNewLevelInProcess = 1;
			SetValueOfLODSmallForNext(4);
			if (CEB) SetValueOfLODBigForNext(4);
			m_data->SetCurrentNumBlockSmall(m_data->GetCurerentNumBlockSmall() + GetValueOfLODSmallForNext());
			m_data->SetCurrentNumBlockBig(m_data->GetCurerentNumBlockBig() + GetValueOfLODBigForNext());
		}
		else if (m_isNewLevel == true && (distance > 2.05 || distance == -1))
		{
			DestroyNextBlocks();

			m_isCreateNewLevelInProcess = 0;
			m_isNewLevel = false;
		}
>>>>>>> VereEngine-Planet
	}

	if (m_isNewLevel == true)
	{
		m_blocks[0]->Render();
		m_blocks[1]->Render();
		m_blocks[2]->Render();
		m_blocks[3]->Render();
	}

	if (m_isNewLevel == false)
	{
		XMFLOAT4X4 VW;
		XMStoreFloat4x4(&VW, RenderMessage::m_View);
		btTransform VW2 = VereMath::ConvertTobtTransform(VW);
		btTransform meshCentre = RenderMessage::m_CameraOffset * GT;
		btTransform Mat = VW2 * meshCentre/* * Mat*/;

		btVector3 Vecd = Mat * m_CentreT;

		if (VereMath::FrustrumCulling2(m_CentreT, m_OffsetCubeT, RenderMessage::m_FarZ, RenderMessage::m_HeightFar, RenderMessage::m_Aspect, Mat))
		{
			DrawTerrain();
		}

		if (VereMath::FrustrumCulling2(m_CentreT, m_OffsetCubeT, RenderMessage::m_FarZ, RenderMessage::m_HeightFar, RenderMessage::m_Aspect, Mat))
		{
			DrawWater();
		}

		if (VereMath::FrustrumCulling2(m_CentreT, m_OffsetCubeT, RenderMessage::m_FarZ, RenderMessage::m_HeightFar, RenderMessage::m_Aspect, Mat))
		{
			DrawAtmosphere();
		}

		if (VereMath::FrustrumCulling2(m_CentreT, m_OffsetCubeT, RenderMessage::m_FarZ, RenderMessage::m_HeightFar, RenderMessage::m_Aspect, Mat))
		{
			DrawClouds();
		}

		m_data->GetDeviceResources()->GetD3DDeviceContext()->HSSetShader(0, 0, 0);
		m_data->GetDeviceResources()->GetD3DDeviceContext()->DSSetShader(0, 0, 0);
	}
}
<<<<<<< HEAD

void PlanetLOD::DrawTerrain()
{
	btMatrix3x3 I = btMatrix3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);
	btScalar S = m_scaling;
	btScalar S2 = m_data->GetScaling().getRow(0).getX();
	btScalar S3 = S * S2;

	btTransform AM = btTransform(m_data->GetBlockAnglMatrix(m_side), btVector3(0.0, 0.0, 0.0));
	btTransform GT = m_data->GetWorldTransform();


=======

void PlanetLOD::DrawTerrain()
{
	btMatrix3x3 I = btMatrix3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);
	btScalar S = m_scaling;
	btScalar S2 = m_data->GetScaling().getRow(0).getX();
	btScalar S3 = S * S2;

	btTransform AM = btTransform(m_data->GetBlockAnglMatrix(m_side), btVector3(0.0, 0.0, 0.0));
	btTransform GT = m_data->GetWorldTransform();


>>>>>>> VereEngine-Planet
	TerrainRenderMessage message;

	message.m_ModelID = m_data->GetRenderId();
	message.m_PlanetData = m_data;
	message.m_Scaling = btTransform(btMatrix3x3(S2, 0.0, 0.0, 0.0, S2, 0.0, 0.0, 0.0, S2), btVector3(0.0, 0.0, 0.0));
	message.m_Transform = m_data->GetWorldTransform();
	message.m_Position = m_position;
	message.m_Coord = m_coord;
	message.m_AngleMatrix = AM;
	message.m_Coord = m_coord;
	message.m_Spacing1 = S;
	message.m_Spacing2 = S2;
	message.m_Level = m_level;
	message.m_Tangent = m_data->GetTangent();
	
	message.m_InverseSide = m_data->GetBlockAnglMatrix(m_side).inverse();
	message.m_Side = m_side;

<<<<<<< HEAD
	if (m_idHeightMap >= 0)
	{
		message.m_HeightSRV = GameRenderDeviceHandle->GetTexture(m_idHeightMap);
	}
	else
	{
		return;
	}
	if (m_idNormalMap >= 0)
	{
		message.m_NormalSRV = GameRenderDeviceHandle->GetTexture(m_idNormalMap);
	}
	else
	{
		return;
	}
=======
	message.m_IsMap = m_isMap;

	if (m_isMap)
	{
		if (m_idHeightMapSmall >= 0)
		{
			message.m_HeightSRV = GameRenderDeviceHandle->GetTexture(m_idHeightMapSRT);
		}
		else
		{
			return;
		}
		if (m_idNormalMapSmall >= 0)
		{
			message.m_NormalSRV = GameRenderDeviceHandle->GetTexture(m_idNormalMapSRT);
		}
		else
		{
			return;
		}
		if (m_idEnviromentMapSmall >= 0)
		{
			message.m_EnviromentSRV = GameRenderDeviceHandle->GetTexture(m_idEnviromentMapSRT);
		}
		else
		{
			return;
		}
		if (m_idTreesMapSmall >= 0)
		{
			message.m_TreesSRV = GameRenderDeviceHandle->GetTexture(m_idTreesMapSRT);
		}
		else
		{
			return;
		}
	}

>>>>>>> VereEngine-Planet
	message.m_RasterizeState = RenderStates::SolidRS;
	message.m_BlendState = RenderStates::NoBlendBS;

	GameRenderDeviceHandle->Render(&message);
}

void PlanetLOD::DrawWater()
{
	btMatrix3x3 I = btMatrix3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);
	btScalar S = m_scaling;
	btScalar S2 = m_data->GetScaling().getRow(0).getX();
	btScalar S3 = S * S2;

	btTransform AM = btTransform(m_data->GetBlockAnglMatrix(m_side), btVector3(0.0, 0.0, 0.0));
	btTransform GT = m_data->GetWorldTransform();


	WaterRenderMessage message;

	message.m_ModelID = m_data->GetRenderIdWater();
	message.m_PlanetData = m_data;
	message.m_Scaling = btTransform(btMatrix3x3(S2, 0.0, 0.0, 0.0, S2, 0.0, 0.0, 0.0, S2), btVector3(0.0, 0.0, 0.0));
	message.m_Transform = m_data->GetWorldTransform();
	message.m_Position = m_position;
	message.m_AngleMatrix = AM;
	message.m_Coord = m_coord;
	message.m_Spacing1 = S;
	message.m_Spacing2 = S2;
	message.m_Level = m_level;
	message.m_RasterizeState = RenderStates::SolidRS;
	message.m_BlendState = RenderStates::NoBlendBS;

	GameRenderDeviceHandle->BindRenderTarget(m_data->GetWaterTopTargetMapRTV(), m_data->GetWaterTopDeepMapDSV());
	GameRenderDeviceHandle->Render(&message);

	message.m_RasterizeState = RenderStates::BackRS;

	GameRenderDeviceHandle->BindRenderTarget(m_data->GetWaterBottomTargetMapRTV(), m_data->GetWaterBottomDeepMapDSV());
	GameRenderDeviceHandle->Render(&message);

	GameRenderDeviceHandle->BindMainRenderTarget();
<<<<<<< HEAD
}

void PlanetLOD::DrawAtmosphere()
{
	btMatrix3x3 I = btMatrix3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);
	btScalar S = m_scaling;
	btScalar S2 = m_data->GetScaling().getRow(0).getX();
	btScalar S3 = S * S2;

	btTransform AM = btTransform(m_data->GetBlockAnglMatrix(m_side), btVector3(0.0, 0.0, 0.0));
	btTransform GT = m_data->GetWorldTransform();


	AtmosphereRenderMessage message;

	message.m_ModelID = m_data->GetRenderIdAtmosphere();
	message.m_PlanetData = m_data;
	message.m_Scaling = btTransform(btMatrix3x3(S2, 0.0, 0.0, 0.0, S2, 0.0, 0.0, 0.0, S2), btVector3(0.0, 0.0, 0.0));
	message.m_Transform = m_data->GetWorldTransform();
	message.m_Position = m_position;
	message.m_AngleMatrix = AM;
	message.m_Coord = m_coord;
	message.m_Spacing1 = S;
	message.m_Spacing2 = S2;
	message.m_Level = m_level;
	message.m_RasterizeState = RenderStates::BackRS;
	message.m_BlendState = RenderStates::NoBlendBS;

	GameRenderDeviceHandle->BindRenderTarget(m_data->GetAtmosphereTargetMapRTV(), m_data->GetAtmosphereDeepMapDSV());
	GameRenderDeviceHandle->Render(&message);
	GameRenderDeviceHandle->BindMainRenderTarget();
}

void PlanetLOD::DrawClouds()
{
	btMatrix3x3 I = btMatrix3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);
	btScalar S = m_scaling;
	btScalar S2 = m_data->GetScaling().getRow(0).getX();
	btScalar S3 = S * S2;

	btTransform AM = btTransform(m_data->GetBlockAnglMatrix(m_side), btVector3(0.0, 0.0, 0.0));
	btTransform GT = m_data->GetWorldTransform();


	CloudsRenderMessage message;

	message.m_ModelID = m_data->GetRenderIdClouds();
	message.m_PlanetData = m_data;
	message.m_Scaling = btTransform(btMatrix3x3(S2, 0.0, 0.0, 0.0, S2, 0.0, 0.0, 0.0, S2), btVector3(0.0, 0.0, 0.0));
	message.m_Transform = m_data->GetWorldTransform();
	message.m_Position = m_position;
	message.m_AngleMatrix = AM;
	message.m_Coord = m_coord;
	message.m_Spacing1 = S;
	message.m_Spacing2 = S2;
	message.m_Level = m_level;
	message.m_RasterizeState = RenderStates::NoCullRS;
	message.m_BlendState = RenderStates::NoBlendBS;

	GameRenderDeviceHandle->BindRenderTarget(m_data->GetCloudsTargetMapRTV(), m_data->GetCloudsDeepMapDSV());
	GameRenderDeviceHandle->Render(&message);
	GameRenderDeviceHandle->BindMainRenderTarget();
}

bool PlanetLOD::ComponentProccess()
{
=======
}

void PlanetLOD::DrawAtmosphere()
{
	btMatrix3x3 I = btMatrix3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);
	btScalar S = m_scaling;
	btScalar S2 = m_data->GetScaling().getRow(0).getX();
	btScalar S3 = S * S2;

	btTransform AM = btTransform(m_data->GetBlockAnglMatrix(m_side), btVector3(0.0, 0.0, 0.0));
	btTransform GT = m_data->GetWorldTransform();


	AtmosphereRenderMessage message;

	message.m_ModelID = m_data->GetRenderIdAtmosphere();
	message.m_PlanetData = m_data;
	message.m_Scaling = btTransform(btMatrix3x3(S2, 0.0, 0.0, 0.0, S2, 0.0, 0.0, 0.0, S2), btVector3(0.0, 0.0, 0.0));
	message.m_Transform = m_data->GetWorldTransform();
	message.m_Position = m_position;
	message.m_AngleMatrix = AM;
	message.m_Coord = m_coord;
	message.m_Spacing1 = S;
	message.m_Spacing2 = S2;
	message.m_Level = m_level;
	message.m_RasterizeState = RenderStates::BackRS;
	message.m_BlendState = RenderStates::NoBlendBS;

	GameRenderDeviceHandle->BindRenderTarget(m_data->GetAtmosphereTargetMapRTV(), m_data->GetAtmosphereDeepMapDSV());
	GameRenderDeviceHandle->Render(&message);
	GameRenderDeviceHandle->BindMainRenderTarget();
}

void PlanetLOD::DrawClouds()
{
	btMatrix3x3 I = btMatrix3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);
	btScalar S = m_scaling;
	btScalar S2 = m_data->GetScaling().getRow(0).getX();
	btScalar S3 = S * S2;

	btTransform AM = btTransform(m_data->GetBlockAnglMatrix(m_side), btVector3(0.0, 0.0, 0.0));
	btTransform GT = m_data->GetWorldTransform();


	CloudsRenderMessage message;

	message.m_ModelID = m_data->GetRenderIdClouds();
	message.m_PlanetData = m_data;
	message.m_Scaling = btTransform(btMatrix3x3(S2, 0.0, 0.0, 0.0, S2, 0.0, 0.0, 0.0, S2), btVector3(0.0, 0.0, 0.0));
	message.m_Transform = m_data->GetWorldTransform();
	message.m_Position = m_position;
	message.m_AngleMatrix = AM;
	message.m_Coord = m_coord;
	message.m_Spacing1 = S;
	message.m_Spacing2 = S2;
	message.m_Level = m_level;
	message.m_RasterizeState = RenderStates::NoCullRS;
	message.m_BlendState = RenderStates::NoBlendBS;

	GameRenderDeviceHandle->BindRenderTarget(m_data->GetCloudsTargetMapRTV(), m_data->GetCloudsDeepMapDSV());
	GameRenderDeviceHandle->Render(&message);
	GameRenderDeviceHandle->BindMainRenderTarget();
}

void PlanetLOD::DestroyNextBlocks()
{
	if (m_blocks[0] != NULL && m_blocks[0]->IsDestroy() == false)
	{
		m_blocks[0]->DestroyNextBlocks();
		m_blocks[0]->Destroy();
		m_blocks[0] = NULL;
	}

	if (m_blocks[1] != NULL && m_blocks[1]->IsDestroy() == false)
	{
		m_blocks[1]->DestroyNextBlocks();
		m_blocks[1]->Destroy();
		m_blocks[1] = NULL;
	}

	if (m_blocks[2] != NULL && m_blocks[2]->IsDestroy() == false)
	{
		m_blocks[2]->DestroyNextBlocks();
		m_blocks[2]->Destroy();
		m_blocks[2] = NULL;
	}

	if (m_blocks[3] != NULL && m_blocks[3]->IsDestroy() == false)
	{
		m_blocks[3]->DestroyNextBlocks();
		m_blocks[3]->Destroy();
		m_blocks[3] = NULL;
	}
}

bool PlanetLOD::ComponentProccess()
{
	m_progress.w = 0.0f;

>>>>>>> VereEngine-Planet
	PlanetData *data = m_data;
	int side = m_side;
	int level = m_level;
	XMINT2 coord = m_coord;
	btScalar scaling = m_scaling;
	btVector3 position = m_position;
	btScalar S = m_data->GetScaling().getRow(0).getX();
	btScalar S2 = S * scaling;
	int PIR = m_data->GetNumPointInRowInCell();
	int size = PIR * PIR;
	float PIRR = 1.0f / ((float)PIR + 1.0f);

<<<<<<< HEAD
	float NMIN = std::numeric_limits<btScalar>::lowest();
	float NMAX = -1 * NMIN;
	btVector3 MaxT = btVector3(NMIN, NMIN, NMIN);
	btVector3 MinT = btVector3(NMAX, NMAX, NMAX);
	btVector3 MaxW = btVector3(NMIN, NMIN, NMIN);
	btVector3 MinW = btVector3(NMAX, NMAX, NMAX);
	btVector3 MaxA = btVector3(NMIN, NMIN, NMIN);
	btVector3 MinA = btVector3(NMAX, NMAX, NMAX);
	btVector3 MaxC = btVector3(NMIN, NMIN, NMIN);
	btVector3 MinC = btVector3(NMAX, NMAX, NMAX);
=======
	int L = 512 + 2 * 512 / m_data->GetNumPointInRowInCell();
	int MS = pow(L, 2);
>>>>>>> VereEngine-Planet

	if (m_progress.z == 0.0f)
	{
		float NMIN = std::numeric_limits<btScalar>::lowest();
		float NMAX = -1 * NMIN;
		m_MaxT = btVector3(NMIN, NMIN, NMIN);
		m_MinT = btVector3(NMAX, NMAX, NMAX);
		m_MaxW = btVector3(NMIN, NMIN, NMIN);
		m_MinW = btVector3(NMAX, NMAX, NMAX);
		m_MaxA = btVector3(NMIN, NMIN, NMIN);
		m_MinA = btVector3(NMAX, NMAX, NMAX);
		m_MaxC = btVector3(NMIN, NMIN, NMIN);
		m_MinC = btVector3(NMAX, NMAX, NMAX);

		if ((level % m_data->GetLoadDataPer()) == 0 && m_level < m_data->GetLoadDataMaxLvl())
		{
			m_levelFromLastLoadData = 0;
			m_coordFromLastLoadData = XMINT2(0, 0);

			m_idHeightMapBig = GameStreamingDataHandle->CreateNewBlockInFLOATDepository(m_data->GetIDHeightMapBig(), NULL);
			m_idNormalMapBig = GameStreamingDataHandle->CreateNewBlockInBYTE4Depository(m_data->GetIDNormalMapBig(), NULL);
			if (level == 0)m_idEnviromentMapBig = GameStreamingDataHandle->CreateNewBlockInBYTE4Depository(m_data->GetIDEnviromentMapBig(), NULL);
			m_idTreesMapBig = GameStreamingDataHandle->CreateNewBlockInBYTE4Depository(m_data->GetIDTreesMapBig(), NULL);

<<<<<<< HEAD
		int L = 512 + 2 * 512 / m_data->GetNumPointInRowInCell();
		int MS = pow(L, 2);
=======
			if (level == 0)
			{
				m_strH << m_data->GetFullPath() << "height_" << m_data->GetPosFix(side) << ".raw";
				m_strN << m_data->GetFullPath() << "normal_" << m_data->GetPosFix(side) << ".raw";
				m_strE << m_data->GetFullPath() << "enviroment_" << m_data->GetPosFix(side) << ".raw";
			}
			else
			{
				m_strH << m_data->GetFullPath() << "height_" << m_data->GetPosFix(side) << "_" << level << "_" << coord.x << "_" << coord.y << ".raw";
				m_strN << m_data->GetFullPath() << "normal_" << m_data->GetPosFix(side) << "_" << level << "_" << coord.x << "_" << coord.y << ".raw";
			}
>>>>>>> VereEngine-Planet

			m_idH = GameStreamingDataHandle->LoadData(m_strH.str());

			if (m_idH < 0)
			{
				m_isMap = false;
			}
			else
			{
				m_isMap = true;
			}

			if (m_isMap == true)
			{
				m_VdH = GameStreamingDataHandle->GetStreamingData(m_idH);
				m_VsH = GameStreamingDataHandle->GetSizeOfStreamingData(m_idH);

				m_idN = GameStreamingDataHandle->LoadData(m_strN.str());
				m_VdN = GameStreamingDataHandle->GetStreamingData(m_idN);
				m_VsN = GameStreamingDataHandle->GetSizeOfStreamingData(m_idN);

				VBYTE4 *tMap = new VBYTE4[MS];
				m_idT = GameStreamingDataHandle->CreateStreamingData(&tMap[0], MS);
				m_VdT = GameStreamingDataHandle->GetStreamingData(m_idT);
				m_VsT = GameStreamingDataHandle->GetSizeOfStreamingData(m_idT);
			}
		}

		if (m_isMap == true)
		{
			m_progress.z = 1.0f;
		}
		else
		{
			m_progress.z = 3.0f;
		}
	}

	if (m_progress.z == 1.0f)
	{
		if ((level % m_data->GetLoadDataPer()) == 0 && m_level < m_data->GetLoadDataMaxLvl())
		{
			GameStreamingDataHandle->SetTextureFromFLOATDepository(m_data->GetIDHeightMapBig(), GetIDHeightMapBig(), (float*)m_VdH);
			GameStreamingDataHandle->DeleteStreamingData(m_idH);

			GameStreamingDataHandle->SetTextureFromBYTE4Depository(m_data->GetIDNormalMapBig(), GetIDNormalMapBig(), (VBYTE4*)m_VdN);
			GameStreamingDataHandle->DeleteStreamingData(m_idN);

			GameStreamingDataHandle->SetTextureFromBYTE4Depository(m_data->GetIDTreesMapBig(), GetIDTreesMapBig(), (VBYTE4*)m_VdT);
			GameStreamingDataHandle->DeleteStreamingData(m_idT);

			if (level == 0)
			{
				int idE = GameStreamingDataHandle->LoadData(m_strE.str());
				void *VdE = GameStreamingDataHandle->GetStreamingData(idE);
				int VsE = GameStreamingDataHandle->GetSizeOfStreamingData(idE);


				GameStreamingDataHandle->SetTextureFromBYTE4Depository(m_data->GetIDEnviromentMapBig(), GetIDEnviromentMapBig(), (VBYTE4*)VdE);

				GameStreamingDataHandle->DeleteStreamingData(idE);
			}
		}

		if (m_data->GetLoadTilesLvl() <= m_level)
		{
			m_planetTile = GamePlanetHelperHandle->GetPlanetTile(0)->CreteNewGamePlanetTileFrom(
				m_levelFromLoadTile, XMFLOAT2(m_coordFromLoadTile.x, m_coordFromLoadTile.y), 0.0f, 64, 64);
		}

		m_progress.z = 2.0f;
	}

	if (m_progress.z == 2.0f)
	{
		m_idHeightMapSmall = GameStreamingDataHandle->CreateNewBlockInFLOATDepository(m_data->GetIDHeightMapSmall(),
			GameStreamingDataHandle->CreateNewMapFromFLOATDepository(m_data->GetIDHeightMapBig(), GetIDHeightMapBig(),
				m_levelFromLastLoadData, XMFLOAT2(m_coordFromLastLoadData.x, m_coordFromLastLoadData.y), 2.0f, PIR, PIR));

		m_idNormalMapSmall = GameStreamingDataHandle->CreateNewBlockInBYTE4Depository(m_data->GetIDNormalMapSmall(),
			GameStreamingDataHandle->CreateNewMapFromBYTE4Depository(m_data->GetIDNormalMapBig(), GetIDNormalMapBig(),
				m_levelFromLastLoadData, XMFLOAT2(m_coordFromLastLoadData.x, m_coordFromLastLoadData.y), 2.0f, PIR, PIR));

		m_idEnviromentMapSmall = GameStreamingDataHandle->CreateNewBlockInBYTE4Depository(m_data->GetIDEnviromentMapSmall(),
			GameStreamingDataHandle->CreateNewMapFromBYTE4Depository(m_data->GetIDEnviromentMapBig(), GetIDEnviromentMapBig(),
				m_level, XMFLOAT2(m_coord.x, m_coord.y), 2.0f, PIR, PIR));

		m_idTreesMapSmall = GameStreamingDataHandle->CreateNewBlockInBYTE4Depository(m_data->GetIDTreesMapSmall(),
			GameStreamingDataHandle->CreateNewMapFromBYTE4Depository(m_data->GetIDTreesMapBig(), GetIDTreesMapBig(),
				m_levelFromLastLoadData, XMFLOAT2(m_coordFromLastLoadData.x, m_coordFromLastLoadData.y), 2.0f, PIR, PIR));

		m_progress.z = 3.0f;
	}

	if (m_progress.z == 3.0f)
	{
		m_progress.w = 0.0f;

		btTransform mesh = btTransform(btMatrix3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0), btVector3(0.0, 0.5, 0.0));
		mesh = btTransform(m_data->GetBlockAnglMatrix(side), btVector3(0.0, 0.0, 0.0)) * mesh;

		if (m_isMap == true)
		{
<<<<<<< HEAD
			btScalar D = 1.0 / (PIR - 1);
			btVector3 V;
			V.setX(((btScalar)coord.x + D * x) * scaling - 0.5);
			V.setY(0.0);
			V.setZ(((btScalar)coord.y + D * y) * scaling - 0.5);

			btVector3 VT, VW, VA, VC;
			VT = V;
			VT = ((mesh * V).normalize()) * (S + m_heightMap->GetVariable((x + 1) * PIRR, (y + 1) * PIRR));
			VW = V;
			VW = ((mesh * V).normalize()) * (S + m_heightMap->GetVariable((x + 1) * PIRR, (y + 1) * PIRR));
			VA = V;
			VA= ((mesh * V).normalize()) * (S + m_heightMap->GetVariable((x + 1) * PIRR, (y + 1) * PIRR));
			VC = V;
			VC = ((mesh * V).normalize()) * (S + m_heightMap->GetVariable((x + 1) * PIRR, (y + 1) * PIRR));

			MaxT = VereMath::MaxVector(MaxT, VT);
			MinT = VereMath::MinVector(MinT, VT);

			MaxW = VereMath::MaxVector(MaxW, VW);
			MinW = VereMath::MinVector(MinW, VW);

			MaxA = VereMath::MaxVector(MaxA, VA);
			MinA = VereMath::MinVector(MinA, VA);

			MaxC = VereMath::MaxVector(MaxC, VC);
			MinC = VereMath::MinVector(MinC, VC);
=======
			for (; m_progress.y < (float)PIR + 2.0f; m_progress.y += 1)
			{
				for (; m_progress.x < (float)PIR + 2.0f; m_progress.x += 1)
				{
					if (m_progress.w > 512.0f) return false;

					VBYTE4 B4 = GameStreamingDataHandle->GetValueFromBYTE4Depository(
						m_data->GetIDNormalMapSmall(), GetIDNormalMapSmall(), m_progress.x * PIRR, m_progress.y * PIRR);
					XMVECTOR N = XMVector3Normalize(XMLoadFloat3(&XMFLOAT3((float)B4.x - 127.5f, (float)B4.y - 127.5f, (float)B4.z - 127.5f)));
					XMVECTOR T = XMVector3Cross(XMLoadFloat3(&XMFLOAT3(1.0f, 0.0f, 0.0f)), N);
					XMVECTOR B = XMVector3Cross(N, XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 1.0f)));
					XMVECTOR Z = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f));
					XMVECTOR UP = XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f));

					XMVECTOR dotV = XMVector3Dot(UP, N);
					float dotF;

					XMStoreFloat(&dotF, dotV);

					float rad = 180.0f * acos(dotF) / PI;

					if (rad < 25.0f)
					{
						GameStreamingDataHandle->GetTextureFromBYTE4Depository(
							m_data->GetIDEnviromentMapSmall(), GetIDEnviromentMapSmall())[(int)m_progress.y * (PIR + 2) + (int)m_progress.x].z = 0;
						GameStreamingDataHandle->GetTextureFromBYTE4Depository(
							m_data->GetIDEnviromentMapSmall(), GetIDEnviromentMapSmall())[(int)m_progress.y * (PIR + 2) + (int)m_progress.x].w = 255;
					}
					else if (rad < 35.0f)
					{
						GameStreamingDataHandle->GetTextureFromBYTE4Depository(
							m_data->GetIDEnviromentMapSmall(), GetIDEnviromentMapSmall())[(int)m_progress.y * (PIR + 2) + (int)m_progress.x].z = 1;
						GameStreamingDataHandle->GetTextureFromBYTE4Depository(
							m_data->GetIDEnviromentMapSmall(), GetIDEnviromentMapSmall())[(int)m_progress.y * (PIR + 2) + (int)m_progress.x]
							.w = 127.5 + 12.75f * (35.0f - rad);
					}
					else if (rad < 45.0f)
					{
						GameStreamingDataHandle->GetTextureFromBYTE4Depository(
							m_data->GetIDEnviromentMapSmall(), GetIDEnviromentMapSmall())[(int)m_progress.y * (PIR + 2) + (int)m_progress.x].z = 2;
						GameStreamingDataHandle->GetTextureFromBYTE4Depository(
							m_data->GetIDEnviromentMapSmall(), GetIDEnviromentMapSmall())[(int)m_progress.y * (PIR + 2) + (int)m_progress.x]
							.w = 12.75f * (-35.0f + 45);
					}
					else
					{
						GameStreamingDataHandle->GetTextureFromBYTE4Depository(
							m_data->GetIDEnviromentMapSmall(), GetIDEnviromentMapSmall())[(int)m_progress.y * (PIR + 2) + (int)m_progress.x].z = 3;
						GameStreamingDataHandle->GetTextureFromBYTE4Depository(
							m_data->GetIDEnviromentMapSmall(), GetIDEnviromentMapSmall())[(int)m_progress.y * (PIR + 2) + (int)m_progress.x].w = 0;
					}

					XMMATRIX TNB = XMMATRIX(T, N, B, Z);

					float px = VereMath::Clamp((m_progress.x - 1) / (PIR - 1), 0.0f, 1.0f);
					float py = VereMath::Clamp((m_progress.y - 1) / (PIR - 1), 0.0f, 1.0f);

					if (m_data->GetLoadTilesLvl() <= m_level)
					{
						float pxo = (px + m_coordFromLoadTile.x) * m_scalingFromLoadTile;
						float pyo = (py + m_coordFromLoadTile.y) * m_scalingFromLoadTile;

						/*XMFLOAT4 N4 = m_planetTile->GetNormal(XMFLOAT2(px, py), 0.003921f * (float)GameStreamingDataHandle->GetTextureFromBYTE4Depository(
							m_data->GetIDEnviromentMapSmall(), GetIDEnviromentMapSmall())[(int)m_progress.y * (PIR + 2) + (int)m_progress.x].w);

						N = XMVector3Transform(XMVector3Normalize(XMLoadFloat3(&XMFLOAT3(N4.x, N4.y, N4.z))), TNB);

						XMStoreFloat4(&N4, N);

						float H = GameStreamingDataHandle->GetValueFromFLOATDepository(
							m_data->GetIDHeightMapSmall(), GetIDHeightMapSmall(),(m_progress.x)* PIRR, (m_progress.y)* PIRR)
							+ 50.0f * m_planetTile->GetHeight(XMFLOAT2(px, py), 0.003921f * (float)GameStreamingDataHandle->GetTextureFromBYTE4Depository(
								m_data->GetIDEnviromentMapSmall(), GetIDEnviromentMapSmall())[(int)m_progress.y * (PIR + 2) + (int)m_progress.x].w
							+ 0.2f * (255 - (float)GameStreamingDataHandle->GetTextureFromBYTE4Depository(
								m_data->GetIDEnviromentMapSmall(), GetIDEnviromentMapSmall())[(int)m_progress.y * (PIR + 2) + (int)m_progress.x].w));

						GameStreamingDataHandle->SetValueFromFLOATDepository(
							m_data->GetIDHeightMapSmall(), GetIDHeightMapSmall(), (int)m_progress.x, (int)m_progress.y, H);

						GameStreamingDataHandle->SetValueFromBYTE4Depository(
							m_data->GetIDNormalMapSmall(), GetIDNormalMapSmall(), (int)m_progress.x, (int)m_progress.y,
							VBYTE4((BYTE)((1.0f + N4.x) * 127.0f), (BYTE)((1.0f + N4.y) * 127.0f), (BYTE)((1.0f + N4.z) * 127.0f), 0));*/

						XMFLOAT2 FE = XMFLOAT2(VereMath::Clamp(0.015686275f * (float)GameStreamingDataHandle->GetTextureFromBYTE4Depository(
							m_data->GetIDEnviromentMapSmall(), GetIDEnviromentMapSmall())[(int)m_progress.y * (PIR + 2) + (int)m_progress.x].x, 0.0f, 4.0f),
							VereMath::Clamp(0.015686275f * (float)GameStreamingDataHandle->GetTextureFromBYTE4Depository(
								m_data->GetIDEnviromentMapSmall(), GetIDEnviromentMapSmall())[(int)m_progress.y * (PIR + 2) + (int)m_progress.x].y, 0.0f, 4.0f));
						XMINT2 IE = XMINT2(FE.x, FE.y);
						XMFLOAT2 FF = XMFLOAT2(FE.x - (float)IE.x, FE.y - (float)IE.y);

						if (FF.x >= GamePlanetHelperHandle->GetTile1()->GetVariable(pxo, pyo))
						{
							GameStreamingDataHandle->GetTextureFromBYTE4Depository(
								m_data->GetIDEnviromentMapSmall(), GetIDEnviromentMapSmall())[(int)m_progress.y * (PIR + 2) + (int)m_progress.x].x = 63.75f * ((float)(IE.x + 1));
						}
						else
						{
							GameStreamingDataHandle->GetTextureFromBYTE4Depository(
								m_data->GetIDEnviromentMapSmall(), GetIDEnviromentMapSmall())[(int)m_progress.y * (PIR + 2) + (int)m_progress.x].x = 63.75f * ((float)IE.x);
						}

						if (FF.y >= GamePlanetHelperHandle->GetTile2()->GetVariable(pxo, pyo))
						{
							GameStreamingDataHandle->GetTextureFromBYTE4Depository(
								m_data->GetIDEnviromentMapSmall(), GetIDEnviromentMapSmall())[(int)m_progress.y * (PIR + 2) + (int)m_progress.x].y = 63.75f * ((float)(IE.y + 1));
						}
						else
						{
							GameStreamingDataHandle->GetTextureFromBYTE4Depository(
								m_data->GetIDEnviromentMapSmall(), GetIDEnviromentMapSmall())[(int)m_progress.y * (PIR + 2) + (int)m_progress.x].y = 63.75f * ((float)IE.y);
						}
					}

					px = VereMath::Clamp(m_progress.x - 1.0f, 0.0f, (float)PIR - 1.0f);
					py = VereMath::Clamp(m_progress.y - 1.0f, 0.0f, (float)PIR - 1.0f);

					btScalar D = 1.0 / (PIR - 1);
					btVector3 V;
					V.setX(((btScalar)coord.x + D * px) * scaling - 0.5);
					V.setY(0.0);
					V.setZ(((btScalar)coord.y + D * py) * scaling - 0.5);

					btVector3 VT, VW, VA, VC;
					VT = V;
					VT = ((mesh * V).normalize()) * (S + GameStreamingDataHandle->GetValueFromFLOATDepository(
						m_data->GetIDHeightMapSmall(), GetIDHeightMapSmall(), (px + 1) * PIRR, (py + 1) * PIRR));
					VW = V;
					VW = ((mesh * V).normalize()) * (m_data->GetRadiusOfWater());
					VA = V;
					VA = ((mesh * V).normalize()) * (m_data->GetRadiusOfAtmosphere());
					VC = V;
					VC = ((mesh * V).normalize()) * (m_data->GetRadiusOfClouds());

					m_MaxT = VereMath::MaxVector(m_MaxT, VT);
					m_MinT = VereMath::MinVector(m_MinT, VT);

					m_MaxW = VereMath::MaxVector(m_MaxW, VW);
					m_MinW = VereMath::MinVector(m_MinW, VW);

					m_MaxA = VereMath::MaxVector(m_MaxA, VA);
					m_MinA = VereMath::MinVector(m_MinA, VA);

					m_MaxC = VereMath::MaxVector(m_MaxC, VC);
					m_MinC = VereMath::MinVector(m_MinC, VC);

					m_progress.w += 1.0f;
				}

				m_progress.x = 0.0f;
			}
		}
		else
		{
			m_progress.w = 0;

			for (; m_progress.y < (float)PIR + 2.0f; m_progress.y += 1)
			{
				for (; m_progress.x < (float)PIR + 2.0f; m_progress.x += 1)
				{
					if (m_progress.w > 512.0f) return false;

					float px = VereMath::Clamp(m_progress.x - 1.0f, 0.0f, (float)PIR - 1.0f);
					float py = VereMath::Clamp(m_progress.y - 1.0f, 0.0f, (float)PIR - 1.0f);

					btScalar D = 1.0 / (PIR - 1);
					btVector3 V;
					V.setX(((btScalar)coord.x + D * px) * scaling - 0.5);
					V.setY(0.0);
					V.setZ(((btScalar)coord.y + D * py) * scaling - 0.5);

					btVector3 VT, VW, VA, VC;
					VT = V;
					VT = ((mesh * V).normalize()) * (m_data->GetRadiusOfTerrain() - 2300.0f);
					VW = V;
					VW = ((mesh * V).normalize()) * (m_data->GetRadiusOfWater());
					VA = V;
					VA = ((mesh * V).normalize()) * (m_data->GetRadiusOfAtmosphere());
					VC = V;
					VC = ((mesh * V).normalize()) * (m_data->GetRadiusOfClouds());

					m_MaxT = VereMath::MaxVector(m_MaxT, VT);
					m_MinT = VereMath::MinVector(m_MinT, VT);

					m_MaxW = VereMath::MaxVector(m_MaxW, VW);
					m_MinW = VereMath::MinVector(m_MinW, VW);

					m_MaxA = VereMath::MaxVector(m_MaxA, VA);
					m_MinA = VereMath::MinVector(m_MinA, VA);

					m_MaxC = VereMath::MaxVector(m_MaxC, VC);
					m_MinC = VereMath::MinVector(m_MinC, VC);

					m_progress.w += 1.0f;
				}

				m_progress.x = 0.0f;
			}
>>>>>>> VereEngine-Planet
		}
		
		m_progress.y = 0.0f;

		m_progress.z = 4.0f;
	}

<<<<<<< HEAD
	btVector3 offsetT = 0.5 * (MaxT - MinT);
	m_OffsetCubeT = offsetT;
	m_CentreT = MinT + offsetT;

	btVector3 offsetW = 0.5 * (MaxW - MinW);
	m_OffsetCubeW = offsetW;
	m_CentreW = MinW + offsetW;

	btVector3 offsetA = 0.5 * (MaxA - MinA);
	m_OffsetCubeA = offsetA;
	m_CentreA = MinA + offsetA;

	btVector3 offsetC = 0.5 * (MaxC - MinC);
	m_OffsetCubeC = offsetC;
	m_CentreC = MinC + offsetC;
=======
	if (m_progress.z == 4.0f)
	{
		btVector3 offsetT = 0.5 * (m_MaxT - m_MinT);
		m_OffsetCubeT = offsetT;
		m_CentreT = m_MinT + offsetT;

		btVector3 offsetW = 0.5 * (m_MaxW - m_MinW);
		m_OffsetCubeW = offsetW;
		m_CentreW = m_MinW + offsetW;

		btVector3 offsetA = 0.5 * (m_MaxA - m_MinA);
		m_OffsetCubeA = offsetA;
		m_CentreA = m_MinA + offsetA;
>>>>>>> VereEngine-Planet

		btVector3 offsetC = 0.5 * (m_MaxC - m_MinC);
		m_OffsetCubeC = offsetC;
		m_CentreC = m_MinC + offsetC;

		if (m_isMap)
		{
				
			m_idHeightMapSRT = GameRenderDeviceHandle->CreateTexture(
				GameStreamingDataHandle->GetTextureFromFLOATDepository(
				m_data->GetIDHeightMapSmall(), GetIDHeightMapSmall()),
				GameStreamingDataHandle->GetHeightFromFLOATDepository(
					m_data->GetIDHeightMapSmall()),
				GameStreamingDataHandle->GetWidthFromFLOATDepository(
						m_data->GetIDHeightMapSmall()), DXGI_FORMAT_R32_FLOAT, 1);

			m_idNormalMapSRT = GameRenderDeviceHandle->CreateTexture(GameStreamingDataHandle->GetTextureFromBYTE4Depository(
				m_data->GetIDNormalMapSmall(), GetIDNormalMapSmall()),
				GameStreamingDataHandle->GetHeightFromBYTE4Depository(
					m_data->GetIDNormalMapSmall()),
				GameStreamingDataHandle->GetWidthFromBYTE4Depository(
					m_data->GetIDNormalMapSmall()), DXGI_FORMAT_R8G8B8A8_UNORM, 1);

			m_idEnviromentMapSRT = GameRenderDeviceHandle->CreateTexture(GameStreamingDataHandle->GetTextureFromBYTE4Depository(
				m_data->GetIDEnviromentMapSmall(), GetIDEnviromentMapSmall()),
				GameStreamingDataHandle->GetHeightFromBYTE4Depository(
					m_data->GetIDEnviromentMapSmall()),
				GameStreamingDataHandle->GetWidthFromBYTE4Depository(
					m_data->GetIDEnviromentMapSmall()), DXGI_FORMAT_R8G8B8A8_UNORM, 1);

			m_idTreesMapSRT = GameRenderDeviceHandle->CreateTexture(GameStreamingDataHandle->GetTextureFromBYTE4Depository(
				m_data->GetIDTreesMapSmall(), GetIDTreesMapSmall()),
				GameStreamingDataHandle->GetHeightFromBYTE4Depository(
					m_data->GetIDTreesMapSmall()),
				GameStreamingDataHandle->GetWidthFromBYTE4Depository(
					m_data->GetIDTreesMapSmall()), DXGI_FORMAT_R8G8B8A8_UNORM, 1);
		}

		m_progress = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

		return true;
	}

	return false;
}


void PlanetLOD::CreateNewLevelOfLoD()
{
	btScalar S = m_scaling;
	btScalar S2 = m_data->GetScaling().getRow(0).getX();
	btScalar S3 = S * S2;

	PlanetLOD * pLOD01 = new PlanetLOD;
	PlanetLOD * pLOD02 = new PlanetLOD;
	PlanetLOD * pLOD03 = new PlanetLOD;
	PlanetLOD * pLOD04 = new PlanetLOD;

	m_blocks[0] = pLOD01;
	m_blocks[1] = pLOD02;
	m_blocks[2] = pLOD03;
	m_blocks[3] = pLOD04;

	m_blocks[0]->SetProccessed(0);
	m_blocks[1]->SetProccessed(0);
	m_blocks[2]->SetProccessed(0);
	m_blocks[3]->SetProccessed(0);

	m_isCreateNewLevelInProcess = 2;

	btVector3 pos[4];
	pos[0] = m_position;
	pos[0].setX(m_position.getX() + 0.25 * S3);
	pos[0].setZ(m_position.getZ() + 0.25 * S3);
	pos[1] = m_position;
	pos[1].setX(m_position.getX() + 0.25 * S3);
	pos[1].setZ(m_position.getZ() - 0.25 * S3);
	pos[2] = m_position;
	pos[2].setX(m_position.getX() - 0.25 * S3);
	pos[2].setZ(m_position.getZ() + 0.25 * S3);
	pos[3] = m_position;
	pos[3].setX(m_position.getX() - 0.25 * S3);
	pos[3].setZ(m_position.getZ() - 0.25 * S3);

<<<<<<< HEAD
	m_blocks[0]->Init(m_data, m_side, m_level + 1, XMINT2(2 * m_coord.x, 2 * m_coord.y), pos[3], 0.5 * m_scaling, m_hMap, m_nMap);
	m_blocks[1]->Init(m_data, m_side, m_level + 1, XMINT2(2 * m_coord.x, 2 * m_coord.y + 1), pos[2], 0.5 * m_scaling, m_hMap, m_nMap);
	m_blocks[2]->Init(m_data, m_side, m_level + 1, XMINT2(2 * m_coord.x + 1, 2 * m_coord.y), pos[1], 0.5 * m_scaling, m_hMap, m_nMap);
	m_blocks[3]->Init(m_data, m_side, m_level + 1, XMINT2(2 * m_coord.x + 1, 2 * m_coord.y + 1), pos[0], 0.5 * m_scaling, m_hMap, m_nMap);

	GameComponentsManagerHandle->InsertToProccess(m_blocks[0]);
	GameComponentsManagerHandle->InsertToProccess(m_blocks[1]);
	GameComponentsManagerHandle->InsertToProccess(m_blocks[2]);
	GameComponentsManagerHandle->InsertToProccess(m_blocks[3]);
=======
	float scalingFromLoadTile = 1.0f;
	int levelFromLoadTile = 0;
	XMINT2 coordFromLoadTile[] = { XMINT2(0, 0),XMINT2(0, 0),XMINT2(0, 0),XMINT2(0, 0) };

	if (m_data->GetLoadTilesLvl() <= m_level)
	{
		scalingFromLoadTile = m_scalingFromLoadTile * 0.5f;
		levelFromLoadTile = m_levelFromLoadTile + 1;

		coordFromLoadTile[0].x = 2 * m_coordFromLoadTile.x;
		coordFromLoadTile[0].y = 2 * m_coordFromLoadTile.y;

		coordFromLoadTile[1].x = 2 * m_coordFromLoadTile.x;
		coordFromLoadTile[1].y = 2 * m_coordFromLoadTile.y + 1;

		coordFromLoadTile[2].x = 2 * m_coordFromLoadTile.x + 1;
		coordFromLoadTile[2].y = 2 * m_coordFromLoadTile.y;

		coordFromLoadTile[3].x = 2 * m_coordFromLoadTile.x + 1;
		coordFromLoadTile[3].y = 2 * m_coordFromLoadTile.y + 1;
	}

	m_blocks[0]->Init(m_data, m_side, m_level + 1, XMINT2(2 * m_coord.x, 2 * m_coord.y), pos[3], 0.5 * m_scaling,
		m_levelFromLastLoadData + 1, XMINT2(2 * m_coordFromLastLoadData.x, 2 * m_coordFromLastLoadData.y), 0.5 * m_scalingFromLastLoadData,
		levelFromLoadTile, coordFromLoadTile[0], scalingFromLoadTile, m_isMap,
		m_idHeightMapBig, m_idNormalMapBig, m_idEnviromentMapBig, m_idTreesMapBig);
	m_blocks[1]->Init(m_data, m_side, m_level + 1, XMINT2(2 * m_coord.x, 2 * m_coord.y + 1), pos[2], 0.5 * m_scaling,
		m_levelFromLastLoadData + 1, XMINT2(2 * m_coordFromLastLoadData.x, 2 * m_coordFromLastLoadData.y + 1), 0.5 * m_scalingFromLastLoadData,
		levelFromLoadTile, coordFromLoadTile[1], scalingFromLoadTile, m_isMap,
		m_idHeightMapBig, m_idNormalMapBig, m_idEnviromentMapBig, m_idTreesMapBig);
	m_blocks[2]->Init(m_data, m_side, m_level + 1, XMINT2(2 * m_coord.x + 1, 2 * m_coord.y), pos[1], 0.5 * m_scaling,
		m_levelFromLastLoadData + 1, XMINT2(2 * m_coordFromLastLoadData.x + 1, 2 * m_coordFromLastLoadData.y), 0.5 * m_scalingFromLastLoadData,
		levelFromLoadTile, coordFromLoadTile[2], scalingFromLoadTile, m_isMap,
		m_idHeightMapBig, m_idNormalMapBig, m_idEnviromentMapBig, m_idTreesMapBig);
	m_blocks[3]->Init(m_data, m_side, m_level + 1, XMINT2(2 * m_coord.x + 1, 2 * m_coord.y + 1), pos[0], 0.5 * m_scaling,
		m_levelFromLastLoadData + 1, XMINT2(2 * m_coordFromLastLoadData.x + 1, 2 * m_coordFromLastLoadData.y + 1), 0.5 * m_scalingFromLastLoadData,
		levelFromLoadTile, coordFromLoadTile[3], scalingFromLoadTile, m_isMap,
		m_idHeightMapBig, m_idNormalMapBig, m_idEnviromentMapBig, m_idTreesMapBig);

	if (GetValueOfLODSmallForNext() == 4)
	{
		SetValueOfLODSmallForNext(0);
		m_blocks[0]->SetValueOfLODSmall(1);
		m_blocks[1]->SetValueOfLODSmall(1);
		m_blocks[2]->SetValueOfLODSmall(1);
		m_blocks[3]->SetValueOfLODSmall(1);
	}

	if (GetValueOfLODBigForNext() == 4)
	{
		SetValueOfLODBigForNext(0);
		m_blocks[0]->SetValueOfLODBig(1);
		m_blocks[1]->SetValueOfLODBig(1);
		m_blocks[2]->SetValueOfLODBig(1);
		m_blocks[3]->SetValueOfLODBig(1);
	}

	int KOT = KT_TERRAIN_LOD2;
	if (m_level < 10)
		KOT = KT_TERRAIN_LOD1;
	if (m_level < 5)
		KOT = KT_TERRAIN_LOD0;

	GameComponentsManagerHandle->InsertToProccess(m_blocks[0], KOT);
	GameComponentsManagerHandle->InsertToProccess(m_blocks[1], KOT);
	GameComponentsManagerHandle->InsertToProccess(m_blocks[2], KOT);
	GameComponentsManagerHandle->InsertToProccess(m_blocks[3], KOT);
>>>>>>> VereEngine-Planet
}