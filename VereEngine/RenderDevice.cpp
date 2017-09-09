#include "pch.h"
#include "DeviceResources.h"
#include "GameTexture.h"
#include "GameModel.h"
#include "GameVertex.h"
#include "GameEffect.h"
#include "GameMeshBuffer.h"
#include "GameInputLayouts.h"
#include "GameAmbientLight.h"
#include "GameDirectionalLight.h"
#include "GamePointLight.h"
#include "GameSpotLight.h"
#include "RenderDevice.h"


btTransform RenderMessage::m_CameraOffset;
XMMATRIX RenderMessage::m_View;
XMMATRIX RenderMessage::m_Proj;
float RenderMessage::m_FarZ = 0.0f;
float RenderMessage::m_FarRangeMod = 0.0f;
float RenderMessage::m_FarModifier = 0.0f;
float RenderMessage::m_HeightFar = 0.0f;
float RenderMessage::m_Aspect = 0.0f;
D3D11_VIEWPORT *RenderMessage::m_ViewPort;

void RenderMessage::Use()
{
	XMFLOAT3 EyePos = { 0.0f, 0.0f, 0.0f };

	if (m_PlanetData)
	{
		btVector3 dir = -(m_PlanetData->GetWorldPosition() - GameObjectStackHandle->GetMainCamera()->GetWorldPosition());
		dir = m_PlanetData->GetWorldTransform().getBasis().inverse() * dir;

		((BaseEffect*)m_BaseEffect)->SetCenterOfPlanet(VereMath::ConvertToXMFLOAT3((m_CameraOffset * m_PlanetData->GetWorldTransform()).getOrigin()));
		((BaseEffect*)m_BaseEffect)->SetDirectOfPlanet(VereMath::ConvertToXMFLOAT3(dir));
		((BaseEffect*)m_BaseEffect)->SetRadiusOfTerrain(m_PlanetData->GetRadiusOfTerrain());
		((BaseEffect*)m_BaseEffect)->SetRadiusOfWater(m_PlanetData->GetRadiusOfWater());
		((BaseEffect*)m_BaseEffect)->SetRadiusOfClouds(m_PlanetData->GetRadiusOfClouds());
		((BaseEffect*)m_BaseEffect)->SetRadiusOfAtmosphere(m_PlanetData->GetRadiusOfAtmosphere());
		((BaseEffect*)m_BaseEffect)->SetFogAColor(m_PlanetData->GetFogAColor());
		((BaseEffect*)m_BaseEffect)->SetFogAStart(m_PlanetData->GetFogAStart());
		((BaseEffect*)m_BaseEffect)->SetFogARange(m_PlanetData->GetFogARange());
		((BaseEffect*)m_BaseEffect)->SetFogWColor(m_PlanetData->GetFogWColor());
		((BaseEffect*)m_BaseEffect)->SetFogWStart(m_PlanetData->GetFogWStart());
		((BaseEffect*)m_BaseEffect)->SetFogWRange(m_PlanetData->GetFogWRange());
	}
	else
	{
		((BaseEffect*)m_BaseEffect)->SetCenterOfPlanet(XMFLOAT3(0.0f, 0.0f, 0.0f));
		((BaseEffect*)m_BaseEffect)->SetRadiusOfTerrain(-1.0f);
		((BaseEffect*)m_BaseEffect)->SetRadiusOfWater(-1.0f);
		((BaseEffect*)m_BaseEffect)->SetRadiusOfClouds(-1.0f);
		((BaseEffect*)m_BaseEffect)->SetRadiusOfAtmosphere(-1.0f);
		((BaseEffect*)m_BaseEffect)->SetFogAColor(XMFLOAT3(-1.0f, -1.0f, -1.0f));
		((BaseEffect*)m_BaseEffect)->SetFogAStart(-1.0f);
		((BaseEffect*)m_BaseEffect)->SetFogARange(-1.0f);
		((BaseEffect*)m_BaseEffect)->SetFogWColor(XMFLOAT3(-1.0f, -1.0f, -1.0f));
		((BaseEffect*)m_BaseEffect)->SetFogWStart(-1.0f);
		((BaseEffect*)m_BaseEffect)->SetFogWRange(-1.0f);
	}

	XMMATRIX mesh = XMLoadFloat4x4(&VereMath::ConvertToXMFLOAT4X4(m_CameraOffset * m_Transform * m_Scaling));
	XMMATRIX viewProj = m_View * m_Proj;
	btMatrix3x3 transformN = m_Transform.getBasis();

	XMMATRIX meshN = XMLoadFloat3x3(&VereMath::ConvertToXMFLOAT3X3(transformN.inverse()));

	((BaseEffect*)m_BaseEffect)->SetEyePosW(EyePos);
	((BaseEffect*)m_BaseEffect)->SetMaterial(*m_Material);
	((BaseEffect*)m_BaseEffect)->SetView(m_View);
	((BaseEffect*)m_BaseEffect)->SetProj(m_Proj);
	((BaseEffect*)m_BaseEffect)->SetViewProj(viewProj);
	((BaseEffect*)m_BaseEffect)->SetWorld(mesh);
	((BaseEffect*)m_BaseEffect)->SetWorldN(meshN);
	((BaseEffect*)m_BaseEffect)->SetFarZ(m_FarZ);
	((BaseEffect*)m_BaseEffect)->SetFarRangeMod(m_FarRangeMod);
	((BaseEffect*)m_BaseEffect)->SetFarModifier(m_FarModifier);
}

