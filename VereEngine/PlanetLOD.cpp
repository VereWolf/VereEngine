#include "pch.h"
#include "PlanetLOD.h"
#include "PlanetData.h"
#include "GameComponent.h"
#include "RenderDevice.h"

bool PlanetLOD::m_onlyRenderText = false;

void PlanetLOD::Init(PlanetData * data, int side, int level, XMINT2 coord, btVector3 position, btScalar scaling,
	int levelFromLastLoadData, XMINT2 coordFromLastLoadData, btScalar scalingFromLastLoadData,
	int levelFromLoadTile, XMINT2 coordFromLoadTile, btScalar scalingFromLoadTile,
	VereTextureFloat *hMap = NULL, VereTextureBYTE4 *nMap = NULL, VereTextureBYTE4 *eMap = NULL, VereTextureBYTE4 *tMap = NULL)
{
	m_data = data;
	m_side = side;
	m_level = level;
	m_coord = coord;
	m_scaling = scaling;
	m_position = position;
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

	while (GetId() > m_data->m_planetElements.size() && GetId() != -1) m_data->m_planetElements.resize(m_data->m_planetElements.size() + 1024);
	m_data->m_planetElements[GetId()] = this;

	if (m_level != 0)
	{
		m_hMap = hMap;
		m_nMap = nMap;
		m_eMap = eMap;
		m_tMap = tMap;
	}

	m_heightMap = NULL;
	m_normalMap = NULL;
	m_enviromentMap = NULL;
	m_treesMap = NULL;

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

		if (m_isNewLevel == false && distance >= 0.0 && distance < 2.0 && m_level < m_data->GetCurrentMaxLevel() && m_isCreateNewLevelInProcess == 0)
		{
			m_data->m_planetElementsInProcess.GiveElement(GetId());
			m_isCreateNewLevelInProcess = 1;
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
	if (m_idNormalMap >= 0)
	{
		message.m_EnviromentSRV = GameRenderDeviceHandle->GetTexture(m_idEnviromentMap);
	}
	else
	{
		return;
	}
	if (m_idNormalMap >= 0)
	{
		message.m_TreesSRV = GameRenderDeviceHandle->GetTexture(m_idTreesMap);
	}
	else
	{
		return;
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

	btTransform mesh = btTransform(btMatrix3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0), btVector3(0.0, 0.5, 0.0));
	mesh = btTransform(m_data->GetBlockAnglMatrix(side), btVector3(0.0, 0.0, 0.0)) * mesh;

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

	if ((level % m_data->GetLoadDataPer()) == 0 && m_level < m_data->GetLoadDataMaxLvl())
	{
		m_levelFromLastLoadData = 0;
		m_coordFromLastLoadData = XMINT2(0, 0);

		m_hMap = new VereTextureFloat;
		m_nMap = new VereTextureBYTE4;
		m_eMap = new VereTextureBYTE4;
		m_tMap = new VereTextureBYTE4;

		int L = 512 + 2 * 512 / m_data->GetNumPointInRowInCell();
		int MS = pow(L, 2);

		std::stringstream strH;
		std::stringstream strN;

		if (level == 0)
		{
			strH << m_data->GetFullPath() << "height_" << m_data->GetPosFix(side) << ".raw";
			strN << m_data->GetFullPath() << "normal_" << m_data->GetPosFix(side) << ".raw";
		}
		else
		{
			strH << m_data->GetFullPath() << "height_" << m_data->GetPosFix(side) << "_" << level << "_" << coord.x << "_" << coord.y << ".raw";
			strN << m_data->GetFullPath() << "normal_" << m_data->GetPosFix(side) << "_" << level << "_" << coord.x << "_" << coord.y << ".raw";
		}

		int idH = GameStreamingDataHandle->LoadData(strH.str());
		void *VdH = GameStreamingDataHandle->GetStreamingData(idH);
		int VsH = GameStreamingDataHandle->GetSizeOfStreamingData(idH);

		int idN = GameStreamingDataHandle->LoadData(strN.str());
		void *VdN = GameStreamingDataHandle->GetStreamingData(idN);
		int VsN = GameStreamingDataHandle->GetSizeOfStreamingData(idN);

		std::vector<VBYTE4> eMap(MS, VBYTE4(0, 0, 0, 0));
		int idE = GameStreamingDataHandle->CreateStreamingData(&eMap[0], MS);
		void *VdE = GameStreamingDataHandle->GetStreamingData(idE);
		int VsE = GameStreamingDataHandle->GetSizeOfStreamingData(idE);

		std::vector<VBYTE4> tMap(MS, VBYTE4(0, 0, 0, 0));
		int idT = GameStreamingDataHandle->CreateStreamingData(&tMap[0], MS);
		void *VdT = GameStreamingDataHandle->GetStreamingData(idT);
		int VsT = GameStreamingDataHandle->GetSizeOfStreamingData(idT);

		void *VB4 = new BYTE[4 * MS];

		for (int i = 0; i < MS; ++i)
		{

			((BYTE*)VB4)[4 * i] = (BYTE)(((XMFLOAT3*)VdN)[i].x * 255.0f);
			((BYTE*)VB4)[4 * i + 1] = (BYTE)(((XMFLOAT3*)VdN)[i].y * 255.0f);
			((BYTE*)VB4)[4 * i + 2] = (BYTE)(((XMFLOAT3*)VdN)[i].z * 255.0f);
			((BYTE*)VB4)[4 * i + 3] = 0;
		}

		m_hMap->Init(VdH, L, L);

		GameStreamingDataHandle->DeleteStreamingData(idH);

		m_nMap->Init(VB4, L, L);

		GameStreamingDataHandle->DeleteStreamingData(idN);

		m_eMap->Init(VdE, L, L);

		GameStreamingDataHandle->DeleteStreamingData(idE);

		m_tMap->Init(VdT, L, L);

		GameStreamingDataHandle->DeleteStreamingData(idT);

		delete VB4;
	}

	if (m_data->GetLoadTilesLvl() <= m_level)
	{
		m_planetTile = GamePlanetHelperHandle->GetPlanetTile(0)->CreteNewGamePlanetTileFrom(
			m_levelFromLoadTile, XMFLOAT2(m_coordFromLoadTile.x, m_coordFromLoadTile.y), 0.0f, 64, 64);
	}

	m_heightMap = m_hMap->CreateNewTextureFrom(m_levelFromLastLoadData, XMFLOAT2(m_coordFromLastLoadData.x, m_coordFromLastLoadData.y), 2.0f, PIR, PIR);
	m_normalMap = m_nMap->CreateNewTextureFrom(m_levelFromLastLoadData, XMFLOAT2(m_coordFromLastLoadData.x, m_coordFromLastLoadData.y), 2.0f, PIR, PIR);
	m_enviromentMap = m_eMap->CreateNewTextureFrom(m_levelFromLastLoadData, XMFLOAT2(m_coordFromLastLoadData.x, m_coordFromLastLoadData.y), 2.0f, PIR, PIR);
	m_treesMap = m_tMap->CreateNewTextureFrom(m_levelFromLastLoadData, XMFLOAT2(m_coordFromLastLoadData.x, m_coordFromLastLoadData.y), 2.0f, PIR, PIR);

	for (float y = 0; y < PIR + 2; y += 1)
	{
		for (float x = 0; x < PIR + 2; x += 1)
		{
			VBYTE4 B4 = m_normalMap->GetVariable(x * PIRR, y * PIRR);
			XMVECTOR N = XMVector3Normalize(XMLoadFloat3(&XMFLOAT3((float)B4.x - 127.0f, (float)B4.y - 127.0f, (float)B4.z - 127.0f)));
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
				((VBYTE4*)m_enviromentMap->GetTexture())[(int)y * (PIR + 2) + (int)x].z = 0;
				((VBYTE4*)m_enviromentMap->GetTexture())[(int)y * (PIR + 2) + (int)x].w = 255;
			}
			else if (rad < 35.0f)
			{
				((VBYTE4*)m_enviromentMap->GetTexture())[(int)y * (PIR + 2) + (int)x].z = 1;
				((VBYTE4*)m_enviromentMap->GetTexture())[(int)y * (PIR + 2) + (int)x].w = 25.5f * (35.0f - rad);
			}
			else if (rad < 45.0f)
			{
				((VBYTE4*)m_enviromentMap->GetTexture())[(int)y * (PIR + 2) + (int)x].z = 2;
				((VBYTE4*)m_enviromentMap->GetTexture())[(int)y * (PIR + 2) + (int)x].w = 25.5f * (rad - 35.0f);
			}
			else
			{
				((VBYTE4*)m_enviromentMap->GetTexture())[(int)y * (PIR + 2) + (int)x].z = 3;
				((VBYTE4*)m_enviromentMap->GetTexture())[(int)y * (PIR + 2) + (int)x].w = 0;
			}

			XMMATRIX TNB = XMMATRIX(T, N, B, Z);

			float px = VereMath::Clamp((x - 1) / (PIR - 1), 0.0f, 1.0f);
			float py = VereMath::Clamp((y - 1) / (PIR - 1), 0.0f, 1.0f);

			if (m_data->GetLoadTilesLvl() <= m_level)
			{
				XMFLOAT4 N4 = m_planetTile->GetNormal(XMFLOAT2(px, py), 1.0f - dotF);

				N = XMVector3Transform(XMVector3Normalize(XMLoadFloat3(&XMFLOAT3(N4.x, N4.y, N4.z))), TNB);

				XMStoreFloat4(&N4, N);

				float H = m_heightMap->GetVariable((x) * PIRR, (y) * PIRR) + 30.0f * m_planetTile->GetHeight(XMFLOAT2(px, py), dotF);

				m_heightMap->SetVariable((int)x, (int)y, H);
				m_normalMap->SetVariable((int)x, (int)y,
					VBYTE4((BYTE)((1.0f + N4.x) * 127.0f), (BYTE)((1.0f + N4.y) * 127.0f), (BYTE)((1.0f + N4.z) * 127.0f), 0));
			}

			px = VereMath::Clamp(x - 1, 0, PIR - 1);
			py = VereMath::Clamp(y - 1, 0, PIR - 1);

			btScalar D = 1.0 / (PIR - 1);
			btVector3 V;
			V.setX(((btScalar)coord.x + D * px) * scaling - 0.5);
			V.setY(0.0);
			V.setZ(((btScalar)coord.y + D * py) * scaling - 0.5);

			btVector3 VT, VW, VA, VC;
			VT = V;
			VT = ((mesh * V).normalize()) * (S + m_heightMap->GetVariable((px + 1) * PIRR, (py + 1) * PIRR));
			VW = V;
			VW = ((mesh * V).normalize()) * (S + m_heightMap->GetVariable((px + 1) * PIRR, (py + 1) * PIRR));
			VA = V;
			VA = ((mesh * V).normalize()) * (S + m_heightMap->GetVariable((px + 1) * PIRR, (py + 1) * PIRR));
			VC = V;
			VC = ((mesh * V).normalize()) * (S + m_heightMap->GetVariable((px + 1) * PIRR, (py + 1) * PIRR));

			MaxT = VereMath::MaxVector(MaxT, VT);
			MinT = VereMath::MinVector(MinT, VT);

			MaxW = VereMath::MaxVector(MaxW, VW);
			MinW = VereMath::MinVector(MinW, VW);

			MaxA = VereMath::MaxVector(MaxA, VA);
			MinA = VereMath::MinVector(MinA, VA);

			MaxC = VereMath::MaxVector(MaxC, VC);
			MinC = VereMath::MinVector(MinC, VC);
		}
	}

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

	m_idHeightMap = GameRenderDeviceHandle->CreateTexture(m_heightMap->GetTexture(), m_heightMap->GetHeight(), m_heightMap->GetWidth(), DXGI_FORMAT_R32_FLOAT, 1);
	m_idNormalMap = GameRenderDeviceHandle->CreateTexture(m_normalMap->GetTexture(), m_normalMap->GetHeight(), m_normalMap->GetWidth(), DXGI_FORMAT_R8G8B8A8_UNORM, 1);
	m_idEnviromentMap = GameRenderDeviceHandle->CreateTexture(m_enviromentMap->GetTexture(), m_enviromentMap->GetHeight(), m_enviromentMap->GetWidth(), DXGI_FORMAT_R8G8B8A8_UNORM, 1);
	m_idTreesMap = GameRenderDeviceHandle->CreateTexture(m_treesMap->GetTexture(), m_treesMap->GetHeight(), m_treesMap->GetWidth(), DXGI_FORMAT_R8G8B8A8_UNORM, 1);

	return true;
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

	if (m_data->GetLoadTilesLvl() <= m_level)
	{
		scalingFromLoadTile *= 0.f;
		levelFromLoadTile += 1;

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
		levelFromLoadTile, coordFromLoadTile[0], scalingFromLoadTile,
		m_hMap, m_nMap, m_eMap, m_tMap);
	m_blocks[1]->Init(m_data, m_side, m_level + 1, XMINT2(2 * m_coord.x, 2 * m_coord.y + 1), pos[2], 0.5 * m_scaling,
		m_levelFromLastLoadData + 1, XMINT2(2 * m_coordFromLastLoadData.x, 2 * m_coordFromLastLoadData.y + 1), 0.5 * m_scalingFromLastLoadData,
		levelFromLoadTile, coordFromLoadTile[1], scalingFromLoadTile,
		m_hMap, m_nMap, m_eMap, m_tMap);
	m_blocks[2]->Init(m_data, m_side, m_level + 1, XMINT2(2 * m_coord.x + 1, 2 * m_coord.y), pos[1], 0.5 * m_scaling,
		m_levelFromLastLoadData + 1, XMINT2(2 * m_coordFromLastLoadData.x + 1, 2 * m_coordFromLastLoadData.y), 0.5 * m_scalingFromLastLoadData,
		levelFromLoadTile, coordFromLoadTile[2], scalingFromLoadTile,
		m_hMap, m_nMap, m_eMap, m_tMap);
	m_blocks[3]->Init(m_data, m_side, m_level + 1, XMINT2(2 * m_coord.x + 1, 2 * m_coord.y + 1), pos[0], 0.5 * m_scaling,
		m_levelFromLastLoadData + 1, XMINT2(2 * m_coordFromLastLoadData.x + 1, 2 * m_coordFromLastLoadData.y + 1), 0.5 * m_scalingFromLastLoadData,
		levelFromLoadTile, coordFromLoadTile[3], scalingFromLoadTile,
		m_hMap, m_nMap, m_eMap, m_tMap);

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