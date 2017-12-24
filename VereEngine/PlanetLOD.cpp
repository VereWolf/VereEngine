#include "pch.h"
#include "PlanetLOD.h"
#include "PlanetData.h"
#include "GameComponent.h"
#include "RenderDevice.h"

bool PlanetLOD::m_onlyRenderText = false;

PlanetLOD::PlanetLOD()
{
	m_idHeightMapBig = XMINT2(-1, -1);
	m_idNormalMapBig = XMINT2(-1, -1);
	m_idEnviromentMapBig = XMINT2(-1, -1);
	m_idTreesMapBig = XMINT2(-1, -1);
	m_idRiverWidth1MapBig = XMINT2(-1, -1);
	m_idRiverWidth2MapBig = XMINT2(-1, -1);
	m_idRiverLength1MapBig = XMINT2(-1, -1);
	m_idRiverLength2MapBig = XMINT2(-1, -1);
	m_idRiverType1MapBig = XMINT2(-1, -1);
	m_idRiverType2MapBig = XMINT2(-1, -1);
	m_idRiverHeightMapBig = XMINT2(-1, -1);

	m_idHeightMapInput = -1;
	m_idNormalMapInput = -1;
	m_idAngleMapInput = -1;
	m_idEnviromentMapInput = -1;
	m_idTreesMapInput = -1;
	m_idRiverWidth1MapInput = -1;
	m_idRiverWidth2MapInput = -1;
	m_idRiverLength1MapInput = -1;
	m_idRiverLength2MapInput = -1;
	m_idRiverType1MapInput = -1;
	m_idRiverType2MapInput = -1;
	m_idRiverHeightMapInput = -1;

	m_idHeightMapOutput = -1;
	m_idNormalMapOutput = -1;
	m_idAngleMapOutput = -1;
	m_idEnviromentMapOutput = -1;
	m_idTreesMapOutput = -1;
	m_idRiverWidth1MapOutput = -1;
	m_idRiverWidth2MapOutput = -1;
	m_idRiverLength1MapOutput = -1;
	m_idRiverLength2MapOutput = -1;
	m_idRiverType1MapOutput = -1;
	m_idRiverType2MapOutput = -1;
	m_idRiverHeightMapOutput = -1;

	m_isCreateNewLevelInProcess = 0;
	m_isNewLevel = false;
	m_isMap = false;
	m_isMap1 = false;
	m_isMap2 = false;
	m_blocks[0] = NULL;
	m_blocks[1] = NULL;
	m_blocks[2] = NULL;
	m_blocks[3] = NULL;

	m_valueOfLODBig = 0;
	m_valueOfLODSmall = 0;
	m_valueOfLODBigForNext = 0;
	m_valueOfLODSmallForNext = 0;

	m_idBillboardTreesVertex = -1;
	m_billboardTreesIndex = NULL;
	m_idBillboardTrees = -1;

	m_MaxH = -2300.0f;
	m_MinH = -2300.0f;
}

PlanetLOD::PlanetLOD(const PlanetLOD & n)
{
	m_idHeightMapBig = XMINT2(-1, -1);
	m_idNormalMapBig = XMINT2(-1, -1);
	m_idEnviromentMapBig = XMINT2(-1, -1);
	m_idTreesMapBig = XMINT2(-1, -1);
	m_idRiverWidth1MapBig = XMINT2(-1, -1);
	m_idRiverWidth2MapBig = XMINT2(-1, -1);
	m_idRiverLength1MapBig = XMINT2(-1, -1);
	m_idRiverLength2MapBig = XMINT2(-1, -1);
	m_idRiverType1MapBig = XMINT2(-1, -1);
	m_idRiverType2MapBig = XMINT2(-1, -1);
	m_idRiverHeightMapBig = XMINT2(-1, -1);

	m_idHeightMapInput = -1;
	m_idNormalMapInput = -1;
	m_idAngleMapInput = -1;
	m_idEnviromentMapInput = -1;
	m_idTreesMapInput = -1;
	m_idRiverWidth1MapInput = -1;
	m_idRiverWidth2MapInput = -1;
	m_idRiverLength1MapInput = -1;
	m_idRiverLength2MapInput = -1;
	m_idRiverType1MapInput = -1;
	m_idRiverType2MapInput = -1;
	m_idRiverHeightMapInput = -1;

	m_idHeightMapOutput = -1;
	m_idNormalMapOutput = -1;
	m_idAngleMapOutput = -1;
	m_idEnviromentMapOutput = -1;
	m_idTreesMapOutput = -1;
	m_idRiverWidth1MapOutput = -1;
	m_idRiverWidth2MapOutput = -1;
	m_idRiverLength1MapOutput = -1;
	m_idRiverLength2MapOutput = -1;
	m_idRiverType1MapOutput = -1;
	m_idRiverType2MapOutput = -1;
	m_idRiverHeightMapOutput = -1;

	m_isCreateNewLevelInProcess = 0;
	m_isNewLevel = false;
	m_isMap = false;
	m_isMap1 = false;
	m_isMap2 = false;
	m_blocks[0] = NULL;
	m_blocks[1] = NULL;
	m_blocks[2] = NULL;
	m_blocks[3] = NULL;

	m_valueOfLODBig = 0;
	m_valueOfLODSmall = 0;
	m_valueOfLODBigForNext = 0;
	m_valueOfLODSmallForNext = 0;

	m_idBillboardTreesVertex = -1;
	m_billboardTreesIndex = NULL;
	m_idBillboardTrees = -1;

	m_MaxH = -2300.0f;
	m_MinH = -2300.0f;
}

void PlanetLOD::Init(PlanetLODInitMeessage message)
{
	m_data = message.master;
	m_side = message.side;
	m_level = message.level;
	m_coord = message.coord;
	m_scaling = message.scaling;
	m_position = message.position;
	m_isMap = message.isMap;
	m_isMap1 = message.isMap1;
	m_isMap2 = message.isMap2;
	m_levelFromLastLoadData = message.levelFromLastLoadData;
	m_coordFromLastLoadData = message.coordFromLastLoadData;
	m_scalingFromLastLoadData = message.scalingFromLastLoadData;
	m_levelFromLoadTile = message.levelFromLoadTile;
	m_coordFromLoadTile = message.coordFromLoadTile;
	m_scalingFromLoadTile = message.scalingFromLoadTile;
	m_MaxH = message.maxH;
	m_MinH = message.minH;

	SetId(m_data->m_planetElementID->TakeElement());

	if (GetId() < 0)
	{
		return;
	}

	m_data->m_planetElements[GetId()] = this;

	if (!((message.level % m_data->GetLoadDataPer()) == 0 && m_level < m_data->GetLoadDataMaxLvl()))
	{
		m_idHeightMapBig = message.idHeightMapBig;
		m_idNormalMapBig = message.idNormalMapBig;
	}

	if (message.level != 0)
	{
		m_idEnviromentMapBig = message.idEnviromentMapBig;
		m_idTreesMapBig = message.idTreesMapBig;
	}

	if (m_level >= m_data->GetLoadDataMaxLvl())
	{
		m_idRiverWidth1MapBig = message.idRiverWidth1MapBig;
		m_idRiverWidth2MapBig = message.idRiverWidth2MapBig;
		m_idRiverLength1MapBig = message.idRiverLength1MapBig;
		m_idRiverLength2MapBig = message.idRiverLength2MapBig;
		m_idRiverType1MapBig = message.idRiverType1MapBig;
		m_idRiverType2MapBig = message.idRiverType2MapBig;
		m_idRiverHeightMapBig = message.idRiverHeightMapBig;
	}

	if (GameRenderDeviceHandle->GetModel(message.master->GetRenderId()))
	{
		Model *model = new Model;
		memcpy(model, GameRenderDeviceHandle->GetModel(message.master->GetRenderId()), sizeof(Model));
		m_modelID = GameRenderDeviceHandle->CreateModel(model);
	}

	m_idBillboardTreesVertex = message.idBillboardTreesVertex;
	m_billboardTreesIndex = message.billboardTreesIndex;
	m_billBoardMatrix = message.billboardMatrix;
	m_idBillboardTrees = message.idBillboardTrees;
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
	}

	if (m_isNewLevel == true)
	{
		m_blocks[0]->Render();
		m_blocks[1]->Render();
		m_blocks[2]->Render();
		m_blocks[3]->Render();
	}

	if (m_level == m_data->GetGenerateTreesLvl())
	{
		XMFLOAT4X4 VW;
		XMStoreFloat4x4(&VW, RenderMessage::m_View);
		btTransform VW2 = VereMath::ConvertTobtTransform(VW);
		btTransform meshCentre = RenderMessage::m_CameraOffset * GT;
		btTransform Mat = VW2 * meshCentre;

		//if (VereMath::FrustrumCulling2(m_CentreT, m_OffsetCubeT, RenderMessage::m_FarZ, RenderMessage::m_HeightFar, RenderMessage::m_Aspect, Mat))
		{
			DrawTreesBillboard();
		}
	}

	if (m_level == 0)
	{
		//if (VereMath::FrustrumCulling2(m_CentreT, m_OffsetCubeT, RenderMessage::m_FarZ, RenderMessage::m_HeightFar, RenderMessage::m_Aspect, Mat))
		{
			//DrawAtmosphere();
		}
	}

	if (m_isNewLevel == false)
	{
		XMFLOAT4X4 VW;
		XMStoreFloat4x4(&VW, RenderMessage::m_View);
		btTransform VW2 = VereMath::ConvertTobtTransform(VW);
		btTransform meshCentre = RenderMessage::m_CameraOffset * GT;
		btTransform Mat = VW2 * meshCentre;

		/*btVector3 Vecd = Mat * m_CentreT;*/

		bool BTW = false;

		for (int i = 0; i < 4; ++i)
		{
			if (m_dir[i].length() != 0.0)
			{
				if ((btVector3(-RenderMessage::m_CameraOffset.getOrigin() - m_data->GetWorldPosition()) * m_data->GetWorldTransform().getBasis()).normalize().dot(m_dir[i]) > 0.0f) BTW = true;
			}
		}

		if (BTW == true)
		{

			//if (VereMath::FrustrumCulling2(m_CentreT, m_OffsetCubeT, RenderMessage::m_FarZ, RenderMessage::m_HeightFar, RenderMessage::m_Aspect, Mat))
			{
				DrawTerrain();
			}

			//if (VereMath::FrustrumCulling2(m_CentreW, m_OffsetCubeW, RenderMessage::m_FarZ, RenderMessage::m_HeightFar, RenderMessage::m_Aspect, Mat))
			{
				DrawWater();
			}

			//if (VereMath::FrustrumCulling2(m_CentreA, m_OffsetCubeA, RenderMessage::m_FarZ, RenderMessage::m_HeightFar, RenderMessage::m_Aspect, Mat))
			{
				DrawAtmosphere();
			}
		}

		//if (VereMath::FrustrumCulling2(m_CentreT, m_OffsetCubeT, RenderMessage::m_FarZ, RenderMessage::m_HeightFar, RenderMessage::m_Aspect, Mat))
		{
			DrawClouds();
		}

		m_data->GetDeviceResources()->GetD3DDeviceContext()->HSSetShader(0, 0, 0);
		m_data->GetDeviceResources()->GetD3DDeviceContext()->DSSetShader(0, 0, 0);
	}
}

