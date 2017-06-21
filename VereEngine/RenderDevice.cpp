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

RenderDevice::RenderDevice()
{
	GameRenderDeviceHandle = this;
}

RenderDevice::RenderDevice(DX::DeviceResources *resources)
{
	GameRenderDeviceHandle = this;

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
}

void RenderDevice::Render(RenderMesage *message)
{

	m_resources->GetD3DDeviceContext()->RSSetState(message->m_RasterizeState);

	Model *model = GetModel(message->m_ModelID);
	ID3D11Buffer *VB = GetVerticesBuffer(model->idMeshBuffer);
	ID3D11Buffer *IB = GetIndicesBuffer(model->idMeshBuffer);
	ID3D11InputLayout *IL = GetInputLayouts(model->idInputLayouts);
	BaseEffect *BE = GetEffect(model->idEffect);
	GameVertex *V = GetVertex(model->idVertex);
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

ID3D11Buffer *RenderDevice::BuildVerticesBuffer(void *vertices, UINT size)
{
	ID3D11Buffer *VB;

	D3D11_BUFFER_DESC vertexBuffer;

	vertexBuffer.Usage = D3D11_USAGE_DEFAULT;
	vertexBuffer.ByteWidth = size;
	vertexBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBuffer.CPUAccessFlags = 0;
	vertexBuffer.MiscFlags = 0;
	vertexBuffer.StructureByteStride = 0;

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
		tbsd.pSysMem = &((std::vector<VBYTE4>*)map)->at(0);
		tbsd.SysMemPitch = width * sizeof(VBYTE4);
		tdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	case DXGI_FORMAT_R32_FLOAT:
		tbsd.pSysMem = &((std::vector<float>*)map)->at(0);
		tbsd.SysMemPitch = width * sizeof(float);
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
	srvDesc.Texture2D.MipLevels = tdesc.MipLevels;
	srvDesc.Format = tdesc.Format;

	m_resources->GetD3DDevice()->CreateShaderResourceView(mapResource, &srvDesc, &textureSRV);

	ReleaseCOM(mapResource);

	return(textureSRV);
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
	if (id >= 0)
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
	return ((GameTexture*)m_renderAssetsStacks.m_gameTextures.GetGameObject(id))->GetTexture();
}

int RenderDevice::CreateInputLayouts(int idVertices, int idEffect)
{
	ElementsVertex *gameVertices = ((GameVertex*)m_renderAssetsStacks.m_gameVertices.GetGameObject(idVertices))->GetVertex();
	BaseEffect *baseEffect = ((GameEffect*)m_renderAssetsStacks.m_gameEffects.GetGameObject(idVertices))->GetEffect();

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
	return ((GameInputLayouts*)m_renderAssetsStacks.m_gameInputLayouts.GetGameObject(id))->GetInputLayouts();;
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

int RenderDevice::CreateEffect(BaseEffect *effect)
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

BaseEffect* RenderDevice::GetEffect(int id)
{
	return ((GameEffect*)m_renderAssetsStacks.m_gameEffects.GetGameObject(id))->GetEffect();
}

int RenderDevice::CreateMeshBuffer(void *vertices, UINT vertexSize, UINT numElements, std::vector<UINT> *indices)
{
	UINT S = vertexSize * numElements;
	ID3D11Buffer * VB = BuildVerticesBuffer(vertices, S);
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

int RenderDevice::CreateTilePlanetData(TilePlanetData * data)
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
}