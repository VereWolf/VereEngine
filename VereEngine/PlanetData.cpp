#include "pch.h"
#include "GameObject.h"
#include "Camera.h"
#include "PlanetData.h"

TerrainPlanetData::TerrainPlanetData()
{
}

void TerrainPlanetData::Init()
{
	SetLocalPosition(btVector3(0.0, 0.0, 0.0));
	SetLocalAngle(btVector3(0.0, 0.0, 0.0));

	btMatrix3x3 M1 = btMatrix3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);
	btMatrix3x3 M2 = btMatrix3x3(-1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);

	for (int i = 0; i < 12; ++i)
	{
		m_blockMatrixs[i].setIdentity();

		switch (i)
		{
		case 1:
			m_blockMatrixs[i].setEulerZYX(0.5 * PI, 0.0, 0.0);
			break;
		case 2:
			m_blockMatrixs[i].setEulerZYX(0.0, 0.0, 0.5 * PI);
			break;
		case 3:
			m_blockMatrixs[i] = m_blockMatrixs[i] * -1.0;
			break;
		case 4:
			m_blockMatrixs[i].setEulerZYX(0.5 * PI, 0.0, 0.0);
			m_blockMatrixs[i] = m_blockMatrixs[i] * -1.0;
			break;
		case 5:
			m_blockMatrixs[i].setEulerZYX(0.0, 0.0, 0.5 * PI);
			m_blockMatrixs[i] = m_blockMatrixs[i] * -1.0;
			break;
		case 7:
			m_blockMatrixs[i].setEulerZYX(0.5 * PI, 0.0, 0.0);
			m_blockMatrixs[i] = m_blockMatrixs[i] * M1;
			break;
		case 8:
			m_blockMatrixs[i].setEulerZYX(0.0, 0.0, 0.5 * PI);
			m_blockMatrixs[i] = m_blockMatrixs[i] * M2;
			break;
		case 9:
			m_blockMatrixs[i] = m_blockMatrixs[i] * -1.0;
			break;
		case 10:
			m_blockMatrixs[i].setEulerZYX(0.5 * PI, 0.0, 0.0);
			m_blockMatrixs[i] = m_blockMatrixs[i] * M1;
			m_blockMatrixs[i] = m_blockMatrixs[i] * -1.0;
			break;
		case 11:
			m_blockMatrixs[i].setEulerZYX(0.0, 0.0, 0.5 * PI);
			m_blockMatrixs[i] = m_blockMatrixs[i] * M2;
			m_blockMatrixs[i] = m_blockMatrixs[i] * -1.0;
			break;
		}
	}
	SetRadius(GetScaling().getRow(0).getX());
}

int TerrainPlanetData::BuildLODBuffers(DX::DeviceResources *resources, UINT &sizeOfVertex, UINT &indicesCount)
{
	int32_t PIC = GetNumPointInRowInCell();
	float W = 1.0f / GetNumPointInRowInCell();
	UINT verticesCount = pow(PIC + 2, 2);

	std::vector <Vertex::TerrainLOD> vertices(verticesCount);

	for (int j = 0; j < (PIC + 2); ++j)
	{
		for (int i = 0; i < (PIC + 2); ++i)
		{
			vertices[j * (PIC + 2) + i].Pos.x = -0.5f + static_cast<float>(i - 1) / (PIC - 1.0f);
			vertices[j * (PIC + 2) + i].Pos.y = 0.5f;
			vertices[j * (PIC + 2) + i].Pos.z = -0.5f + static_cast<float>(j - 1) / (PIC - 1.0f);

			vertices[j * (PIC + 2) + i].TexTess.x = static_cast<float>(i - 1) / (PIC - 1.0f);
			vertices[j * (PIC + 2) + i].TexTess.y = static_cast<float>(j - 1) / (PIC - 1.0f);
		}
	}

	PIC = GetNumPointInRowInCell() + 2;
	indicesCount = 4 * pow(PIC - 1, 2);

	std::vector<UINT> indices(indicesCount);

	UINT n = 0;

	for (UINT j = 0; j < (PIC - 1); ++j)
	{
		for (UINT i = 0; i < (PIC - 1); ++i)
		{
			indices[n] = j * PIC + i;
			indices[n + 1] = j * PIC + i + 1;
			indices[n + 3] = (j + 1) * PIC + i + 1;
			indices[n + 2] = (j + 1) * PIC + i;

			n += 4;
		}
	}

	sizeOfVertex = sizeof(Vertex::TerrainLOD);

	return GameRenderDeviceHandle->CreateMeshBuffer(&vertices[0], sizeOfVertex, verticesCount, &indices);
};