void PlanetLOD::DrawTerrain()
{
	btMatrix3x3 I = btMatrix3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);
	btScalar S = m_scaling;
	btScalar S2 = m_data->GetScaling().getRow(0).getX();
	btScalar S3 = S * S2;

	btTransform AM = btTransform(m_data->GetBlockAnglMatrix(m_side), btVector3(0.0, 0.0, 0.0));
	btTransform GT = m_data->GetWorldTransform();


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

	message.m_IsMap = m_isMap;

	if (m_isMap)
	{
		if (m_idHeightMapInput >= 0)
		{
			message.m_HeightSRV = GameRenderDeviceHandle->GetTexture(m_idHeightMapInput);
		}
		else
		{
			return;
		}
		if (m_idNormalMapInput >= 0)
		{
			message.m_NormalSRV = GameRenderDeviceHandle->GetTexture(m_idNormalMapInput);
		}
		else
		{
			return;
		}
		if (m_idEnviromentMapInput >= 0)
		{
			message.m_EnviromentSRV = GameRenderDeviceHandle->GetTexture(m_idEnviromentMapInput);
		}
		else
		{
			return;
		}
		if (m_idTreesMapInput >= 0)
		{
			message.m_TreesSRV = GameRenderDeviceHandle->GetTexture(m_idTreesMapInput);
		}
		else
		{
			return;
		}
	}

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
	
	if (m_isMap1 || m_isMap2)
	{
		message.m_IsHeightMap = true;

		if (m_idRiverHeightMapInput >= 0)
		{
			message.m_HeightSRV = GameRenderDeviceHandle->GetTexture(m_idRiverHeightMapInput);
		}
		else
		{
			return;
		}
	}
	else
	{
		message.m_IsHeightMap = false;
	}

	GameRenderDeviceHandle->BindRenderTarget(m_data->GetWaterTopTargetMapRTV(), m_data->GetWaterTopDeepMapDSV());
	GameRenderDeviceHandle->Render(&message);

	message.m_RasterizeState = RenderStates::BackRS;

	GameRenderDeviceHandle->BindRenderTarget(m_data->GetWaterBottomTargetMapRTV(), m_data->GetWaterBottomDeepMapDSV());
	GameRenderDeviceHandle->Render(&message);

	GameRenderDeviceHandle->BindMainRenderTarget();
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

