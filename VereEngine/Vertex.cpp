#include "pch.h"
#include "Effects.h"
#include "Vertex.h"

#pragma region InputLayoutDesc

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::SkyBox[2] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::TerrainBlock[2] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::TerrainLOD[2] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::Atmosphere[2] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosNormalTexTan[4] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::Body[10] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "POSITION", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "POSITION", 2, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 2, DXGI_FORMAT_R32G32B32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 72, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 84, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 2, DXGI_FORMAT_R32G32B32_FLOAT, 0, 96, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 108, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::Billboard[3] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

#pragma endregion

#pragma region InputLayouts

ID3D11InputLayout* InputLayouts::SkyBox = 0;
ID3D11InputLayout* InputLayouts::TerrainBlock = 0;
ID3D11InputLayout* InputLayouts::TerrainLOD = 0;
ID3D11InputLayout* InputLayouts::TerrainPlanetLOD = 0;
ID3D11InputLayout* InputLayouts::Atmosphere = 0;
ID3D11InputLayout* InputLayouts::PosNormalTexTan = 0;
ID3D11InputLayout* InputLayouts::Body = 0;
ID3D11InputLayout* InputLayouts::Billboard = 0;

bool InputLayouts::InitAll(DX::DeviceResources *resources)
{
	D3DX11_PASS_DESC passDesc;
	
	Effects::AtmosphereFX->Light1Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(resources->GetD3DDevice()->CreateInputLayout(InputLayoutDesc::Atmosphere, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &Atmosphere));

	Effects::SkyBoxFX->Light1Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(resources->GetD3DDevice()->CreateInputLayout(InputLayoutDesc::SkyBox, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &SkyBox));

	Effects::TerrainFX->Light1Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(resources->GetD3DDevice()->CreateInputLayout(InputLayoutDesc::TerrainBlock, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &TerrainBlock));

	Effects::TerrainLODFX->Light1Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(resources->GetD3DDevice()->CreateInputLayout(InputLayoutDesc::TerrainLOD, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &TerrainLOD));

	Effects::PosNormalTexTanFX->Light1Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(resources->GetD3DDevice()->CreateInputLayout(InputLayoutDesc::PosNormalTexTan, 4, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &PosNormalTexTan));

	Effects::BodyFX->Light1Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(resources->GetD3DDevice()->CreateInputLayout(InputLayoutDesc::Body, 10, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &Body))

		Effects::BillboardFX->Light1Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(resources->GetD3DDevice()->CreateInputLayout(InputLayoutDesc::Billboard, 3, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &Billboard));

	Effects::TerrainPlanetLODFX->Light1Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(resources->GetD3DDevice()->CreateInputLayout(InputLayoutDesc::TerrainLOD, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &TerrainPlanetLOD));

	return true;
}

void InputLayouts::DestroyAll()
{
	ReleaseCOM(SkyBox);
	ReleaseCOM(TerrainBlock);
	ReleaseCOM(TerrainLOD);
	ReleaseCOM(TerrainPlanetLOD);
	ReleaseCOM(PosNormalTexTan);
	ReleaseCOM(Body);
	ReleaseCOM(Billboard);
}

#pragma endregion