void RenderToScreenMessage::Use()
{
	((RenderToScreen*)m_BaseEffect)->SetView(m_View);
	((RenderToScreen*)m_BaseEffect)->SetTargetMap(m_TargetSRV);
	((RenderToScreen*)m_BaseEffect)->SetDepthMap(m_DepthSRV);
}

RenderDevice::RenderDevice()
{
	GameRenderDeviceHandle = this;
	m_mainViewPort.TopLeftX = 0.0f;
	m_mainViewPort.TopLeftY = 0.0f;
	m_mainViewPort.Height = 965.0f;
	m_mainViewPort.Width = 1920.0f;
	m_mainViewPort.MinDepth = 0.0f;
	m_mainViewPort.MaxDepth = 1.0f;
}

RenderDevice::RenderDevice(DX::DeviceResources *resources, float farRangeMod)
{
	GameRenderDeviceHandle = this;
	m_mainViewPort.TopLeftX = 0.0f;
	m_mainViewPort.TopLeftY = 0.0f;
	m_mainViewPort.Height = 965.0f;
	m_mainViewPort.Width = 1920.0f;
	m_mainViewPort.MinDepth = 0.0f;
	m_mainViewPort.MaxDepth = 1.0f;

	RenderMessage::m_FarRangeMod = farRangeMod;

	Init(resources);
}

RenderDevice::~RenderDevice()
{

}