bool TerrainPlanetData::LoadHeightFile(string nameFile)
{
	std::ifstream fin(nameFile, std::ios::binary);

	if (!fin)
	{
		return false;
	}

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);

	if (size <= 0)
	{
		return false;
	}

	std::vector<char> dataBuffer;

	dataBuffer.resize(size);

	fin.read(&dataBuffer[0], size);

	m_heightMap.resize(4096 * 4096);
	CopyMemory(&m_heightMap[0], &dataBuffer[0], size);

	return true;
}

bool TerrainPlanetData::LoadNormalFile(string nameFile)
{
	std::ifstream fin(nameFile, std::ios::binary);

	if (!fin)
	{
		return false;
	}

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);

	if (size <= 0)
	{
		return false;
	}

	std::vector<char> dataBuffer;

	dataBuffer.resize(size);

	fin.read(&dataBuffer[0], size);

	m_normalMap.resize(4096 * 4096);
	CopyMemory(&m_normalMap[0], &dataBuffer[0], size);

	return true;
}

/*int TerrainPlanetData::BuildHeightResource(DX::DeviceResources *resources)
{
	LoadHeightFile("LevelHeight.raw");

	D3D11_SUBRESOURCE_DATA tbsd;

	tbsd.pSysMem = &m_heightMap[0];
	tbsd.SysMemPitch = 4096 * sizeof(float);

	D3D11_TEXTURE2D_DESC tdesc;

	tdesc.Width = 4096;
	tdesc.Height = 4096;
	tdesc.MipLevels = 1;
	tdesc.ArraySize = 1;

	tdesc.SampleDesc.Count = 1;
	tdesc.SampleDesc.Quality = 0;
	tdesc.Usage = D3D11_USAGE_DEFAULT;
	tdesc.Format = DXGI_FORMAT_R32_FLOAT;
	tdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	tdesc.CPUAccessFlags = 0;
	tdesc.MiscFlags = 0;

	ID3D11Texture2D *heightResource;

	if (FAILED(resources->GetD3DDevice()->CreateTexture2D(&tdesc, &tbsd, &heightResource)))
		return(false);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = tdesc.MipLevels;
	srvDesc.Format = tdesc.Format;

	resources->GetD3DDevice()->CreateShaderResourceView(heightResource, &srvDesc, &m_heightSRV);

	return GameRenderDeviceHandle->BuildTexture()
}*/

/*int TerrainPlanetData::BuildNormalResource(DX::DeviceResources *resources)
{
	LoadNormalFile("LevelNormal.raw");

	D3D11_SUBRESOURCE_DATA tbsd;

	ZeroMemory(&tbsd, sizeof(D3D11_SUBRESOURCE_DATA));

	tbsd.pSysMem = &m_normalMap[0];
	tbsd.SysMemPitch = 4096 * sizeof(VBYTE4);

	D3D11_TEXTURE2D_DESC tdesc;

	ZeroMemory(&tdesc, sizeof(D3D11_TEXTURE2D_DESC));

	tdesc.Width = 4096;
	tdesc.Height = 4096;
	tdesc.MipLevels = 1;
	tdesc.ArraySize = 1;

	tdesc.SampleDesc.Count = 1;
	tdesc.SampleDesc.Quality = 0;
	tdesc.Usage = D3D11_USAGE_DEFAULT;
	tdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	tdesc.CPUAccessFlags = 0;
	tdesc.MiscFlags = 0;

	ID3D11Texture2D *normalResource;

	if (FAILED(resources->GetD3DDevice()->CreateTexture2D(&tdesc, &tbsd, &normalResource)))
		return(false);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	srvDesc.ViewDimension =
		D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = tdesc.MipLevels;
	srvDesc.Format = tdesc.Format;

	resources->GetD3DDevice()->CreateShaderResourceView(normalResource, &srvDesc, &m_normalSRV);

	ReleaseCOM(normalResource);

	return(true);
};*/