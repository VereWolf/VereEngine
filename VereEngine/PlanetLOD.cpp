#include "pch.h"
#include "PlanetLOD.h"
#include "PlanetData.h"
#include "GameComponent.h"
#include "RenderDevice.h"

bool PlanetLOD::m_onlyRenderText = false;

void PlanetLOD::Init(PlanetData * data, int side, int level, XMINT2 coord, btVector3 position, btScalar scaling, VereTextureFloat *hMap = NULL, VereTextureBYTE4 *nMap = NULL)
{
	m_data = data;
	m_side = side;
	m_level = level;
	m_coord = coord;
	m_scaling = scaling;
	m_position = position;
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
	}

	m_heightMap = NULL;
	m_normalMap = NULL;

	if (GameRenderDeviceHandle->GetModel(data->GetRenderId()))
	{
		Model *model = new Model;
		memcpy(model, GameRenderDeviceHandle->GetModel(data->GetRenderId()), sizeof(Model));
		m_modelID = GameRenderDeviceHandle->CreateModel(model);
	}
}

void PlanetLOD::Render(btTransform camOffset, XMMATRIX camView, XMMATRIX camProj,
	float camFarZ, btScalar heightFar, btScalar aspect,
	float camFarRangeMod, float camModifier)
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
		btTransform M = camOffset * GT;

		//btScalar distance = (M * m_CentreT).length();
		btVector3 coord = PlanetCordinateMat::GetCoordForCube(dir,
			btTransform(m_data->GetBlockAnglMatrix(m_side))) * (1.0 / S + 1.0) - btVector3(0.5 * S, 0.5 * S, 0.0);
		
		btScalar distance = -1.0;
		if (coord.getZ() == 0.0)
		{
			distance = btVector3(coord.getX() - m_coord.x, coord.getY() - m_coord.y, 0.0).length();
		}

		if (m_isCreateNewLevelInProcess == true)
		{
			if (m_isNewLevel == false && m_blocks[0]->GetProccessed() == 2 && m_blocks[1]->GetProccessed() == 2
				&& m_blocks[2]->GetProccessed() == 2 && m_blocks[3]->GetProccessed() == 2)
			{
				m_isCreateNewLevelInProcess = false;
				m_isNewLevel = true;
			}
		}

		if (m_isNewLevel == false && distance != -1 && distance < 1.7 && m_level < m_data->GetMaxLevel() && m_isCreateNewLevelInProcess == false)
		{
			m_data->m_planetElementsInProcess.GiveElement(GetId());
			SetProccessed(1);
		}
		else if (m_isNewLevel == true && (distance > 1.8 || distance == -1))
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
	}

	if (m_isNewLevel == true)
	{
		m_blocks[0]->Render(camOffset, camView, camProj, camFarZ, heightFar, aspect, camFarRangeMod, camModifier);
		m_blocks[1]->Render(camOffset, camView, camProj, camFarZ, heightFar, aspect, camFarRangeMod, camModifier);
		m_blocks[2]->Render(camOffset, camView, camProj, camFarZ, heightFar, aspect, camFarRangeMod, camModifier);
		m_blocks[3]->Render(camOffset, camView, camProj, camFarZ, heightFar, aspect, camFarRangeMod, camModifier);
	}

	if (m_isNewLevel == false)
	{
		XMFLOAT4X4 VW;
		XMStoreFloat4x4(&VW, camView);
		btTransform VW2 = VereMath::ConvertTobtTransform(VW);
		btTransform meshCentre = camOffset * GT;
		btTransform Mat = VW2 * meshCentre/* * Mat*/;

		btVector3 Vecd = Mat * m_CentreT;

		if (VereMath::FrustrumCulling2(m_CentreT, m_OffsetCubeT, camFarZ, heightFar, aspect, Mat))
		{
			DrawTerrain(camOffset, camView, camProj, camFarZ, heightFar, aspect, camFarRangeMod, camModifier);
		}

		if (VereMath::FrustrumCulling2(m_CentreW, m_OffsetCubeW, camFarZ, heightFar, aspect, Mat))
		{
			DrawWater(camOffset, camView, camProj, camFarZ, heightFar, aspect, camFarRangeMod, camModifier);
		}

		if (VereMath::FrustrumCulling2(m_CentreA, m_OffsetCubeA, camFarZ, heightFar, aspect, Mat))
		{
			DrawAtmosphere(camOffset, camView, camProj, camFarZ, heightFar, aspect, camFarRangeMod, camModifier);
		}

		if (VereMath::FrustrumCulling2(m_CentreC, m_OffsetCubeC, camFarZ, heightFar, aspect, Mat))
		{
			DrawClouds(camOffset, camView, camProj, camFarZ, heightFar, aspect, camFarRangeMod, camModifier);
		}

		m_data->GetDeviceResources()->GetD3DDeviceContext()->HSSetShader(0, 0, 0);
		m_data->GetDeviceResources()->GetD3DDeviceContext()->DSSetShader(0, 0, 0);
	}
}

