#include "pch.h"
#include "PlanetLOD.h"
#include "Atmosphere.h"
#include "PlanetData.h"
#include "GameObject.h"
#include "Planet.h"

void QuadScreenWCAMessage::Use()
{
	((QuadScreenWCA*)m_BaseEffect)->SetView(m_View);
	((QuadScreenWCA*)m_BaseEffect)->SetMainDepthMap(m_MainDepthSRV);
	((QuadScreenWCA*)m_BaseEffect)->SetMainTargetMap(m_MainTargetSRV);
	((QuadScreenWCA*)m_BaseEffect)->SetWaterTopDepthMap(m_WaterTopDepthSRV);
	((QuadScreenWCA*)m_BaseEffect)->SetWaterTopTargetMap(m_WaterTopTargetSRV);
	((QuadScreenWCA*)m_BaseEffect)->SetWaterBottomDepthMap(m_WaterBottomDepthSRV);
	((QuadScreenWCA*)m_BaseEffect)->SetWaterBottomTargetMap(m_WaterBottomTargetSRV);
	((QuadScreenWCA*)m_BaseEffect)->SetCloudsDepthMap(m_CloudsDepthSRV);
	((QuadScreenWCA*)m_BaseEffect)->SetCloudsTargetMap(m_CloudsTargetSRV);
	((QuadScreenWCA*)m_BaseEffect)->SetAtmosphereDepthMap(m_AtmosphereDepthSRV);
	((QuadScreenWCA*)m_BaseEffect)->SetAtmosphereTargetMap(m_AtmosphereTargetSRV);
	((QuadScreenWCA*)m_BaseEffect)->SetDepth(m_Depth);
	((QuadScreenWCA*)m_BaseEffect)->SetSize(m_Size);
	((QuadScreenWCA*)m_BaseEffect)->SetWaterRatio(m_WaterRatio);
	((QuadScreenWCA*)m_BaseEffect)->SetCloudsRatio(m_CloudsRatio);
}

Planet::Planet()
{
	m_interator = 0;
	m_CP = btVector3(0.0, 0.0, 0.0);
	m_PP = btVector3(0.0, 0.0, 0.0);
}

