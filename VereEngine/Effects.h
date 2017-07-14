#pragma once

#include "EnviromentHelper.h"
#include "DeviceResources.h"
#include "pch.h"

struct Coordinate
{
	Coordinate()
	{
		x = 0;
		y = 0;
		level = 0;
	}

	Coordinate(UINT itsX, UINT itsY, UINT itsL)
	{
		x = itsX;
		y = itsY;
		level = itsL;
	}


	UINT x;
	UINT y;
	UINT level;
};

#pragma region Effect
class Effect
{
public:
	Effect(DX::DeviceResources *resources, const std::string& filename);

	virtual ~Effect();

private:
	Effect(const Effect& rhs) {};
	Effect& operator=(const Effect& rhs);

protected:
	ID3DX11Effect* mFX;
};
#pragma endregion

#pragma region BaseEffect
class BaseEffect : public Effect
{
public:
	BaseEffect(DX::DeviceResources *resources, const std::string& filename);
	~BaseEffect() {};

	void SetViewProj(CXMMATRIX &M) { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProj(CXMMATRIX &M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetProj(CXMMATRIX &M) { Proj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetView(CXMMATRIX &M) { View->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX &M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldN(CXMMATRIX &M) { WorldN->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(XMFLOAT3 &V) { EyePosW->SetRawValue(&V, 0, sizeof(XMFLOAT3)); }
	void SetFogColor(const XMFLOAT3& v) { FogColor->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetFarZ(const float& v) { FarZ->SetRawValue(&v, 0, sizeof(float)); }
	void SetFarRangeMod(const float& v) { FarRangeMod->SetRawValue(&v, 0, sizeof(float)); }
	void SetFarModifier(const float& v) { FarModifier->SetRawValue(&v, 0, sizeof(float)); }
	void SetFogStart(const float& v) { FogStart->SetRawValue(&v, 0, sizeof(float)); }
	void SetFogRange(const float& v) { FogRange->SetRawValue(&v, 0, sizeof(float)); }
	void SetCenterOfPlanet(const XMFLOAT3& v) { CenterOfPlanet->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetRadiusOfPlanet(const float& v) { RadiusOfPlanet->SetRawValue(&v, 0, sizeof(float)); }
	void SetRadiusOfAtmosphere(const float& v) { RadiusOfPlanet->SetRawValue(&v, 0, sizeof(float)); }
	void SetSkyColor(const XMFLOAT4& v) { SkyColor->SetRawValue(&v, 0, sizeof(XMFLOAT4)); }
	void SetMaterial(const Material& m) { Mat->SetRawValue(&m, 0, sizeof(Material)); }

	void SetVariable(const void* V, UINT size, UINT index) { m_Variable[index]->SetRawValue(V, 0, size); }
	void SetVector(const void* V, UINT size, UINT index) { m_Vector[index]->SetRawValue(V, 0, size); }
	void SetMatrix(CXMMATRIX &M, UINT index) { m_Matrix[index]->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetShaderResource(ID3D11ShaderResourceView *SR, UINT index) { m_ShaderResource[index]->SetResource(SR); }

	void SetVariableSize(int size) { m_VariableSize = size; }
	void SetVectorSize(int size) { m_VectorSize = size; }
	void SetMatrixSize(int size) { m_MatrixSize = size; }
	void SetShaderResourceSize(int size) { m_ShaderResourceSize = size; }

	void SetOnlyGetSize(bool B) { m_onlyGetSize = B; }
	void SetAllSize();

	void BindTechnique(UINT index, LPCSTR name);
	void BindValue(UINT index, LPCSTR name);
	void BindVector(UINT index, LPCSTR name);
	void BindMatrix(UINT index, LPCSTR name);
	void BindShaderResource(UINT index, LPCSTR name);


	ID3DX11EffectTechnique* Light1Tech;

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* Proj;
	ID3DX11EffectMatrixVariable* View;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldN;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* FogColor;
	ID3DX11EffectVariable* FarZ;
	ID3DX11EffectVariable* FarRangeMod;
	ID3DX11EffectVariable* FarModifier;
	ID3DX11EffectVariable* FogStart;
	ID3DX11EffectVariable* FogRange;
	ID3DX11EffectVectorVariable* CenterOfPlanet;
	ID3DX11EffectVariable* RadiusOfPlanet;
	ID3DX11EffectVariable* RadiusOfAtmosphere;
	ID3DX11EffectVectorVariable* SkyColor;
	ID3DX11EffectVariable* Mat;

	std::vector<ID3DX11EffectTechnique*> m_Technique;
	std::vector<LPCSTR> m_TechniqueName;
	UINT m_TechniqueSize;

	std::vector<ID3DX11EffectVariable*> m_Variable;
	std::vector<LPCSTR> m_VariableName;
	UINT m_VariableSize;

	std::vector<ID3DX11EffectVectorVariable*> m_Vector;
	std::vector<LPCSTR> m_VectorName;
	UINT m_VectorSize;

	std::vector<ID3DX11EffectMatrixVariable*> m_Matrix;
	std::vector<LPCSTR> m_MatrixName;
	UINT m_MatrixSize;

	std::vector<ID3DX11EffectShaderResourceVariable*> m_ShaderResource;
	std::vector<LPCSTR> m_ShaderResourceName;
	UINT m_ShaderResourceSize;

	bool m_onlyGetSize;
};
#pragma endregion

#pragma region WorldEffect
class WorldEffect : public Effect
{
public:
	WorldEffect(DX::DeviceResources *resources, const std::string& filename);
	~WorldEffect() {};


	void SetViewProj(CXMMATRIX &M) { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProj(CXMMATRIX &M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetProj(CXMMATRIX &M) { Proj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetView(CXMMATRIX &M) { View->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX &M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldN(CXMMATRIX &M) { WorldN->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(XMFLOAT3 &V) { EyePosW->SetRawValue(&V, 0, sizeof(XMFLOAT3)); }
	void SetFogColor(const XMFLOAT3& v) { FogColor->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetFogStart(const float& v) { FogStart->SetRawValue(&v, 0, sizeof(float)); }
	void SetFogRange(const float& v) { FogRange->SetRawValue(&v, 0, sizeof(float)); }
	void SetCenterOfPlanet(const XMFLOAT3& v) { CenterOfPlanet->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetRadiusOfPlanet(const float& v) { RadiusOfPlanet->SetRawValue(&v, 0, sizeof(float)); }
	void SetRadiusOfAtmosphere(const float& v) { RadiusOfPlanet->SetRawValue(&v, 0, sizeof(float)); }
	void SetSkyColor(const XMFLOAT4& v) { SkyColor->SetRawValue(&v, 0, sizeof(XMFLOAT4)); }
	void SetMaterial(const Material& m) { Mat->SetRawValue(&m, 0, sizeof(Material)); }
	//void SetSunVector(const XMFLOAT3& v) { SunVector->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	//void SetLight(const float& v) { Light->SetRawValue(&v, 0, sizeof(float)); }


	ID3DX11EffectTechnique* Light1Tech;

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* Proj;
	ID3DX11EffectMatrixVariable* View;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldN;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* FogColor;
	ID3DX11EffectVariable* FogStart;
	ID3DX11EffectVariable* FogRange;
	ID3DX11EffectVectorVariable* CenterOfPlanet;
	ID3DX11EffectVariable* RadiusOfPlanet;
	ID3DX11EffectVariable* RadiusOfAtmosphere;
	ID3DX11EffectVectorVariable* SkyColor;
	ID3DX11EffectVariable* Mat;
	//ID3DX11EffectVectorVariable* SunVector;
	//ID3DX11EffectVariable* Light;
};
#pragma endregion

#pragma region Block
class SkyBoxEffect : public WorldEffect
{
public:
	SkyBoxEffect(DX::DeviceResources *resources, const std::string& filename);
	~SkyBoxEffect() {};

	void SetViewProj(CXMMATRIX &M) { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX &M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }


	ID3DX11EffectTechnique* Light1Tech;

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* World;
};

#pragma endregion

#pragma region LOD
class LODEffect : public WorldEffect
{
public:
	LODEffect(DX::DeviceResources *resources, const std::string& filename);
	~LODEffect() {};

	void SetViewProj(CXMMATRIX &M) { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProj(CXMMATRIX &M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetProj(CXMMATRIX &M) { Proj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetView(CXMMATRIX &M) { View->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX &M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(XMFLOAT3 &V) { EyePosW->SetRawValue(&V, 0, sizeof(XMFLOAT3)); }
	void SetStartOfLOD(const float& v) { StartOfLOD->SetRawValue(&v, 0, sizeof(float)); }
	void SetStartOfLODOfTrees(const float& v) { StartOfLODOfTrees->SetRawValue(&v, 0, sizeof(float)); }
	void SetCoord(const XMINT2& v) { Coord->SetRawValue(&v, 0, sizeof(XMINT2)); }
	void SetSpacing(const float& v) { Spacing->SetRawValue(&v, 0, sizeof(float)); }
	void SetHeightMap(ID3D11ShaderResourceView* tex) { HeightMap->SetResource(tex); }
	void SetNormalMap(ID3D11ShaderResourceView* tex) { NormalMap->SetResource(tex); }
	void SetTreesMap(ID3D11ShaderResourceView* tex) { TreesMap->SetResource(tex); }
	void SetHeightTile_1(ID3D11ShaderResourceView* tex) { HeightTile_1->SetResource(tex); }
	void SetHeightTile_2(ID3D11ShaderResourceView* tex) { HeightTile_2->SetResource(tex); }
	void SetNormalTile_1(ID3D11ShaderResourceView* tex) { NormalTile_1->SetResource(tex); }
	void SetNormalTile_2(ID3D11ShaderResourceView* tex) { NormalTile_2->SetResource(tex); }


	ID3DX11EffectTechnique* Light1Tech;

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* Proj;
	ID3DX11EffectMatrixVariable* View;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVariable* StartOfLOD;
	ID3DX11EffectVariable* StartOfLODOfTrees;
	ID3DX11EffectVectorVariable* Coord;
	ID3DX11EffectVariable* Spacing;

	ID3DX11EffectShaderResourceVariable* HeightMap;
	ID3DX11EffectShaderResourceVariable* NormalMap;
	ID3DX11EffectShaderResourceVariable* TreesMap;
	ID3DX11EffectShaderResourceVariable* HeightTile_1;
	ID3DX11EffectShaderResourceVariable* HeightTile_2;
	ID3DX11EffectShaderResourceVariable* NormalTile_1;
	ID3DX11EffectShaderResourceVariable* NormalTile_2;
};

#pragma endregion

#pragma region TerrainLOD
class TerrainLODEffect : public BaseEffect
{
public:
	TerrainLODEffect(DX::DeviceResources *resources, const std::string& filename);
	~TerrainLODEffect() {};

	void SetViewProj(CXMMATRIX &M) { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProj(CXMMATRIX &M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetProj(CXMMATRIX &M) { Proj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetView(CXMMATRIX &M) { View->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX &M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(XMFLOAT3 &V) { EyePosW->SetRawValue(&V, 0, sizeof(XMFLOAT3)); }
	void SetCentrePos(XMFLOAT3 &V) { CentrePos->SetRawValue(&V, 0, sizeof(XMFLOAT3)); }
	void SetOffset(XMFLOAT2 &V) { Offset->SetRawValue(&V, 0, sizeof(XMFLOAT2)); }
	void SetTang(XMFLOAT3 &V) { Tang->SetRawValue(&V, 0, sizeof(XMFLOAT3)); }
	void SetSpacing(const float& v) { Spacing->SetRawValue(&v, 0, sizeof(float)); }
	void SetRadius(const float& v) { Radius->SetRawValue(&v, 0, sizeof(float)); }
	void SetLevel(const float& v) { Level->SetRawValue(&v, 0, sizeof(float)); }

	void SetHeightMap(ID3D11ShaderResourceView* tex) { HeightMap->SetResource(tex); }
	void SetNormalMap(ID3D11ShaderResourceView* tex) { NormalMap->SetResource(tex); }

	ID3DX11EffectTechnique* Light1Tech;

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* Proj;
	ID3DX11EffectMatrixVariable* View;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* CentrePos;
	ID3DX11EffectVectorVariable* Offset;
	ID3DX11EffectVectorVariable* Tang;
	ID3DX11EffectVariable* Spacing;
	ID3DX11EffectVariable* Radius;
	ID3DX11EffectVariable* Level;

	ID3DX11EffectShaderResourceVariable* HeightMap;
	ID3DX11EffectShaderResourceVariable* NormalMap;
};

#pragma endregion

#pragma region Atmosphere
class AtmosphereEffect : public BaseEffect
{
public:
	AtmosphereEffect(DX::DeviceResources *resources, const std::string& filename);
	~AtmosphereEffect() {};

	void SetViewProj(CXMMATRIX &M) { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProj(CXMMATRIX &M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetProj(CXMMATRIX &M) { Proj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetView(CXMMATRIX &M) { View->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX &M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(XMFLOAT3 &V) { EyePosW->SetRawValue(&V, 0, sizeof(XMFLOAT3)); }
	void SetCentrePos(XMFLOAT3 &V) { CentrePos->SetRawValue(&V, 0, sizeof(XMFLOAT3)); }
	void SetOffset(XMFLOAT2 &V) { Offset->SetRawValue(&V, 0, sizeof(XMFLOAT2)); }
	void SetSpacing(const float& v) { Spacing->SetRawValue(&v, 0, sizeof(float)); }
	void SetRadius(const float& v) { Radius->SetRawValue(&v, 0, sizeof(float)); }
	void SetLevel(const float& v) { Level->SetRawValue(&v, 0, sizeof(float)); }

	ID3DX11EffectTechnique* Light1Tech;

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* Proj;
	ID3DX11EffectMatrixVariable* View;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* CentrePos;
	ID3DX11EffectVectorVariable* Offset;
	ID3DX11EffectVariable* Spacing;
	ID3DX11EffectVariable* Radius;
	ID3DX11EffectVariable* Level;
};

#pragma endregion

#pragma region Clouds
class CloudsEffect : public BaseEffect
{
public:
	CloudsEffect(DX::DeviceResources *resources, const std::string& filename);
	~CloudsEffect() {};

	void SetViewProj(CXMMATRIX &M) { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProj(CXMMATRIX &M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetProj(CXMMATRIX &M) { Proj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetView(CXMMATRIX &M) { View->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX &M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(XMFLOAT3 &V) { EyePosW->SetRawValue(&V, 0, sizeof(XMFLOAT3)); }
	void SetCentrePos(XMFLOAT3 &V) { CentrePos->SetRawValue(&V, 0, sizeof(XMFLOAT3)); }
	void SetOffset(XMFLOAT2 &V) { Offset->SetRawValue(&V, 0, sizeof(XMFLOAT2)); }
	void SetSpacing(const float& v) { Spacing->SetRawValue(&v, 0, sizeof(float)); }
	void SetRadius(const float& v) { Radius->SetRawValue(&v, 0, sizeof(float)); }
	void SetLevel(const float& v) { Level->SetRawValue(&v, 0, sizeof(float)); }

	ID3DX11EffectTechnique* Light1Tech;

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* Proj;
	ID3DX11EffectMatrixVariable* View;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* CentrePos;
	ID3DX11EffectVectorVariable* Offset;
	ID3DX11EffectVariable* Spacing;
	ID3DX11EffectVariable* Radius;
	ID3DX11EffectVariable* Level;
};

#pragma region WaterLOD
class WaterLODEffect : public BaseEffect
{
public:
	WaterLODEffect(DX::DeviceResources *resources, const std::string& filename);
	~WaterLODEffect() {};

	void SetViewProj(CXMMATRIX &M) { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProj(CXMMATRIX &M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetProj(CXMMATRIX &M) { Proj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetView(CXMMATRIX &M) { View->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX &M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(XMFLOAT3 &V) { EyePosW->SetRawValue(&V, 0, sizeof(XMFLOAT3)); }
	void SetCentrePos(XMFLOAT3 &V) { CentrePos->SetRawValue(&V, 0, sizeof(XMFLOAT3)); }
	void SetOffset(XMFLOAT2 &V) { Offset->SetRawValue(&V, 0, sizeof(XMFLOAT2)); }
	void SetSpacing(const float& v) { Spacing->SetRawValue(&v, 0, sizeof(float)); }
	void SetRadius(const float& v) { Radius->SetRawValue(&v, 0, sizeof(float)); }
	void SetLevel(const float& v) { Level->SetRawValue(&v, 0, sizeof(float)); }

	ID3DX11EffectTechnique* Light1Tech;

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* Proj;
	ID3DX11EffectMatrixVariable* View;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* CentrePos;
	ID3DX11EffectVectorVariable* Offset;
	ID3DX11EffectVariable* Spacing;
	ID3DX11EffectVariable* Radius;
	ID3DX11EffectVariable* Level;
};

#pragma endregion

#pragma region PosNormalTexTan
class PosNormalTexTanEffect : public WorldEffect
{
public:
	PosNormalTexTanEffect(DX::DeviceResources *resources, const std::string& filename);
	~PosNormalTexTanEffect() {};

	void SetWorld(CXMMATRIX M)									{ World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldN(CXMMATRIX M)									{ WorldN->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetViewProj(CXMMATRIX M)								{ ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M)						{ WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v)							{ EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetDirLights(const DirectionalLight* lights)			{ DirLights->SetRawValue(lights, 0, sizeof(DirectionalLight)); }
	void SetMaterial(const Material& mat)						{ Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetDiffuseMap(ID3D11ShaderResourceView* hMap)			{ DiffuseMap->SetResource(hMap); }
	void SetSpecularMap(ID3D11ShaderResourceView* hMap)			{ SpecularMap->SetResource(hMap); }
	void SetNormalMap(ID3D11ShaderResourceView* hMap)			{ NormalMap->SetResource(hMap); }
	void SetIsDiffuseMap(const bool & isDiffuseM)				{ isDiffuseMap->SetRawValue(&isDiffuseM, 0, sizeof(bool)); }
	void SetIsSpecularMap(const bool & isSpecularM)				{ isNormalMap->SetRawValue(&isSpecularM, 0, sizeof(bool)); }
	void SetIsNormalMap(const bool & isNormalM)					{ isNormalMap->SetRawValue(&isNormalM, 0, sizeof(bool)); }

	ID3DX11EffectTechnique* Light1Tech;

	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldN;
	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* Mat;

	ID3DX11EffectVariable* isDiffuseMap;
	ID3DX11EffectVariable* isSpecularMap;
	ID3DX11EffectVariable* isNormalMap;

	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* SpecularMap;
	ID3DX11EffectShaderResourceVariable* NormalMap;
};

#pragma endregion

#pragma region BodyEffect
class BodyEffect : public WorldEffect
{
public:
	BodyEffect(DX::DeviceResources *resources, const std::string& filename);
	~BodyEffect() {};

	void SetWorld(CXMMATRIX M)									{ World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldN(CXMMATRIX M)									{ WorldN->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetViewProj(CXMMATRIX M)								{ ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M)						{ WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v)							{ EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetMaterial(const Material& mat)						{ Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetTS(const float & tS)								{ TS->SetRawValue(&tS, 0, sizeof(float)); }

	ID3DX11EffectTechnique* Light1Tech;

	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldN;
	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVariable* Mat;
	ID3DX11EffectVariable* TS;
};

#pragma endregion

#pragma region BillboardEffect
class BillboardEffect : public WorldEffect
{
public:
	BillboardEffect(DX::DeviceResources *resources, const std::string& filename);
	~BillboardEffect() {};

	void SetWorld(CXMMATRIX M)									{ World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetViewProj(CXMMATRIX M)								{ ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v)							{ EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetRenderStart(const float& v)							{ RenderStart->SetRawValue(&v, 0, sizeof(float)); }
	void SetRenderEnd(const float& v)							{ RenderEnd->SetRawValue(&v, 0, sizeof(float)); }void SetCoord(const XMINT2& v)								{ Coord->SetRawValue(&v, 0, sizeof(XMINT2)); }
	void SetSpacing(const float& v)								{ Spacing->SetRawValue(&v, 0, sizeof(float)); }
	void SetHeightMap(ID3D11ShaderResourceView* tex)			{ HeightMap->SetResource(tex); }
	void SetNormalMap(ID3D11ShaderResourceView* tex)			{ NormalMap->SetResource(tex); }
	void SetTreesMap(ID3D11ShaderResourceView* tex)				{ TreesMap->SetResource(tex); }
	void SetHeightTile_1(ID3D11ShaderResourceView* tex)			{ HeightTile_1->SetResource(tex); }
	void SetHeightTile_2(ID3D11ShaderResourceView* tex)			{ HeightTile_2->SetResource(tex); }
	void SetNormalTile_1(ID3D11ShaderResourceView* tex)			{ NormalTile_1->SetResource(tex); }
	void SetNormalTile_2(ID3D11ShaderResourceView* tex)			{ NormalTile_2->SetResource(tex); }

	ID3DX11EffectTechnique* Light1Tech;

	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVariable* RenderStart;
	ID3DX11EffectVariable* RenderEnd;
	ID3DX11EffectVectorVariable* Coord;
	ID3DX11EffectVariable* Spacing;

	ID3DX11EffectShaderResourceVariable* HeightMap;
	ID3DX11EffectShaderResourceVariable* NormalMap;
	ID3DX11EffectShaderResourceVariable* TreesMap;
	ID3DX11EffectShaderResourceVariable* HeightTile_1;
	ID3DX11EffectShaderResourceVariable* HeightTile_2;
	ID3DX11EffectShaderResourceVariable* NormalTile_1;
	ID3DX11EffectShaderResourceVariable* NormalTile_2;
};

#pragma endregion

#pragma region Effects
class Effects
{
public:
	static void InitAll(DX::DeviceResources *resources);

	static void DestroyAll();

	static SkyBoxEffect* SkyBoxFX;
	static TerrainLODEffect* TerrainLODFX;
	static AtmosphereEffect* AtmosphereFX;
	static CloudsEffect* CloudsFX;
	static WaterLODEffect* WaterLODFX;
	static PosNormalTexTanEffect* PosNormalTexTanFX;
	static BodyEffect* BodyFX;
	static BillboardEffect* BillboardFX;
};
#pragma endregion