void PlanetLOD::DrawTerrain(btTransform camOffset, XMMATRIX camView, XMMATRIX camProj,
	float camFarZ, btScalar heightFar, btScalar aspect,
	float camFarRangeMod, float camModifier)
{
	btMatrix3x3 I = btMatrix3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);
	btScalar S = m_scaling;
	btScalar S2 = m_data->GetScaling().getRow(0).getX();
	btScalar S3 = S * S2;

	btTransform AM = btTransform(m_data->GetBlockAnglMatrix(m_side), btVector3(0.0, 0.0, 0.0));
	btTransform GT = m_data->GetWorldTransform();


	TerrainRenderMessage message;

	message.m_ModelID = m_data->GetRenderId();
	message.m_Scaling = btTransform(btMatrix3x3(S2, 0.0, 0.0, 0.0, S2, 0.0, 0.0, 0.0, S2), btVector3(0.0, 0.0, 0.0));
	message.m_Transform = m_data->GetWorldTransform();
	message.m_CameraOffset = camOffset;
	message.m_View = camView;
	message.m_Proj = camProj;
	message.m_FarZ = camFarZ;
	message.m_FarRangeMod = camFarRangeMod;
	message.m_FarModifier = camModifier;
	message.m_Position = m_position;
	message.m_AngleMatrix = AM;
	message.m_Coord = m_coord;
	message.m_Spacing1 = S;
	message.m_Spacing2 = S2;
	message.m_Radius = m_data->GetRadiusOfTerrain();
	message.m_Level = m_level;
	message.m_Tangent = m_data->GetTangent();

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
	message.m_ViewPort = GameRenderDeviceHandle->GetMainViewPort();
	message.m_RasterizeState = RenderStates::SolidRS;
	message.m_BlendState = RenderStates::NoBlendBS;

	GameRenderDeviceHandle->Render(&message);
}

void PlanetLOD::DrawWater(btTransform camOffset, XMMATRIX camView, XMMATRIX camProj,
	float camFarZ, btScalar heightFar, btScalar aspect,
	float camFarRangeMod, float camModifier)
{
	btMatrix3x3 I = btMatrix3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);
	btScalar S = m_scaling;
	btScalar S2 = m_data->GetScaling().getRow(0).getX();
	btScalar S3 = S * S2;

	btTransform AM = btTransform(m_data->GetBlockAnglMatrix(m_side), btVector3(0.0, 0.0, 0.0));
	btTransform GT = m_data->GetWorldTransform();


	WaterRenderMessage message;

	message.m_ModelID = m_data->GetRenderIdWater();
	message.m_Scaling = btTransform(btMatrix3x3(S2, 0.0, 0.0, 0.0, S2, 0.0, 0.0, 0.0, S2), btVector3(0.0, 0.0, 0.0));
	message.m_Transform = m_data->GetWorldTransform();
	message.m_CameraOffset = camOffset;
	message.m_View = camView;
	message.m_Proj = camProj;
	message.m_FarZ = camFarZ;
	message.m_FarRangeMod = camFarRangeMod;
	message.m_FarModifier = camModifier;
	message.m_Position = m_position;
	message.m_AngleMatrix = AM;
	message.m_Coord = m_coord;
	message.m_Spacing1 = S;
	message.m_Spacing2 = S2;
	message.m_Radius = m_data->GetRadiusOfWater();
	message.m_Level = m_level;
	message.m_ViewPort = GameRenderDeviceHandle->GetMainViewPort();
	message.m_RasterizeState = RenderStates::NoCullRS;
	message.m_BlendState = RenderStates::NoBlendBS;

	GameRenderDeviceHandle->BindRenderTarget(m_data->GetWaterTargetMapRTV(), m_data->GetWaterDeepMapDSV());
	GameRenderDeviceHandle->Render(&message);
	GameRenderDeviceHandle->BindMainRenderTarget();
}