Planet::Planet(Planet & other)
{

}
void Planet::Init()
{
	GameObjectSpace::Init();
	PlanetData::Init();
}
void Planet::Render()
{
	m_resources->GetD3DDeviceContext()->ClearRenderTargetView(GetAtmosphereTargetMapRTV(), DirectX::Colors::Transparent);
	m_resources->GetD3DDeviceContext()->ClearRenderTargetView(GetCloudsTargetMapRTV(), DirectX::Colors::Transparent);
	m_resources->GetD3DDeviceContext()->ClearRenderTargetView(GetWaterTopTargetMapRTV(), DirectX::Colors::Transparent);
	m_resources->GetD3DDeviceContext()->ClearRenderTargetView(GetWaterBottomTargetMapRTV(), DirectX::Colors::Transparent);
	m_resources->GetD3DDeviceContext()->ClearRenderTargetView(GetPlanetTargetMapRTV(), DirectX::Colors::Transparent);
	m_resources->GetD3DDeviceContext()->ClearDepthStencilView(GetAtmosphereDeepMapDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_resources->GetD3DDeviceContext()->ClearDepthStencilView(GetCloudsDeepMapDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_resources->GetD3DDeviceContext()->ClearDepthStencilView(GetWaterTopDeepMapDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_resources->GetD3DDeviceContext()->ClearDepthStencilView(GetWaterBottomDeepMapDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_resources->GetD3DDeviceContext()->ClearDepthStencilView(GetPlanetDeepMapDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_PP = m_CP;
	m_CP = GameObjectStackHandle->GetMainCamera()->GetLocalPosition();

	btVector3 CH = m_CP - m_PP;

	SetCurrentMaxLevel(VereMath::Clamp(GetMaxLevel() - 5 * CH.length() * GetScaling().getRow(0).getX() / pow(GetMaxLevel(), 2), 1, GetMaxLevel()));

	if (m_planetElementsInProcess.GetSize() > 0)
	{
		int id = m_planetElementsInProcess.TakeElement();

		if (m_planetElements[id])
		{
			((PlanetLOD*)m_planetElements[id])->CreateNewLevelOfLoD();
		}
	}
	m_resources->GetD3DDeviceContext()->RSSetState(RenderStates::SolidRS);

	float blendFactor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	for (int i = 0; i < 6; ++i)
	{
		m_PlanetLOD[i].Render();
	}

	GameComponentsManagerHandle->DeleteUselessElement(KT_TERRAIN_LOD0, KT_TERRAIN_LOD2);

	for (int i = 0; i < 12; ++i)
	{
		if (m_interator >= m_planetElementID->GetRangeOfElement()) m_interator = 0;

		if (m_planetElements[m_interator] != NULL)
		{
			if (m_planetElements[m_interator]->IsDestroy() == true)
			{
				if (m_planetElements[m_interator]->GetProccessed() == 2)
				{
					m_planetElementID->ReturnElement(m_interator);
					SafeDelete(m_planetElements[m_interator]);
				}
			}
		}
	}

	DrawWCA();
	DrawPlanet();
}

void Planet::DrawWCA()
{
	XMMATRIX world(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	QuadScreenWCAMessage message;

	message.m_ModelID = m_RenderIdWCAQuadScreen;
	message.m_MainDepthSRV = GameRenderDeviceHandle->GetMainDeepMapSRV();
	message.m_MainTargetSRV = GameRenderDeviceHandle->GetMainTargetMapSRV();
	message.m_WaterTopDepthSRV = GetWaterTopDeepMapSRV();
	message.m_WaterTopTargetSRV = GetWaterTopTargetMapSRV();
	message.m_WaterBottomDepthSRV = GetWaterBottomDeepMapSRV();
	message.m_WaterBottomTargetSRV = GetWaterBottomTargetMapSRV();
	message.m_CloudsDepthSRV = GetCloudsDeepMapSRV();
	message.m_CloudsTargetSRV = GetCloudsTargetMapSRV();
	message.m_AtmosphereDepthSRV = GetAtmosphereDeepMapSRV();
	message.m_AtmosphereTargetSRV = GetAtmosphereTargetMapSRV();
	message.m_View = world;
	btTransform T = GameObjectStackHandle->GetMainCamera()->btOffset() * GetWorldTransform();

	btScalar D = GameObjectStackHandle->GetMainCamera()->GetLook().dot(T.getOrigin().normalize());
	if (D < 0.0f)
	{
		message.m_Depth = -BUFFERUNIT * T.getOrigin().length();
	}
	else
	{
		message.m_Depth = BUFFERUNIT * T.getOrigin().length();
	}
	message.m_Size = BUFFERUNIT * GetRadiusOfAtmosphere();
	message.m_WaterRatio = GetRadiusOfWater() / GetRadiusOfAtmosphere();
	message.m_CloudsRatio = GetRadiusOfClouds() / GetRadiusOfAtmosphere();
	message.m_ViewPort = GameRenderDeviceHandle->GetMainViewPort();
	message.m_RasterizeState = RenderStates::NoCullRS;
	message.m_BlendState = RenderStates::TransparentBS;

	GameRenderDeviceHandle->BindRenderTarget(GetPlanetTargetMapRTV(), GetPlanetDeepMapDSV());
	GameRenderDeviceHandle->Render(&message);
	GameRenderDeviceHandle->BindMainRenderTarget();
}

void Planet::DrawPlanet()
{

	XMMATRIX world(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	RenderToScreenMessage message;

	message.m_ModelID = GameRenderDeviceHandle->GetRenderIdRTS();
	message.m_TargetSRV = GetPlanetTargetMapSRV();
	message.m_DepthSRV = GetPlanetDeepMapSRV();
	message.m_View = world;
	message.m_ViewPort = GameRenderDeviceHandle->GetMainViewPort();
	message.m_RasterizeState = RenderStates::NoCullRS;
	message.m_BlendState = RenderStates::TransparentBS;

	GameRenderDeviceHandle->BindMainRenderTarget();

	GameRenderDeviceHandle->Render(&message);
}

void Planet::BuildPlanet(std::string planetPath, int cellSize, int maxLevel, int loadDataMaxLvl, int loadTilesLvl, int loadDataPer,
	XMFLOAT3 fogColor, XMFLOAT3 waterColor, float waterDeep)
{
	m_planetPath = planetPath;

	SetNumPointInRowInCell(cellSize);
	SetMaxLevel(maxLevel);
	SetLoadDataMaxLvl(loadDataMaxLvl);
	SetLoadTilesLvl(loadTilesLvl);
	SetLoadDataPer(loadDataPer);

	SetFogAColor(fogColor);
	SetFogWColor(waterColor);
	SetFogWRange(waterDeep);

	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> *terrainLOD = new std::vector<D3D11_INPUT_ELEMENT_DESC>(2);
		terrainLOD->at(0) = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		terrainLOD->at(1) = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };

		Model *model = new Model;
		model->idMeshBuffer = BuildLODBuffers(m_resources, model->sizeOfVertex, model->faceCount);
		model->faceCount /= 4;
		model->faceStart = 0;
		model->idEffect = GameRenderDeviceHandle->CreateEffect(Effects::TerrainLODFX);
		model->idVertex = GameRenderDeviceHandle->CreateVertex(terrainLOD);
		model->idInputLayouts = GameRenderDeviceHandle->CreateInputLayouts(model->idVertex, model->idEffect);
		model->topology = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
		model->material.Ambient.x = 1.0f;
		model->material.Ambient.y = 1.0f;
		model->material.Ambient.z = 1.0f;
		model->material.Ambient.w = 1.0f;
		model->material.Diffuse.x = 1.0f;
		model->material.Diffuse.y = 1.0f;
		model->material.Diffuse.z = 1.0f;
		model->material.Diffuse.w = 1.0f;
		model->material.Specular.x = 0.0f;
		model->material.Specular.y = 0.0f;
		model->material.Specular.z = 0.0f;
		model->material.Specular.w = 0.0f;
		model->material.Reflect.x = 0.0f;
		model->material.Reflect.y = 0.0f;
		model->material.Reflect.z = 0.0f;
		model->material.Reflect.w = 0.0f;

		m_RenderId = GameRenderDeviceHandle->CreateModel(model);

		Model *modelW = new Model(*model);
		Model *modelA = new Model(*model);
		Model *modelC = new Model(*model);

		modelW->idEffect = GameRenderDeviceHandle->CreateEffect(Effects::WaterLODFX);
		modelW->idInputLayouts = GameRenderDeviceHandle->CreateInputLayouts(modelW->idVertex, modelW->idEffect);

		modelA->idEffect = GameRenderDeviceHandle->CreateEffect(Effects::AtmosphereFX);
		modelA->idInputLayouts = GameRenderDeviceHandle->CreateInputLayouts(modelA->idVertex, modelA->idEffect);

		modelC->idEffect = GameRenderDeviceHandle->CreateEffect(Effects::CloudsFX);
		modelC->idInputLayouts = GameRenderDeviceHandle->CreateInputLayouts(modelC->idVertex, modelC->idEffect);

		SetRenderIdWater(GameRenderDeviceHandle->CreateModel(modelW));
		SetRenderIdAtmosphere(GameRenderDeviceHandle->CreateModel(modelA));
		SetRenderIdClouds(GameRenderDeviceHandle->CreateModel(modelC));
	}

	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> *quadScreen = new std::vector<D3D11_INPUT_ELEMENT_DESC>(2);
		quadScreen->at(0) = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		quadScreen->at(1) = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };

		Model *modelWCAQS = new Model;
		modelWCAQS->idMeshBuffer = GameRenderDeviceHandle->GetQuadScreenID();
		modelWCAQS->sizeOfVertex = sizeof(Vertex::TerrainLOD);
		modelWCAQS->faceCount = 2;
		modelWCAQS->faceStart = 0;
		modelWCAQS->idEffect = GameRenderDeviceHandle->CreateEffect(Effects::QuadScreenWCAFX);
		modelWCAQS->idVertex = GameRenderDeviceHandle->CreateVertex(quadScreen);
		modelWCAQS->idInputLayouts = GameRenderDeviceHandle->CreateInputLayouts(modelWCAQS->idVertex, modelWCAQS->idEffect);
		modelWCAQS->topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		modelWCAQS->material.Ambient.x = 1.0f;
		modelWCAQS->material.Ambient.y = 1.0f;
		modelWCAQS->material.Ambient.z = 1.0f;
		modelWCAQS->material.Ambient.w = 1.0f;
		modelWCAQS->material.Diffuse.x = 1.0f;
		modelWCAQS->material.Diffuse.y = 1.0f;
		modelWCAQS->material.Diffuse.z = 1.0f;
		modelWCAQS->material.Diffuse.w = 1.0f;
		modelWCAQS->material.Specular.x = 0.0f;
		modelWCAQS->material.Specular.y = 0.0f;
		modelWCAQS->material.Specular.z = 0.0f;
		modelWCAQS->material.Specular.w = 0.0f;
		modelWCAQS->material.Reflect.x = 0.0f;
		modelWCAQS->material.Reflect.y = 0.0f;
		modelWCAQS->material.Reflect.z = 0.0f;
		modelWCAQS->material.Reflect.w = 0.0f;

		m_RenderIdWCAQuadScreen = GameRenderDeviceHandle->CreateModel(modelWCAQS);
	}

	for (int i = 0; i < 6; ++i)
	{
		m_PlanetLOD[i].Init(this, i, 0, XMINT2(0, 0), btVector3(0.0, 0.0, 0.0), 1.0, 0, XMINT2(0, 0), 0.0f, 1.0f, XMINT2(0, 0), 1.0f, NULL, NULL, NULL, NULL);
		m_PlanetLOD[i].ComponentProccess();
	}
}