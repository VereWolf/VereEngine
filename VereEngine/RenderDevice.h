#pragma once

#include "pch.h"
#include "DeviceResources.h"
#include "GameBaseObjectsStack.h"
#include "GameTexture.h"
#include "GameOutputTexture.h"
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
#include "PlanetData.h"

struct RenderVariables
{
	D3D11_PRIMITIVE_TOPOLOGY topology;
	ID3D11InputLayout *inputLayout;
	ID3DX11EffectTechnique* activeTech;
	UINT idBuffer;
	UINT stride;
	std::vector<Material> material;
};


class RenderMessage: public BaseMessage
{
public:
	virtual void Use();

	UINT m_ModelID;
	Material *m_Material;
	PlanetData *m_PlanetData;
	btTransform m_Transform;
	btTransform m_Scaling;
	static btTransform m_CameraOffset;
	static XMMATRIX m_View;
	static XMMATRIX m_Proj;
	static float m_FarZ;
	static float m_FarRangeMod;
	static float m_FarModifier;
	static float m_HeightFar;
	static float m_Aspect;
	int m_StartIndex;
	int m_CountIndex;
	ID3D11RasterizerState* m_RasterizeState;
	ID3D11BlendState * m_BlendState;
	static D3D11_VIEWPORT * m_ViewPort;
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
	RenderDevice(DX::DeviceResources *resources, float farRangeMod);
	~RenderDevice();
	void Init(DX::DeviceResources *resources);

	void Render(RenderMessage *message);
	void RenderToScreen();
	void ComputeShader(ComputeMessage *message);

	void ClearMainRenderTarget();

	RenderAssetsStacks *GetRenderAssetsStacks() { return &m_renderAssetsStacks; }

	ID3D11Buffer *BuildVerticesBuffer(void *vertices, UINT size, UINT stride);
	ID3D11Buffer *BuildIndicesBuffer(std::vector<UINT> *indices);
	ID3D11ShaderResourceView *BuildTexture(void *map, UINT height, UINT width, UINT format, UINT mipLevels);

	void BindMainRenderTarget();
	void BindRenderTarget(ID3D11RenderTargetView *target, ID3D11DepthStencilView *depthStencil);
	D3D11_VIEWPORT *GetMainViewPort() { return &m_mainViewPort; }

	ID3D11ShaderResourceView* GetMainDeepMapSRV() { return m_MainDeepMapSRV; }
	ID3D11ShaderResourceView* GetMainTargetMapSRV() { return m_MainTargetMapSRV; }

	void CreateQuadScreen();
	int GetQuadScreenID() { return m_quadScreenID; }
	int GetRenderIdRTS() { return m_RenderIdRTS; }

	int CreateModel(Model * model);
	void DeleteModel(int id);
	Model* GetModel(int id);

	int CreateTexture(void *map, UINT height, UINT width, UINT format, UINT mipLevels);
	int AddTexture(ID3D11ShaderResourceView * srv);
	void DeleteTexture(int id);
	ID3D11ShaderResourceView* GetTexture(int id);

	int CreateOutputTexture(UINT width, UINT height, DXGI_FORMAT format, UINT mipLevels);
	void DeleteOutputTexture(int id);
	int DeleteOutputTextureButSRVToTextureStack(int id);// UAV is deleted and SRV is moved to texture stack
	ID3D11ShaderResourceView* GetOutputTextureSRV(int id);
	ID3D11UnorderedAccessView* GetOutputTextureUAV(int id);

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

	/*int CreateTilePlanetData(TilePlanetData * light);
	int CreateTilePlanetDataFromAnother(int sourceID, XMINT2 whichSquare, float height, float with, float multiplierH, float multiplierN);
	void DeleteTilePlanetData(int id);
	TilePlanetData* GetTilePlanetData(int id);*/
private:
	DX::DeviceResources *m_resources;

	RenderAssetsStacks m_renderAssetsStacks;

	D3D11_VIEWPORT m_mainViewPort;

	int m_quadScreenID;
	int m_RenderIdRTS;

	ID3D11ShaderResourceView* m_MainDeepMapSRV;

	ID3D11ShaderResourceView* m_MainTargetMapSRV;

	ID3D11DepthStencilView* m_MainDeepMapDSV;

	ID3D11RenderTargetView* m_MainTargetMapRTV;
};

__declspec(selectany) RenderDevice *GameRenderDeviceHandle = 0;