void PlanetLOD::DrawAtmosphere(btTransform camOffset, XMMATRIX camView, XMMATRIX camProj,
	float camFarZ, btScalar heightFar, btScalar aspect,
	float camFarRangeMod, float camModifier)
{
	btMatrix3x3 I = btMatrix3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);
	btScalar S = m_scaling;
	btScalar S2 = m_data->GetScaling().getRow(0).getX();
	btScalar S3 = S * S2;

	btTransform AM = btTransform(m_data->GetBlockAnglMatrix(m_side), btVector3(0.0, 0.0, 0.0));
	btTransform GT = m_data->GetWorldTransform();


	AtmosphereRenderMessage message;

	message.m_ModelID = m_data->GetRenderIdAtmosphere();
	message.m_Scaling = btTransform(btMatrix3x3(S2, 0.0, 0.0, 0.0, S2, 0.0, 0.0, 0.0, S2), btVector3(0.0, 0.0, 0.0));
	message.m_Transform = m_data->GetWorldTransform();
	message.m_CameraOffset = camOffset;
	message.m_View = camView;
	message.m_Proj = camProj;
	message.m_FarZ = camFarZ;
	message.m_FarRangeMod = camFarRangeMod;
	message.m_FarModifier = camModifier;
	message.m_Position = m_position;
	message.m_AngleMatrix = AM;
	message.m_Coord = m_coord;
	message.m_Spacing1 = S;
	message.m_Spacing2 = S2;
	message.m_Radius = m_data->GetRadiusOfAtmosphere();
	message.m_Level = m_level;
	message.m_ViewPort = GameRenderDeviceHandle->GetMainViewPort();
	message.m_RasterizeState = RenderStates::NoCullRS;
	message.m_BlendState = RenderStates::NoBlendBS;

	GameRenderDeviceHandle->BindRenderTarget(m_data->GetAtmosphereTargetMapRTV(), m_data->GetAtmosphereDeepMapDSV());
	GameRenderDeviceHandle->Render(&message);
	GameRenderDeviceHandle->BindMainRenderTarget();
}

