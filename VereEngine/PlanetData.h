#pragma once

#include "pch.h"
#include "GameObjectSpace.h"
#include "Camera.h"
#include "IDRegistr.h"

class PlanetData : public GameObjectSpace
{
public:
	PlanetData();
	~PlanetData()
	{
		ReleaseCOM(m_heightSRV);
		ReleaseCOM(m_normalSRV);
		ReleaseCOM(m_LODVB);
		ReleaseCOM(m_LODIB);

		ReleaseCOM(m_AtmosphereDeepMapSRV);
		ReleaseCOM(m_CloudsDeepMapSRV);
		ReleaseCOM(m_WaterTopDeepMapSRV);
		ReleaseCOM(m_PlanetDeepMapSRV);

		ReleaseCOM(m_AtmosphereTargetMapSRV);
		ReleaseCOM(m_CloudsTargetMapSRV);
		ReleaseCOM(m_WaterTopTargetMapSRV);
		ReleaseCOM(m_PlanetTargetMapSRV);

		ReleaseCOM(m_AtmosphereDeepMapDSV);
		ReleaseCOM(m_CloudsDeepMapDSV);
		ReleaseCOM(m_WaterTopDeepMapDSV);
		ReleaseCOM(m_PlanetDeepMapDSV);

		ReleaseCOM(m_AtmosphereTargetMapRTV);
		ReleaseCOM(m_CloudsTargetMapRTV);
		ReleaseCOM(m_WaterTopTargetMapRTV);
		ReleaseCOM(m_PlanetTargetMapRTV);

		SafeDelete(m_planetElementID);
	}

	void Init();
	void Update() {}

	DX::DeviceResources *GetDeviceResources() { return m_resources; }

	int BuildLODBuffers(DX::DeviceResources *resources, UINT &sizeOfVertex, UINT &indicesCount);

	void SetRenderIdWater(int n) { m_RenderIdWater = n; }
	int GetRenderIdWater() { return m_RenderIdWater; }
	void SetRenderIdAtmosphere(int n) { m_RenderIdAtmosphere = n; }
	int GetRenderIdAtmosphere() { return m_RenderIdAtmosphere; }
	void SetRenderIdClouds(int n) { m_RenderIdClouds = n; }
	int GetRenderIdClouds() { return m_RenderIdClouds; }

	ID3D11ShaderResourceView* GetAtmosphereDeepMapSRV() { return m_AtmosphereDeepMapSRV; }
	ID3D11ShaderResourceView* GetCloudsDeepMapSRV() { return m_CloudsDeepMapSRV; }
	ID3D11ShaderResourceView* GetWaterTopDeepMapSRV() { return m_WaterTopDeepMapSRV; }
	ID3D11ShaderResourceView* GetWaterBottomDeepMapSRV() { return m_WaterBottomDeepMapSRV; }
	ID3D11ShaderResourceView* GetPlanetDeepMapSRV() { return m_PlanetDeepMapSRV; }

	ID3D11ShaderResourceView* GetAtmosphereTargetMapSRV() { return m_AtmosphereTargetMapSRV; }
	ID3D11ShaderResourceView* GetCloudsTargetMapSRV() { return m_CloudsTargetMapSRV; }
	ID3D11ShaderResourceView* GetWaterTopTargetMapSRV() { return m_WaterTopTargetMapSRV; }
	ID3D11ShaderResourceView* GetWaterBottomTargetMapSRV() { return m_WaterBottomTargetMapSRV; }
	ID3D11ShaderResourceView* GetPlanetTargetMapSRV() { return m_PlanetTargetMapSRV; }

	ID3D11DepthStencilView* GetAtmosphereDeepMapDSV() { return m_AtmosphereDeepMapDSV; }
	ID3D11DepthStencilView* GetCloudsDeepMapDSV() { return m_CloudsDeepMapDSV; }
	ID3D11DepthStencilView* GetWaterTopDeepMapDSV() { return m_WaterTopDeepMapDSV; }
	ID3D11DepthStencilView* GetWaterBottomDeepMapDSV() { return m_WaterBottomDeepMapDSV; }
	ID3D11DepthStencilView* GetPlanetDeepMapDSV() { return m_PlanetDeepMapDSV; }

	ID3D11RenderTargetView* GetAtmosphereTargetMapRTV() { return m_AtmosphereTargetMapRTV; }
	ID3D11RenderTargetView* GetCloudsTargetMapRTV() { return m_CloudsTargetMapRTV; }
	ID3D11RenderTargetView* GetWaterTopTargetMapRTV() { return m_WaterTopTargetMapRTV; }
	ID3D11RenderTargetView* GetWaterBottomTargetMapRTV() { return m_WaterBottomTargetMapRTV; }
	ID3D11RenderTargetView* GetPlanetTargetMapRTV() { return m_PlanetTargetMapRTV; }

	D3D11_VIEWPORT *GetViewPort() { return &m_Viewport; }

	ID3D11Buffer *const *GetLODVB() { return &m_LODVB; }
	ID3D11Buffer *GetLODIB() { return m_LODIB; }

	inline float GetRadiusOfTerrain() { return m_radiusOfTerrain; }
	inline void SetRadiusOfTerrain(float n) { m_radiusOfTerrain = n; }

	inline float GetRadiusOfWater() { return m_radiusOfWater; }
	inline void SetRadiusOfWater(float n) { m_radiusOfWater = n; }