void RenderDevice::Init(DX::DeviceResources *resources)
{
	m_resources = resources;

	GetRenderAssetsStacks()->m_gameAnimations.Init(resources);
	GetRenderAssetsStacks()->m_gameEffects.Init(resources);
	GetRenderAssetsStacks()->m_gameVertices.Init(resources);
	GetRenderAssetsStacks()->m_gameInputLayouts.Init(resources);
	GetRenderAssetsStacks()->m_gameMeshBuffers.Init(resources);
	GetRenderAssetsStacks()->m_gameModels.Init(resources);
	GetRenderAssetsStacks()->m_gameTextures.Init(resources);
	
	{
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = m_mainViewPort.Width;
		texDesc.Height = m_mainViewPort.Height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		ID3D11Texture2D* depthMap = 0;
		m_resources->GetD3DDevice()->CreateTexture2D(&texDesc, 0, &depthMap);

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = 0;
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;
		m_resources->GetD3DDevice()->CreateDepthStencilView(depthMap, &dsvDesc, &m_MainDeepMapDSV);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		m_resources->GetD3DDevice()->CreateShaderResourceView(depthMap, &srvDesc, &m_MainDeepMapSRV);

		ReleaseCOM(depthMap);
	}

	{
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = m_mainViewPort.Width;
		texDesc.Height = m_mainViewPort.Height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		ID3D11Texture2D* targetMap = 0;
		m_resources->GetD3DDevice()->CreateTexture2D(&texDesc, 0, &targetMap);

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		rtvDesc.Format = texDesc.Format;
		rtvDesc.Texture2D.MipSlice = 0;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		m_resources->GetD3DDevice()->CreateRenderTargetView(targetMap, &rtvDesc, &m_MainTargetMapRTV);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = texDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		m_resources->GetD3DDevice()->CreateShaderResourceView(targetMap, &srvDesc, &m_MainTargetMapSRV);

		ReleaseCOM(targetMap);
	}

	CreateQuadScreen();

	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> *renderToScreen = new std::vector<D3D11_INPUT_ELEMENT_DESC>(2);
		renderToScreen->at(0) = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		renderToScreen->at(1) = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };

		Model *modelRTS = new Model;
		modelRTS->idMeshBuffer = GameRenderDeviceHandle->GetQuadScreenID();
		modelRTS->sizeOfVertex = sizeof(Vertex::TerrainLOD);
		modelRTS->faceCount = 2;
		modelRTS->faceStart = 0;
		modelRTS->idEffect = GameRenderDeviceHandle->CreateEffect(Effects::RenderToScreenFX);
		modelRTS->idVertex = GameRenderDeviceHandle->CreateVertex(renderToScreen);
		modelRTS->idInputLayouts = GameRenderDeviceHandle->CreateInputLayouts(modelRTS->idVertex, modelRTS->idEffect);
		modelRTS->topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;;
		modelRTS->material.Ambient.x = 1.0f;
		modelRTS->material.Ambient.y = 1.0f;
		modelRTS->material.Ambient.z = 1.0f;
		modelRTS->material.Ambient.w = 1.0f;
		modelRTS->material.Diffuse.x = 1.0f;
		modelRTS->material.Diffuse.y = 1.0f;
		modelRTS->material.Diffuse.z = 1.0f;
		modelRTS->material.Diffuse.w = 1.0f;
		modelRTS->material.Specular.x = 0.0f;
		modelRTS->material.Specular.y = 0.0f;
		modelRTS->material.Specular.z = 0.0f;
		modelRTS->material.Specular.w = 0.0f;
		modelRTS->material.Reflect.x = 0.0f;
		modelRTS->material.Reflect.y = 0.0f;
		modelRTS->material.Reflect.z = 0.0f;
		modelRTS->material.Reflect.w = 0.0f;

		m_RenderIdRTS = GameRenderDeviceHandle->CreateModel(modelRTS);
	}
}

void RenderDevice::Render(RenderMessage *message)
{
	m_resources->GetD3DDeviceContext()->RSSetViewports(1, message->m_ViewPort);
	m_resources->GetD3DDeviceContext()->RSSetState(message->m_RasterizeState);

	float blendFactor[] = { 0.0f,0.0f,0.0f,0.0f };

	m_resources->GetD3DDeviceContext()->OMSetBlendState(message->m_BlendState, blendFactor, 0xffffffff);

	Model *model = GetModel(message->m_ModelID);
	if (model == NULL) return;
	ID3D11Buffer *VB = GetVerticesBuffer(model->idMeshBuffer);
	ID3D11Buffer *IB = GetIndicesBuffer(model->idMeshBuffer);
	ID3D11InputLayout *IL = GetInputLayouts(model->idInputLayouts);
	Effect *BE = GetEffect(model->idEffect);
	GameVertex *V = GetVertex(model->idVertex);
	std::vector<D3D11_INPUT_ELEMENT_DESC> *IED = V->GetVertex();
	UINT stride = model->sizeOfVertex;

	message->m_Material = &model->material;
	message->m_BaseEffect = BE;

	int M = 3;

	if (model->topology == D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST)
	{
		M = 4;
	}

	if (true)
	{
		m_resources->GetD3DDeviceContext()->IASetPrimitiveTopology(model->topology);
		m_resources->GetD3DDeviceContext()->IASetInputLayout(IL);

		for (UINT n = 0; n < 1; ++n)
		{
			UINT offset = 0;
			m_resources->GetD3DDeviceContext()->IASetVertexBuffers(0, 1, &VB, &stride, &offset);

			m_resources->GetD3DDeviceContext()->IASetIndexBuffer(IB, DXGI_FORMAT_R32_UINT, 0);

			ID3DX11EffectTechnique* activeTech = BE->Light1Tech;

			D3DX11_TECHNIQUE_DESC techDesc;
			activeTech->GetDesc(&techDesc);

			message->Use();

			for (UINT p = 0; p < techDesc.Passes; ++p)
			{
				activeTech->GetPassByIndex(p)->Apply(0, m_resources->GetD3DDeviceContext());
				m_resources->GetD3DDeviceContext()->DrawIndexed(M * model->faceCount, 0, 0);
			}
		}
	}
}

