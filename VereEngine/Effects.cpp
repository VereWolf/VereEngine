#include "pch.h"
#include "Effects.h"

#pragma region Effect
Effect::Effect(DX::DeviceResources *resources, const std::string& filename)
	: mFX(0)
{
	std::ifstream fin(&filename[0], std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	D3DX11CreateEffectFromMemory(&compiledShader[0], size,
		0, resources->GetD3DDevice(), &mFX);

	Light1Tech = mFX->GetTechniqueByName("LightTech");
}

Effect::~Effect()
{
	ReleaseCOM(mFX)
}
#pragma endregion

#pragma region RenderToScreen
RenderToScreen::RenderToScreen(DX::DeviceResources *resources, const std::string& filename)
	: BaseEffect(resources, filename)
{
	View = mFX->GetVariableByName("gView")->AsMatrix();
	TargetMap = mFX->GetVariableByName("gTargetMap")->AsShaderResource();
	DepthMap = mFX->GetVariableByName("gDepthMap")->AsShaderResource();
}
#pragma endregion

#pragma region BaseEffect
BaseEffect::BaseEffect(DX::DeviceResources *resources, const std::string& filename)
	: Effect(resources, filename)
{
	ViewProj = mFX->GetVariableByName("gViewProj")->AsMatrix();
	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	Proj = mFX->GetVariableByName("gProj")->AsMatrix();
	View = mFX->GetVariableByName("gView")->AsMatrix();
	World = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldN = mFX->GetVariableByName("gWorldN")->AsMatrix();
	EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
	FogColor = mFX->GetVariableByName("gFogColor")->AsVector();
	FarZ = mFX->GetVariableByName("gFarZ");
	FarRangeMod = mFX->GetVariableByName("gFarRangeMod");
	FarModifier = mFX->GetVariableByName("gFarModifier");
	FogStart = mFX->GetVariableByName("gFogStart");
	FogRange = mFX->GetVariableByName("gFogRange");
	CenterOfPlanet = mFX->GetVariableByName("gCenterOfPlanet")->AsVector();
	SkyColor = mFX->GetVariableByName("gSkyColor")->AsVector();
	Mat = mFX->GetVariableByName("gMaterial");
}

void BaseEffect::SetAllSize()
{
	m_Technique.resize(m_TechniqueSize);
	m_TechniqueName.resize(m_TechniqueSize);

	m_Variable.resize(m_VariableSize);
	m_VariableName.resize(m_VariableSize);

	m_Vector.resize(m_VectorSize);
	m_VectorName.resize(m_VectorSize);

	m_Matrix.resize(m_MatrixSize);
	m_MatrixName.resize(m_MatrixSize);

	m_ShaderResource.resize(m_ShaderResourceSize);
	m_ShaderResourceName.resize(m_ShaderResourceSize);
}

void BaseEffect::BindTechnique(UINT index, LPCSTR name)
{
	if (m_onlyGetSize)
	{
		++m_TechniqueSize;
	}
	else
	{
		m_Technique.at(index) = mFX->GetTechniqueByName(name);
		m_TechniqueName.at(index) = name;
	}
}

void BaseEffect::BindValue(UINT index, LPCSTR name)
{
	if (m_onlyGetSize)
	{
		++m_VariableSize;
	}
	else
	{
		m_Variable.at(index) = mFX->GetVariableByName(name);
		m_VariableName.at(index) = name;
	}
}

void BaseEffect::BindVector(UINT index, LPCSTR name)
{
	if (m_onlyGetSize)
	{
		++m_VectorSize;
	}
	else
	{
		m_Vector.at(index) = mFX->GetVariableByName(name)->AsVector();
		m_VectorName.at(index) = name;
	}
}

void BaseEffect::BindMatrix(UINT index, LPCSTR name)
{
	if (m_onlyGetSize)
	{
		++m_MatrixSize;
	}
	else
	{
		m_Matrix.at(index) = mFX->GetVariableByName(name)->AsMatrix();
		m_MatrixName.at(index) = name;
	}
}

void BaseEffect::BindShaderResource(UINT index, LPCSTR name)
{
	if (m_onlyGetSize)
	{
		++m_ShaderResourceSize;
	}
	else
	{
		m_ShaderResource.at(index) = mFX->GetVariableByName(name)->AsShaderResource();
		m_ShaderResourceName.at(index) = name;
	}
}
#pragma endregion

#pragma region WorldEffect
WorldEffect::WorldEffect(DX::DeviceResources *resources, const std::string& filename)
	: Effect(resources, filename)
{
	ViewProj = mFX->GetVariableByName("gViewProj")->AsMatrix();
	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	Proj = mFX->GetVariableByName("gProj")->AsMatrix();
	View = mFX->GetVariableByName("gView")->AsMatrix();
	World = mFX->GetVariableByName("gWorld")->AsMatrix();
	World = mFX->GetVariableByName("gWorldN")->AsMatrix();
	EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
	FogColor = mFX->GetVariableByName("gFogColor")->AsVector();
	FogStart = mFX->GetVariableByName("gFogStart");
	FogRange = mFX->GetVariableByName("gFogRange");
	RadiusOfPlanet = mFX->GetVariableByName("gRadiusOfPlanet");
	CenterOfPlanet = mFX->GetVariableByName("gCenterOfPlanet")->AsVector();
	SkyColor = mFX->GetVariableByName("gSkyColor")->AsVector();
	Mat = mFX->GetVariableByName("gMaterial");
	//SunVector = mFX->GetVariableByName("gSunVector")->AsVector();
	//Light = mFX->GetVariableByName("gLight");
}
#pragma endregion

#pragma region SkyBox
SkyBoxEffect::SkyBoxEffect(DX::DeviceResources *resources, const std::string& filename)
	: WorldEffect(resources, filename)
{
	ViewProj = mFX->GetVariableByName("gViewProj")->AsMatrix();
	World = mFX->GetVariableByName("gWorld")->AsMatrix();
}
#pragma endregion

#pragma region TerrainLOD
TerrainLODEffect::TerrainLODEffect(DX::DeviceResources *resources, const std::string& filename)
	: BaseEffect(resources, filename)
{
	ViewProj = mFX->GetVariableByName("gViewProj")->AsMatrix();
	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	Proj = mFX->GetVariableByName("gProj")->AsMatrix();
	View = mFX->GetVariableByName("gView")->AsMatrix();
	World = mFX->GetVariableByName("gWorld")->AsMatrix();
	EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
	CentrePos = mFX->GetVariableByName("gCentrePos")->AsVector();
	Offset = mFX->GetVariableByName("gOffset")->AsVector();
	Tang = mFX->GetVariableByName("gTang")->AsVector();
	Spacing = mFX->GetVariableByName("gSpacing");
	Radius = mFX->GetVariableByName("gRadius");
	Level = mFX->GetVariableByName("gLevel");

	HeightMap = mFX->GetVariableByName("gHeightMap")->AsShaderResource();
	NormalMap = mFX->GetVariableByName("gNormalMap")->AsShaderResource();
}
#pragma endregion

#pragma region Atmosphere
AtmosphereEffect::AtmosphereEffect(DX::DeviceResources *resources, const std::string& filename)
	: BaseEffect(resources, filename)
{
	ViewProj = mFX->GetVariableByName("gViewProj")->AsMatrix();
	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	Proj = mFX->GetVariableByName("gProj")->AsMatrix();
	View = mFX->GetVariableByName("gView")->AsMatrix();
	World = mFX->GetVariableByName("gWorld")->AsMatrix();
	EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
	CentrePos = mFX->GetVariableByName("gCentrePos")->AsVector();
	Offset = mFX->GetVariableByName("gOffset")->AsVector();
	Spacing = mFX->GetVariableByName("gSpacing");
	Radius = mFX->GetVariableByName("gRadius");
	Level = mFX->GetVariableByName("gLevel");
}
#pragma endregion

#pragma region Clouds
CloudsEffect::CloudsEffect(DX::DeviceResources *resources, const std::string& filename)
	: BaseEffect(resources, filename)
{
	ViewProj = mFX->GetVariableByName("gViewProj")->AsMatrix();
	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	Proj = mFX->GetVariableByName("gProj")->AsMatrix();
	View = mFX->GetVariableByName("gView")->AsMatrix();
	World = mFX->GetVariableByName("gWorld")->AsMatrix();
	EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
	CentrePos = mFX->GetVariableByName("gCentrePos")->AsVector();
	Offset = mFX->GetVariableByName("gOffset")->AsVector();
	Spacing = mFX->GetVariableByName("gSpacing");
	Radius = mFX->GetVariableByName("gRadius");
	Level = mFX->GetVariableByName("gLevel");
}
#pragma endregion

#pragma region WaterLOD
WaterLODEffect::WaterLODEffect(DX::DeviceResources *resources, const std::string& filename)
	: BaseEffect(resources, filename)
{
	ViewProj = mFX->GetVariableByName("gViewProj")->AsMatrix();
	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	Proj = mFX->GetVariableByName("gProj")->AsMatrix();
	View = mFX->GetVariableByName("gView")->AsMatrix();
	World = mFX->GetVariableByName("gWorld")->AsMatrix();
	EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
	CentrePos = mFX->GetVariableByName("gCentrePos")->AsVector();
	Offset = mFX->GetVariableByName("gOffset")->AsVector();
	Spacing = mFX->GetVariableByName("gSpacing");
	Radius = mFX->GetVariableByName("gRadius");
	Level = mFX->GetVariableByName("gLevel");
}
#pragma endregion

#pragma region QuadScreenWCA
QuadScreenWCA::QuadScreenWCA(DX::DeviceResources *resources, const std::string& filename)
	: BaseEffect(resources, filename)
{
	View = mFX->GetVariableByName("gView")->AsMatrix();
	MainTargetMap = mFX->GetVariableByName("gMainTargetMap")->AsShaderResource();
	MainDepthMap = mFX->GetVariableByName("gMainDepthMap")->AsShaderResource();
	CoordTargetMap = mFX->GetVariableByName("gCoordTargetMap")->AsShaderResource();
	CoordDepthMap = mFX->GetVariableByName("gCoordDepthMap")->AsShaderResource();
	WaterTargetMap = mFX->GetVariableByName("gWaterTargetMap")->AsShaderResource();
	WaterDepthMap = mFX->GetVariableByName("gWaterDepthMap")->AsShaderResource();
	CloudsTargetMap = mFX->GetVariableByName("gCloudsTargetMap")->AsShaderResource();
	CloudsDepthMap = mFX->GetVariableByName("gCloudsDepthMap")->AsShaderResource();
	AtmosphereTargetMap = mFX->GetVariableByName("gAtmosphereTargetMap")->AsShaderResource();
	AtmosphereDepthMap = mFX->GetVariableByName("gAtmosphereDepthMap")->AsShaderResource();
	SphereDepthPatternMap = mFX->GetVariableByName("gSphereDepthPatternMap")->AsShaderResource();

	Depth = mFX->GetVariableByName("gDepth");
	Size = mFX->GetVariableByName("gSize");
	WaterRatio = mFX->GetVariableByName("gWaterRatio");
	CloudsRatio = mFX->GetVariableByName("gCloudsRatio");
}
#pragma endregion

#pragma region QuadScreenWithCoord
QuadScreenWithCoord::QuadScreenWithCoord(DX::DeviceResources *resources, const std::string& filename)
	: BaseEffect(resources, filename)
{
	World = mFX->GetVariableByName("gWorld")->AsMatrix();
	ViewProj = mFX->GetVariableByName("gViewProj")->AsMatrix();
	Size = mFX->GetVariableByName("gSize");
}
#pragma endregion

#pragma region PosNormalTexTan
PosNormalTexTanEffect::PosNormalTexTanEffect(DX::DeviceResources *resources, const std::string& filename)
	: WorldEffect(resources, filename)
{
	World = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldN = mFX->GetVariableByName("gWorldN")->AsMatrix();
	ViewProj = mFX->GetVariableByName("gViewProj")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
	DirLights = mFX->GetVariableByName("gDirLight");
	Mat = mFX->GetVariableByName("gMaterial");
	isDiffuseMap = mFX->GetVariableByName("gIsDiffuseMap");
	isSpecularMap = mFX->GetVariableByName("gIsSpecularMap");
	isNormalMap = mFX->GetVariableByName("gIsNormalMap");


	DiffuseMap = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	SpecularMap = mFX->GetVariableByName("gSpecularMap")->AsShaderResource();
	NormalMap = mFX->GetVariableByName("gNormalMap")->AsShaderResource();
}
#pragma endregion

#pragma region BodyEffect
BodyEffect::BodyEffect(DX::DeviceResources *resources, const std::string& filename)
	: WorldEffect(resources, filename)
{
	World = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldN = mFX->GetVariableByName("gWorldN")->AsMatrix();
	ViewProj = mFX->GetVariableByName("gViewProj")->AsMatrix();
	EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
	Mat = mFX->GetVariableByName("gMaterial");
	TS = mFX->GetVariableByName("gTS");
}
#pragma endregion

#pragma region BillboardEffect
BillboardEffect::BillboardEffect(DX::DeviceResources *resources, const std::string& filename)
	: WorldEffect(resources, filename)
{
	World = mFX->GetVariableByName("gWorld")->AsMatrix();
	ViewProj = mFX->GetVariableByName("gViewProj")->AsMatrix();
	EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
	RenderStart = mFX->GetVariableByName("gRenderStart");
	RenderEnd = mFX->GetVariableByName("gRenderEnd");
	Coord = mFX->GetVariableByName("gCoord")->AsVector();
	Spacing = mFX->GetVariableByName("gSpacing");

	HeightMap = mFX->GetVariableByName("gHeightMap")->AsShaderResource();
	NormalMap = mFX->GetVariableByName("gNormalMap")->AsShaderResource();
	TreesMap = mFX->GetVariableByName("gTreesMap")->AsShaderResource();
	HeightTile_1 = mFX->GetVariableByName("gHeightTile_1")->AsShaderResource();
	HeightTile_2 = mFX->GetVariableByName("gHeightTile_2")->AsShaderResource();
	NormalTile_1 = mFX->GetVariableByName("gNormalTile_1")->AsShaderResource();
	NormalTile_2 = mFX->GetVariableByName("gNormalTile_2")->AsShaderResource();
}
#pragma endregion

#pragma region Effects

RenderToScreen* Effects::RenderToScreenFX = 0;
SkyBoxEffect* Effects::SkyBoxFX = 0;
TerrainLODEffect* Effects::TerrainLODFX = 0;
AtmosphereEffect* Effects::AtmosphereFX = 0;
CloudsEffect* Effects::CloudsFX = 0;
WaterLODEffect* Effects::WaterLODFX = 0;
QuadScreenWCA* Effects::QuadScreenWCAFX = 0;
QuadScreenWithCoord* Effects::QuadScreenWithCoordFX = 0;
PosNormalTexTanEffect* Effects::PosNormalTexTanFX = 0;
BodyEffect* Effects::BodyFX = 0;
BillboardEffect* Effects::BillboardFX = 0;

void Effects::InitAll(DX::DeviceResources *resources)
{
	RenderToScreenFX = new RenderToScreen(resources, "FX/RenderToScreen.fxo");
	SkyBoxFX = new SkyBoxEffect(resources, "FX/SkyBox.fxo");
	TerrainLODFX = new TerrainLODEffect(resources, "FX/TerrainLOD.fxo");
	AtmosphereFX = new AtmosphereEffect(resources, "FX/Atmosphere.fxo");
	CloudsFX = new CloudsEffect(resources, "FX/Clouds.fxo");
	WaterLODFX = new WaterLODEffect(resources, "FX/WaterLOD.fxo");
	QuadScreenWCAFX = new QuadScreenWCA(resources, "FX/QuadScreenWCA.fxo");
	QuadScreenWithCoordFX = new QuadScreenWithCoord(resources, "FX/QuadScreenWithCoord.fxo");
	PosNormalTexTanFX = new PosNormalTexTanEffect(resources, "FX/PosNormalTexTan.fxo");
	BodyFX = new BodyEffect(resources, "FX/Body.fxo");
	BillboardFX = new BillboardEffect(resources, "FX/Billboard.fxo");
};

void Effects::DestroyAll()
{
	delete RenderToScreenFX;
	delete SkyBoxFX;
	delete TerrainLODFX;
	delete AtmosphereFX;
	delete CloudsFX;
	delete WaterLODFX;
	delete QuadScreenWCAFX;
	delete QuadScreenWithCoordFX;
	delete PosNormalTexTanFX;
	delete BodyFX;
	delete BillboardFX;
}
#pragma endregion