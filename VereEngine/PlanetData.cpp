#include "pch.h"
#include "GameObjectSpace.h"
#include "Camera.h"
#include "PlanetData.h"
#include "IDRegistr.h"

PlanetData::PlanetData()
{
	m_planetElementID = NULL;
}

void PlanetData::Init()
{
	SetLocalPosition(btVector3(0.0, 0.0, 0.0));
	SetLocalAngle(btVector3(0.0, 0.0, 0.0));

	btMatrix3x3 M1 = btMatrix3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);
	btMatrix3x3 M2 = btMatrix3x3(-1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);

	IDRegistr *IDR = new IDRegistr(10);
	m_planetElementID = IDR;
	m_planetElements.resize(pow(2, 10), NULL);

	for (int i = 0; i < 12; ++i)
	{
		m_blockMatrixs[i].setIdentity();

		m_tang = btVector3(0.0, 1.0, 0.0);
		
		switch (i)
		{
		case 1:
			m_blockMatrixs[i].setEulerZYX(0.5 * PI, 0.0, 0.0);
			break;
		case 2:
			m_blockMatrixs[i].setEulerZYX(0.0, 0.0, 0.5 * PI);
			break;
		case 3:
			m_blockMatrixs[i].setValue(
				1.0, 0.0, 1.0,
				0.0, 1.0, 0.0,
				0.0, 0.0, -1.0);
			m_blockMatrixs[i].setEulerZYX(1.0 * PI, 0.0, 0.0);
			break;
		case 4:
			m_blockMatrixs[i].setEulerZYX(-0.5 * PI, 0.0, 0.0);
			break;
		case 5:
			m_blockMatrixs[i].setEulerZYX(0.0, 0.0, -0.5 * PI);
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
			m_blockMatrixs[i].setValue(
				1.0, 0.0, 1.0,
				0.0, 1.0, 0.0,
				0.0, 0.0, -1.0);
			m_blockMatrixs[i].setEulerZYX(1.0 * PI, 0.0, 0.0);
			break;
		case 10:
			m_blockMatrixs[i].setEulerZYX(-0.5 * PI, 0.0, 0.0);
			m_blockMatrixs[i] = m_blockMatrixs[i] * M1;
			break;
		case 11:
			m_blockMatrixs[i].setEulerZYX(0.0, 0.0, -0.5 * PI);
			m_blockMatrixs[i] = m_blockMatrixs[i] * M2;
			break;
		}

		btVector3 V2 = btVector3(0.0, 1.0, 0.0) * GetBlockAnglMatrix(i);

		if (abs(V2.getX()) > abs(V2.getY()) && abs(V2.getX()) > abs(V2.getZ()))
		{
			if (V2.getX() > 0.0)
			{
				m_posfix[i] = "+X";
			}
			else
			{
				m_posfix[i] = "-X";
			}
		}

		else if (abs(V2.getY()) > abs(V2.getZ()) && abs(V2.getY()) > abs(V2.getX()))
		{
			if (V2.getY() > 0.0)
			{
				m_posfix[i] = "+Y";
			}
			else
			{
				m_posfix[i] = "-Y";
			}
		}

		else if (abs(V2.getZ()) > abs(V2.getX()) && abs(V2.getZ()) > abs(V2.getY()))
		{
			if (V2.getZ() > 0.0)
			{
				m_posfix[i] = "+Z";
			}
			else
			{
				m_posfix[i] = "-Z";
			}
		}
	}

	SetRadiusOfTerrain(GetScaling().getRow(0).getX());
	SetRadiusOfWater(GetRadiusOfTerrain());
	SetRadiusOfAtmosphere(GetRadiusOfTerrain() + 35000.0f);
	SetRadiusOfClouds(GetRadiusOfTerrain() + 1500.0f);

	m_Height = GameRenderDeviceHandle->GetMainViewPort()->Height;
	m_Width = GameRenderDeviceHandle->GetMainViewPort()->Width;

	{
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = m_Width;
		texDesc.Height = m_Height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		ID3D11Texture2D* depthMap = 0;
		m_resources->GetD3DDevice()->CreateTexture2D(&texDesc, 0, &depthMap);

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = 0;
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;
		m_resources->GetD3DDevice()->CreateDepthStencilView(depthMap, &dsvDesc, &m_AtmosphereDeepMapDSV);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		m_resources->GetD3DDevice()->CreateShaderResourceView(depthMap, &srvDesc, &m_AtmosphereDeepMapSRV);

		ReleaseCOM(depthMap);
	}

	{
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = m_Width;
		texDesc.Height = m_Height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		ID3D11Texture2D* depthMap = 0;
		m_resources->GetD3DDevice()->CreateTexture2D(&texDesc, 0, &depthMap);

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = 0;
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;
		m_resources->GetD3DDevice()->CreateDepthStencilView(depthMap, &dsvDesc, &m_CloudsDeepMapDSV);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		m_resources->GetD3DDevice()->CreateShaderResourceView(depthMap, &srvDesc, &m_CloudsDeepMapSRV);

		ReleaseCOM(depthMap);
	}

	{
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = m_Width;
		texDesc.Height = m_Height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		ID3D11Texture2D* depthMap = 0;
		m_resources->GetD3DDevice()->CreateTexture2D(&texDesc, 0, &depthMap);

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = 0;
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;
		m_resources->GetD3DDevice()->CreateDepthStencilView(depthMap, &dsvDesc, &m_WaterDeepMapDSV);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		m_resources->GetD3DDevice()->CreateShaderResourceView(depthMap, &srvDesc, &m_WaterDeepMapSRV);

		ReleaseCOM(depthMap);
	}

	{
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = m_Width;
		texDesc.Height = m_Height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		ID3D11Texture2D* depthMap = 0;
		m_resources->GetD3DDevice()->CreateTexture2D(&texDesc, 0, &depthMap);

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = 0;
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;
		m_resources->GetD3DDevice()->CreateDepthStencilView(depthMap, &dsvDesc, &m_PlanetDeepMapDSV);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		m_resources->GetD3DDevice()->CreateShaderResourceView(depthMap, &srvDesc, &m_PlanetDeepMapSRV);

		ReleaseCOM(depthMap);
	}

	{
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = m_Width;
		texDesc.Height = m_Height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		ID3D11Texture2D* depthMap = 0;
		m_resources->GetD3DDevice()->CreateTexture2D(&texDesc, 0, &depthMap);

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = 0;
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;
		m_resources->GetD3DDevice()->CreateDepthStencilView(depthMap, &dsvDesc, &m_CoordDeepMapDSV);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		m_resources->GetD3DDevice()->CreateShaderResourceView(depthMap, &srvDesc, &m_CoordDeepMapSRV);

		ReleaseCOM(depthMap);
	}

	{
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = m_Width;
		texDesc.Height = m_Height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		ID3D11Texture2D* targetMap = 0;
		m_resources->GetD3DDevice()->CreateTexture2D(&texDesc, 0, &targetMap);

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		rtvDesc.Format = texDesc.Format;
		rtvDesc.Texture2D.MipSlice = 0;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		m_resources->GetD3DDevice()->CreateRenderTargetView(targetMap, &rtvDesc, &m_AtmosphereTargetMapRTV);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = texDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		m_resources->GetD3DDevice()->CreateShaderResourceView(targetMap, &srvDesc, &m_AtmosphereTargetMapSRV);

		ReleaseCOM(targetMap);
	}

	{
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = m_Width;
		texDesc.Height = m_Height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		ID3D11Texture2D* targetMap = 0;
		m_resources->GetD3DDevice()->CreateTexture2D(&texDesc, 0, &targetMap);

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		rtvDesc.Format = texDesc.Format;
		rtvDesc.Texture2D.MipSlice = 0;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		m_resources->GetD3DDevice()->CreateRenderTargetView(targetMap, &rtvDesc, &m_CloudsTargetMapRTV);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = texDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		m_resources->GetD3DDevice()->CreateShaderResourceView(targetMap, &srvDesc, &m_CloudsTargetMapSRV);

		ReleaseCOM(targetMap);
	}

	{
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = m_Width;
		texDesc.Height = m_Height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		ID3D11Texture2D* targetMap = 0;
		m_resources->GetD3DDevice()->CreateTexture2D(&texDesc, 0, &targetMap);

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		rtvDesc.Format = texDesc.Format;
		rtvDesc.Texture2D.MipSlice = 0;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		m_resources->GetD3DDevice()->CreateRenderTargetView(targetMap, &rtvDesc, &m_WaterTargetMapRTV);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = texDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		m_resources->GetD3DDevice()->CreateShaderResourceView(targetMap, &srvDesc, &m_WaterTargetMapSRV);

		ReleaseCOM(targetMap);
	}

	{
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = m_Width;
		texDesc.Height = m_Height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		ID3D11Texture2D* targetMap = 0;
		m_resources->GetD3DDevice()->CreateTexture2D(&texDesc, 0, &targetMap);

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		rtvDesc.Format = texDesc.Format;
		rtvDesc.Texture2D.MipSlice = 0;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		m_resources->GetD3DDevice()->CreateRenderTargetView(targetMap, &rtvDesc, &m_PlanetTargetMapRTV);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = texDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		m_resources->GetD3DDevice()->CreateShaderResourceView(targetMap, &srvDesc, &m_PlanetTargetMapSRV);

		ReleaseCOM(targetMap);
	}

	{
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = m_Width;
		texDesc.Height = m_Height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		ID3D11Texture2D* targetMap = 0;
		m_resources->GetD3DDevice()->CreateTexture2D(&texDesc, 0, &targetMap);

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		rtvDesc.Format = texDesc.Format;
		rtvDesc.Texture2D.MipSlice = 0;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		m_resources->GetD3DDevice()->CreateRenderTargetView(targetMap, &rtvDesc, &m_CoordTargetMapRTV);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = texDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		m_resources->GetD3DDevice()->CreateShaderResourceView(targetMap, &srvDesc, &m_CoordTargetMapSRV);

		ReleaseCOM(targetMap);
	}
}

