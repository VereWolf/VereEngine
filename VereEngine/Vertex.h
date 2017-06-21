#ifndef VERTEX_H
#define VERTEX_H

//#include "d3dUtil.h"

namespace Vertex
{

	struct SkyBox
	{
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
	};

	struct TerrainBlock
	{
		XMFLOAT3 Pos;
		XMFLOAT2 TexTess;
	};

	struct TerrainLOD
	{
		XMFLOAT3 Pos;
		XMFLOAT2 TexTess;
	};

	struct Atmosphere
	{
		XMFLOAT3 Pos;
		XMFLOAT3 TexTess;
	};

	struct PosNormalTexTan
	{
		XMFLOAT3 Position;
		XMFLOAT3 Tangent;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
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
		XMFLOAT2 Size;
		XMFLOAT2 Tex;
	};
}

class InputLayoutDesc
{
public:
	static const D3D11_INPUT_ELEMENT_DESC SkyBox[2];
	static const D3D11_INPUT_ELEMENT_DESC TerrainBlock[2];
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
	static ID3D11InputLayout* TerrainBlock;
	static ID3D11InputLayout* TerrainLOD;
	static ID3D11InputLayout* TerrainPlanetLOD;
	static ID3D11InputLayout* Atmosphere;
	static ID3D11InputLayout* PosNormalTexTan;
	static ID3D11InputLayout* Body;
	static ID3D11InputLayout* Billboard;
};

#endif // VERTEX_H
