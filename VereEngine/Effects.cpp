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
	FarZ = mFX->GetVariableByName("gFarZ");
	FarRangeMod = mFX->GetVariableByName("gFarRangeMod");
	FarModifier = mFX->GetVariableByName("gFarModifier");
	FogAColor = mFX->GetVariableByName("gFogAColor")->AsVector();
	FogAStart = mFX->GetVariableByName("gFogAStart");
	FogARange = mFX->GetVariableByName("gFogARange");
	FogWColor = mFX->GetVariableByName("gFogWColor")->AsVector();
	FogWStart = mFX->GetVariableByName("gFogWStart");
	FogWRange = mFX->GetVariableByName("gFogWRange");
	CenterOfPlanet = mFX->GetVariableByName("gCenterOfPlanet")->AsVector();
	DirectOfPlanet = mFX->GetVariableByName("gDirectOfPlanet")->AsVector();
	RadiusOfTerrain = mFX->GetVariableByName("gRadiusOfTerrain");
	RadiusOfWater = mFX->GetVariableByName("gRadiusOfWater");
	RadiusOfClouds = mFX->GetVariableByName("gRadiusOfClouds");
	RadiusOfAtmosphere = mFX->GetVariableByName("gRadiusOfAtmosphere");
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

#pragma region SkyBox
SkyBoxEffect::SkyBoxEffect(DX::DeviceResources *resources, const std::string& filename)
	: BaseEffect(resources, filename)
{
	ViewProj = mFX->GetVariableByName("gViewProj")->AsMatrix();
	World = mFX->GetVariableByName("gWorld")->AsMatrix();
}
#pragma endregion

#pragma region TerrainLOD
TerrainLODEffect::TerrainLODEffect(DX::DeviceResources *resources, const std::string& filename)
	: BaseEffect(resources, filename)
{
	CentrePos = mFX->GetVariableByName("gCentrePos")->AsVector();
	Offset = mFX->GetVariableByName("gOffset")->AsVector();
	Tang = mFX->GetVariableByName("gTang")->AsVector();
	Spacing = mFX->GetVariableByName("gSpacing");
	Level = mFX->GetVariableByName("gLevel");
	Side = mFX->GetVariableByName("gSide");

	InverseSide = mFX->GetVariableByName("gInverseSide")->AsMatrix();
	Side = mFX->GetVariableByName("gSide");

	IsMap = mFX->GetVariableByName("gIsMap");

	HeightMap = mFX->GetVariableByName("gHeightMap")->AsShaderResource();
	NormalMap = mFX->GetVariableByName("gNormalMap")->AsShaderResource();
	EnviromentMap = mFX->GetVariableByName("gEnviromentMap")->AsShaderResource();
	TreesMap = mFX->GetVariableByName("gTreesMap")->AsShaderResource();
}
#pragma endregion

#pragma region Atmosphere
AtmosphereEffect::AtmosphereEffect(DX::DeviceResources *resources, const std::string& filename)
	: BaseEffect(resources, filename)
{
	Offset = mFX->GetVariableByName("gOffset")->AsVector();
	Spacing = mFX->GetVariableByName("gSpacing");
	Level = mFX->GetVariableByName("gLevel");
}
#pragma endregion

#pragma region Clouds
CloudsEffect::CloudsEffect(DX::DeviceResources *resources, const std::string& filename)
	: BaseEffect(resources, filename)
{
	Offset = mFX->GetVariableByName("gOffset")->AsVector();
	Spacing = mFX->GetVariableByName("gSpacing");
	Level = mFX->GetVariableByName("gLevel");
}
#pragma endregion

#pragma region WaterLOD
WaterLODEffect::WaterLODEffect(DX::DeviceResources *resources, const std::string& filename)
	: BaseEffect(resources, filename)
{
	Offset = mFX->GetVariableByName("gOffset")->AsVector();
	Spacing = mFX->GetVariableByName("gSpacing");
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
	WaterTopTargetMap = mFX->GetVariableByName("gWaterTopTargetMap")->AsShaderResource();
	WaterTopDepthMap = mFX->GetVariableByName("gWaterTopDepthMap")->AsShaderResource();
	WaterBottomTargetMap = mFX->GetVariableByName("gWaterBottomTargetMap")->AsShaderResource();
	WaterBottomDepthMap = mFX->GetVariableByName("gWaterBottomDepthMap")->AsShaderResource();
	CloudsTargetMap = mFX->GetVariableByName("gCloudsTargetMap")->AsShaderResource();
	CloudsDepthMap = mFX->GetVariableByName("gCloudsDepthMap")->AsShaderResource();
	AtmosphereTargetMap = mFX->GetVariableByName("gAtmosphereTargetMap")->AsShaderResource();
	AtmosphereDepthMap = mFX->GetVariableByName("gAtmosphereDepthMap")->AsShaderResource();

	Depth = mFX->GetVariableByName("gDepth");
	Size = mFX->GetVariableByName("gSize");
	WaterRatio = mFX->GetVariableByName("gWaterRatio");
	CloudsRatio = mFX->GetVariableByName("gCloudsRatio");
}
#pragma endregion

