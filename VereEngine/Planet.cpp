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
	((QuadScreenWCA*)m_BaseEffect)->SetCoordDepthMap(m_CoordDepthSRV);
	((QuadScreenWCA*)m_BaseEffect)->SetCoordTargetMap(m_CoordTargetSRV);
	((QuadScreenWCA*)m_BaseEffect)->SetWaterDepthMap(m_WaterDepthSRV);
	((QuadScreenWCA*)m_BaseEffect)->SetWaterTargetMap(m_WaterTargetSRV);
	((QuadScreenWCA*)m_BaseEffect)->SetCloudsDepthMap(m_CloudsDepthSRV);
	((QuadScreenWCA*)m_BaseEffect)->SetCloudsTargetMap(m_CloudsTargetSRV);
	((QuadScreenWCA*)m_BaseEffect)->SetAtmosphereDepthMap(m_AtmosphereDepthSRV);
	((QuadScreenWCA*)m_BaseEffect)->SetAtmosphereTargetMap(m_AtmosphereTargetSRV);
	((QuadScreenWCA*)m_BaseEffect)->SetSphereDepthPatternMap(m_SphereDepthPatternMap);
	((QuadScreenWCA*)m_BaseEffect)->SetDepth(m_Depth);
	((QuadScreenWCA*)m_BaseEffect)->SetSize(m_Size);
	((QuadScreenWCA*)m_BaseEffect)->SetWaterRatio(m_WaterRatio);
	((QuadScreenWCA*)m_BaseEffect)->SetCloudsRatio(m_CloudsRatio);
}

void QuadScreenWithCoordMessage::Use()
{
	XMMATRIX mesh = XMLoadFloat4x4(&VereMath::ConvertToXMFLOAT4X4(m_CameraOffset * m_Transform));
	((QuadScreenWithCoord*)m_BaseEffect)->SetWorld(mesh);
	((QuadScreenWithCoord*)m_BaseEffect)->SetViewProj(m_View * m_Proj);
	((QuadScreenWithCoord*)m_BaseEffect)->SetView(m_View);
	((QuadScreenWithCoord*)m_BaseEffect)->SetProj(m_Proj);
	((QuadScreenWithCoord*)m_BaseEffect)->SetSize(m_size);
}

Planet::Planet()
{
}

