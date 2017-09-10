#pragma once

#include "pch.h"
#include "GameObjectSpace.h"
#include "Camera.h"
#include "IDRegistr.h"
#include "GamePlanetTile.h"

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

<<<<<<< HEAD
=======
	std::string GetFullPath();

>>>>>>> VereEngine-Planet
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
	inline int16_t GetCurrentMaxLevel() { return m_currentMaxLevel; }
	inline void SetCurrentMaxLevel(int16_t n) { m_currentMaxLevel = n; }
	inline int16_t GetNumPointInRowInCell() { return m_numPointsInRowInCell; }
	inline void SetNumPointInRowInCell(int n) { m_numPointsInRowInCell = n; }
	inline int16_t GetNumPointInRowInBigCell() { return m_numPointsInRowInBigCell; }
	inline void SetNumPointInRowInBigCell(int n) { m_numPointsInRowInBigCell = n; }
	inline int16_t GetLoadDataPer() { return m_loadDataPer; }
	inline void SetLoadDataPer(int16_t d) { m_loadDataPer = d; }
	inline int16_t GetLoadDataMaxLvl() { return m_loadDataMaxLvl; }
	inline void SetLoadDataMaxLvl(int16_t d) { m_loadDataMaxLvl = d; }
	inline int16_t GetLoadTilesLvl() { return m_loadTilesLvl; }
	inline void SetLoadTilesLvl(int16_t d) { m_loadTilesLvl = d; }
	inline string GetPosFix(int side) { return m_posfix[side]; }

	int16_t GetMaxNumBlockBig() { return m_maxNumBlockBig; }
	void SetMaxNumBlockBig(int16_t d) { m_maxNumBlockBig = d; }
	int16_t GetCurerentNumBlockBig() { return m_currentNumBlockBig; }
	void SetCurrentNumBlockBig(int16_t d) { m_currentNumBlockBig = d; }

	int16_t GetMaxNumBlockSmall() { return m_maxNumBlockSmall; }
	void SetMaxNumBlockSmall(int16_t d) { m_maxNumBlockSmall = d; }
	int16_t GetCurerentNumBlockSmall() { return m_currentNumBlockSmall; }
	void SetCurrentNumBlockSmall(int16_t d) { m_currentNumBlockSmall = d; }

	ID3D11ShaderResourceView *GetNormalSRV() { return m_normalSRV; }
	ID3D11ShaderResourceView *GetHeightSRV() { return m_heightSRV; }

	int GetIDHeightMapBig() { return m_idHeightMapBig; }
	int GetIDNormalMapBig() { return m_idNormalMapBig; }
	int GetIDEnviromentMapBig() { return m_idEnviromentMapBig; }
	int GetIDTreesMapBig() { return m_idTreesMapBig; }

	int GetIDHeightMapSmall() { return m_idHeightMapSmall; }
	int GetIDNormalMapSmall() { return m_idNormalMapSmall; }
	int GetIDEnviromentMapSmall() { return m_idEnviromentMapSmall; }
	int GetIDTreesMapSmall() { return m_idTreesMapSmall; }

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
	int16_t m_numPointsInRowInBigCell;
	int16_t m_maxLevel;
	int16_t m_currentMaxLevel;
	int16_t m_loadDataPer;
	int16_t m_loadDataMaxLvl;
	int16_t m_loadTilesLvl;
	int16_t m_maxNumBlockBig;
	int16_t m_currentNumBlockBig;
	int16_t m_maxNumBlockSmall;
	int16_t m_currentNumBlockSmall;

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

<<<<<<< HEAD
=======
protected:
	std::string m_planetPath;
	static std::string m_rootFolder;

	int m_idHeightMapBig;
	int m_idNormalMapBig;
	int m_idEnviromentMapBig;
	int m_idTreesMapBig;

	int m_idHeightMapSmall;
	int m_idNormalMapSmall;
	int m_idEnviromentMapSmall;
	int m_idTreesMapSmall;
>>>>>>> VereEngine-Planet
public:
	std::vector<GameComponent*> m_planetElements;
	IDRegistr *m_planetElementID;
	VereQueue<int> m_planetElementsInProcess;
};

