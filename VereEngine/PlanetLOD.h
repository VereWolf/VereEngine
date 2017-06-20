#ifndef TERRAIN_PLANET_LOD_H
#define TERRAIN_PLANET_LOD_H

#include "pch.h"
#include "PlanetData.h"

class PlanetRenderMessage : public RenderMesage
{
public:
	void Use()
	{
		btScalar S = m_Spacing1;
		btScalar S2 = m_Spacing2;

		btTransform mesh = btTransform(btMatrix3x3(S * S2, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, S * S2), m_Position + btVector3(0.0, 0.5 * S2, 0.0));
		mesh = btTransform(m_AngleMatrix) * mesh;
		btTransform meshCentre = m_CameraOffset * m_Transform;
		mesh = meshCentre * mesh;

		XMFLOAT3 EyePos = { 0.0f, 0.0f, 0.0f };
		XMFLOAT3 T = XMFLOAT3(0.0f, 1.0f, 0.0f);

		((TerrainPlanetLODEffect*)m_BaseEffect)->SetViewProj(m_View * m_Proj);
		((TerrainPlanetLODEffect*)m_BaseEffect)->SetWorld(XMLoadFloat4x4(&VereMath::ConvertToXMFLOAT4X4(mesh)));
		((TerrainPlanetLODEffect*)m_BaseEffect)->SetCentrePos(VereMath::ConvertToXMFLOAT3(meshCentre.getOrigin()));
		((TerrainPlanetLODEffect*)m_BaseEffect)->SetSpacing(((float)S2));
		((TerrainPlanetLODEffect*)m_BaseEffect)->SetRadius(((float)m_Radius));
		((TerrainPlanetLODEffect*)m_BaseEffect)->SetLevel(1.0f / pow(2, m_Level));
		((TerrainPlanetLODEffect*)m_BaseEffect)->SetOffset(XMFLOAT2(m_Coord.x, m_Coord.y));
		((TerrainPlanetLODEffect*)m_BaseEffect)->SetHeightMap(m_HeightSRV);
		((TerrainPlanetLODEffect*)m_BaseEffect)->SetNormalMap(m_NormalSRV);
		((TerrainPlanetLODEffect*)m_BaseEffect)->SetTang(T);
		((TerrainPlanetLODEffect*)m_BaseEffect)->SetFarZ(m_FarZ);
		((TerrainPlanetLODEffect*)m_BaseEffect)->SetFarRangeMod(m_FarRangeMod);
		((TerrainPlanetLODEffect*)m_BaseEffect)->SetFarModifier(m_FarModifier);
		((TerrainPlanetLODEffect*)m_BaseEffect)->SetCenterOfPlanet(VereMath::ConvertToXMFLOAT3(meshCentre.getOrigin()));
		((TerrainPlanetLODEffect*)m_BaseEffect)->SetRadiusOfPlanet(1.2f * m_Radius);
		((TerrainPlanetLODEffect*)m_BaseEffect)->SetFogStart(500.0f);
		((TerrainPlanetLODEffect*)m_BaseEffect)->SetFogRange(10000.0f);
		((TerrainPlanetLODEffect*)m_BaseEffect)->SetFogColor(XMFLOAT3(0.1f, 0.3f, 0.9f));
	}

	btVector3	m_Position;
	btTransform m_AngleMatrix;
	XMINT2 m_Coord;
	btScalar m_Spacing1;
	btScalar m_Spacing2;
	btScalar m_Radius;
	float m_Level;

	ID3D11ShaderResourceView * m_NormalSRV;
	ID3D11ShaderResourceView * m_HeightSRV;
};

class TerrainPlanetLOD
{
public:
	TerrainPlanetLOD()
	{

	}
	~TerrainPlanetLOD()
	{
		GameRenderDeviceHandle->DeleteModel(m_modelID);
	}

	void Init(TerrainPlanetData * master, int side, int level, XMINT2 coord, btVector3 position, btScalar scaling);
	void Render(btTransform camOffset, XMMATRIX camView, XMMATRIX camProj,
		float camFarZ, btScalar heightFar, btScalar aspect,
		float camFarRangeMod, float camModifier);

	static bool m_onlyRenderText;
private:
	btVector3  m_position;
	btVector3  m_angle;
	int m_side;
	int m_level;
	XMINT2 m_coord;
	btScalar m_scaling;
	//btVector3  m_normal[9];
	int m_modelID;

	std::vector<btVector3> m_hMap;
	//btVector3 m_CubeBox[8];
	btVector3 m_OffsetCube;
	btVector3 m_Centre;

	std::vector<TerrainPlanetLOD> m_blocks;

	TerrainPlanetData *m_data;
};

#endif // !TERRAIN_PLANET_LOD_H