void PlanetLOD::DrawClouds(btTransform camOffset, XMMATRIX camView, XMMATRIX camProj,
	float camFarZ, btScalar heightFar, btScalar aspect,
	float camFarRangeMod, float camModifier)
{
	btMatrix3x3 I = btMatrix3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);
	btScalar S = m_scaling;
	btScalar S2 = m_data->GetScaling().getRow(0).getX();
	btScalar S3 = S * S2;

	btTransform AM = btTransform(m_data->GetBlockAnglMatrix(m_side), btVector3(0.0, 0.0, 0.0));
	btTransform GT = m_data->GetWorldTransform();


	CloudsRenderMessage message;

	message.m_ModelID = m_data->GetRenderIdClouds();
	message.m_Scaling = btTransform(btMatrix3x3(S2, 0.0, 0.0, 0.0, S2, 0.0, 0.0, 0.0, S2), btVector3(0.0, 0.0, 0.0));
	message.m_Transform = m_data->GetWorldTransform();
	message.m_CameraOffset = camOffset;
	message.m_View = camView;
	message.m_Proj = camProj;
	message.m_FarZ = camFarZ;
	message.m_FarRangeMod = camFarRangeMod;
	message.m_FarModifier = camModifier;
	message.m_Position = m_position;
	message.m_AngleMatrix = AM;
	message.m_Coord = m_coord;
	message.m_Spacing1 = S;
	message.m_Spacing2 = S2;
	message.m_Radius = m_data->GetRadiusOfClouds();
	message.m_Level = m_level;
	message.m_ViewPort = GameRenderDeviceHandle->GetMainViewPort();
	message.m_RasterizeState = RenderStates::NoCullRS;
	message.m_BlendState = RenderStates::NoBlendBS;

	GameRenderDeviceHandle->BindRenderTarget(m_data->GetCloudsTargetMapRTV(), m_data->GetCloudsDeepMapDSV());
	GameRenderDeviceHandle->Render(&message);
	GameRenderDeviceHandle->BindMainRenderTarget();
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
	float PIRR = 1.0f / ((float)PIR + 2.0f);

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

	if (level == 0)
	{
		m_hMap = new VereTextureFloat;
		m_nMap = new VereTextureBYTE4;

		int L = 512 + 2 * 512 / m_data->GetNumPointInRowInCell();
		int MS = pow(L, 2);

		int idH = GameStreamingDataHandle->LoadData("height_" + m_data->GetPosFix(side) + ".raw");
		void *Vd = GameStreamingDataHandle->GetStreamingData(idH);
		int Vs = GameStreamingDataHandle->GetSizeOfStreamingData(idH);

		m_hMap->Init(Vd, L, L);

		GameStreamingDataHandle->DeleteStreamingData(idH);

		int idN = GameStreamingDataHandle->LoadData("normal_" + m_data->GetPosFix(side) + ".raw");
		Vd = GameStreamingDataHandle->GetStreamingData(idN);
		Vs = GameStreamingDataHandle->GetSizeOfStreamingData(idH);

		void *VB4 = new BYTE[4 * MS];

		for (int i = 0; i < MS; ++i)
		{
			((BYTE*)VB4)[4 * i] = (BYTE)(((XMFLOAT3*)Vd)[i].x * 255.0f);
			((BYTE*)VB4)[4 * i + 1] = (BYTE)(((XMFLOAT3*)Vd)[i].y * 255.0f);
			((BYTE*)VB4)[4 * i + 2] = (BYTE)(((XMFLOAT3*)Vd)[i].z * 255.0f);
			((BYTE*)VB4)[4 * i + 3] = 0;
		}

		GameStreamingDataHandle->DeleteStreamingData(idN);

		m_nMap->Init(VB4, L, L);

		delete VB4;
	}

	m_heightMap = m_hMap->CreateNewTextureFrom(level, XMFLOAT2(coord.x, coord.y), PIR, PIR);
	m_normalMap = m_nMap->CreateNewTextureFrom(level, XMFLOAT2(coord.x, coord.y), PIR, PIR);

	for (btScalar y = 0; y < PIR; y += 1)
	{
		for (btScalar x = 0; x < PIR; x += 1)
		{
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

	m_isCreateNewLevelInProcess = true;

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

	m_blocks[0]->Init(m_data, m_side, m_level + 1, XMINT2(2 * m_coord.x, 2 * m_coord.y), pos[3], 0.5 * m_scaling, m_hMap, m_nMap);
	m_blocks[1]->Init(m_data, m_side, m_level + 1, XMINT2(2 * m_coord.x, 2 * m_coord.y + 1), pos[2], 0.5 * m_scaling, m_hMap, m_nMap);
	m_blocks[2]->Init(m_data, m_side, m_level + 1, XMINT2(2 * m_coord.x + 1, 2 * m_coord.y), pos[1], 0.5 * m_scaling, m_hMap, m_nMap);
	m_blocks[3]->Init(m_data, m_side, m_level + 1, XMINT2(2 * m_coord.x + 1, 2 * m_coord.y + 1), pos[0], 0.5 * m_scaling, m_hMap, m_nMap);

	GameComponentsManagerHandle->InsertToProccess(m_blocks[0]);
	GameComponentsManagerHandle->InsertToProccess(m_blocks[1]);
	GameComponentsManagerHandle->InsertToProccess(m_blocks[2]);
	GameComponentsManagerHandle->InsertToProccess(m_blocks[3]);
}