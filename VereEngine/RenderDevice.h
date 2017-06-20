#pragma once

#include "pch.h"
#include "DeviceResources.h"
#include "GameBaseObjectsStack.h"
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
#include "GameTilePlanetData.h"

struct RenderVariables
{
	D3D11_PRIMITIVE_TOPOLOGY topology;
	ID3D11InputLayout *inputLayout;
	ID3DX11EffectTechnique* activeTech;
	UINT idBuffer;
	UINT stride;
	std::vector<Material> material;
};

class RenderMesage
{
public:
	virtual void Use()
	{
		XMFLOAT3 EyePos = { 0.0f, 0.0f, 0.0f };

		/*btVector3 dirNorm = m_Transform.getOrigin();
		if (dirNorm.getX() != 0.0 && dirNorm.getY() != 0.0 && dirNorm.getZ() != 0.0)
		{
			dirNorm = dirNorm.normalize();
		}
		XMFLOAT3 dir = XMFLOAT3(dirNorm.getX(), dirNorm.getY(), dirNorm.getZ());*/

		XMMATRIX mesh = XMLoadFloat4x4(&VereMath::ConvertToXMFLOAT4X4(m_CameraOffset * m_Transform * m_Scaling));
		XMMATRIX viewProj = m_View * m_Proj;
		btTransform transformN = m_Transform;
		transformN.getOrigin().setZero();

		XMMATRIX meshN = XMLoadFloat4x4(&VereMath::ConvertToXMFLOAT4X4(transformN.inverse()));

		m_BaseEffect->SetEyePosW(EyePos);
		m_BaseEffect->SetMaterial(*m_Material);
		m_BaseEffect->SetViewProj(viewProj);
		m_BaseEffect->SetWorld(mesh);
		m_BaseEffect->SetWorldN(meshN);
		m_BaseEffect->SetFarZ(m_FarZ);
		m_BaseEffect->SetFarRangeMod(m_FarRangeMod);
		m_BaseEffect->SetFarModifier(m_FarModifier);
	}

	UINT m_ModelID;
	Material *m_Material;
	BaseEffect *m_BaseEffect;
	btTransform m_Transform;
	btTransform m_Scaling;
	btTransform m_CameraOffset;
	XMMATRIX m_View;
	XMMATRIX m_Proj;
	float m_FarZ;
	float m_FarRangeMod;
	float m_FarModifier;
	ID3D11RasterizerState* m_RasterizeState;
	};

class RenderDevice
{
public:
	RenderDevice();
	RenderDevice(DX::DeviceResources *resources);
	~RenderDevice();
	void Init(DX::DeviceResources *resources);

	void Render(RenderMesage *message);

	RenderAssetsStacks *GetRenderAssetsStacks() { return &m_renderAssetsStacks; }

	ID3D11Buffer *BuildVerticesBuffer(void *vertices, UINT size);
	ID3D11Buffer *BuildIndicesBuffer(std::vector<UINT> *indices);
	ID3D11ShaderResourceView *BuildTexture(void *map, UINT height, UINT width, UINT format, UINT mipLevels);

	int CreateModel(Model * model);
	void DeleteModel(int id);
	Model* GetModel(int id);

	int CreateTexture(void *map, UINT height, UINT width, UINT format, UINT mipLevels);
	void DeleteTexture(int id);
	ID3D11ShaderResourceView* GetTexture(int id);

	int CreateInputLayouts(int idVertex, int idEffect);
	void DeleteInputLayouts(int id);
	ID3D11InputLayout* GetInputLayouts(int id);

	int CreateVertex(ElementsVertex *vertex);
	void DeleteVertex(int id);
	GameVertex* GetVertex(int id);

	int CreateEffect(BaseEffect *effect);
	void DeleteEffect(int id);
	BaseEffect* GetEffect(int id);

	int CreateMeshBuffer(void *vertices, UINT vertexSize, UINT numElements, std::vector<UINT> *indeices);
	void DeleteMeshBuffer(int id);
	ID3D11Buffer* GetVerticesBuffer(int id);
	ID3D11Buffer* GetIndicesBuffer(int id);

	int CreateAmbientLight(AmbientLight * light);
	void DeleteAmbientLight(int id);
	AmbientLight* GetAmbientLight(int id);

	int CreateDirectionalLight(DirectionalLight * light);
	void DeleteDirectionalLight(int id);
	DirectionalLight* GetDirectionalLight(int id);

	int CreatePointLight(PointLight * light);
	void DeletePointLight(int id);
	PointLight* GetPointLight(int id);

	int CreateSpotLight(SpotLight * light);
	void DeleteSpotLight(int id);
	SpotLight* GetSpotLight(int id);

	int CreateTilePlanetData(TilePlanetData * light);
	int CreateTilePlanetDataFromAnother(int sourceID, XMINT2 whichSquare, float height, float with, float multiplierH, float multiplierN);
	void DeleteTilePlanetData(int id);
	TilePlanetData* GetTilePlanetData(int id);
private:
	DX::DeviceResources *m_resources;

	RenderAssetsStacks m_renderAssetsStacks;
};

__declspec(selectany) RenderDevice *GameRenderDeviceHandle = 0;
