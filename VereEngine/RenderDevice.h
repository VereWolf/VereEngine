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

class RenderMessage
{
public:
	virtual void Use();

	UINT m_ModelID;
	Material *m_Material;
	Effect *m_BaseEffect;
	btTransform m_Transform;
	btTransform m_Scaling;
	btTransform m_CameraOffset;
	XMMATRIX m_View;
	XMMATRIX m_Proj;
	float m_FarZ;
	float m_FarRangeMod;
	float m_FarModifier;
	ID3D11RasterizerState* m_RasterizeState;
	ID3D11BlendState * m_BlendState;
	D3D11_VIEWPORT * m_ViewPort;


	};

class RenderToScreenMessage : public RenderMessage
{
public:
	void Use();

	ID3D11ShaderResourceView * m_TargetSRV;
	ID3D11ShaderResourceView * m_DepthSRV;
};

class RenderDevice
{
public:
	RenderDevice();
	RenderDevice(DX::DeviceResources *resources);
	~RenderDevice();
	void Init(DX::DeviceResources *resources);

	void Render(RenderMessage *message);
	void RenderToScreen();

	void ClearMainRenderTarget();

	RenderAssetsStacks *GetRenderAssetsStacks() { return &m_renderAssetsStacks; }

	ID3D11Buffer *BuildVerticesBuffer(void *vertices, UINT size, UINT stride);
	ID3D11Buffer *BuildIndicesBuffer(std::vector<UINT> *indices);
	ID3D11ShaderResourceView *BuildTexture(void *map, UINT height, UINT width, UINT format, UINT mipLevels);

	void BindMainRenderTarget();
	void BindRenderTarget(ID3D11RenderTargetView *target, ID3D11DepthStencilView *depthStencil);
	D3D11_VIEWPORT *GetMainViewPort() { return &m_mainViewPort; }
	void CopyShaderResourceView(ID3D11ShaderResourceView *dest, ID3D11ShaderResourceView *src);

	ID3D11ShaderResourceView* GetMainDeepMapSRV() { return m_MainDeepMapSRV; }
	ID3D11ShaderResourceView* GetMainTargetMapSRV() { return m_MainTargetMapSRV; }

	void CreateQuadScreen();
	int GetQuadScreenID() { return m_quadScreenID; }
	int GetRenderIdRTS() { return m_RenderIdRTS; }

	void GenerateDeepOfSphere(int size);
	int GetDeepOfSphereTextureID() { return m_deepOfCircleID; }

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

	int CreateEffect(Effect *effect);
	void DeleteEffect(int id);
	Effect* GetEffect(int id);

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

	D3D11_VIEWPORT m_mainViewPort;

	int m_quadScreenID;
	int m_RenderIdRTS;

	int m_deepOfCircleID;

	ID3D11ShaderResourceView* m_MainDeepMapSRV;

	ID3D11ShaderResourceView* m_MainTargetMapSRV;

	ID3D11DepthStencilView* m_MainDeepMapDSV;

	ID3D11RenderTargetView* m_MainTargetMapRTV;
};

__declspec(selectany) RenderDevice *GameRenderDeviceHandle = 0;
