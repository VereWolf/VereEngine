#include "pch.h"
#include "PlanetLOD.h"
#include "Atmosphere.h"
#include "PlanetData.h"
#include "GameObject.h"
#include "Planet.h"

TerrainPlanet::TerrainPlanet()
{
}

TerrainPlanet::TerrainPlanet(TerrainPlanet & other)
{

}
void TerrainPlanet::Init()
{
	GameObjectSpace::Init();
	TerrainPlanetData::Init();
}
void TerrainPlanet::Render(btTransform camOffset, XMMATRIX camView, XMMATRIX camProj,
	float camFarZ, btScalar heightFar, btScalar aspect,
	float camFarRangeMod, float camModifier)
{
	if (m_planetElementsInProcess.GetSize() > 0)
	{
		int id = m_planetElementsInProcess.TakeElement();

		if (m_planetElements[id])
		{
			((TerrainPlanetLOD*)m_planetElements[id])->CreateNewLevelOfLoD();
		}
	}
	m_resources->GetD3DDeviceContext()->RSSetState(RenderStates::SolidRS);

	float blendFactor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	for (int i = 0; i < 6; ++i)
	{
		m_PlanetLOD[i].Render(camOffset, camView, camProj,
			camFarZ,heightFar, aspect, camFarRangeMod, camModifier);
	}

	int a = 0;

	//GameObjectStackHandle->GetMainCamera()->SetUp((GameObjectStackHandle->GetMainCamera()->GetPosition() - GetLocalPosition()).normalize());
}

void TerrainPlanet::BuildPlanet(int cellSize, int maxLevel, int maxRenderLevel, UINT loadDataAfterAgain, UINT maxLevelOfStreaming)
{
	SetNumPointInRowInCell(cellSize);
	SetMaxLevel(maxLevel);
	SetLoadDataAfterAgain(loadDataAfterAgain);
	SetMaxLevelOfStreaming(maxLevelOfStreaming);

	std::vector<D3D11_INPUT_ELEMENT_DESC> terrainLOD =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	Model *model = new Model;
	model->idMeshBuffer = BuildLODBuffers(m_resources, model->sizeOfVertex, model->faceCount);
	model->faceCount /= 4;
	model->faceStart = 0;
	model->idEffect = GameRenderDeviceHandle->CreateEffect(Effects::TerrainPlanetLODFX);
	model->idVertex = GameRenderDeviceHandle->CreateVertex(&terrainLOD);
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

	for (int i = 0; i < 6; ++i)
	{
		m_PlanetLOD[i].Init(this, i, 0, XMINT2(0, 0), btVector3(0.0, 0.0, 0.0), 1.0, NULL, NULL);
	}
}