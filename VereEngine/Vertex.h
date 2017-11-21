#pragma once

//#include "d3dUtil.h"

namespace Vertex
{

	struct SkyBox
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
	};

	struct TerrainLOD
	{
		XMFLOAT3 Pos;
		XMFLOAT2 TexTess;
	};

	struct PosNormalTexTan
	{
		XMFLOAT3 Position;
		XMFLOAT3 Tangent;
		XMFLOAT3 Normal;
		XMFLOAT2 TexTess;
	};

	struct Body
	{
		XMFLOAT3 Position;
		XMFLOAT3 PositionS;
		XMFLOAT3 PositionT;
		XMFLOAT3 Tangent;
		XMFLOAT3 TangentS;
		XMFLOAT3 TangentT;
		XMFLOAT3 Normal;
		XMFLOAT3 NormalS;
		XMFLOAT3 NormalT;
		XMFLOAT2 Tex;
	};

	struct Billboard
	{
		XMFLOAT3 Position;
		XMFLOAT3 Direct;
		XMFLOAT2 Size;
	};
}

class InputLayoutDesc
{
public:
	static const D3D11_INPUT_ELEMENT_DESC SkyBox[2];
	static const D3D11_INPUT_ELEMENT_DESC TerrainLOD[2];
	static const D3D11_INPUT_ELEMENT_DESC Atmosphere[2];
	static const D3D11_INPUT_ELEMENT_DESC PosNormalTexTan[4];
	static const D3D11_INPUT_ELEMENT_DESC Body[10];
	static const D3D11_INPUT_ELEMENT_DESC Billboard[3];
};

class InputLayouts
{
public:
	static bool InitAll(DX::DeviceResources *resources);
	static void DestroyAll();

	static ID3D11InputLayout* SkyBox;
	static ID3D11InputLayout* TerrainLOD;
	static ID3D11InputLayout* Atmosphere;
	static ID3D11InputLayout* Clouds;
	static ID3D11InputLayout* WaterLOD;
	static ID3D11InputLayout* PosNormalTexTan;
	static ID3D11InputLayout* Body;
	static ID3D11InputLayout* Billboard;
};
