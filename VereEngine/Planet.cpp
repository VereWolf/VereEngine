#include "pch.h"
#include "PlanetLOD.h"
#include "Atmosphere.h"
#include "PlanetData.h"
#include "GameObject.h"
#include "Planet.h"

void QuadScreenWCAMessage::Use()
{
	((QuadScreenWCA*)m_Effect)->SetView(m_View);
	((QuadScreenWCA*)m_Effect)->SetMainDepthMap(m_MainDepthSRV);
	((QuadScreenWCA*)m_Effect)->SetMainTargetMap(m_MainTargetSRV);
	((QuadScreenWCA*)m_Effect)->SetWaterTopDepthMap(m_WaterTopDepthSRV);
	((QuadScreenWCA*)m_Effect)->SetWaterTopTargetMap(m_WaterTopTargetSRV);
	((QuadScreenWCA*)m_Effect)->SetWaterBottomDepthMap(m_WaterBottomDepthSRV);
	((QuadScreenWCA*)m_Effect)->SetWaterBottomTargetMap(m_WaterBottomTargetSRV);
	((QuadScreenWCA*)m_Effect)->SetCloudsDepthMap(m_CloudsDepthSRV);
	((QuadScreenWCA*)m_Effect)->SetCloudsTargetMap(m_CloudsTargetSRV);
	((QuadScreenWCA*)m_Effect)->SetAtmosphereDepthMap(m_AtmosphereDepthSRV);
	((QuadScreenWCA*)m_Effect)->SetAtmosphereTargetMap(m_AtmosphereTargetSRV);
	((QuadScreenWCA*)m_Effect)->SetDepth(m_Depth);
	((QuadScreenWCA*)m_Effect)->SetSize(m_Size);
	((QuadScreenWCA*)m_Effect)->SetWaterRatio(m_WaterRatio);
	((QuadScreenWCA*)m_Effect)->SetCloudsRatio(m_CloudsRatio);
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

<<<<<<< HEAD
	/*m_PP = m_CP;
	m_CP = GameObjectStackHandle->GetMainCamera()->GetLocalPosition();

	btVector3 CH = m_CP - m_PP;

	SetCurrentMaxLevel(VereMath::Clamp(GetMaxLevel() - 5 * CH.length() * GetScaling().getRow(0).getX() / pow(GetMaxLevel(), 2), 1, GetMaxLevel()));*/

=======
>>>>>>> master
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

	for (int i = 0; i < 64; ++i)
	{
		if (m_interator >= m_planetElementID->GetRangeOfElement()) m_interator = 0;

		if (m_planetElements[m_interator] != NULL)
		{
			if (m_planetElements[m_interator]->IsDestroy() == true)
			{
				if (m_planetElements[m_interator]->GetProccessed() == 2)
				{
					m_planetElementID->ReturnElement(m_interator);

					delete (PlanetLOD*)m_planetElements[m_interator];
					m_planetElements[m_interator] = NULL;
				}
			}
		}

		++m_interator;
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

<<<<<<< HEAD
void Planet::BuildPlanet(std::string planetPath, int cellSize, int maxLevel, int loadDataMaxLvl, int generateTreesLvl, int loadTilesLvl, int loadDataPer,
	XMFLOAT3 fogColor, XMFLOAT3 waterColor, float waterDeep,
	int sizeOfBigTile, int levelOfSmallBlock, int levelOfBigBlock)
{
	m_planetPath = planetPath;

	SetNumPointInRowInCell(cellSize);
	SetNumPointInRowInBigCell(sizeOfBigTile);
	SetMaxLevel(maxLevel);
	SetCurrentMaxLevel(maxLevel);
	SetLoadDataMaxLvl(loadDataMaxLvl);
	SetLoadTilesLvl(loadTilesLvl);
	SetLoadDataPer(loadDataPer);
	SetGenerateTreesLvl(generateTreesLvl);

	SetFogAColor(fogColor);
	SetFogWColor(waterColor);
	SetFogWRange(waterDeep);

	m_idHeightMapBig = GameStreamingDataHandle->CreateFLOATDepository(levelOfBigBlock, sizeOfBigTile, sizeOfBigTile);
	m_idNormalMapBig = GameStreamingDataHandle->CreateBYTE4Depository(levelOfBigBlock, sizeOfBigTile, sizeOfBigTile);
	m_idEnviromentMapBig = GameStreamingDataHandle->CreateBYTE4Depository(3, sizeOfBigTile, sizeOfBigTile);
	m_idTreesMapBig = GameStreamingDataHandle->CreateBYTE4Depository(levelOfBigBlock, sizeOfBigTile, sizeOfBigTile);

	m_idHeightMapSmall = GameStreamingDataHandle->CreateFLOATDepository(levelOfSmallBlock, cellSize + 2, cellSize + 2);
	m_idNormalMapSmall = GameStreamingDataHandle->CreateBYTE4Depository(levelOfSmallBlock, cellSize + 2, cellSize + 2);
	m_idEnviromentMapSmall = GameStreamingDataHandle->CreateBYTE4Depository(levelOfSmallBlock, cellSize + 2, cellSize + 2);
	m_idTreesMapSmall = GameStreamingDataHandle->CreateBYTE4Depository(levelOfSmallBlock, cellSize + 2, cellSize + 2);
=======
void Planet::BuildPlanet(BuildPlanetMessage message)
{
	m_planetPath = message.planetPath;

	SetNumPointInRowInCell(message.cellSize);
	SetNumPointInRowInBigCell(message.sizeOfBigTile);
	SetMaxLevel(message.maxLevel);
	SetCurrentMaxLevel(message.maxLevel);
	SetLoadDataMaxLvl(message.loadDataMaxLvl);
	SetLoadTilesLvl(message.loadTilesLvl);
	SetLoadDataPer(message.loadDataPer);
	SetGenerateTreesLvl(message.generateTreesLvl);

	SetFogAColor(message.fogColor);
	SetFogWColor(message.waterColor);
	SetFogWRange(message.waterDeep);

	m_idHeightMapBig = GameStreamingDataHandle->CreateFLOATDepository(message.levelOfBigBlock, message.sizeOfBigTile, message.sizeOfBigTile);
	m_idNormalMapBig = GameStreamingDataHandle->CreateBYTE4Depository(message.levelOfBigBlock, message.sizeOfBigTile, message.sizeOfBigTile);
	m_idEnviromentMapBig = GameStreamingDataHandle->CreateBYTE4Depository(3, message.sizeOfBigTile, message.sizeOfBigTile);
	m_idTreesMapBig = GameStreamingDataHandle->CreateBYTE4Depository(message.levelOfBigBlock, message.sizeOfBigTile, message.sizeOfBigTile);
	m_idRiverWidth1MapBig = GameStreamingDataHandle->CreateFLOATDepository(message.levelOfBigBlock, message.sizeOfBigTile, message.sizeOfBigTile);
	m_idRiverWidth2MapBig = GameStreamingDataHandle->CreateFLOATDepository(message.levelOfBigBlock, message.sizeOfBigTile, message.sizeOfBigTile);
	m_idRiverLength1MapBig = GameStreamingDataHandle->CreateFLOATDepository(message.levelOfBigBlock, message.sizeOfBigTile, message.sizeOfBigTile);
	m_idRiverLength2MapBig = GameStreamingDataHandle->CreateFLOATDepository(message.levelOfBigBlock, message.sizeOfBigTile, message.sizeOfBigTile);
	m_idRiverType1MapBig = GameStreamingDataHandle->CreateFLOATDepository(message.levelOfBigBlock, message.sizeOfBigTile, message.sizeOfBigTile);
	m_idRiverType2MapBig = GameStreamingDataHandle->CreateFLOATDepository(message.levelOfBigBlock, message.sizeOfBigTile, message.sizeOfBigTile);
	m_idRiverHeightMapBig = GameStreamingDataHandle->CreateFLOATDepository(message.levelOfBigBlock, message.sizeOfBigTile, message.sizeOfBigTile);


	m_idHeightMapSmall = GameStreamingDataHandle->CreateFLOATDepository(message.levelOfSmallBlock, message.cellSize + 2, message.cellSize + 2);
	m_idNormalMapSmall = GameStreamingDataHandle->CreateBYTE4Depository(message.levelOfSmallBlock, message.cellSize + 2, message.cellSize + 2);
	m_idEnviromentMapSmall = GameStreamingDataHandle->CreateBYTE4Depository(message.levelOfSmallBlock, message.cellSize + 2, message.cellSize + 2);
	m_idTreesMapSmall = GameStreamingDataHandle->CreateBYTE4Depository(message.levelOfSmallBlock, message.cellSize + 2, message.cellSize + 2);
	m_idRiverWidth1MapSmall = GameStreamingDataHandle->CreateFLOATDepository(message.levelOfSmallBlock, message.cellSize + 2, message.cellSize + 2);
	m_idRiverWidth2MapSmall = GameStreamingDataHandle->CreateFLOATDepository(message.levelOfSmallBlock, message.cellSize + 2, message.cellSize + 2);
	m_idRiverLength1MapSmall = GameStreamingDataHandle->CreateFLOATDepository(message.levelOfSmallBlock, message.cellSize + 2, message.cellSize + 2);
	m_idRiverLength2MapSmall = GameStreamingDataHandle->CreateFLOATDepository(message.levelOfSmallBlock, message.cellSize + 2, message.cellSize + 2);
	m_idRiverType1MapSmall = GameStreamingDataHandle->CreateFLOATDepository(message.levelOfSmallBlock, message.cellSize + 2, message.cellSize + 2);
	m_idRiverType2MapSmall = GameStreamingDataHandle->CreateFLOATDepository(message.levelOfSmallBlock, message.cellSize + 2, message.cellSize + 2);
	m_idRiverHeightMapSmall = GameStreamingDataHandle->CreateFLOATDepository(message.levelOfSmallBlock, message.cellSize + 2, message.cellSize + 2);
>>>>>>> master

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

	SetMaxNumBlockBig(pow(2, message.levelOfBigBlock));
	SetCurrentNumBlockBig(6);
	SetMaxNumBlockSmall(pow(2, message.levelOfSmallBlock));
	SetCurrentNumBlockSmall(6);

	IDRegistr *IDR = new IDRegistr(message.levelOfSmallBlock);
	m_planetElementID = IDR;
	m_planetElements.resize(GetMaxNumBlockSmall(), NULL);

	PlanetLODInitMeessage initMessage;

	initMessage.master = this;
	initMessage.level = 0;
	initMessage.coord = XMINT2(0, 0);
	initMessage.position = btVector3(0.0, 0.0, 0.0);
	initMessage.scaling = 1.0;
	initMessage.levelFromLastLoadData = 0;
	initMessage.coordFromLastLoadData = XMINT2(0, 0);
	initMessage.scalingFromLastLoadData = 0.0f;
	initMessage.levelFromLoadTile = 1.0f;
	initMessage.coordFromLoadTile = XMINT2(0, 0);
	initMessage.scalingFromLoadTile = 1.0f;
	initMessage.isMap = false;
	initMessage.isMap1 = false;
	initMessage.isMap2 = false;
	initMessage.idHeightMapBig = XMINT2(-1, -1);
	initMessage.idNormalMapBig = XMINT2(-1, -1);
	initMessage.idEnviromentMapBig = XMINT2(-1, -1);
	initMessage.idTreesMapBig = XMINT2(-1, -1);
	initMessage.idRiverWidth1MapBig = XMINT2(-1, -1);
	initMessage.idRiverWidth2MapBig = XMINT2(-1, -1);
	initMessage.idRiverLength1MapBig = XMINT2(-1, -1);
	initMessage.idRiverLength2MapBig = XMINT2(-1, -1);
	initMessage.idRiverType1MapBig = XMINT2(-1, -1);
	initMessage.idRiverType2MapBig = XMINT2(-1, -1);
	initMessage.idRiverHeightMapBig = XMINT2(-1, -1);
	initMessage.idBillboardTreesVertex = -1;
	initMessage.billboardTreesIndex = NULL;
	initMessage.billboardMatrix = btTransform(btMatrix3x3::getIdentity(), btVector3(0.0, 0.0, 0.0));
	initMessage.idBillboardTrees = -1;

	for (int i = 0; i < 6; ++i)
	{
<<<<<<< HEAD
		m_PlanetLOD[i].Init(this, i, 0, XMINT2(0, 0), btVector3(0.0, 0.0, 0.0), 1.0, 0, XMINT2(0, 0), 0.0f, 1.0f, XMINT2(0, 0), 1.0f, false, -1, -1, -1, -1,
			-1, NULL, btTransform(btMatrix3x3::getIdentity(), btVector3(0.0, 0.0, 0.0)), -1);
=======
		initMessage.side = i;

		m_PlanetLOD[i].Init(initMessage);
>>>>>>> master
		m_PlanetLOD[i].SetValueOfLODSmall(1);
		m_PlanetLOD[i].SetValueOfLODBig(1);
		while (m_PlanetLOD[i].ComponentProccess() == false);
	}
}