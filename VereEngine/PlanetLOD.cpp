#include "pch.h"
#include "PlanetLOD.h"
#include "PlanetData.h"
#include "GameBaseObject.h"
#include "GameComponent.h"
#include "Vertex.h"
#include "Effects.h"

bool TerrainPlanetLOD::m_onlyRenderText = false;

void TerrainPlanetLOD::Init(TerrainPlanetData * data, int side, int level, XMINT2 coord, btVector3 position, btScalar scaling, VereTextureFloat *hMap = NULL, VereTextureBYTE4 *nMap = NULL)
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
	m_data->m_planetElements[GetId()] = this;

	if (m_level != 0)
	{
		m_hMap = hMap;
		m_nMap = nMap;
	}
	
	if (GameRenderDeviceHandle->GetModel(data->GetRenderId()))
	{
		Model *model = new Model;
		memcpy(model, GameRenderDeviceHandle->GetModel(data->GetRenderId()), sizeof(Model));
		m_modelID = GameRenderDeviceHandle->CreateModel(model);
	}
}

void TerrainPlanetLOD::Render(btTransform camOffset, XMMATRIX camView, XMMATRIX camProj,
	float camFarZ, btScalar heightFar, btScalar aspect,
	float camFarRangeMod, float camModifier)
{
	btMatrix3x3 I = btMatrix3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);
	btScalar S = m_scaling;
	btScalar S2 = m_data->GetScaling().getRow(0).getX();
	btScalar S3 = S * S2;

	btTransform AM = btTransform(m_data->GetBlockAnglMatrix(m_side), btVector3(0.0, 0.0, 0.0));
	btTransform GT = m_data->GetWorldTransform();
	{
		btTransform M = camOffset * GT;

		btScalar distance = (M * m_Centre).length();
		if (m_isCreateNewLevelInProcess == true)
		{
			if (m_blocks.size() == 4 && m_blocks[0].GetProccessed() == false && m_blocks[1].GetProccessed() == false
				&& m_blocks[2].GetProccessed() == false && m_blocks[3].GetProccessed() == false)
			{
				m_isCreateNewLevelInProcess = false;
			}
		}

		if (m_blocks.size() == 0 && distance < 1.35 * S3 && m_level < m_data->GetMaxLevel() && m_isCreateNewLevelInProcess == false)
		{
			m_data->m_planetElementsInProcess.GiveElement(GetId());
		}
		else if (m_blocks.size() != 0 && distance > 1.4 * S3)
		{
			m_blocks.resize(0);
		}

	}
	if (m_blocks.size() == 4 && m_isCreateNewLevelInProcess == false)
	{
		m_blocks[0].Render(camOffset, camView, camProj, camFarZ, heightFar, aspect, camFarRangeMod, camModifier);
		m_blocks[1].Render(camOffset, camView, camProj, camFarZ, heightFar, aspect, camFarRangeMod, camModifier);
		m_blocks[2].Render(camOffset, camView, camProj, camFarZ, heightFar, aspect, camFarRangeMod, camModifier);
		m_blocks[3].Render(camOffset, camView, camProj, camFarZ, heightFar, aspect, camFarRangeMod, camModifier);
	}

	if (m_blocks.size() == 0 || m_blocks.size() == 4 && m_isCreateNewLevelInProcess == true)
	{
		XMFLOAT4X4 VW;
		XMStoreFloat4x4(&VW, camView);
		btTransform VW2 = VereMath::ConvertTobtTransform(VW);
		btTransform meshCentre = camOffset * GT;
		btTransform Mat = VW2 * meshCentre/* * Mat*/;

		btVector3 Vecd = Mat * m_Centre;

		if (VereMath::FrustrumCulling2(m_Centre, m_OffsetCube, camFarZ, heightFar, aspect, Mat))
		{
			PlanetRenderMessage message;

			message.m_ModelID = m_modelID;
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
			message.m_Radius = m_data->GetRadius();
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

			//if (m_side == 0 || m_side == 1 || m_side == 2)
			{
				message.m_RasterizeState = RenderStates::SolidRS;
			}
			/*else
			{
				message.m_RasterizeState = RenderStates::BackRS;
			}*/
			//message.m_RasterizeState = RenderStates::WireframeRS;

			if (m_onlyRenderText == false)
			{

				GameRenderDeviceHandle->Render(&message);

				m_data->GetDeviceResources()->GetD3DDeviceContext()->HSSetShader(0, 0, 0);
				m_data->GetDeviceResources()->GetD3DDeviceContext()->DSSetShader(0, 0, 0);
			}
			else
			{

				std::wstring text = L"Cull: " + std::to_wstring(Vecd.getX()) + L" " + std::to_wstring(Vecd.getY()) + L" " + std::to_wstring(Vecd.getZ());

				TextMessage textMessage;
				textMessage.colorBrushID = GameTextRenderDeviceHandle->CreateColorBrush(D2D1::ColorF(D2D1::ColorF::Blue));
				textMessage.textFormatID = 0;
				textMessage.textLayoutID = GameTextRenderDeviceHandle->CreateTextLayout(text, 0, 900.0f, 50.0f);
				textMessage.position = XMFLOAT2(0.0f, GameTextRenderDeviceHandle->GetOffset());
				GameTextRenderDeviceHandle->Render(&textMessage);
				GameTextRenderDeviceHandle->DeleteColorBrush(textMessage.colorBrushID);
				GameTextRenderDeviceHandle->DeleteTextLayout(textMessage.textLayoutID);

				GameTextRenderDeviceHandle->NextStepOfOffset(0.015);
			}
		}
	}
}

