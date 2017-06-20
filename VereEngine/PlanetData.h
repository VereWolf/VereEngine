#ifndef TERRAIN_PLANET_DATA_H
#define TERRAIN_PLANET_DATA_H

#include "pch.h"
#include "GameObjectSpace.h"
#include "Camera.h"

class TerrainPlanetData: public GameObjectSpace
{
public:
	TerrainPlanetData();
	~TerrainPlanetData()
	{
		ReleaseCOM(m_heightSRV);
		ReleaseCOM(m_normalSRV);
		ReleaseCOM(m_TerrainLODVB);
		ReleaseCOM(m_TerrainLODIB);
	}

	void Init();
	void Update() {}

	DX::DeviceResources *GetDeviceResources() { return m_resources; }

	int BuildLODBuffers(DX::DeviceResources *resources, UINT &sizeOfVertex, UINT &indicesCount);

	bool LoadHeightFile(string nameFile);
	bool LoadNormalFile(string nameFile);

	//int BuildHeightResource(DX::DeviceResources *resources);
	//int BuildNormalResource(DX::DeviceResources *resources);

	ID3D11Buffer *const *GetTerrainLODVB() { return &m_TerrainLODVB; }
	ID3D11Buffer *GetTerrainLODIB() { return m_TerrainLODIB; }

	inline float GetRadius() { return m_radius; }
	inline void SetRadius(float n) { m_radius = n; }

	inline btMatrix3x3 GetBlockAnglMatrix(int side) { return m_blockMatrixs[side]; }
	inline int16_t GetMaxLevel() { return m_maxLevel; }
	inline void SetMaxLevel(int16_t n) { m_maxLevel = n; }
	inline int16_t GetMaxRenderLevel() { return m_maxRenderLevel; }
	inline void SetMaxRenderLevel(int16_t n) { m_maxRenderLevel = n; }
	inline int32_t GetNumPointInRowInCell() { return m_numPointsInRowInCell; }
	inline void SetNumPointInRowInCell(int n) { m_numPointsInRowInCell = n; }



	ID3D11ShaderResourceView *GetNormalSRV() { return m_normalSRV; }
	ID3D11ShaderResourceView *GetHeightSRV() { return m_heightSRV; }
private:
	float m_radius;

	int32_t m_numPointsInRowInCell;
	int16_t m_maxLevel;
	int16_t m_maxRenderLevel;

	ID3D11Buffer *m_TerrainLODVB;
	ID3D11Buffer *m_TerrainLODIB;

	ID3D11ShaderResourceView * m_normalSRV;
	ID3D11ShaderResourceView * m_heightSRV;
	std::vector<float> m_heightMap;
	std::vector<VBYTE4> m_normalMap;

	btMatrix3x3 m_blockMatrixs[12];
};

#endif // !TERRAIN_PLANET_DATA_H