int PlanetData::BuildLODBuffers(DX::DeviceResources *resources, UINT &sizeOfVertex, UINT &indicesCount)
{
	int32_t PIC = GetNumPointInRowInCell();
	float W = 1.0f / GetNumPointInRowInCell();
	UINT verticesCount = pow(PIC + 2, 2);

	std::vector <Vertex::TerrainLOD> vertices(verticesCount);

	for (int j = 0; j < (PIC + 2); ++j)
	{
		for (int i = 0; i < (PIC + 2); ++i)
		{
			if (i == 0 || i == PIC + 1 || j == 0 || j == PIC + 1)
			{
				vertices[j * (PIC + 2) + i].Pos.y = -0.2f;
			}
			else
			{
				vertices[j * (PIC + 2) + i].Pos.y = 0.0f;
			}
			vertices[j * (PIC + 2) + i].Pos.x = -0.5f + static_cast<float>(i - 1) / (PIC - 1.0f);
			vertices[j * (PIC + 2) + i].Pos.z = -0.5f + static_cast<float>(j - 1) / (PIC - 1.0f);

			vertices[j * (PIC + 2) + i].TexTess.x = static_cast<float>(i) / (PIC + 1.0f);
			vertices[j * (PIC + 2) + i].TexTess.y = static_cast<float>(j) / (PIC + 1.0f);
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
}

void PlanetData::GenerateCoord(float height, float width, float level)
{
	int PL = pow(2, level);
	btScalar ofs = 1.0 / PL;
	btScalar HI = ofs / (height - 1);
	btScalar WI = ofs / (width - 1);
	btScalar HE1 = 1.0 / (m_numPointsInRowInCell * PL);
	btScalar WE1 = 1.0 / (m_numPointsInRowInCell * PL);
	int HE2 = height / m_numPointsInRowInCell;
	int WE2 = width / m_numPointsInRowInCell;
	btScalar H, W = 0.0f;
	btVector3 V = btVector3(0.0, 0.0, 0.0);
	btVector3 V2 = btVector3(0.0, 0.0, 0.0);
	std::vector<float> map(2 * (height + 2 * HE2) * (width + 2 * WE2));

	//for (int i = 0; i < 6; ++i)
	{
		int i = 2;
		for (int m = 0; m < PL; ++m)
		{
			for (int n = 0; n < PL; ++n)
			{
				H = -0.5 - HE1 + ofs;
				for (int y = 0; y < ((int)height + 2 * HE2); y += 1)
				{
					W = -0.5 - WE1 + ofs;
					for (int x = 0; x < ((int)width + 2 * WE2); x += 1)
					{
						V = PlanetCordinateMat::GetCoordForCylinder(btVector3(btVector3(W, 0.5, H) * GetBlockAnglMatrix(i + 6)).normalize()); // convert from cylinder map to cube side map
						map.at(2 * (y * (width + 2 * WE2) + x)) = V.getX();
						map.at(2 * (y * (width + 2 * WE2) + x) + 1) = V.getY();
						W += WI;
					}
					H += HI;
				}
				int id = GameStreamingDataHandle->CreateStreamingData(&map.at(0), sizeof(float) * map.size());

				//Platform::String ^str;

				stringstream str;

				/*if (level == 0)
				{
					str = "coord_" + m_posfix[i] + ".txt";
				}
				else
				{
					str = "coord_" + m_posfix[i] + "_" + level + "_" + n + "_" + m + ".txt";
				}*/
				
				if (level == 0)
				{
					str << "coord_" << GetPosFix(i) << ".txt";
				}
				else
				{
					str << "coord_" << GetPosFix(i) << "_" << to_string((int)level) << "_" + to_string(n) << "_" + to_string(m) << ".txt";
				}

				GameStreamingDataHandle->SaveData(str.str(), id);
				GameStreamingDataHandle->DeleteStreamingData(id);
			}
		}
	}
}

int PlanetData::GetNearestSide(bool withOffset)
{
	int O = 0;
	if (withOffset) O = 6;

	btVector3 V = (GetWorldPosition() - GameObjectStackHandle->GetMainCamera()->GetWorldPosition()).normalize();

	btVector3 V6[] = { m_blockMatrixs[O + 0] * btVector3(0.0,1.0,0.0),m_blockMatrixs[O + 1] * btVector3(0.0,1.0,0.0),m_blockMatrixs[O + 2] * btVector3(0.0,1.0,0.0),
		m_blockMatrixs[O + 3] * btVector3(0.0,1.0,0.0) ,m_blockMatrixs[O + 4] * btVector3(0.0,1.0,0.0) ,m_blockMatrixs[O + 5] * btVector3(0.0,1.0,0.0) };

	int S3[] = { 0,0,0 };

	if (V.dot(V6[0]) > V.dot(V6[3]))
	{
		S3[0] = 0;
	}
	else
	{
		S3[0] = 3;
	}

	if (V.dot(V6[1]) > V.dot(V6[4]))
	{
		S3[1] = 1;
	}
	else
	{
		S3[1] = 4;
	}

	if (V.dot(V6[2]) > V.dot(V6[5]))
	{
		S3[2] = 2;
	}
	else
	{
		S3[2] = 5;
	}

	int S = 0;

	if (V.dot(V6[S3[0]]) > V.dot(V6[S3[1]]) && V.dot(V6[S3[0]]) > V.dot(V6[S3[2]]))
	{
		S = S3[0];
	}
	else if (V.dot(V6[S3[1]]) > V.dot(V6[S3[2]]) && V.dot(V6[S3[1]]) > V.dot(V6[S3[0]]))
	{
		S = S3[1];
	}
	else
	{
		S = S3[2];
	}

	if (withOffset)
	{
		return S + 6;
	}

	return S;
}