bool TerrainPlanetLOD::ComponentProccess()
{
	TerrainPlanetData *data = m_data;
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
	btVector3 Max = btVector3(NMIN, NMIN, NMIN);
	btVector3 Min = btVector3(NMAX, NMAX, NMAX);

	if (level == 0)
	{
		m_hMap = new VereTextureFloat;
		m_nMap = new VereTextureBYTE4;

		int L = 512 + 2 * 512 / m_data->GetNumPointInRowInCell();
		int MS = pow(L, 2);
		int a;
		//string posfix = m_data->GetPosFix(side);
		//posfix.resize(m_data->GetPosFix(side)->Length());
		//for (int i = 0; i < m_data->GetPosFix(side)->Length(); ++i)wctomb_s(&a, &posfix[i], sizeof(char), *(m_data->GetPosFix(side)->Begin() + i));

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
			V = ((mesh * V).normalize()) * (S + m_heightMap->GetVariable((x + 1) * PIRR, (y + 1) * PIRR));

			Max = VereMath::MaxVector(Max, V);
			Min = VereMath::MinVector(Min, V);
		}
	}

	btVector3 offset = 0.5 * (Max - Min);
	m_OffsetCube = offset;
	m_Centre = Min + offset;

	m_idHeightMap = GameRenderDeviceHandle->CreateTexture(m_heightMap->GetTexture(), m_heightMap->GetHeight(), m_heightMap->GetWidth(), DXGI_FORMAT_R32_FLOAT, 1);
	m_idNormalMap = GameRenderDeviceHandle->CreateTexture(m_normalMap->GetTexture(), m_normalMap->GetHeight(), m_normalMap->GetWidth(), DXGI_FORMAT_R8G8B8A8_UNORM, 1);

	return true;
}


void TerrainPlanetLOD::CreateNewLevelOfLoD()
{
	btScalar S = m_scaling;
	btScalar S2 = m_data->GetScaling().getRow(0).getX();
	btScalar S3 = S * S2;

	m_blocks.resize(4);

	m_blocks[0].SetProccessed(true);
	m_blocks[1].SetProccessed(true);
	m_blocks[2].SetProccessed(true);
	m_blocks[3].SetProccessed(true);

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

	m_blocks[0].Init(m_data, m_side, m_level + 1, XMINT2(2 * m_coord.x, 2 * m_coord.y), pos[3], 0.5 * m_scaling, m_hMap, m_nMap);
	m_blocks[1].Init(m_data, m_side, m_level + 1, XMINT2(2 * m_coord.x, 2 * m_coord.y + 1), pos[2], 0.5 * m_scaling, m_hMap, m_nMap);
	m_blocks[2].Init(m_data, m_side, m_level + 1, XMINT2(2 * m_coord.x + 1, 2 * m_coord.y), pos[1], 0.5 * m_scaling, m_hMap, m_nMap);
	m_blocks[3].Init(m_data, m_side, m_level + 1, XMINT2(2 * m_coord.x + 1, 2 * m_coord.y + 1), pos[0], 0.5 * m_scaling, m_hMap, m_nMap);

	GameComponentsManagerHandle->InsertToProccess(&m_blocks[0]);
	GameComponentsManagerHandle->InsertToProccess(&m_blocks[1]);
	GameComponentsManagerHandle->InsertToProccess(&m_blocks[2]);
	GameComponentsManagerHandle->InsertToProccess(&m_blocks[3]);
}