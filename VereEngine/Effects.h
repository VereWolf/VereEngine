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

public:
	ID3DX11EffectTechnique* Light1Tech;
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
	void SetFarZ(const float& v) { FarZ->SetRawValue(&v, 0, sizeof(float)); }
	void SetFarRangeMod(const float& v) { FarRangeMod->SetRawValue(&v, 0, sizeof(float)); }
	void SetFarModifier(const float& v) { FarModifier->SetRawValue(&v, 0, sizeof(float)); }
	void SetFogAColor(const XMFLOAT3& v) { FogAColor->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetFogAStart(const float& v) { FogAStart->SetRawValue(&v, 0, sizeof(float)); }
	void SetFogARange(const float& v) { FogARange->SetRawValue(&v, 0, sizeof(float)); }
	void SetFogWColor(const XMFLOAT3& v) { FogWColor->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetFogWStart(const float& v) { FogWStart->SetRawValue(&v, 0, sizeof(float)); }
	void SetFogWRange(const float& v) { FogWRange->SetRawValue(&v, 0, sizeof(float)); }
	void SetCenterOfPlanet(const XMFLOAT3& v) { CenterOfPlanet->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetDirectOfPlanet(const XMFLOAT3& v) { DirectOfPlanet->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetSkyColor(const XMFLOAT4& v) { SkyColor->SetRawValue(&v, 0, sizeof(XMFLOAT4)); }
	void SetRadiusOfTerrain(const float& v) { RadiusOfTerrain->SetRawValue(&v, 0, sizeof(float)); }
	void SetRadiusOfWater(const float& v) { RadiusOfWater->SetRawValue(&v, 0, sizeof(float)); }
	void SetRadiusOfClouds(const float& v) { RadiusOfClouds->SetRawValue(&v, 0, sizeof(float)); }
	void SetRadiusOfAtmosphere(const float& v) { RadiusOfAtmosphere->SetRawValue(&v, 0, sizeof(float)); }
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

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* Proj;
	ID3DX11EffectMatrixVariable* View;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldN;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVariable* FarZ;
	ID3DX11EffectVariable* FarRangeMod;
	ID3DX11EffectVariable* FarModifier;
	ID3DX11EffectVectorVariable* FogAColor;
	ID3DX11EffectVariable* FogAStart;
	ID3DX11EffectVariable* FogARange;
	ID3DX11EffectVectorVariable* FogWColor;
	ID3DX11EffectVariable* FogWStart;
	ID3DX11EffectVariable* FogWRange;
	ID3DX11EffectVectorVariable* CenterOfPlanet;
	ID3DX11EffectVectorVariable* DirectOfPlanet;
	ID3DX11EffectVariable* RadiusOfTerrain;
	ID3DX11EffectVariable* RadiusOfWater;
	ID3DX11EffectVariable* RadiusOfClouds;
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

#pragma region RenderToScreen
class RenderToScreen : public BaseEffect
{
public:
	RenderToScreen(DX::DeviceResources *resources, const std::string& filename);
	~RenderToScreen() {};

	void SetTargetMap(ID3D11ShaderResourceView* hMap) { TargetMap->SetResource(hMap); }
	void SetDepthMap(ID3D11ShaderResourceView* hMap) { DepthMap->SetResource(hMap); }

	ID3DX11EffectShaderResourceVariable* TargetMap;
	ID3DX11EffectShaderResourceVariable* DepthMap;
};

#pragma endregion

#pragma region SkyBox
class SkyBoxEffect : public BaseEffect
{
public:
	SkyBoxEffect(DX::DeviceResources *resources, const std::string& filename);
	~SkyBoxEffect() {};

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* World;
};

#pragma endregion

#pragma region TerrainLOD
class TerrainLODEffect : public BaseEffect
{
public:
	TerrainLODEffect(DX::DeviceResources *resources, const std::string& filename);
	~TerrainLODEffect() {};

	void SetOffset(XMFLOAT2 &V) { Offset->SetRawValue(&V, 0, sizeof(XMFLOAT2)); }
	void SetTang(XMFLOAT3 &V) { Tang->SetRawValue(&V, 0, sizeof(XMFLOAT3)); }
	void SetSpacing(const float& v) { Spacing->SetRawValue(&v, 0, sizeof(float)); }
	void SetLevel(const float& v) { Level->SetRawValue(&v, 0, sizeof(float)); }

	void SetInverseSide(CXMMATRIX &M) { InverseSide->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetSide(const float& v) { Side->SetRawValue(&v, 0, sizeof(float)); }

	void SetIsMap(const bool& b) { IsMap->SetRawValue(&b, 0, sizeof(bool)); }

	void SetHeightMap(ID3D11ShaderResourceView* tex) { HeightMap->SetResource(tex); }
	void SetNormalMap(ID3D11ShaderResourceView* tex) { NormalMap->SetResource(tex); }
	void SetEnviromentMap(ID3D11ShaderResourceView* tex) { EnviromentMap->SetResource(tex); }
	void SetTreesMap(ID3D11ShaderResourceView* tex) { TreesMap->SetResource(tex); }

	ID3DX11EffectVectorVariable* CentrePos;
	ID3DX11EffectVectorVariable* Offset;
	ID3DX11EffectVectorVariable* Tang;
	ID3DX11EffectVariable* Spacing;
	ID3DX11EffectVariable* Level;

	ID3DX11EffectMatrixVariable* InverseSide;
	ID3DX11EffectVariable* Side;

	ID3DX11EffectVariable* IsMap;

	ID3DX11EffectShaderResourceVariable* HeightMap;
	ID3DX11EffectShaderResourceVariable* NormalMap;
	ID3DX11EffectShaderResourceVariable* EnviromentMap;
	ID3DX11EffectShaderResourceVariable* TreesMap;
};

#pragma endregion

#pragma region Atmosphere
class AtmosphereEffect : public BaseEffect
{
public:
	AtmosphereEffect(DX::DeviceResources *resources, const std::string& filename);
	~AtmosphereEffect() {};

	void SetOffset(XMFLOAT2 &V) { Offset->SetRawValue(&V, 0, sizeof(XMFLOAT2)); }
	void SetSpacing(const float& v) { Spacing->SetRawValue(&v, 0, sizeof(float)); }
	void SetLevel(const float& v) { Level->SetRawValue(&v, 0, sizeof(float)); }

	ID3DX11EffectVectorVariable* CentrePos;
	ID3DX11EffectVectorVariable* Offset;
	ID3DX11EffectVariable* Spacing;
	ID3DX11EffectVariable* Level;
};

#pragma endregion

#pragma region Clouds
class CloudsEffect : public BaseEffect
{
public:
	CloudsEffect(DX::DeviceResources *resources, const std::string& filename);
	~CloudsEffect() {};

	void SetOffset(XMFLOAT2 &V) { Offset->SetRawValue(&V, 0, sizeof(XMFLOAT2)); }
	void SetSpacing(const float& v) { Spacing->SetRawValue(&v, 0, sizeof(float)); }
	void SetLevel(const float& v) { Level->SetRawValue(&v, 0, sizeof(float)); }

	ID3DX11EffectVectorVariable* Offset;
	ID3DX11EffectVariable* Spacing;
	ID3DX11EffectVariable* Level;
};

#pragma region WaterLOD
class WaterLODEffect : public BaseEffect
{
public:
	WaterLODEffect(DX::DeviceResources *resources, const std::string& filename);
	~WaterLODEffect() {};

	void SetOffset(XMFLOAT2 &V) { Offset->SetRawValue(&V, 0, sizeof(XMFLOAT2)); }
	void SetSpacing(const float& v) { Spacing->SetRawValue(&v, 0, sizeof(float)); }
	void SetLevel(const float& v) { Level->SetRawValue(&v, 0, sizeof(float)); }

	ID3DX11EffectVectorVariable* Offset;
	ID3DX11EffectVariable* Spacing;
	ID3DX11EffectVariable* Level;
};

#pragma endregion

#pragma region QuadScreenWCA
class QuadScreenWCA : public BaseEffect
{
public:
	QuadScreenWCA(DX::DeviceResources *resources, const std::string& filename);
	~QuadScreenWCA() {};

	void SetMainDepthMap(ID3D11ShaderResourceView* hMap) { MainDepthMap->SetResource(hMap); }
	void SetMainTargetMap(ID3D11ShaderResourceView* hMap) { MainTargetMap->SetResource(hMap); }
	void SetWaterTopDepthMap(ID3D11ShaderResourceView* hMap) { WaterTopDepthMap->SetResource(hMap); }
	void SetWaterTopTargetMap(ID3D11ShaderResourceView* hMap) { WaterTopTargetMap->SetResource(hMap); }
	void SetWaterBottomDepthMap(ID3D11ShaderResourceView* hMap) { WaterBottomDepthMap->SetResource(hMap); }
	void SetWaterBottomTargetMap(ID3D11ShaderResourceView* hMap) { WaterBottomTargetMap->SetResource(hMap); }
	void SetCloudsDepthMap(ID3D11ShaderResourceView* hMap) { CloudsDepthMap->SetResource(hMap); }
	void SetCloudsTargetMap(ID3D11ShaderResourceView* hMap) { CloudsTargetMap->SetResource(hMap); }
	void SetAtmosphereDepthMap(ID3D11ShaderResourceView* hMap) { AtmosphereDepthMap->SetResource(hMap); }
	void SetAtmosphereTargetMap(ID3D11ShaderResourceView* hMap) { AtmosphereTargetMap->SetResource(hMap); }
	void SetDepth(const float& V) { Depth->SetRawValue(&V, 0, sizeof(float)); }
	void SetSize(const float& V) { Size->SetRawValue(&V, 0, sizeof(float)); }
	void SetWaterRatio(const float& V) { WaterRatio->SetRawValue(&V, 0, sizeof(float)); }
	void SetCloudsRatio(const float& V) { CloudsRatio->SetRawValue(&V, 0, sizeof(float)); }

	ID3DX11EffectShaderResourceVariable* MainDepthMap;
	ID3DX11EffectShaderResourceVariable* MainTargetMap;

	ID3DX11EffectShaderResourceVariable* WaterTopDepthMap;
	ID3DX11EffectShaderResourceVariable* WaterTopTargetMap;
	ID3DX11EffectShaderResourceVariable* WaterBottomDepthMap;
	ID3DX11EffectShaderResourceVariable* WaterBottomTargetMap;
	ID3DX11EffectShaderResourceVariable* CloudsDepthMap;
	ID3DX11EffectShaderResourceVariable* CloudsTargetMap;
	ID3DX11EffectShaderResourceVariable* AtmosphereDepthMap;
	ID3DX11EffectShaderResourceVariable* AtmosphereTargetMap;

	ID3DX11EffectVariable* Depth;
	ID3DX11EffectVariable* Size;
	ID3DX11EffectVariable* WaterRatio;
	ID3DX11EffectVariable* CloudsRatio;
};

#pragma endregion

#pragma region PosNormalTexTan
class PosNormalTexTanEffect : public BaseEffect
{
public:
	PosNormalTexTanEffect(DX::DeviceResources *resources, const std::string& filename);
	~PosNormalTexTanEffect() {};

	void SetWorldInvTranspose(CXMMATRIX M)						{ WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetDirLights(const DirectionalLight* lights)			{ DirLights->SetRawValue(lights, 0, sizeof(DirectionalLight)); }
	void SetMaterial(const Material& mat)						{ Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetDiffuseMap(ID3D11ShaderResourceView* hMap)			{ DiffuseMap->SetResource(hMap); }
	void SetSpecularMap(ID3D11ShaderResourceView* hMap)			{ SpecularMap->SetResource(hMap); }
	void SetNormalMap(ID3D11ShaderResourceView* hMap)			{ NormalMap->SetResource(hMap); }
	void SetIsDiffuseMap(const bool & isDiffuseM)				{ isDiffuseMap->SetRawValue(&isDiffuseM, 0, sizeof(bool)); }
	void SetIsSpecularMap(const bool & isSpecularM)				{ isNormalMap->SetRawValue(&isSpecularM, 0, sizeof(bool)); }
	void SetIsNormalMap(const bool & isNormalM)					{ isNormalMap->SetRawValue(&isNormalM, 0, sizeof(bool)); }

	ID3DX11EffectMatrixVariable* WorldInvTranspose;
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
class BodyEffect : public BaseEffect
{
public:
	BodyEffect(DX::DeviceResources *resources, const std::string& filename);
	~BodyEffect() {};

	void SetWorldInvTranspose(CXMMATRIX M)						{ WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetMaterial(const Material& mat)						{ Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetTS(const float & tS)								{ TS->SetRawValue(&tS, 0, sizeof(float)); }

	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVariable* Mat;
	ID3DX11EffectVariable* TS;
};

#pragma endregion

#pragma region BillboardEffect
class BillboardEffect : public BaseEffect
{
public:
	BillboardEffect(DX::DeviceResources *resources, const std::string& filename);
	~BillboardEffect() {};

	/*void SetRenderStart(const float& v)							{ RenderStart->SetRawValue(&v, 0, sizeof(float)); }
	void SetRenderEnd(const float& v)							{ RenderEnd->SetRawValue(&v, 0, sizeof(float)); }void SetCoord(const XMINT2& v)								{ Coord->SetRawValue(&v, 0, sizeof(XMINT2)); }
	void SetSpacing(const float& v)								{ Spacing->SetRawValue(&v, 0, sizeof(float)); }
	void SetHeightMap(ID3D11ShaderResourceView* tex)			{ HeightMap->SetResource(tex); }
	void SetNormalMap(ID3D11ShaderResourceView* tex)			{ NormalMap->SetResource(tex); }
	void SetTreesMap(ID3D11ShaderResourceView* tex)				{ TreesMap->SetResource(tex); }
	void SetHeightTile_1(ID3D11ShaderResourceView* tex)			{ HeightTile_1->SetResource(tex); }
	void SetHeightTile_2(ID3D11ShaderResourceView* tex)			{ HeightTile_2->SetResource(tex); }
	void SetNormalTile_1(ID3D11ShaderResourceView* tex)			{ NormalTile_1->SetResource(tex); }
	void SetNormalTile_2(ID3D11ShaderResourceView* tex)			{ NormalTile_2->SetResource(tex); }

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
	ID3DX11EffectShaderResourceVariable* NormalTile_2;*/
};

#pragma endregion

#pragma region GenerateTexturesFromTexture
class GenerateTexturesFromTextureEffect : public Effect
{
public:
	GenerateTexturesFromTextureEffect(DX::DeviceResources *resources, const std::string& filename);
	~GenerateTexturesFromTextureEffect() {};

	void SetStartPos(XMFLOAT2 &V) { StartPos->SetRawValue(&V, 0, sizeof(XMFLOAT2)); }
	void SetStepSize(XMFLOAT2 &V) { StepSize->SetRawValue(&V, 0, sizeof(XMFLOAT2)); }

	void SetInputMap(ID3D11ShaderResourceView* tex) { InputMap->SetResource(tex); }
	void SetOutputMap(ID3D11UnorderedAccessView* tex) { OutputMap->SetUnorderedAccessView(tex); }

	ID3DX11EffectVectorVariable* StartPos;
	ID3DX11EffectVectorVariable* StepSize;

	ID3DX11EffectShaderResourceVariable* InputMap;
	ID3DX11EffectUnorderedAccessViewVariable* OutputMap;
};

#pragma endregion

#pragma region GenerateBlockOfLOD
class GenerateBlockOfLODEffect : public Effect
{
public:
	GenerateBlockOfLODEffect(DX::DeviceResources *resources, const std::string& filename);
	~GenerateBlockOfLODEffect() {};

	void SetOffset(float &V) { Offset->SetRawValue(&V, 0, sizeof(float)); }
	void SetScaling(float &V) { Scaling->SetRawValue(&V, 0, sizeof(float)); }

	void SetInputHeightMap(ID3D11ShaderResourceView* tex) { InputHeightMap->SetResource(tex); }
	void SetInputNormalMap(ID3D11ShaderResourceView* tex) { InputNormalMap->SetResource(tex); }
	void SetInputEnviromentMap(ID3D11ShaderResourceView* tex) { InputEnviromentMap->SetResource(tex); }
	void SetInputTreesMap(ID3D11ShaderResourceView* tex) { InputTreesMap->SetResource(tex); }
	void SetInputTileMap(ID3D11ShaderResourceView* tex) { InputTileMap->SetResource(tex); }
	void SetOutputAngleMap(ID3D11UnorderedAccessView* tex) { OutputAngleMap->SetUnorderedAccessView(tex); }
	void SetOutputEnviromentMap(ID3D11UnorderedAccessView* tex) { OutputEnviromentMap->SetUnorderedAccessView(tex); }
	void SetOutputTreesMap(ID3D11UnorderedAccessView* tex) { OutputTreesMap->SetUnorderedAccessView(tex); }

	ID3DX11EffectVariable* Offset;
	ID3DX11EffectVariable* Scaling;

	ID3DX11EffectShaderResourceVariable* InputHeightMap;
	ID3DX11EffectShaderResourceVariable* InputNormalMap;
	ID3DX11EffectShaderResourceVariable* InputEnviromentMap;
	ID3DX11EffectShaderResourceVariable* InputTreesMap;
	ID3DX11EffectShaderResourceVariable* InputTileMap;
	ID3DX11EffectUnorderedAccessViewVariable* OutputAngleMap;
	ID3DX11EffectUnorderedAccessViewVariable* OutputEnviromentMap;
	ID3DX11EffectUnorderedAccessViewVariable* OutputTreesMap;
};

#pragma endregion

#pragma region GenerateHeightAndNormalMapWithNoise
class GenerateHeightAndNormalMapWithNoiseEffect : public Effect
{
public:
	GenerateHeightAndNormalMapWithNoiseEffect(DX::DeviceResources *resources, const std::string& filename);
	~GenerateHeightAndNormalMapWithNoiseEffect() {};

	void SetStartPos(XMFLOAT2 &V) { StartPos->SetRawValue(&V, 0, sizeof(XMFLOAT2)); }
	void SetStepSize(XMFLOAT2 &V) { StepSize->SetRawValue(&V, 0, sizeof(XMFLOAT2)); }
	void SetSpacingMap(float &V) { SpacingMap->SetRawValue(&V, 0, sizeof(float)); }
	void SetSpacingWorld(float &V) { SpacingWorld->SetRawValue(&V, 0, sizeof(float)); }
	void SetRangeNoise(float &V) { RangeNoise->SetRawValue(&V, 0, sizeof(float)); }
	void SetIsMap1(bool &V) { IsMap1->SetRawValue(&V, 0, sizeof(bool)); }
	void SetIsMap2(bool &V) { IsMap2->SetRawValue(&V, 0, sizeof(bool)); }

	void SetInputHeightMap(ID3D11ShaderResourceView* tex) { InputHeightMap->SetResource(tex); }
	void SetInputNormalMap(ID3D11ShaderResourceView* tex) { InputNormalMap->SetResource(tex); }
	void SetInputRiverWidth1Map(ID3D11ShaderResourceView* tex) { InputRiverWidth1Map->SetResource(tex); }
	void SetInputRiverWidth2Map(ID3D11ShaderResourceView* tex) { InputRiverWidth2Map->SetResource(tex); }
	void SetInputRiverLength1Map(ID3D11ShaderResourceView* tex) { InputRiverLength1Map->SetResource(tex); }
	void SetInputRiverLength2Map(ID3D11ShaderResourceView* tex) { InputRiverLength2Map->SetResource(tex); }
	void SetInputRiverType1Map(ID3D11ShaderResourceView* tex) { InputRiverType1Map->SetResource(tex); }
	void SetInputRiverType2Map(ID3D11ShaderResourceView* tex) { InputRiverType2Map->SetResource(tex); }
	void SetInputRiverHeightMap(ID3D11ShaderResourceView* tex) { InputRiverHeightMap->SetResource(tex); }
	void SetTileMap(ID3D11ShaderResourceView* tex) { TileMap->SetResource(tex); }
	void SetOutputHeightMap(ID3D11UnorderedAccessView* tex) { OutputHeightMap->SetUnorderedAccessView(tex); }
	void SetOutputNormalMap(ID3D11UnorderedAccessView* tex) { OutputNormalMap->SetUnorderedAccessView(tex); }

	ID3DX11EffectVectorVariable* StartPos;
	ID3DX11EffectVectorVariable* StepSize;
	ID3DX11EffectVariable* SpacingMap;
	ID3DX11EffectVariable* SpacingWorld;
	ID3DX11EffectVariable* RangeNoise;
	ID3DX11EffectVariable* IsMap1;
	ID3DX11EffectVariable* IsMap2;

	ID3DX11EffectShaderResourceVariable* InputHeightMap;
	ID3DX11EffectShaderResourceVariable* InputNormalMap;
	ID3DX11EffectShaderResourceVariable* InputRiverWidth1Map;
	ID3DX11EffectShaderResourceVariable* InputRiverWidth2Map;
	ID3DX11EffectShaderResourceVariable* InputRiverLength1Map;
	ID3DX11EffectShaderResourceVariable* InputRiverLength2Map;
	ID3DX11EffectShaderResourceVariable* InputRiverType1Map;
	ID3DX11EffectShaderResourceVariable* InputRiverType2Map;
	ID3DX11EffectShaderResourceVariable* InputRiverHeightMap;
	ID3DX11EffectShaderResourceVariable* TileMap;
	ID3DX11EffectUnorderedAccessViewVariable* OutputHeightMap;
	ID3DX11EffectUnorderedAccessViewVariable* OutputNormalMap;
};

#pragma endregion

#pragma region Effects
class Effects
{
public:
	static void InitAll(DX::DeviceResources *resources);

	static void DestroyAll();

	static RenderToScreen* RenderToScreenFX;
	static SkyBoxEffect* SkyBoxFX;
	static TerrainLODEffect* TerrainLODFX;
	static AtmosphereEffect* AtmosphereFX;
	static CloudsEffect* CloudsFX;
	static WaterLODEffect* WaterLODFX;
	static QuadScreenWCA* QuadScreenWCAFX;
	static PosNormalTexTanEffect* PosNormalTexTanFX;
	static BodyEffect* BodyFX;
	static BillboardEffect* BillboardFX;

	static GenerateTexturesFromTextureEffect *GenerateFloatTexFromFloatTexFX;
	static GenerateTexturesFromTextureEffect *GenerateBYTE4TexFromBYTE4TexFX;
	static GenerateBlockOfLODEffect *GenerateBlockOfLODFX;
	static GenerateHeightAndNormalMapWithNoiseEffect *GenerateHeightAndNormalMapWithNoiseEffectFX;
};
#pragma endregion