#pragma region PosNormalTexTan
PosNormalTexTanEffect::PosNormalTexTanEffect(DX::DeviceResources *resources, const std::string& filename)
	: BaseEffect(resources, filename)
{
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
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
	: BaseEffect(resources, filename)
{
	Mat = mFX->GetVariableByName("gMaterial");
	TS = mFX->GetVariableByName("gTS");
}
#pragma endregion

#pragma region BillboardEffect
BillboardEffect::BillboardEffect(DX::DeviceResources *resources, const std::string& filename)
	: BaseEffect(resources, filename)
{
	/*RenderStart = mFX->GetVariableByName("gRenderStart");
	RenderEnd = mFX->GetVariableByName("gRenderEnd");
	Coord = mFX->GetVariableByName("gCoord")->AsVector();
	Spacing = mFX->GetVariableByName("gSpacing");

	HeightMap = mFX->GetVariableByName("gHeightMap")->AsShaderResource();
	NormalMap = mFX->GetVariableByName("gNormalMap")->AsShaderResource();
	TreesMap = mFX->GetVariableByName("gTreesMap")->AsShaderResource();
	HeightTile_1 = mFX->GetVariableByName("gHeightTile_1")->AsShaderResource();
	HeightTile_2 = mFX->GetVariableByName("gHeightTile_2")->AsShaderResource();
	NormalTile_1 = mFX->GetVariableByName("gNormalTile_1")->AsShaderResource();
	NormalTile_2 = mFX->GetVariableByName("gNormalTile_2")->AsShaderResource();*/
}
#pragma endregion

#pragma region GenerateTexturesFromTexture
GenerateTexturesFromTextureEffect::GenerateTexturesFromTextureEffect(DX::DeviceResources *resources, const std::string& filename)
	: Effect(resources, filename)
{
	StartPos = mFX->GetVariableByName("gStartPos")->AsVector();
	StepSize = mFX->GetVariableByName("gStepSize")->AsVector();

	InputMap = mFX->GetVariableByName("gInput")->AsShaderResource();
	OutputMap = mFX->GetVariableByName("gOutput")->AsUnorderedAccessView();

}
#pragma endregion

#pragma region GenerateBlockOfLODEffect
GenerateBlockOfLODEffect::GenerateBlockOfLODEffect(DX::DeviceResources *resources, const std::string& filename)
	: Effect(resources, filename)
{
	Offset = mFX->GetVariableByName("gOffset");
	Scaling = mFX->GetVariableByName("gScaling");

	InputHeightMap = mFX->GetVariableByName("gInputH")->AsShaderResource();
	InputNormalMap = mFX->GetVariableByName("gInputN")->AsShaderResource();
	InputEnviromentMap = mFX->GetVariableByName("gInputE")->AsShaderResource();
	InputTreesMap = mFX->GetVariableByName("gInputT")->AsShaderResource();
	InputTileMap = mFX->GetVariableByName("gTile")->AsShaderResource();
	OutputAngleMap = mFX->GetVariableByName("gOutputA")->AsUnorderedAccessView();
	OutputEnviromentMap = mFX->GetVariableByName("gOutputE")->AsUnorderedAccessView();
	OutputTreesMap = mFX->GetVariableByName("gOutputT")->AsUnorderedAccessView();
}
#pragma endregion

#pragma region GenerateHeightAndNormalMapWithNoise
GenerateHeightAndNormalMapWithNoiseEffect::GenerateHeightAndNormalMapWithNoiseEffect(DX::DeviceResources *resources, const std::string& filename)
	: Effect(resources, filename)
{
	StartPos = mFX->GetVariableByName("gStartPos")->AsVector();
	StepSize = mFX->GetVariableByName("gStepSize")->AsVector();
	SpacingMap = mFX->GetVariableByName("gSpacingMap");
	SpacingWorld = mFX->GetVariableByName("gSpacingWorld");
	RangeNoise = mFX->GetVariableByName("gRangeNoise");
	IsMap1 = mFX->GetVariableByName("gIsMap1");
	IsMap2 = mFX->GetVariableByName("gIsMap2");

	InputHeightMap = mFX->GetVariableByName("gInputHeight")->AsShaderResource();
	InputNormalMap = mFX->GetVariableByName("gInputNormal")->AsShaderResource();
	InputRiverWidth1Map = mFX->GetVariableByName("gInputRiverWidth1")->AsShaderResource();
	InputRiverWidth2Map = mFX->GetVariableByName("gInputRiverWidth2")->AsShaderResource();
	InputRiverLength1Map = mFX->GetVariableByName("gInputRiverLength1")->AsShaderResource();
	InputRiverLength2Map = mFX->GetVariableByName("gInputRiverLength2")->AsShaderResource();
	InputRiverType1Map = mFX->GetVariableByName("gInputRiverType1")->AsShaderResource();
	InputRiverType2Map = mFX->GetVariableByName("gInputRiverType2")->AsShaderResource();
	InputRiverHeightMap = mFX->GetVariableByName("gInputRiverHeight")->AsShaderResource();
	TileMap = mFX->GetVariableByName("gTile")->AsShaderResource();
	OutputHeightMap = mFX->GetVariableByName("gOutputHeight")->AsUnorderedAccessView();
	OutputNormalMap = mFX->GetVariableByName("gOutputNormal")->AsUnorderedAccessView();

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
PosNormalTexTanEffect* Effects::PosNormalTexTanFX = 0;
BodyEffect* Effects::BodyFX = 0;
BillboardEffect* Effects::BillboardFX = 0;

GenerateTexturesFromTextureEffect* Effects::GenerateFloatTexFromFloatTexFX = 0;
GenerateTexturesFromTextureEffect* Effects::GenerateBYTE4TexFromBYTE4TexFX = 0;
GenerateBlockOfLODEffect* Effects::GenerateBlockOfLODFX = 0;
GenerateHeightAndNormalMapWithNoiseEffect* Effects::GenerateHeightAndNormalMapWithNoiseEffectFX = 0;

void Effects::InitAll(DX::DeviceResources *resources)
{
	RenderToScreenFX = new RenderToScreen(resources, "FX/RenderToScreen.fxo");
	SkyBoxFX = new SkyBoxEffect(resources, "FX/SkyBox.fxo");
	TerrainLODFX = new TerrainLODEffect(resources, "FX/TerrainLOD.fxo");
	AtmosphereFX = new AtmosphereEffect(resources, "FX/Atmosphere.fxo");
	CloudsFX = new CloudsEffect(resources, "FX/Clouds.fxo");
	WaterLODFX = new WaterLODEffect(resources, "FX/WaterLOD.fxo");
	QuadScreenWCAFX = new QuadScreenWCA(resources, "FX/QuadScreenWCA.fxo");
	PosNormalTexTanFX = new PosNormalTexTanEffect(resources, "FX/PosNormalTexTan.fxo");
	BodyFX = new BodyEffect(resources, "FX/Body.fxo");
	BillboardFX = new BillboardEffect(resources, "FX/Billboard.fxo");

	GenerateFloatTexFromFloatTexFX = new GenerateTexturesFromTextureEffect(resources, "FX/GenerateFloatTexFromFloatTex.fxo");
	GenerateBYTE4TexFromBYTE4TexFX = new GenerateTexturesFromTextureEffect(resources, "FX/GenerateBYTE4TexFromBYTE4Tex.fxo");
	GenerateBlockOfLODFX = new GenerateBlockOfLODEffect(resources, "FX/GenerateBlockOfLOD.fxo");
	GenerateHeightAndNormalMapWithNoiseEffectFX = new GenerateHeightAndNormalMapWithNoiseEffect(resources, "FX/GenerateHeightAndNormalMapWithNoise.fxo");
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
	delete PosNormalTexTanFX;
	delete BodyFX;
	delete BillboardFX;

	delete GenerateFloatTexFromFloatTexFX;
	delete GenerateBYTE4TexFromBYTE4TexFX;
	delete GenerateBlockOfLODFX;
	delete GenerateHeightAndNormalMapWithNoiseEffectFX;
}
#pragma endregion