Planet::Planet(Planet & other)
{

}
void Planet::Init()
{
	GameObjectSpace::Init();
	PlanetData::Init();
}
void Planet::Render(btTransform camOffset, XMMATRIX camView, XMMATRIX camProj,
	float camFarZ, btScalar heightFar, btScalar aspect,
	float camFarRangeMod, float camModifier)
{
	m_resources->GetD3DDeviceContext()->ClearRenderTargetView(GetAtmosphereTargetMapRTV(), DirectX::Colors::Transparent);
	m_resources->GetD3DDeviceContext()->ClearRenderTargetView(GetCloudsTargetMapRTV(), DirectX::Colors::Transparent);
	m_resources->GetD3DDeviceContext()->ClearRenderTargetView(GetWaterTargetMapRTV(), DirectX::Colors::Transparent);
	m_resources->GetD3DDeviceContext()->ClearRenderTargetView(GetPlanetTargetMapRTV(), DirectX::Colors::Transparent);
	m_resources->GetD3DDeviceContext()->ClearRenderTargetView(GetCoordTargetMapRTV(), DirectX::Colors::Transparent);
	m_resources->GetD3DDeviceContext()->ClearDepthStencilView(GetAtmosphereDeepMapDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_resources->GetD3DDeviceContext()->ClearDepthStencilView(GetCloudsDeepMapDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_resources->GetD3DDeviceContext()->ClearDepthStencilView(GetWaterDeepMapDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_resources->GetD3DDeviceContext()->ClearDepthStencilView(GetPlanetDeepMapDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_resources->GetD3DDeviceContext()->ClearDepthStencilView(GetCoordDeepMapDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

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
		m_PlanetLOD[i].Render(camOffset, camView, camProj,
			camFarZ,heightFar, aspect, camFarRangeMod, camModifier);
	}

	DrawQuadWithCoord();
	DrawWCA();
	DrawPlanet();

	for (int i = 0; i < m_planetElementID->GetRangeOfElement(); ++i)
	{
		if (m_planetElements[i] != NULL)
		{
			if (m_planetElements[i]->IsDestroy() == true)
			{
				if (m_planetElements[i]->GetProccessed() == 2)
				{
					m_planetElementID->ReturnElement(i);
					SafeDelete(m_planetElements[i]);
				}
			}
			/*else
			{
				((PlanetLOD*)m_planetElements[i])->Render(camOffset, camView, camProj,
					camFarZ, heightFar, aspect, camFarRangeMod, camModifier);
			}*/
		}
	}

	//GameObjectStackHandle->GetMainCamera()->SetUp((GameObjectStackHandle->GetMainCamera()->GetPosition() - GetLocalPosition()).normalize());
}

void Planet::DrawQuadWithCoord()
{
	QuadScreenWithCoordMessage message;

	message.m_ModelID = m_RenderIdQuadScreenWithCoord;
	message.m_Transform = GetWorldTransform();
	message.m_View = GameObjectStackHandle->GetMainCamera()->View();
	message.m_Proj = GameObjectStackHandle->GetMainCamera()->Proj();
	message.m_CameraOffset = GameObjectStackHandle->GetMainCamera()->btOffset();
	message.m_size = 2.0f * GetRadiusOfAtmosphere();
	message.m_FarZ = GameObjectStackHandle->GetMainCamera()->GetFarZ();
	message.m_FarModifier = GameObjectStackHandle->GetMainCamera()->GetFarModifier();
	message.m_ViewPort = GameRenderDeviceHandle->GetMainViewPort();
	message.m_RasterizeState = RenderStates::NoCullRS;
	message.m_BlendState = RenderStates::NoBlendBS;

	//GameRenderDeviceHandle->BindMainRenderTarget();
	GameRenderDeviceHandle->BindRenderTarget(GetCoordTargetMapRTV(), GetCoordDeepMapDSV());
	GameRenderDeviceHandle->Render(&message);
	GameRenderDeviceHandle->BindMainRenderTarget();
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
	message.m_CoordDepthSRV = GetCoordDeepMapSRV();
	message.m_CoordTargetSRV = GetCoordTargetMapSRV();
	message.m_WaterDepthSRV = GetWaterDeepMapSRV();
	message.m_WaterTargetSRV = GetWaterTargetMapSRV();
	message.m_CloudsDepthSRV = GetCloudsDeepMapSRV();
	message.m_CloudsTargetSRV = GetCloudsTargetMapSRV();
	message.m_AtmosphereDepthSRV = GetAtmosphereDeepMapSRV();
	message.m_AtmosphereTargetSRV = GetAtmosphereTargetMapSRV();
	message.m_SphereDepthPatternMap = GameRenderDeviceHandle->GetTexture(GameRenderDeviceHandle->GetDeepOfSphereTextureID());
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

void Planet::BuildPlanet(int cellSize, int maxLevel, int maxRenderLevel, UINT loadDataAfterAgain, UINT maxLevelOfStreaming)
{
	SetNumPointInRowInCell(cellSize);
	SetMaxLevel(maxLevel);
	SetLoadDataAfterAgain(loadDataAfterAgain);
	SetMaxLevelOfStreaming(maxLevelOfStreaming);

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

		Model *modelQSWC = new Model;
		modelQSWC->idMeshBuffer = GameRenderDeviceHandle->GetQuadScreenID();
		modelQSWC->sizeOfVertex = sizeof(Vertex::TerrainLOD);
		modelQSWC->faceCount = 2;
		modelQSWC->faceStart = 0;
		modelQSWC->idEffect = GameRenderDeviceHandle->CreateEffect(Effects::QuadScreenWithCoordFX);
		modelQSWC->idVertex = GameRenderDeviceHandle->CreateVertex(quadScreen);
		modelQSWC->idInputLayouts = GameRenderDeviceHandle->CreateInputLayouts(modelQSWC->idVertex, modelQSWC->idEffect);
		modelQSWC->topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		modelQSWC->material.Ambient.x = 1.0f;
		modelQSWC->material.Ambient.y = 1.0f;
		modelQSWC->material.Ambient.z = 1.0f;
		modelQSWC->material.Ambient.w = 1.0f;
		modelQSWC->material.Diffuse.x = 1.0f;
		modelQSWC->material.Diffuse.y = 1.0f;
		modelQSWC->material.Diffuse.z = 1.0f;
		modelQSWC->material.Diffuse.w = 1.0f;
		modelQSWC->material.Specular.x = 0.0f;
		modelQSWC->material.Specular.y = 0.0f;
		modelQSWC->material.Specular.z = 0.0f;
		modelQSWC->material.Specular.w = 0.0f;
		modelQSWC->material.Reflect.x = 0.0f;
		modelQSWC->material.Reflect.y = 0.0f;
		modelQSWC->material.Reflect.z = 0.0f;
		modelQSWC->material.Reflect.w = 0.0f;

		m_RenderIdQuadScreenWithCoord = GameRenderDeviceHandle->CreateModel(modelQSWC);
	}

	for (int i = 0; i < 6; ++i)
	{
		m_PlanetLOD[i].Init(this, i, 0, XMINT2(0, 0), btVector3(0.0, 0.0, 0.0), 1.0, NULL, NULL);
		m_PlanetLOD[i].ComponentProccess();
	}
}