void RenderDevice::RenderToScreen()
{

	XMMATRIX world(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	RenderToScreenMessage message;

	message.m_ModelID = m_RenderIdRTS;
	message.m_TargetSRV = GetMainTargetMapSRV();
	message.m_DepthSRV = GetMainDeepMapSRV();
	message.m_View = world;
	message.m_ViewPort = GameRenderDeviceHandle->GetMainViewPort();
	message.m_RasterizeState = RenderStates::NoCullRS;
	message.m_BlendState = RenderStates::TransparentBS;

	BindRenderTarget(m_resources->GetBackBufferRenderTargetView(), m_resources->GetDepthStencilView());

	GameRenderDeviceHandle->Render(&message);
}

void RenderDevice::ClearMainRenderTarget()
{
	m_resources->GetD3DDeviceContext()->ClearRenderTargetView(m_MainTargetMapRTV, DirectX::Colors::Black);
	m_resources->GetD3DDeviceContext()->ClearDepthStencilView(m_MainDeepMapDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

ID3D11Buffer *RenderDevice::BuildVerticesBuffer(void *vertices, UINT size, UINT stride)
{
	ID3D11Buffer *VB;

	D3D11_BUFFER_DESC vertexBuffer;

	vertexBuffer.Usage = D3D11_USAGE_DEFAULT;
	vertexBuffer.ByteWidth = size;
	vertexBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBuffer.CPUAccessFlags = 0;
	vertexBuffer.MiscFlags = 0;
	vertexBuffer.StructureByteStride = stride;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;

	if (FAILED(m_resources->GetD3DDevice()->CreateBuffer(&vertexBuffer, &vinitData, &VB)))
		return NULL;

	return VB;
}

ID3D11Buffer *RenderDevice::BuildIndicesBuffer(std::vector<UINT> *indices)
{
	ID3D11Buffer *IB;

	D3D11_BUFFER_DESC indexBuffer;

	indexBuffer.Usage = D3D11_USAGE_DEFAULT;
	indexBuffer.ByteWidth = sizeof(UINT) * indices->size();
	indexBuffer.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBuffer.CPUAccessFlags = 0;
	indexBuffer.MiscFlags = 0;
	indexBuffer.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices->at(0);

	if (FAILED(m_resources->GetD3DDevice()->CreateBuffer(&indexBuffer, &iinitData, &IB)))
		return NULL;

	return IB;
}

ID3D11ShaderResourceView *RenderDevice::BuildTexture(void *map, UINT height, UINT width, UINT format, UINT mipLevels)
{
	ID3D11ShaderResourceView * textureSRV;

	UINT size = height * width;

	D3D11_SUBRESOURCE_DATA tbsd;

	ZeroMemory(&tbsd, sizeof(D3D11_SUBRESOURCE_DATA));


	D3D11_TEXTURE2D_DESC tdesc;

	ZeroMemory(&tdesc, sizeof(D3D11_TEXTURE2D_DESC));

	tdesc.Width = width;
	tdesc.Height = height;
	tdesc.MipLevels = mipLevels;
	tdesc.ArraySize = 1;

	tdesc.SampleDesc.Count = 1;
	tdesc.SampleDesc.Quality = 0;
	tdesc.Usage = D3D11_USAGE_DEFAULT;
	tdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	tdesc.CPUAccessFlags = 0;
	tdesc.MiscFlags = 0;

	ID3D11Texture2D *mapResource;

	switch (format)
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		tbsd.pSysMem = map;
		tbsd.SysMemPitch = width * sizeof(VBYTE4);
		tbsd.SysMemSlicePitch = 0;
		tdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	case DXGI_FORMAT_R32_FLOAT:
		tbsd.pSysMem = map;
		tbsd.SysMemPitch = width * sizeof(float);
		tbsd.SysMemSlicePitch = 0;
		tdesc.Format = DXGI_FORMAT_R32_FLOAT;
		break;
	default:
		return (NULL);
	}

	if (FAILED(m_resources->GetD3DDevice()->CreateTexture2D(&tdesc, &tbsd, &mapResource)))
		return(NULL);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	srvDesc.ViewDimension =
		D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	srvDesc.Format = tdesc.Format;

	m_resources->GetD3DDevice()->CreateShaderResourceView(mapResource, &srvDesc, &textureSRV);

	ReleaseCOM(mapResource);

	return(textureSRV);
}

void RenderDevice::BindMainRenderTarget()
{
	ID3D11RenderTargetView *const targets[1] = { m_MainTargetMapRTV };
	m_resources->GetD3DDeviceContext()->OMSetRenderTargets(1, targets, m_MainDeepMapDSV);
}

void RenderDevice::BindRenderTarget(ID3D11RenderTargetView *target, ID3D11DepthStencilView *depthStencil)
{
	ID3D11RenderTargetView *const targets[1] = { target };
	m_resources->GetD3DDeviceContext()->OMSetRenderTargets(1, targets, depthStencil);
}

void RenderDevice::CreateQuadScreen()
{
	std::vector<Vertex::TerrainLOD> quadV(4);
	std::vector<UINT> quadI(6);

	quadV[0].Pos = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	quadV[0].TexTess = XMFLOAT2(0.0f, 1.0f);
	quadV[1].Pos = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	quadV[1].TexTess = XMFLOAT2(0.0f, 0.0f);
	quadV[2].Pos = XMFLOAT3(1.0f, 1.0f, 0.0f);
	quadV[2].TexTess = XMFLOAT2(1.0f, 0.0f);
	quadV[3].Pos = XMFLOAT3(1.0f, -1.0f, 0.0f);
	quadV[3].TexTess = XMFLOAT2(1.0f, 1.0f);

	quadI[0] = 0;
	quadI[1] = 1;
	quadI[2] = 2;

	quadI[3] = 0;
	quadI[4] = 2;
	quadI[5] = 3;

	m_quadScreenID = CreateMeshBuffer(&quadV[0], sizeof(Vertex::TerrainLOD), 4, &quadI);
}

int RenderDevice::CreateModel(Model * model)
{
	GameModel *gameModel = new GameModel;
	gameModel->PreInit(m_resources);
	gameModel->SetModel(model);

	return m_renderAssetsStacks.m_gameModels.CreateGameObject(gameModel);
}

void RenderDevice::DeleteModel(int id)
{
	m_renderAssetsStacks.m_gameModels.DeleteGameObject(id);
}

Model* RenderDevice::GetModel(int id)
{
	if (id >= 0 && m_renderAssetsStacks.m_gameModels.GetGameObject(id) != NULL)
	{
		return ((GameModel*)m_renderAssetsStacks.m_gameModels.GetGameObject(id))->GetModel();
	}

	return NULL;
}

int RenderDevice::CreateTexture(void *map, UINT height, UINT width, UINT format, UINT mipLevels)
{
	ID3D11ShaderResourceView * textureSRV = BuildTexture(map, height, width, format, mipLevels);
	if (!textureSRV)
	{
		return -1;
	}

	GameTexture *texture = new GameTexture;
	texture->PreInit(m_resources);
	texture->SetTexture(textureSRV);

	return m_renderAssetsStacks.m_gameTextures.CreateGameObject(texture);
}

void RenderDevice::DeleteTexture(int id)
{
	m_renderAssetsStacks.m_gameTextures.DeleteGameObject(id);
}

ID3D11ShaderResourceView* RenderDevice::GetTexture(int id)
{
	if (id >= 0 && m_renderAssetsStacks.m_gameTextures.GetGameObject(id) != NULL)
	{
		return ((GameTexture*)m_renderAssetsStacks.m_gameTextures.GetGameObject(id))->GetTexture();
	}

	return NULL;
}

int RenderDevice::CreateInputLayouts(int idVertices, int idEffect)
{
	ElementsVertex *gameVertices = ((GameVertex*)m_renderAssetsStacks.m_gameVertices.GetGameObject(idVertices))->GetVertex();
	Effect *baseEffect = ((GameEffect*)m_renderAssetsStacks.m_gameEffects.GetGameObject(idVertices))->GetEffect();

	ID3D11InputLayout* inputLayouts = 0;

	D3DX11_PASS_DESC passDesc;

	baseEffect->Light1Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR2(m_resources->GetD3DDevice()->CreateInputLayout(&gameVertices->at(0), gameVertices->size(), passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &inputLayouts));

	GameInputLayouts *gameInputLayouts = new GameInputLayouts;
	gameInputLayouts->PreInit(m_resources);
	gameInputLayouts->SetInputLayouts(inputLayouts);

	return m_renderAssetsStacks.m_gameInputLayouts.CreateGameObject(gameInputLayouts);
}

void RenderDevice::DeleteInputLayouts(int id)
{
	m_renderAssetsStacks.m_gameInputLayouts.DeleteGameObject(id);
}

ID3D11InputLayout* RenderDevice::GetInputLayouts(int id)
{
	return ((GameInputLayouts*)m_renderAssetsStacks.m_gameInputLayouts.GetGameObject(id))->GetInputLayouts();
}

int RenderDevice::CreateVertex(ElementsVertex *vertex)
{
	GameVertex *gameVertex = new GameVertex;
	gameVertex->PreInit(m_resources);
	gameVertex->SetVertex(vertex);

	return m_renderAssetsStacks.m_gameVertices.CreateGameObject(gameVertex);
}

void RenderDevice::DeleteVertex(int id)
{
	m_renderAssetsStacks.m_gameVertices.DeleteGameObject(id);
}

GameVertex* RenderDevice::GetVertex(int id)
{
	return ((GameVertex*)m_renderAssetsStacks.m_gameVertices.GetGameObject(id));
}

int RenderDevice::CreateEffect(Effect *effect)
{
	GameEffect *gameEffect = new GameEffect;
	gameEffect->PreInit(m_resources);
	gameEffect->SetEffect(effect);

	return m_renderAssetsStacks.m_gameEffects.CreateGameObject(gameEffect);
}

void RenderDevice::DeleteEffect(int id)
{
	m_renderAssetsStacks.m_gameEffects.DeleteGameObject(id);
}

Effect* RenderDevice::GetEffect(int id)
{
	return ((GameEffect*)m_renderAssetsStacks.m_gameEffects.GetGameObject(id))->GetEffect();
}

int RenderDevice::CreateMeshBuffer(void *vertices, UINT vertexSize, UINT numElements, std::vector<UINT> *indices)
{
	UINT S = vertexSize * numElements;
	ID3D11Buffer * VB = BuildVerticesBuffer(vertices, S, vertexSize);
	if (!VB)
	{
		return -1;
	}
	ID3D11Buffer * IB = BuildIndicesBuffer(indices);
	if (!IB)
	{
		return -1;
	}

	std::vector<char> V(S);
	memcpy(&V[0], vertices, S);
	XMFLOAT3 P;

	float I = std::numeric_limits<btScalar>::infinity();
	btVector3 min = btVector3(I, I, I);
	btVector3 max = btVector3(-I, -I, -I);

	for (int i = 0; i < numElements; ++i)
	{
		memcpy(&P, &V[vertexSize * i], sizeof(XMFLOAT3));

		min = VereMath::MinVector(min, btVector3(P.x, P.y, P.z));
		max = VereMath::MaxVector(max, btVector3(P.x, P.y, P.z));
	}

	GameMeshBuffer *meshBuffer = new GameMeshBuffer;
	meshBuffer->PreInit(m_resources);
	meshBuffer->SetVertexBuffer(VB);
	meshBuffer->SetIndexBuffer(IB);
	meshBuffer->SetMinVector(min);
	meshBuffer->SetMaxVector(max);

	return m_renderAssetsStacks.m_gameMeshBuffers.CreateGameObject(meshBuffer);
}

void RenderDevice::DeleteMeshBuffer(int id)
{
	m_renderAssetsStacks.m_gameMeshBuffers.DeleteGameObject(id);
}

ID3D11Buffer* RenderDevice::GetVerticesBuffer(int id)
{
	return ((GameMeshBuffer*)m_renderAssetsStacks.m_gameMeshBuffers.GetGameObject(id))->GetVertexBuffer();
}

ID3D11Buffer* RenderDevice::GetIndicesBuffer(int id)
{
	return ((GameMeshBuffer*)m_renderAssetsStacks.m_gameMeshBuffers.GetGameObject(id))->GetIndexBuffer();
}

int RenderDevice::CreateAmbientLight(AmbientLight * light)
{
	GameAmbientLight *gameAmbientLight = new GameAmbientLight;
	gameAmbientLight->PreInit(m_resources);
	gameAmbientLight->SetAmbientLight(light);

	return m_renderAssetsStacks.m_gameAmbientLight.CreateGameObject(gameAmbientLight);
}

void RenderDevice::DeleteAmbientLight(int id)
{
	m_renderAssetsStacks.m_gameAmbientLight.DeleteGameObject(id);
}

AmbientLight* RenderDevice::GetAmbientLight(int id)
{
	return ((GameAmbientLight*)m_renderAssetsStacks.m_gameAmbientLight.GetGameObject(id))->GetAmbientLight();
}

int RenderDevice::CreateDirectionalLight(DirectionalLight * light)
{
	GameDirectionalLight *gameDirectionalLight = new GameDirectionalLight;
	gameDirectionalLight->PreInit(m_resources);
	gameDirectionalLight->SetDirectionalLight(light);

	return m_renderAssetsStacks.m_gameDirectionalLight.CreateGameObject(gameDirectionalLight);
}

void RenderDevice::DeleteDirectionalLight(int id)
{
	m_renderAssetsStacks.m_gameDirectionalLight.DeleteGameObject(id);
}

DirectionalLight* RenderDevice::GetDirectionalLight(int id)
{
	return ((GameDirectionalLight*)m_renderAssetsStacks.m_gameDirectionalLight.GetGameObject(id))->GetDirectionalLight();
}

int RenderDevice::CreatePointLight(PointLight * light)
{
	GamePointLight *gamePointLight = new GamePointLight;
	gamePointLight->PreInit(m_resources);
	gamePointLight->SetPointLight(light);

	return m_renderAssetsStacks.m_gamePointLight.CreateGameObject(gamePointLight);
}

void RenderDevice::DeletePointLight(int id)
{
	m_renderAssetsStacks.m_gamePointLight.DeleteGameObject(id);
}

PointLight* RenderDevice::GetPointLight(int id)
{
	return ((GamePointLight*)m_renderAssetsStacks.m_gamePointLight.GetGameObject(id))->GetPointLight();
}

int RenderDevice::CreateSpotLight(SpotLight * light)
{
	GameSpotLight *gameSpotLight = new GameSpotLight;
	gameSpotLight->PreInit(m_resources);
	gameSpotLight->SetSpotLight(light);

	return m_renderAssetsStacks.m_gameSpotLight.CreateGameObject(gameSpotLight);
}

void RenderDevice::DeleteSpotLight(int id)
{
	m_renderAssetsStacks.m_gameSpotLight.DeleteGameObject(id);
}

SpotLight* RenderDevice::GetSpotLight(int id)
{
	return ((GameSpotLight*)m_renderAssetsStacks.m_gameSpotLight.GetGameObject(id))->GetSpotLight();
}

/*int RenderDevice::CreateTilePlanetData(TilePlanetData * data)
{
	GameTilePlanetData *gameTilePlanetData = new GameTilePlanetData;
	gameTilePlanetData->PreInit(m_resources);
	gameTilePlanetData->SetTilePlanetData(data);

	return m_renderAssetsStacks.m_gameTilePlanetData.CreateGameObject(gameTilePlanetData);
}

int RenderDevice::CreateTilePlanetDataFromAnother(int sourceID, XMINT2 whichSquare, float height, float with, float multiplierH = 1.0f, float multiplierN = 4.0f)
{
	TilePlanetData *source = GetTilePlanetData(sourceID);
	TilePlanetData * data = new TilePlanetData;
	data->m_heightMap = source->m_heightMap->CreateNewTextureFrom(1, XMFLOAT2(whichSquare.x, whichSquare.y), height * multiplierH, with * multiplierH);
	data->m_normalMap = source->m_normalMap->CreateNewTextureFrom(1, XMFLOAT2(whichSquare.x, whichSquare.y), height * multiplierN, with * multiplierN);

	GameTilePlanetData *gameTilePlanetData = new GameTilePlanetData;
	gameTilePlanetData->PreInit(m_resources);
	gameTilePlanetData->SetTilePlanetData(data);

	return m_renderAssetsStacks.m_gameTilePlanetData.CreateGameObject(gameTilePlanetData);
}

void RenderDevice::DeleteTilePlanetData(int id)
{
	m_renderAssetsStacks.m_gameTilePlanetData.DeleteGameObject(id);
}

TilePlanetData* RenderDevice::GetTilePlanetData(int id)
{
	return ((GameTilePlanetData*)m_renderAssetsStacks.m_gameTilePlanetData.GetGameObject(id))->GetTilePlanetData();
}*/