	inline float GetRadiusOfAtmosphere() { return m_radiusOfAtmosphere; }
	inline void SetRadiusOfAtmosphere(float n) { m_radiusOfAtmosphere = n; }

	inline float GetRadiusOfClouds() { return m_radiusOfClouds; }
	inline void SetRadiusOfClouds(float n) { m_radiusOfClouds = n; }

	float GetFogAStart() { return m_fogAStart; }
	void SetFogAStart(float s) { m_fogAStart = s; }
	float GetFogARange() { return m_fogARange; }
	void SetFogARange(float r) { m_fogARange = r; }
	XMFLOAT3 GetFogAColor() { return m_fogAColor; }
	void SetFogAColor(XMFLOAT3 c) { m_fogAColor = c; }

	float GetFogWStart() { return m_fogWStart; }
	void SetFogWStart(float s) { m_fogWStart = s; }
	float GetFogWRange() { return m_fogWRange; }
	void SetFogWRange(float r) { m_fogWRange = r; }
	XMFLOAT3 GetFogWColor() { return m_fogWColor; }
	void SetFogWColor(XMFLOAT3 c) { m_fogWColor = c; }

	inline btMatrix3x3 GetBlockAnglMatrix(int side) { return m_blockMatrixs[side]; }
	int GetNearestSide(bool withOffset);
	inline btVector3 GetTangent() { return m_tang; }
	inline int16_t GetMaxLevel() { return m_maxLevel; }
	inline void SetMaxLevel(int16_t n) { m_maxLevel = n; }
	inline int16_t GetMaxRenderLevel() { return m_maxRenderLevel; }
	inline void SetMaxRenderLevel(int16_t n) { m_maxRenderLevel = n; }
	inline int16_t GetNumPointInRowInCell() { return m_numPointsInRowInCell; }
	inline void SetNumPointInRowInCell(int n) { m_numPointsInRowInCell = n; }
	inline int16_t GetLoadDataAfterAgain() { return m_loadDataAfterAgain; }
	inline void SetLoadDataAfterAgain(int16_t d) { m_loadDataAfterAgain = d; }
	inline int16_t GetMaxLevelOfStreaming() { return m_maxLevelOfStreaming; }
	inline void SetMaxLevelOfStreaming(int16_t d) { m_maxLevelOfStreaming = d; }
	inline string GetPosFix(int side) { return m_posfix[side]; }

	ID3D11ShaderResourceView *GetNormalSRV() { return m_normalSRV; }
	ID3D11ShaderResourceView *GetHeightSRV() { return m_heightSRV; }

	void GenerateCoord(float height, float width, float level);
private:
	float m_radiusOfTerrain;
	float m_radiusOfWater;
	float m_radiusOfAtmosphere;
	float m_radiusOfClouds;

	float m_fogAStart;
	float m_fogARange;
	XMFLOAT3 m_fogAColor;

	float m_fogWStart;
	float m_fogWRange;
	XMFLOAT3 m_fogWColor;

	int16_t m_numPointsInRowInCell;
	int16_t m_maxLevel;
	int16_t m_maxRenderLevel;
	int16_t m_loadDataAfterAgain;
	int16_t m_maxLevelOfStreaming;

	std::string m_posfix[12];

	int m_RenderIdWater;
	int m_RenderIdAtmosphere;
	int m_RenderIdClouds;

	ID3D11Buffer *m_LODVB;
	ID3D11Buffer *m_LODIB;

	ID3D11ShaderResourceView * m_normalSRV;
	ID3D11ShaderResourceView * m_heightSRV;
	std::vector<float> m_heightMap;
	std::vector<VBYTE4> m_normalMap;

	btMatrix3x3 m_blockMatrixs[12];
	btVector3 m_tang;

	UINT m_Width;
	UINT m_Height;

	ID3D11ShaderResourceView* m_AtmosphereDeepMapSRV;
	ID3D11ShaderResourceView* m_CloudsDeepMapSRV;
	ID3D11ShaderResourceView* m_WaterTopDeepMapSRV;
	ID3D11ShaderResourceView* m_WaterBottomDeepMapSRV;
	ID3D11ShaderResourceView* m_PlanetDeepMapSRV;

	ID3D11ShaderResourceView* m_AtmosphereTargetMapSRV;
	ID3D11ShaderResourceView* m_CloudsTargetMapSRV;
	ID3D11ShaderResourceView* m_WaterTopTargetMapSRV;
	ID3D11ShaderResourceView* m_WaterBottomTargetMapSRV;
	ID3D11ShaderResourceView* m_PlanetTargetMapSRV;

	ID3D11DepthStencilView* m_AtmosphereDeepMapDSV;
	ID3D11DepthStencilView* m_CloudsDeepMapDSV;
	ID3D11DepthStencilView* m_WaterTopDeepMapDSV;
	ID3D11DepthStencilView* m_WaterBottomDeepMapDSV;
	ID3D11DepthStencilView* m_PlanetDeepMapDSV;

	ID3D11RenderTargetView* m_AtmosphereTargetMapRTV;
	ID3D11RenderTargetView* m_CloudsTargetMapRTV;
	ID3D11RenderTargetView* m_WaterTopTargetMapRTV;
	ID3D11RenderTargetView* m_WaterBottomTargetMapRTV;
	ID3D11RenderTargetView* m_PlanetTargetMapRTV;

	D3D11_VIEWPORT m_Viewport;

public:
	std::vector<GameComponent*> m_planetElements;
	IDRegistr *m_planetElementID;
	VereQueue<int> m_planetElementsInProcess;
};