void PlanetLOD::DrawTreesBillboard()
{

	if (m_idBillboardTrees >= 0)
	{
		TreesBillboardRenderMessage message;

		message.m_ModelID = m_idBillboardTrees;
		message.m_PlanetData = m_data;
		message.m_Scaling = btTransform(btMatrix3x3(1.0f, 0.0, 0.0, 0.0, 1.0f, 0.0, 0.0, 0.0, 1.0f), btVector3(0.0, 0.0, 0.0));
		message.m_Transform = m_data->GetWorldTransform() * m_billBoardMatrix;
		message.m_ViewPort = GameRenderDeviceHandle->GetMainViewPort();
		message.m_RasterizeState = RenderStates::NoCullRS;
		message.m_BlendState = RenderStates::NoBlendBS;

		for (int y = 0; y < 8; ++y)
		{
			for (int x = 0; x < 8; ++x)
			{
				message.m_StartIndex = m_billboardTreesIndex->at(4 * (y * 8 + x));
				message.m_CountIndex = m_billboardTreesIndex->at(4 * (y * 8 + x) + 4) - message.m_StartIndex;
				GameRenderDeviceHandle->Render(&message);
			}
		}
	}
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
	if (m_level == 8)
	{
		int a = 0;
	}


	m_progress.w = 0.0f;

	PlanetData *data = m_data;
	int side = m_side;
	int level = m_level;
	XMINT2 coord = m_coord;
	btScalar scaling = m_scaling;
	btVector3 position = m_position;
	btScalar S = m_data->GetScaling().getRow(0).getX();
	btScalar S2 = S * scaling;
	int PIR = m_data->GetNumPointInRowInCell();
	/*int size = pow(PIR + 1, 2);
	float PIRR = 1.0f / ((float)PIR + 2.0f);*/

	int PIRB = m_data->GetNumPointInRowInBigCell();;
	int sizeB = pow(PIRB + 1 + 2 * PIRB / PIR, 2);
	//float PIRRB = 1.0f / ((float)PIR + 2.0f);

	int L = 512 + 2 * 512 / m_data->GetNumPointInRowInCell();
	int MS = pow(L, 2);

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
			m_scalingFromLastLoadData = 1.0f;
			m_levelFromLastLoadData = 0;
			m_coordFromLastLoadData = XMINT2(0, 0);

			m_idH = -1;

			m_idHeightMapBig.x = GameStreamingDataHandle->CreateNewBlockInFLOATDepository(m_data->GetIDHeightMapBig(), NULL);
			m_idNormalMapBig.x = GameStreamingDataHandle->CreateNewBlockInBYTE4Depository(m_data->GetIDNormalMapBig(), NULL);

			if (level == 0)
			{
				m_idEnviromentMapBig.x = GameStreamingDataHandle->CreateNewBlockInBYTE4Depository(m_data->GetIDEnviromentMapBig(), NULL);
				ID3D11ShaderResourceView *inputEnviromentSRV;
				GameStreamingDataHandle->GetTextureInputFromBYTE4(m_data->GetIDEnviromentMapBig(), GetIDEnviromentMapBig().x, &inputEnviromentSRV);
				m_idEnviromentMapBig.y = GameRenderDeviceHandle->AddTexture(inputEnviromentSRV);

				m_idTreesMapBig.x = GameStreamingDataHandle->CreateNewBlockInBYTE4Depository(m_data->GetIDTreesMapBig(), NULL);
				ID3D11ShaderResourceView *inputTreesSRV;
				GameStreamingDataHandle->GetTextureInputFromBYTE4(m_data->GetIDTreesMapBig(), GetIDTreesMapBig().x, &inputTreesSRV);
				m_idTreesMapBig.y = GameRenderDeviceHandle->AddTexture(inputTreesSRV);
			}

			if (level != 0)
			{
				m_idRiverWidth1MapBig.x = GameStreamingDataHandle->CreateNewBlockInFLOATDepository(m_data->GetIDRiverWidth1MapBig(), NULL);
				ID3D11ShaderResourceView *inputRiverWidth1SRV;
				GameStreamingDataHandle->GetTextureInputFromFloat(m_data->GetIDRiverWidth1MapBig(), GetIDRiverWidth1MapBig().x, &inputRiverWidth1SRV);
				m_idRiverWidth1MapBig.y = GameRenderDeviceHandle->AddTexture(inputRiverWidth1SRV);

				m_idRiverWidth2MapBig.x = GameStreamingDataHandle->CreateNewBlockInFLOATDepository(m_data->GetIDRiverWidth2MapBig(), NULL);
				ID3D11ShaderResourceView *inputRiverWidth2SRV;
				GameStreamingDataHandle->GetTextureInputFromFloat(m_data->GetIDRiverWidth2MapBig(), GetIDRiverWidth2MapBig().x, &inputRiverWidth2SRV);
				m_idRiverWidth2MapBig.y = GameRenderDeviceHandle->AddTexture(inputRiverWidth2SRV);


				m_idRiverLength1MapBig.x = GameStreamingDataHandle->CreateNewBlockInFLOATDepository(m_data->GetIDRiverLength1MapBig(), NULL);
				ID3D11ShaderResourceView *inputRiverLength1SRV;
				GameStreamingDataHandle->GetTextureInputFromFloat(m_data->GetIDRiverLength1MapBig(), GetIDRiverLength1MapBig().x, &inputRiverLength1SRV);
				m_idRiverLength1MapBig.y = GameRenderDeviceHandle->AddTexture(inputRiverLength1SRV);


				m_idRiverLength2MapBig.x = GameStreamingDataHandle->CreateNewBlockInFLOATDepository(m_data->GetIDRiverLength2MapBig(), NULL);
				ID3D11ShaderResourceView *inputRiverLength2SRV;
				GameStreamingDataHandle->GetTextureInputFromFloat(m_data->GetIDRiverLength2MapBig(), GetIDRiverLength2MapBig().x, &inputRiverLength2SRV);
				m_idRiverLength2MapBig.y = GameRenderDeviceHandle->AddTexture(inputRiverLength2SRV);


				m_idRiverType1MapBig.x = GameStreamingDataHandle->CreateNewBlockInFLOATDepository(m_data->GetIDRiverType1MapBig(), NULL);
				ID3D11ShaderResourceView *inputRiverType1SRV;
				GameStreamingDataHandle->GetTextureInputFromFloat(m_data->GetIDRiverType1MapBig(), GetIDRiverType1MapBig().x, &inputRiverType1SRV);
				m_idRiverType1MapBig.y = GameRenderDeviceHandle->AddTexture(inputRiverType1SRV);


				m_idRiverType2MapBig.x = GameStreamingDataHandle->CreateNewBlockInFLOATDepository(m_data->GetIDRiverType2MapBig(), NULL);
				ID3D11ShaderResourceView *inputRiverType2SRV;
				GameStreamingDataHandle->GetTextureInputFromFloat(m_data->GetIDRiverType2MapBig(), GetIDRiverType2MapBig().x, &inputRiverType2SRV);
				m_idRiverType2MapBig.y = GameRenderDeviceHandle->AddTexture(inputRiverType2SRV);


				m_idRiverHeightMapBig.x = GameStreamingDataHandle->CreateNewBlockInFLOATDepository(m_data->GetIDRiverHeightMapBig(), NULL);
				ID3D11ShaderResourceView *inputRiverHeightSRV;
				GameStreamingDataHandle->GetTextureInputFromFloat(m_data->GetIDRiverHeightMapBig(), GetIDRiverHeightMapBig().x, &inputRiverHeightSRV);
				m_idRiverHeightMapBig.y = GameRenderDeviceHandle->AddTexture(inputRiverHeightSRV);
			}

			if (level == 0)
			{
				m_strH << m_data->GetFullPath() << "height_" << m_data->GetPosFix(side) << ".raw";
				m_strN << m_data->GetFullPath() << "normal_" << m_data->GetPosFix(side) << ".raw";
				m_strE << m_data->GetFullPath() << "enviroment_" << m_data->GetPosFix(side) << ".raw";
				m_strT << m_data->GetFullPath() << "trees_" << m_data->GetPosFix(side) << ".raw";
			}
			else
			{
				m_strH << m_data->GetFullPath() << "height_" << m_data->GetPosFix(side) << "_" << level << "_" << coord.x << "_" << coord.y << ".raw";
				m_strN << m_data->GetFullPath() << "normal_" << m_data->GetPosFix(side) << "_" << level << "_" << coord.x << "_" << coord.y << ".raw";
				m_strRW1 << m_data->GetFullPath() << "river_width1_" << m_data->GetPosFix(side) << "_" << level << "_" << coord.x << "_" << coord.y << ".raw";
				m_strRW2 << m_data->GetFullPath() << "river_width2_" << m_data->GetPosFix(side) << "_" << level << "_" << coord.x << "_" << coord.y << ".raw";
				m_strRL1 << m_data->GetFullPath() << "river_length1_" << m_data->GetPosFix(side) << "_" << level << "_" << coord.x << "_" << coord.y << ".raw";
				m_strRL2 << m_data->GetFullPath() << "river_length2_" << m_data->GetPosFix(side) << "_" << level << "_" << coord.x << "_" << coord.y << ".raw";
				m_strRT1 << m_data->GetFullPath() << "river_type1_" << m_data->GetPosFix(side) << "_" << level << "_" << coord.x << "_" << coord.y << ".raw";
				m_strRT2 << m_data->GetFullPath() << "river_type2_" << m_data->GetPosFix(side) << "_" << level << "_" << coord.x << "_" << coord.y << ".raw";
				m_strRH << m_data->GetFullPath() << "river_height_" << m_data->GetPosFix(side) << "_" << level << "_" << coord.x << "_" << coord.y << ".raw";
			}

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
				if (m_level < m_data->GetLoadDataMaxLvl())
				{
					m_VdH = GameStreamingDataHandle->GetStreamingData(m_idH);
					m_VsH = GameStreamingDataHandle->GetSizeOfStreamingData(m_idH);

					m_idN = GameStreamingDataHandle->LoadData(m_strN.str());
					m_VdN = GameStreamingDataHandle->GetStreamingData(m_idN);
					m_VsN = GameStreamingDataHandle->GetSizeOfStreamingData(m_idN);

					m_MinH = ((float*)m_VdH)[m_VsH / sizeof(float) - 2];
					m_MaxH = ((float*)m_VdH)[m_VsH / sizeof(float) - 1];
				}
			}
		}

		float p[8] = { 0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f };

		for (int i = 0; i < 4; ++i)
		{
			btVector3 V;
			V.setX(((btScalar)coord.x + p[2 * i] * scaling) - 0.5f);
			V.setY(0.5f);
			V.setZ(((btScalar)coord.y + p[2 * i + 1] * scaling) - 0.5f);

			m_dir[i] = V;

			m_dir[i] = m_data->GetBlockAnglMatrix(m_side) * m_dir[i];
		}

		if (m_isMap == true || ((level % m_data->GetLoadDataPer()) == 0 && m_level >= m_data->GetLoadDataMaxLvl()))
		{
			m_progress.z = 1.0f;
		}
		else
		{
			m_progress.z = 2.0f;
		}
	}

	if (m_progress.z == 1.0f)
	{
		if ((level % m_data->GetLoadDataPer()) == 0)
		{
			if (m_level < m_data->GetLoadDataMaxLvl())
			{
				GameStreamingDataHandle->SetTextureFromFLOATDepository(m_data->GetIDHeightMapBig(), GetIDHeightMapBig().x, (float*)m_VdH);
				GameStreamingDataHandle->DeleteStreamingData(m_idH);

				GameStreamingDataHandle->SetTextureFromBYTE4Depository(m_data->GetIDNormalMapBig(), GetIDNormalMapBig().x, (VBYTE4*)m_VdN);
				GameStreamingDataHandle->DeleteStreamingData(m_idN);

				ID3D11ShaderResourceView *inputHeightSRV;
				GameStreamingDataHandle->GetTextureInputFromFloat(m_data->GetIDHeightMapBig(), GetIDHeightMapBig().x, &inputHeightSRV);
				m_idHeightMapBig.y = GameRenderDeviceHandle->AddTexture(inputHeightSRV);

				ID3D11ShaderResourceView *inputNormalSRV;
				GameStreamingDataHandle->GetTextureInputFromBYTE4(m_data->GetIDNormalMapBig(), GetIDNormalMapBig().x, &inputNormalSRV);
				m_idNormalMapBig.y = GameRenderDeviceHandle->AddTexture(inputNormalSRV);

			}

			if (level == 0)
			{
				int idE = GameStreamingDataHandle->LoadData(m_strE.str());
				void *VdE = GameStreamingDataHandle->GetStreamingData(idE);
				int VsE = GameStreamingDataHandle->GetSizeOfStreamingData(idE);

				int idT = GameStreamingDataHandle->LoadData(m_strT.str());
				void *VdT = GameStreamingDataHandle->GetStreamingData(idT);
				int VsT = GameStreamingDataHandle->GetSizeOfStreamingData(idT);

				GameStreamingDataHandle->SetTextureFromBYTE4Depository(m_data->GetIDEnviromentMapBig(), GetIDEnviromentMapBig().x, (VBYTE4*)VdE);
				GameStreamingDataHandle->DeleteStreamingData(idE);

				GameStreamingDataHandle->SetTextureFromBYTE4Depository(m_data->GetIDTreesMapBig(), GetIDTreesMapBig().x, (VBYTE4*)VdT);
				GameStreamingDataHandle->DeleteStreamingData(idT);

				ID3D11ShaderResourceView *inputEnviromentSRV;
				GameStreamingDataHandle->GetTextureInputFromBYTE4(m_data->GetIDEnviromentMapBig(), GetIDEnviromentMapBig().x, &inputEnviromentSRV);
				m_idEnviromentMapBig.y = GameRenderDeviceHandle->AddTexture(inputEnviromentSRV);

				ID3D11ShaderResourceView *inputTreesSRV;
				GameStreamingDataHandle->GetTextureInputFromBYTE4(m_data->GetIDTreesMapBig(), GetIDTreesMapBig().x, &inputTreesSRV);
				m_idTreesMapBig.y = GameRenderDeviceHandle->AddTexture(inputTreesSRV);
			}
			else if ((level % m_data->GetLoadDataPer()) == 0 && m_level < m_data->GetLoadDataMaxLvl())
			{
				int idRW1 = GameStreamingDataHandle->LoadData(m_strRW1.str());

				if (idRW1 < 0.0f)
				{
					m_isMap1 = false;
				}
				else
				{
					m_isMap1 = true;
				}

				int idRW2 = GameStreamingDataHandle->LoadData(m_strRW2.str());

				if (idRW2 < 0.0f)
				{
					m_isMap2 = false;
				}
				else
				{
					m_isMap2 = true;
				}

				if (m_isMap1 == true)
				{
					std::vector<float> test(sizeB);

					void *VdRW1 = GameStreamingDataHandle->GetStreamingData(idRW1);
					int VsRW1 = GameStreamingDataHandle->GetSizeOfStreamingData(idRW1);

					memcpy(&test[0], VdRW1, test.size());

					int idRL1 = GameStreamingDataHandle->LoadData(m_strRL1.str());
					void *VdRL1 = GameStreamingDataHandle->GetStreamingData(idRL1);
					int VsRL1 = GameStreamingDataHandle->GetSizeOfStreamingData(idRL1);

					memcpy(&test[0], VdRL1, test.size());

					int idRT1 = GameStreamingDataHandle->LoadData(m_strRT1.str());
					void *VdRT1 = GameStreamingDataHandle->GetStreamingData(idRT1);
					int VsRT1 = GameStreamingDataHandle->GetSizeOfStreamingData(idRT1);

					memcpy(&test[0], VdRT1, test.size());

					GameStreamingDataHandle->SetTextureFromFLOATDepository(m_data->GetIDRiverWidth1MapBig(), GetIDRiverWidth1MapBig().x, (float*)VdRW1);
					GameStreamingDataHandle->DeleteStreamingData(idRW1);

					GameStreamingDataHandle->SetTextureFromFLOATDepository(m_data->GetIDRiverLength1MapBig(), GetIDRiverLength1MapBig().x, (float*)VdRL1);
					GameStreamingDataHandle->DeleteStreamingData(idRL1);

					GameStreamingDataHandle->SetTextureFromFLOATDepository(m_data->GetIDRiverType1MapBig(), GetIDRiverType1MapBig().x, (float*)VdRT1);
					GameStreamingDataHandle->DeleteStreamingData(idRT1);

					ID3D11ShaderResourceView *inputRiverWidth1SRV;
					GameStreamingDataHandle->GetTextureInputFromFloat(m_data->GetIDRiverWidth1MapBig(), GetIDRiverWidth1MapBig().x, &inputRiverWidth1SRV);
					m_idRiverWidth1MapBig.y = GameRenderDeviceHandle->AddTexture(inputRiverWidth1SRV);

					ID3D11ShaderResourceView *inputRiverLength1SRV;
					GameStreamingDataHandle->GetTextureInputFromFloat(m_data->GetIDRiverLength1MapBig(), GetIDRiverLength1MapBig().x, &inputRiverLength1SRV);
					m_idRiverLength1MapBig.y = GameRenderDeviceHandle->AddTexture(inputRiverLength1SRV);

					ID3D11ShaderResourceView *inputRiverType1SRV;
					GameStreamingDataHandle->GetTextureInputFromFloat(m_data->GetIDRiverType1MapBig(), GetIDRiverType1MapBig().x, &inputRiverType1SRV);
					m_idRiverType1MapBig.y = GameRenderDeviceHandle->AddTexture(inputRiverType1SRV);
				}

				if (m_isMap2 == true)
				{
					void *VdRW2 = GameStreamingDataHandle->GetStreamingData(idRW2);
					int VsRW2 = GameStreamingDataHandle->GetSizeOfStreamingData(idRW2);

					int idRL2 = GameStreamingDataHandle->LoadData(m_strRL2.str());
					void *VdRL2 = GameStreamingDataHandle->GetStreamingData(idRL2);
					int VsRL2 = GameStreamingDataHandle->GetSizeOfStreamingData(idRL2);

					int idRT2 = GameStreamingDataHandle->LoadData(m_strRT2.str());
					void *VdRT2 = GameStreamingDataHandle->GetStreamingData(idRT2);
					int VsRT2 = GameStreamingDataHandle->GetSizeOfStreamingData(idRT2);

					GameStreamingDataHandle->SetTextureFromFLOATDepository(m_data->GetIDRiverWidth2MapBig(), GetIDRiverWidth2MapBig().x, (float*)VdRW2);
					GameStreamingDataHandle->DeleteStreamingData(idRW2);

					GameStreamingDataHandle->SetTextureFromFLOATDepository(m_data->GetIDRiverLength2MapBig(), GetIDRiverLength2MapBig().x, (float*)VdRL2);
					GameStreamingDataHandle->DeleteStreamingData(idRL2);

					GameStreamingDataHandle->SetTextureFromFLOATDepository(m_data->GetIDRiverType2MapBig(), GetIDRiverType2MapBig().x, (float*)VdRT2);
					GameStreamingDataHandle->DeleteStreamingData(idRT2);

					ID3D11ShaderResourceView *inputRiverWidth2SRV;
					GameStreamingDataHandle->GetTextureInputFromFloat(m_data->GetIDRiverWidth2MapBig(), GetIDRiverWidth2MapBig().x, &inputRiverWidth2SRV);
					m_idRiverWidth2MapBig.y = GameRenderDeviceHandle->AddTexture(inputRiverWidth2SRV);

					ID3D11ShaderResourceView *inputRiverLength2SRV;
					GameStreamingDataHandle->GetTextureInputFromFloat(m_data->GetIDRiverLength2MapBig(), GetIDRiverLength2MapBig().x, &inputRiverLength2SRV);
					m_idRiverLength2MapBig.y = GameRenderDeviceHandle->AddTexture(inputRiverLength2SRV);

					ID3D11ShaderResourceView *inputRiverType2SRV;
					GameStreamingDataHandle->GetTextureInputFromFloat(m_data->GetIDRiverType2MapBig(), GetIDRiverType2MapBig().x, &inputRiverType2SRV);
					m_idRiverType2MapBig.y = GameRenderDeviceHandle->AddTexture(inputRiverType2SRV);
				}

				if (m_isMap1 == true || m_isMap2 == true)
				{
					int idRH = GameStreamingDataHandle->LoadData(m_strRH.str());
					void *VdRH = GameStreamingDataHandle->GetStreamingData(idRH);
					int VsRH = GameStreamingDataHandle->GetSizeOfStreamingData(idRH);

					GameStreamingDataHandle->SetTextureFromFLOATDepository(m_data->GetIDRiverHeightMapBig(), GetIDRiverHeightMapBig().x, (float*)VdRH);
					GameStreamingDataHandle->DeleteStreamingData(idRH);

					ID3D11ShaderResourceView *inputRiverHeightSRV;
					GameStreamingDataHandle->GetTextureInputFromFloat(m_data->GetIDRiverHeightMapBig(), GetIDRiverHeightMapBig().x, &inputRiverHeightSRV);
					m_idRiverHeightMapBig.y = GameRenderDeviceHandle->AddTexture(inputRiverHeightSRV);
				}
			}
		}

		m_progress.z = 2.0f;
	}

	if (m_progress.z == 2.0f)
	{
		/*if (m_data->GetLoadTilesLvl() <= m_level)
		{
			m_planetTile = GamePlanetHelperHandle->GetPlanetTile(0)->CreteNewGamePlanetTileFrom(
				m_levelFromLoadTile, XMFLOAT2(m_coordFromLoadTile.x, m_coordFromLoadTile.y), 0.0f, 64, 64);
		}*/
		{
			float L2 = 1.0f / pow(2, m_levelFromLastLoadData);
			float H2 = 1.0f / (PIR + 2.0f);
			float W2 = 1.0f / (PIR + 2.0f);
			float OC = ((float)PIR) / (PIR + 2.0f);
			int idOutput;

			//float width = GameStreamingDataHandle->GetWidthFromFLOATDepository(m_data->GetIDHeightMapBig());
			//float height = GameStreamingDataHandle->GetHeightFromFLOATDepository(m_data->GetIDHeightMapBig());

			XMFLOAT2 OF = XMFLOAT2(m_coordFromLastLoadData.x * L2 * OC, m_coordFromLastLoadData.y * L2 * OC);
			XMFLOAT2 INC = XMFLOAT2(L2 * W2, L2 * H2);

			XMFLOAT2 I = XMFLOAT2(OF.x + W2 - INC.x, OF.y + H2 - INC.y);

			idOutput = GameRenderDeviceHandle->CreateOutputTexture(PIR + 2, PIR + 2, DXGI_FORMAT_R32_FLOAT, 1);

			CreateTextureFromTextureMessage messageT;
			messageT.m_Effect = Effects::GenerateFloatTexFromFloatTexFX;
			messageT.m_startPos = I;
			messageT.m_stepSize = INC;
			messageT.m_inputSRV = GameRenderDeviceHandle->GetTexture(GetIDHeightMapBig().y);
			messageT.m_outputUAV = GameRenderDeviceHandle->GetOutputTextureUAV(idOutput);
			messageT.m_Width = PIR + 2;
			messageT.m_Height = PIR + 2;
			messageT.Thread = XMFLOAT3(16, 16, 1);

			GameRenderDeviceHandle->ComputeShader(&messageT);

			m_idHeightMapInput = GameRenderDeviceHandle->DeleteOutputTextureButSRVToTextureStack(idOutput);

			if (m_isMap1)
			{
				idOutput = GameRenderDeviceHandle->CreateOutputTexture(PIR + 2, PIR + 2, DXGI_FORMAT_R32_FLOAT, 1);

				messageT.m_inputSRV = GameRenderDeviceHandle->GetTexture(GetIDRiverWidth1MapBig().y);
				messageT.m_outputUAV = GameRenderDeviceHandle->GetOutputTextureUAV(idOutput);

				GameRenderDeviceHandle->ComputeShader(&messageT);

				m_idRiverWidth1MapInput = GameRenderDeviceHandle->DeleteOutputTextureButSRVToTextureStack(idOutput);

				idOutput = GameRenderDeviceHandle->CreateOutputTexture(PIR + 2, PIR + 2, DXGI_FORMAT_R32_FLOAT, 1);

				messageT.m_inputSRV = GameRenderDeviceHandle->GetTexture(GetIDRiverLength1MapBig().y);
				messageT.m_outputUAV = GameRenderDeviceHandle->GetOutputTextureUAV(idOutput);

				GameRenderDeviceHandle->ComputeShader(&messageT);

				m_idRiverLength1MapInput = GameRenderDeviceHandle->DeleteOutputTextureButSRVToTextureStack(idOutput);

				idOutput = GameRenderDeviceHandle->CreateOutputTexture(PIR + 2, PIR + 2, DXGI_FORMAT_R32_FLOAT, 1);

				messageT.m_inputSRV = GameRenderDeviceHandle->GetTexture(GetIDRiverType1MapBig().y);
				messageT.m_outputUAV = GameRenderDeviceHandle->GetOutputTextureUAV(idOutput);

				GameRenderDeviceHandle->ComputeShader(&messageT);

				m_idRiverType1MapInput = GameRenderDeviceHandle->DeleteOutputTextureButSRVToTextureStack(idOutput);
			}

			if (m_isMap2)
			{
				idOutput = GameRenderDeviceHandle->CreateOutputTexture(PIR + 2, PIR + 2, DXGI_FORMAT_R32_FLOAT, 1);

				messageT.m_inputSRV = GameRenderDeviceHandle->GetTexture(GetIDRiverWidth2MapBig().y);
				messageT.m_outputUAV = GameRenderDeviceHandle->GetOutputTextureUAV(idOutput);

				GameRenderDeviceHandle->ComputeShader(&messageT);

				m_idRiverWidth2MapInput = GameRenderDeviceHandle->DeleteOutputTextureButSRVToTextureStack(idOutput);

				idOutput = GameRenderDeviceHandle->CreateOutputTexture(PIR + 2, PIR + 2, DXGI_FORMAT_R32_FLOAT, 1);

				messageT.m_inputSRV = GameRenderDeviceHandle->GetTexture(GetIDRiverLength2MapBig().y);
				messageT.m_outputUAV = GameRenderDeviceHandle->GetOutputTextureUAV(idOutput);

				GameRenderDeviceHandle->ComputeShader(&messageT);

				m_idRiverLength2MapInput = GameRenderDeviceHandle->DeleteOutputTextureButSRVToTextureStack(idOutput);

				idOutput = GameRenderDeviceHandle->CreateOutputTexture(PIR + 2, PIR + 2, DXGI_FORMAT_R32_FLOAT, 1);

				messageT.m_inputSRV = GameRenderDeviceHandle->GetTexture(GetIDRiverType2MapBig().y);
				messageT.m_outputUAV = GameRenderDeviceHandle->GetOutputTextureUAV(idOutput);

				GameRenderDeviceHandle->ComputeShader(&messageT);

				m_idRiverType2MapInput = GameRenderDeviceHandle->DeleteOutputTextureButSRVToTextureStack(idOutput);
			}

			if (m_isMap1 || m_isMap2)
			{
				idOutput = GameRenderDeviceHandle->CreateOutputTexture(PIR + 2, PIR + 2, DXGI_FORMAT_R32_FLOAT, 1);

				messageT.m_inputSRV = GameRenderDeviceHandle->GetTexture(GetIDRiverHeightMapBig().y);
				messageT.m_outputUAV = GameRenderDeviceHandle->GetOutputTextureUAV(idOutput);

				GameRenderDeviceHandle->ComputeShader(&messageT);

				m_idRiverHeightMapInput = GameRenderDeviceHandle->DeleteOutputTextureButSRVToTextureStack(idOutput);
			}


			idOutput = GameRenderDeviceHandle->CreateOutputTexture(PIR + 2, PIR + 2, DXGI_FORMAT_R8G8B8A8_UNORM, 1);

			messageT.m_Effect = Effects::GenerateBYTE4TexFromBYTE4TexFX;
			messageT.m_inputSRV = GameRenderDeviceHandle->GetTexture(GetIDNormalMapBig().y);
			messageT.m_outputUAV = GameRenderDeviceHandle->GetOutputTextureUAV(idOutput);

			GameRenderDeviceHandle->ComputeShader(&messageT);

			m_idNormalMapInput = GameRenderDeviceHandle->DeleteOutputTextureButSRVToTextureStack(idOutput);

			L2 = 1.0f / pow(2, m_level);
			OF = XMFLOAT2(m_coord.x * L2 * OC, m_coord.y * L2 * OC);
			INC = XMFLOAT2(L2 * W2, L2 * H2);
			I = XMFLOAT2(OF.x + W2 - INC.x, OF.y + H2 - INC.y);

			idOutput = GameRenderDeviceHandle->CreateOutputTexture(PIR + 2, PIR + 2, DXGI_FORMAT_R8G8B8A8_UNORM, 1);

			messageT.m_startPos = I;
			messageT.m_stepSize = INC;
			messageT.m_inputSRV = GameRenderDeviceHandle->GetTexture(GetIDEnviromentMapBig().y);
			messageT.m_outputUAV = GameRenderDeviceHandle->GetOutputTextureUAV(idOutput);

			GameRenderDeviceHandle->ComputeShader(&messageT);

			m_idEnviromentMapInput = GameRenderDeviceHandle->DeleteOutputTextureButSRVToTextureStack(idOutput);

			idOutput = GameRenderDeviceHandle->CreateOutputTexture(PIR + 2, PIR + 2, DXGI_FORMAT_R8G8B8A8_UNORM, 1);

			messageT.m_inputSRV = GameRenderDeviceHandle->GetTexture(GetIDTreesMapBig().y);
			messageT.m_outputUAV = GameRenderDeviceHandle->GetOutputTextureUAV(idOutput);

			GameRenderDeviceHandle->ComputeShader(&messageT);

			m_idTreesMapInput = GameRenderDeviceHandle->DeleteOutputTextureButSRVToTextureStack(idOutput);

			int idAngleMapOutput = GameRenderDeviceHandle->CreateOutputTexture(PIR + 2, PIR + 2, DXGI_FORMAT_R8G8B8A8_UNORM, 1);
			int idEnviromentMapOutput = GameRenderDeviceHandle->CreateOutputTexture(PIR + 2, PIR + 2, DXGI_FORMAT_R8G8B8A8_UNORM, 1);
			int idTreesMapOutput = GameRenderDeviceHandle->CreateOutputTexture(PIR + 2, PIR + 2, DXGI_FORMAT_R8G8B8A8_UNORM, 1);

			GenerateBlockOfLODMessage message;
			message.m_Effect = Effects::GenerateBlockOfLODFX;
			message.m_Height = PIR + 2;
			message.m_Width = PIR + 2;
			message.m_offset = -1.0f;
			message.m_scaling = 1.0f / PIR;
			message.m_inputHeightMapSRV = GameRenderDeviceHandle->GetTexture(m_idHeightMapInput);
			message.m_inputNormalMapSRV = GameRenderDeviceHandle->GetTexture(m_idNormalMapInput);
			message.m_inputEnviromentMapSRV = GameRenderDeviceHandle->GetTexture(m_idEnviromentMapInput);
			message.m_inputTreesMapSRV = GameRenderDeviceHandle->GetTexture(m_idTreesMapInput);
			message.m_tileSRV = GamePlanetHelperHandle->GetTileNoiseSRV();
			message.m_outputAngleMapUAV = GameRenderDeviceHandle->GetOutputTextureUAV(idAngleMapOutput);
			message.m_outputEnviromentMapUAV = GameRenderDeviceHandle->GetOutputTextureUAV(idEnviromentMapOutput);
			message.m_outputTreesMapUAV = GameRenderDeviceHandle->GetOutputTextureUAV(idTreesMapOutput);
			message.Thread = XMFLOAT3(16, 16, 1);

			GameRenderDeviceHandle->ComputeShader(&message);

			GameRenderDeviceHandle->DeleteTexture(m_idEnviromentMapInput);
			GameRenderDeviceHandle->DeleteTexture(m_idTreesMapInput);

			m_idAngleMapInput = GameRenderDeviceHandle->DeleteOutputTextureButSRVToTextureStack(idAngleMapOutput);
			m_idEnviromentMapInput = GameRenderDeviceHandle->DeleteOutputTextureButSRVToTextureStack(idEnviromentMapOutput);
			m_idTreesMapInput = GameRenderDeviceHandle->DeleteOutputTextureButSRVToTextureStack(idTreesMapOutput);
		}

		if ((level % m_data->GetLoadDataPer()) == 0 && m_level >= m_data->GetLoadDataMaxLvl() && m_level <= 8 && m_idHeightMapBig.y >= 0 && m_idNormalMapBig.y >= 0)
		{
			float L2 = 1.0f / pow(2, m_levelFromLastLoadData);
			int W = GameStreamingDataHandle->GetWidthFromFLOATDepository(m_data->GetIDHeightMapBig());
			int H = GameStreamingDataHandle->GetHeightFromFLOATDepository(m_data->GetIDHeightMapBig());
			float H2 = 1.0f / W;
			float W2 = 1.0f / H;
			float OC = ((float)(W - 1)) / ((W - 1) + 16.0f);
			int idOutputH;
			int idOutputN;

			XMFLOAT2 OF = XMFLOAT2(m_coordFromLastLoadData.x * L2 * OC, m_coordFromLastLoadData.y * L2 * OC);
			XMFLOAT2 INC = XMFLOAT2(L2 * W2, L2 * H2);

			XMFLOAT2 I = XMFLOAT2(OF.x + 8.0f * W2 - INC.x, OF.y + 8.0f * H2 - INC.y);

			idOutputH = GameRenderDeviceHandle->CreateOutputTexture(W, H, DXGI_FORMAT_R32_FLOAT, 1);
			idOutputN = GameRenderDeviceHandle->CreateOutputTexture(W, H, DXGI_FORMAT_R8G8B8A8_UNORM, 1);

			GenerateHeightAndNormalMapWithNoiseMessage messageT;
			messageT.m_Effect = Effects::GenerateHeightAndNormalMapWithNoiseEffectFX;
			messageT.m_StartPos = I;
			messageT.m_StepSize = INC;
			messageT.m_SpacingMap = 1.0f / H;
			messageT.m_SpacingWorld = S2 / H;
			messageT.m_RangeNoise = messageT.m_SpacingMap;
			messageT.m_IsMap1 = m_isMap1;
			messageT.m_IsMap2 = m_isMap2;
			messageT.m_inputHeightMapSRV = GameRenderDeviceHandle->GetTexture(m_idHeightMapBig.y);
			messageT.m_inputNormalMapSRV = GameRenderDeviceHandle->GetTexture(m_idNormalMapBig.y);
			messageT.m_inputRiverWidth1MapSRV = GameRenderDeviceHandle->GetTexture(m_idRiverWidth1MapBig.y);
			messageT.m_inputRiverWidth2MapSRV = GameRenderDeviceHandle->GetTexture(m_idRiverWidth2MapBig.y);
			messageT.m_inputRiverLength1MapSRV = GameRenderDeviceHandle->GetTexture(m_idRiverLength1MapBig.y);
			messageT.m_inputRiverLength2MapSRV = GameRenderDeviceHandle->GetTexture(m_idRiverLength2MapBig.y);
			messageT.m_inputRiverType1MapSRV = GameRenderDeviceHandle->GetTexture(m_idRiverType1MapBig.y);
			messageT.m_inputRiverType2MapSRV = GameRenderDeviceHandle->GetTexture(m_idRiverType2MapBig.y);
			messageT.m_inputRiverHeightMapSRV = GameRenderDeviceHandle->GetTexture(m_idRiverHeightMapBig.y);
			messageT.m_tileSRV = GamePlanetHelperHandle->GetTileNoiseSRV();
			messageT.m_outputHeightMapUAV = GameRenderDeviceHandle->GetOutputTextureUAV(idOutputH);
			messageT.m_outputNormalMapUAV = GameRenderDeviceHandle->GetOutputTextureUAV(idOutputN);
			messageT.m_Width = W;
			messageT.m_Height = H;
			messageT.Thread = XMFLOAT3(16, 16, 1);

			GameRenderDeviceHandle->ComputeShader(&messageT);

			m_idHeightMapBig.y = GameRenderDeviceHandle->DeleteOutputTextureButSRVToTextureStack(idOutputH);
			m_idNormalMapBig.y = GameRenderDeviceHandle->DeleteOutputTextureButSRVToTextureStack(idOutputN);

			m_scalingFromLastLoadData = 1.0f;
			m_levelFromLastLoadData = 0;
			m_coordFromLastLoadData = XMINT2(0, 0);
		}

		m_progress.y = 0.0f;

		m_progress.z = 4.0f;
	}

	if (m_progress.z == 4.0f)
	{
		/*m_MaxT = btVector3(m_position.getX() + 0.5 * S2, S, m_position.getZ() + 0.5 * S2).normalize();
		m_MinT = btVector3(m_position.getX() - 0.5 * S2, S, m_position.getZ() - 0.5 * S2).normalize();
		m_MaxW = m_MaxT;
		m_MinW = m_MinT;
		m_MaxC = m_MaxT;
		m_MinC = m_MinT;
		m_MaxA = m_MaxT;
		m_MinA = m_MinT;

		btScalar HLP1 = ((m_data->GetRadiusOfTerrain() + m_MaxH) * m_MaxT).getY();
		btScalar HLP2 = ((m_data->GetRadiusOfTerrain() + m_MaxH) * m_MinT).getY();

		if (HLP1 > HLP2)
		{
			m_MaxT.setY(HLP1);
		}
		else
		{
			m_MaxT.setY(HLP2);
		}

		btScalar HLP1 = ((m_data->GetRadiusOfTerrain() + m_MinH) * m_MaxT).getY();
		btScalar HLP2 = ((m_data->GetRadiusOfTerrain() + m_MinH) * m_MinT).getY();

		if (HLP1 < HLP2)
		{
			m_MinT.setY(HLP1);
		}
		else
		{
			m_MinT.setY(HLP2);
		}*/

		m_MaxT = btVector3(-100000000.0, -100000000.0, -100000000.0);
		m_MinT = btVector3(100000000.0, 100000000.0, 100000000.0);
		m_MaxW = btVector3(-100000000.0, -100000000.0, -100000000.0);
		m_MinW = btVector3(100000000.0, 100000000.0, 100000000.0);
		m_MaxC = btVector3(-100000000.0, -100000000.0, -100000000.0);
		m_MinC = btVector3(100000000.0, 100000000.0, 100000000.0);
		m_MaxA = btVector3(-100000000.0, -100000000.0, -100000000.0);
		m_MinA = btVector3(100000000.0, 100000000.0, 100000000.0);

		{
			btVector3 CUB[] = { btVector3(m_position.getX() + 0.5 * S2, -S, m_position.getZ() + 0.5 * S2).normalize(),
				btVector3(m_position.getX() + 0.5 * S2, -S, m_position.getZ() - 0.5 * S2).normalize(),
				btVector3(m_position.getX() - 0.5 * S2, -S, m_position.getZ() + 0.5 * S2).normalize(),
				btVector3(m_position.getX() - 0.5 * S2, -S, m_position.getZ() - 0.5 * S2).normalize(),
				btVector3(m_position.getX() + 0.5 * S2, -S, m_position.getZ() + 0.5 * S2).normalize(),
				btVector3(m_position.getX() + 0.5 * S2, -S, m_position.getZ() - 0.5 * S2).normalize(),
				btVector3(m_position.getX() - 0.5 * S2, -S, m_position.getZ() + 0.5 * S2).normalize(),
				btVector3(m_position.getX() - 0.5 * S2, -S, m_position.getZ() - 0.5 * S2).normalize() };

			for (int i = 0; i < 4; ++i)
			{
				CUB[i] = -1 * ((m_data->GetRadiusOfWater() + m_MaxH) * CUB[i]) * m_data->GetBlockAnglMatrix(m_side);
			}

			for (int i = 4; i < 8; ++i)
			{
				CUB[i] = ((m_data->GetRadiusOfWater() + m_MinH) * CUB[i]) * m_data->GetBlockAnglMatrix(m_side);
			}


			for (int i = 0; i < 8; ++i)
			{
				if (CUB[i].getX() > m_MaxT.getX()) m_MaxT.setX(CUB[i].getX());
				if (CUB[i].getY() > m_MaxT.getY()) m_MaxT.setY(CUB[i].getY());
				if (CUB[i].getZ() > m_MaxT.getZ()) m_MaxT.setZ(CUB[i].getZ());

				if (CUB[i].getX() < m_MinT.getX()) m_MinT.setX(CUB[i].getX());
				if (CUB[i].getY() < m_MinT.getY()) m_MinT.setY(CUB[i].getY());
				if (CUB[i].getZ() < m_MinT.getZ()) m_MinT.setZ(CUB[i].getZ());
			}
		}

		{
			btVector3 CUB[] = { btVector3(m_position.getX() + 0.5 * S2, -S, m_position.getZ() + 0.5 * S2).normalize(),
				btVector3(m_position.getX() + 0.5 * S2, -S, m_position.getZ() - 0.5 * S2).normalize(),
				btVector3(m_position.getX() - 0.5 * S2, -S, m_position.getZ() + 0.5 * S2).normalize(),
				btVector3(m_position.getX() - 0.5 * S2, -S, m_position.getZ() - 0.5 * S2).normalize() };

			for (int i = 0; i < 4; ++i)
			{
				CUB[i] = ((m_data->GetRadiusOfWater()) * CUB[i]) * m_data->GetBlockAnglMatrix(m_side);

				if (CUB[i].getX() > m_MaxW.getX()) m_MaxW.setX(CUB[i].getX());
				if (CUB[i].getY() > m_MaxW.getY()) m_MaxW.setY(CUB[i].getY());
				if (CUB[i].getZ() > m_MaxW.getZ()) m_MaxW.setZ(CUB[i].getZ());

				if (CUB[i].getX() < m_MinW.getX()) m_MinW.setX(CUB[i].getX());
				if (CUB[i].getY() < m_MinW.getY()) m_MinW.setY(CUB[i].getY());
				if (CUB[i].getZ() < m_MinW.getZ()) m_MinW.setZ(CUB[i].getZ());
			}
		}

		{
			btVector3 CUB[] = { btVector3(m_position.getX() + 0.5 * S2, -S, m_position.getZ() + 0.5 * S2).normalize(),
				btVector3(m_position.getX() + 0.5 * S2, -S, m_position.getZ() - 0.5 * S2).normalize(),
				btVector3(m_position.getX() - 0.5 * S2, -S, m_position.getZ() + 0.5 * S2).normalize(),
				btVector3(m_position.getX() - 0.5 * S2, -S, m_position.getZ() - 0.5 * S2).normalize() };

			for (int i = 0; i < 4; ++i)
			{
				CUB[i] =((m_data->GetRadiusOfClouds()) * CUB[i]) * m_data->GetBlockAnglMatrix(m_side);

				if (CUB[i].getX() > m_MaxC.getX()) m_MaxC.setX(CUB[i].getX());
				if (CUB[i].getY() > m_MaxC.getY()) m_MaxC.setY(CUB[i].getY());
				if (CUB[i].getZ() > m_MaxC.getZ()) m_MaxC.setZ(CUB[i].getZ());

				if (CUB[i].getX() < m_MinC.getX()) m_MinC.setX(CUB[i].getX());
				if (CUB[i].getY() < m_MinC.getY()) m_MinC.setY(CUB[i].getY());
				if (CUB[i].getZ() < m_MinC.getZ()) m_MinC.setZ(CUB[i].getZ());
			}
		}

		{
			btVector3 CUB[] = { btVector3(m_position.getX() + 0.5 * S2, -S, m_position.getZ() + 0.5 * S2).normalize(),
				btVector3(m_position.getX() + 0.5 * S2, -S, m_position.getZ() - 0.5 * S2).normalize(),
				btVector3(m_position.getX() - 0.5 * S2, -S, m_position.getZ() + 0.5 * S2).normalize(),
				btVector3(m_position.getX() - 0.5 * S2, -S, m_position.getZ() - 0.5 * S2).normalize() };

			for (int i = 0; i < 4; ++i)
			{
				CUB[i] = ((m_data->GetRadiusOfAtmosphere()) * CUB[i]) * m_data->GetBlockAnglMatrix(m_side);

				if (CUB[i].getX() > m_MaxA.getX()) m_MaxA.setX(CUB[i].getX());
				if (CUB[i].getY() > m_MaxA.getY()) m_MaxA.setY(CUB[i].getY());
				if (CUB[i].getZ() > m_MaxA.getZ()) m_MaxA.setZ(CUB[i].getZ());

				if (CUB[i].getX() < m_MinA.getX()) m_MinA.setX(CUB[i].getX());
				if (CUB[i].getY() < m_MinA.getY()) m_MinA.setY(CUB[i].getY());
				if (CUB[i].getZ() < m_MinA.getZ()) m_MinA.setZ(CUB[i].getZ());
			}
		}

		btVector3 offsetT = 0.5 * (m_MaxT - m_MinT);
		m_OffsetCubeT = offsetT;
		m_CentreT = m_MinT + offsetT;

		btVector3 offsetW = 0.5 * (m_MaxW - m_MinW);
		m_OffsetCubeW = offsetW;
		m_CentreW = m_MinW + offsetW;

		btVector3 offsetA = 0.5 * (m_MaxA - m_MinA);
		m_OffsetCubeA = offsetA;
		m_CentreA = m_MinA + offsetA;

		btVector3 offsetC = 0.5 * (m_MaxC - m_MinC);
		m_OffsetCubeC = offsetC;
		m_CentreC = m_MinC + offsetC;

		if (m_isMap)
		{

			/*m_idHeightMapSRT = GameRenderDeviceHandle->CreateTexture(
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
					m_data->GetIDTreesMapSmall()), DXGI_FORMAT_R8G8B8A8_UNORM, 1);*/
		}

		m_progress = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

		if (m_isMap == true && m_level == m_data->GetGenerateTreesLvl())
		{
			m_progress.z = 5.0f;
		}
		else
		{
			return true;
		}
	}

	if (m_progress.z == 5.0f)
	{
		m_treesVertices.resize(GamePlanetHelperHandle->GetTreesTilesSize());
		m_treesIndices.resize(GamePlanetHelperHandle->GetTreesIndexSize());

		m_posArray.resize(GamePlanetHelperHandle->GetTreesTilesSize());
		m_posCounter.setZero();

		m_treesOffset = btVector3(double(coord.x) * S2 - S * 0.5, S * 0.5, double(coord.y) * S2 - S * 0.5);

		int m_index = 0;
		m_treesIndices.at(0) = 0;

		m_progress.z = 6.0f;
	}

	if (m_progress.z == 6.0f)
	{
		m_progress.w = 0.0f;

		float TS = 1.0f / pow(2.0f, (float)m_data->GetGenerateTreesLvl() - (m_data->GetLoadDataMaxLvl() - ((m_data->GetLoadDataMaxLvl() - 1) % m_data->GetLoadDataPer() + 1)));

		/*for (; m_progress.x < 256; ++m_progress.x)
		{
			for (; m_progress.y < GamePlanetHelperHandle->GetTreesIndex(m_progress.x + 1) - GamePlanetHelperHandle->GetTreesIndex(m_progress.x); ++m_progress.y)
			{
				if (m_progress.w > 512.0f) return false;

				XMFLOAT2 pos1 = GamePlanetHelperHandle->GetTreesTiles(GamePlanetHelperHandle->GetTreesIndex(m_progress.x) + m_progress.y);
				XMFLOAT2 pos2 = XMFLOAT2(TS * ((float)m_coordFromLastLoadData.x + pos1.x), TS * ((float)m_coordFromLastLoadData.y + pos1.y));

				VBYTE4 N4 = GameStreamingDataHandle->GetValueFromBYTE4Depository(m_data->GetIDNormalMapBig(), GetIDNormalMapBig(), pos2.x, pos2.y);

				XMVECTOR D = XMVector3Dot(XMVector3Normalize(XMLoadFloat3(&XMFLOAT3((float)N4.x - 127.5f, (float)N4.y - 127.5f, (float)N4.z - 127.5f))),
					XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f)));
				XMFLOAT3 D3;
				XMStoreFloat3(&D3, D);
				float A = acos(D3.x) * 0.63661977f;

				if (A < 0.4f)
				{
					float H = GameStreamingDataHandle->GetValueFromFLOATDepository(m_data->GetIDHeightMapBig(), GetIDHeightMapBig(), pos2.x, pos2.y);

					if (H > 0.0f)
					{
						btVector3 pos64 = btVector3(
							m_treesOffset.getX() + pos1.x * S2, m_treesOffset.getY(), m_treesOffset.getZ() + pos1.y * S2);

						btVector3 dir = pos64.normalize();

						pos64 = dir * (m_data->GetRadiusOfTerrain() + H);

						m_posArray[m_index] = pos64;
						m_posCounter += pos64;

						m_treesVertices.at(m_index).Direct = XMFLOAT3(dir.getX(), dir.getY(), dir.getZ());
						m_treesVertices.at(m_index).Size = XMFLOAT2(2.5f, 5.0f);

						++m_index;
					}
				}
				m_progress.w += 1;
			}

			m_treesIndices.at(m_progress.x + 1) = m_index;

			m_progress.y = 0.0f;
		}*/

		m_progress = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

		m_progress.z = 7.0f;
	}

	if (m_progress.z == 7.0f)
	{
		m_treesVertices.resize(m_index);

		m_posCounter /= (btScalar)m_index;

		m_progress.z = 8.0f;
	}

	if (m_progress.z == 8.0f)
	{
		for (int i = 0; i < m_treesVertices.size(); ++i)
		{
			m_treesVertices.at(i).Position = XMFLOAT3(m_posArray[i].getX() - m_posCounter.getX(), m_posArray[i].getY() - m_posCounter.getY(), m_posArray[i].getZ() - m_posCounter.getZ());
		}

		m_progress.z = 9.0f;
	}

	if (m_progress.z == 9.0f)
	{
		if (m_treesVertices.size() != 0)
		{
			m_billBoardMatrix = btTransform(btTransform(m_data->GetBlockAnglMatrix(m_side), btVector3(0.0, 0.0, 0.0)) * btTransform(btMatrix3x3::getIdentity(), m_posCounter));

			Model *model = new Model;

			model->idMeshBuffer = GameRenderDeviceHandle->CreateMeshBuffer(&m_treesVertices[0], sizeof(Vertex::Billboard), m_treesVertices.size(), NULL);

			std::vector<D3D11_INPUT_ELEMENT_DESC> *billboard = new std::vector<D3D11_INPUT_ELEMENT_DESC>(3);

			billboard->at(0) = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			billboard->at(1) = { "POSITION", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			billboard->at(2) = { "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 };

			model->idVertex = GameRenderDeviceHandle->CreateVertex(billboard);
			model->idEffect = GameRenderDeviceHandle->CreateEffect(Effects::BillboardFX);
			model->idInputLayouts = GameRenderDeviceHandle->CreateInputLayouts(model->idVertex, model->idEffect);
			model->faceStart = 0;
			model->faceCount = m_treesVertices.size();
			model->topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
			model->sizeOfVertex = sizeof(Vertex::Billboard);
			model->material.Ambient.x = 1.0f;
			model->material.Ambient.y = 1.0f;
			model->material.Ambient.z = 1.0f;
			model->material.Ambient.w = 1.0f;
			model->material.Diffuse.x = 1.0f;
			model->material.Diffuse.x = 1.0f;
			model->material.Diffuse.y = 1.0f;
			model->material.Diffuse.z = 1.0f;
			model->material.Specular.w = 1.0f;
			model->material.Specular.y = 1.0f;
			model->material.Specular.z = 1.0f;
			model->material.Specular.w = 1.0f;
			model->material.Reflect.x = 0.0f;
			model->material.Reflect.y = 0.0f;
			model->material.Reflect.z = 0.0f;
			model->material.Reflect.w = 0.0f;

			m_idBillboardTrees = GameRenderDeviceHandle->CreateModel(model);
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

	float scalingFromLoadTile = 1.0f;
	int levelFromLoadTile = 0;
	XMINT2 coordFromLoadTile[] = { XMINT2(0, 0),XMINT2(0, 0),XMINT2(0, 0),XMINT2(0, 0) };

	if (m_data->GetLoadTilesLvl() != m_level && m_level != 0)
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

	PlanetLODInitMeessage initMessage;

	initMessage.master = m_data;
	initMessage.side = m_side;
	initMessage.level = m_level + 1;
	initMessage.scaling = 0.5 * m_scaling;
	initMessage.levelFromLastLoadData = m_levelFromLastLoadData + 1;
	initMessage.scalingFromLastLoadData = 0.5f * m_scalingFromLastLoadData;
	initMessage.levelFromLoadTile = levelFromLoadTile;
	initMessage.scalingFromLoadTile = scalingFromLoadTile;
	initMessage.isMap = m_isMap;
	initMessage.isMap1 = m_isMap1;
	initMessage.isMap2 = m_isMap2;
	initMessage.idHeightMapBig = m_idHeightMapBig;
	initMessage.idNormalMapBig = m_idNormalMapBig;
	initMessage.idEnviromentMapBig = m_idEnviromentMapBig;
	initMessage.idTreesMapBig = m_idTreesMapBig;
	initMessage.idRiverWidth1MapBig = m_idRiverWidth1MapBig;
	initMessage.idRiverWidth2MapBig = m_idRiverWidth2MapBig;
	initMessage.idRiverLength1MapBig = m_idRiverLength1MapBig;
	initMessage.idRiverLength2MapBig = m_idRiverLength2MapBig;
	initMessage.idRiverType1MapBig = m_idRiverType1MapBig;
	initMessage.idRiverType2MapBig = m_idRiverType2MapBig;
	initMessage.idRiverHeightMapBig = m_idRiverHeightMapBig;
	initMessage.idBillboardTreesVertex = m_idBillboardTreesVertex;
	initMessage.billboardTreesIndex = m_billboardTreesIndex;
	initMessage.billboardMatrix = m_billBoardMatrix;
	initMessage.idBillboardTrees = m_idBillboardTrees;
	initMessage.maxH = m_MaxH;
	initMessage.minH = m_MinH;

	initMessage.coord = XMINT2(2 * m_coord.x, 2 * m_coord.y);
	initMessage.position = pos[3];
	initMessage.coordFromLastLoadData = XMINT2(2 * m_coordFromLastLoadData.x, 2 * m_coordFromLastLoadData.y);
	initMessage.coordFromLoadTile = coordFromLoadTile[0];

	m_blocks[0]->Init(initMessage);

	initMessage.coord = XMINT2(2 * m_coord.x, 2 * m_coord.y + 1);
	initMessage.position = pos[2];
	initMessage.coordFromLastLoadData = XMINT2(2 * m_coordFromLastLoadData.x, 2 * m_coordFromLastLoadData.y + 1);
	initMessage.coordFromLoadTile = coordFromLoadTile[1];


	m_blocks[1]->Init(initMessage);

	initMessage.coord = XMINT2(2 * m_coord.x + 1, 2 * m_coord.y);
	initMessage.position = pos[1];
	initMessage.coordFromLastLoadData = XMINT2(2 * m_coordFromLastLoadData.x + 1, 2 * m_coordFromLastLoadData.y);
	initMessage.coordFromLoadTile = coordFromLoadTile[2];


	m_blocks[2]->Init(initMessage);

	initMessage.coord = XMINT2(2 * m_coord.x + 1, 2 * m_coord.y + 1);
	initMessage.position = pos[0];
	initMessage.coordFromLastLoadData = XMINT2(2 * m_coordFromLastLoadData.x + 1, 2 * m_coordFromLastLoadData.y + 1);
	initMessage.coordFromLoadTile = coordFromLoadTile[3];

	m_blocks[3]->Init(initMessage);

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
}