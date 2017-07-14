#pragma once

#include "pch.h"

/*class AtmosphereData
{
public:
	AtmosphereData()
	{
	}

	~AtmosphereData()
	{
		ReleaseCOM(m_AtmosphereVB);
		ReleaseCOM(m_AtmosphereIB);
	}
	void Init(DX::DeviceResources* resources)
	{
		m_resources = resources

		XMMATRIX I = XMMatrixIdentity();

		XMStoreFloat4x4(&m_positionMatrix, I);
		XMStoreFloat4x4(&m_angleMatrix, I);

		SetPosition(XMDOUBLE3(0.0, 0.0, 0.0));
		SetAngle(XMFLOAT3(0.0f, 0.0f, 0.0f));
		SetScaling(270000);
		SetRadius(GetScaling() / 2.0f);
		SetNumPointsInRow(8);
		SetMaxLevel(6);
		SetMaxRenderLevel(5);

		BuildVerticesAtmosphereBuffer(resources->GetD3DDevice());
		BuildIndicesAtmosphereBuffer(resources->GetD3DDevice());
	}

	   ID3D11Device *GetDevice() { return m_resources->GetD3DDevice(); }
	   ID3D11DeviceContext *GetDeviceContext() { return m_resources->GetD3DDeviceContext(); }
	   Camera *GetCamera() { return GameObjectStackHandle->GetMainCamera(); }

	bool BuildVerticesAtmosphereBuffer(DX::DeviceResources *resources)
	{
		int32_t PIC = GetNumPointsInRow();
		int32_t V = pow(PIC, 3);

		D3D11_BUFFER_DESC vertexBuffer;

		vertexBuffer.Usage = D3D11_USAGE_DEFAULT;
		vertexBuffer.ByteWidth = sizeof(Vertex::Atmosphere) * V;
		vertexBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBuffer.CPUAccessFlags = 0;
		vertexBuffer.MiscFlags = 0;
		vertexBuffer.StructureByteStride = 0;

		std::vector <Vertex::Atmosphere> vertices(V);

		for (int k = 0; k < (PIC); ++k)
		{
			for (int j = 0; j < (PIC); ++j)
			{
				for (int i = 0; i < (PIC); ++i)
				{
					vertices[k * PIC * PIC + j * PIC + i].Pos.x = -0.5f + static_cast<float>(i) / (PIC - 1.0f);
					vertices[k * PIC * PIC + j * PIC + i].Pos.y = -0.5f + static_cast<float>(j) / (PIC - 1.0f);
					vertices[k * PIC * PIC + j * PIC + i].Pos.z = -0.5f + static_cast<float>(k) / (PIC - 1.0f);

					vertices[k * PIC * PIC + j * PIC + i].TexTess.x = static_cast<float>(i) / (PIC - 1.0f);
					vertices[k * PIC * PIC + j * PIC + i].TexTess.y = static_cast<float>(j) / (PIC - 1.0f);
					vertices[k * PIC * PIC + j * PIC + i].TexTess.z = static_cast<float>(k) / (PIC - 1.0f);
				}
			}
		}

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &vertices.at(0);

		HR(resources->GetD3DDevice()->CreateBuffer(&vertexBuffer, &vinitData, &m_AtmosphereVB));

		return true;
	};

	bool BuildIndicesAtmosphereBuffer(DX::DeviceResources *resources)
	{
		int32_t PIC = GetNumPointsInRow();
		int V = 36 * pow(PIC - 1, 3);

		D3D11_BUFFER_DESC indexBuffer;

		indexBuffer.Usage = D3D11_USAGE_DEFAULT;
		indexBuffer.ByteWidth = sizeof(UINT) * V;
		indexBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		indexBuffer.CPUAccessFlags = 0;
		indexBuffer.MiscFlags = 0;
		indexBuffer.StructureByteStride = 0;

		std::vector<UINT> indices(V);

		UINT n = 0;

		for (UINT k = 0; k < (PIC - 1); ++k)
		{
			for (UINT j = 0; j < (PIC - 1); ++j)
			{
				for (UINT i = 0; i < (PIC - 1); ++i)
				{
					indices[n] = k * PIC * PIC + j * PIC + i;
					indices[n + 1] = k * PIC * PIC + j * PIC + i + 1;
					indices[n + 2] = k * PIC * PIC + (j + 1) * PIC + i + 1;
					indices[n + 3] = k * PIC * PIC + j * PIC + i;
					indices[n + 4] = k * PIC * PIC + (j + 1) * PIC + i + 1;
					indices[n + 5] = k * PIC * PIC + (j + 1) * PIC + i;

					indices[n + 6] = (k + 1) * PIC * PIC + j * PIC + i;
					indices[n + 7] = (k + 1) * PIC * PIC + j * PIC + i + 1;
					indices[n + 8] = (k + 1) * PIC * PIC + (j + 1) * PIC + i + 1;
					indices[n + 9] = (k + 1) * PIC * PIC + j * PIC + i;
					indices[n + 10] = (k + 1) * PIC * PIC + (j + 1) * PIC + i + 1;
					indices[n + 11] = (k + 1) * PIC * PIC + (j + 1) * PIC + i;

					indices[n + 12] = k * PIC * PIC + j * PIC + i;
					indices[n + 13] = k * PIC * PIC + j * PIC + i + 1;
					indices[n + 14] = (k + 1) * PIC * PIC + j * PIC + i + 1;
					indices[n + 15] = k * PIC * PIC + j * PIC + i;
					indices[n + 16] = (k + 1) * PIC * PIC + j * PIC + i + 1;
					indices[n + 17] = (k + 1) * PIC * PIC + j * PIC + i;

					indices[n + 18] = k * PIC * PIC + (j + 1) * PIC + i;
					indices[n + 19] = k * PIC * PIC + (j + 1) * PIC + i + 1;
					indices[n + 20] = (k + 1) * PIC * PIC + (j + 1) * PIC + i + 1;
					indices[n + 21] = k * PIC * PIC + (j + 1) * PIC + i;
					indices[n + 22] = (k + 1) * PIC * PIC + (j + 1) * PIC + i + 1;
					indices[n + 23] = (k + 1) * PIC * PIC + (j + 1) * PIC + i;

					indices[n + 24] = k * PIC * PIC + j * PIC + i;
					indices[n + 25] = (k + 1) * PIC * PIC + j * PIC + i;
					indices[n + 26] = (k + 1) * PIC * PIC + (j + 1) * PIC + i;
					indices[n + 27] = k * PIC * PIC + j * PIC + i;
					indices[n + 28] = (k + 1) * PIC * PIC + (j + 1) * PIC + i;
					indices[n + 29] = k * PIC * PIC + (j + 1) * PIC + i;

					indices[n + 30] = k * PIC * PIC + j * PIC + i + 1;
					indices[n + 31] = (k + 1) * PIC * PIC + j * PIC + i + 1;
					indices[n + 32] = (k + 1) * PIC * PIC + (j + 1) * PIC + i + 1;
					indices[n + 33] = k * PIC * PIC + j * PIC + i + 1;
					indices[n + 34] = (k + 1) * PIC * PIC + (j + 1) * PIC + i + 1;
					indices[n + 35] = k * PIC * PIC + (j + 1) * PIC + i + 1;

					n += 36;
				}
			}
		}

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &indices.at(0);

		HR(resources->GetD3DDevice()->CreateBuffer(&indexBuffer, &iinitData, &m_AtmosphereIB));

		return true;
	};

	ID3D11Buffer *const *GetAtmosphereVB() { return &m_AtmosphereVB; }
	ID3D11Buffer *GetAtmosphereIB() { return m_AtmosphereIB; }

	XMDOUBLE3 GetPosition() { return m_position; }
	XMFLOAT4X4 GetPositionMatrix() { return m_positionMatrix; }
	XMFLOAT4X4 GetPositionMatrixInv() { return m_positionMatrixInv; }
	void SetPosition(XMDOUBLE3 pos)
	{
		m_position = pos;
		XMFLOAT3 P;
		P.x = GameObjectStackHandle->GetMainCamera()->GetPosition().x - m_position.x;
		P.y = GameObjectStackHandle->GetMainCamera()->GetPosition().y - m_position.y;
		P.z = GameObjectStackHandle->GetMainCamera()->GetPosition().z - m_position.z;

		XMMATRIX M = XMMatrixTranslationFromVector(XMLoadFloat3(&P));
		XMVECTOR D = XMMatrixDeterminant(M);
		XMMATRIX MI = XMMatrixInverse(&D, M);

		XMStoreFloat4x4(&m_positionMatrix, M);
		XMStoreFloat4x4(&m_positionMatrixInv, MI);
	}

	XMFLOAT3 GetAngle() { return m_angle; }
	XMFLOAT4X4 GetAngleMatrix() { return m_angleMatrix; }
	XMFLOAT4X4 GetAngleMatrixInv() { return m_angleMatrixInv; }
	void SetAngle(XMFLOAT3 angl)
	{
		m_angle = angl;

		XMMATRIX M = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&angl));
		XMVECTOR D = XMMatrixDeterminant(M);
		XMMATRIX MI = XMMatrixInverse(&D, M);

		XMStoreFloat4x4(&m_angleMatrix, M);
		XMStoreFloat4x4(&m_angleMatrixInv, MI);
	}
	double GetScaling() { return m_scaling; }
	void SetScaling(double n) { m_scaling = n; }

	float GetRadius() { return m_radius; }
	void SetRadius(float n) { m_radius = n; }

	int16_t GetMaxLevel() { return m_maxLevel; }
	void SetMaxLevel(int16_t n) { m_maxLevel = n; }
	int16_t GetMaxRenderLevel() { return m_maxRenderLevel; }
	void SetMaxRenderLevel(int16_t n) { m_maxRenderLevel = n; }
	int GetNumPointsInRow() { return m_numPointsInRow; }
	void SetNumPointsInRow(int p) { m_numPointsInRow = p; }

	private:
	DX::DeviceResources* m_resources;

	XMDOUBLE3 m_position;
	XMFLOAT3 m_angle;
	double m_scaling;
	float m_radius;

	int m_numPointsInRow;
	int16_t m_maxLevel;
	int16_t m_maxRenderLevel;

	ID3D11Buffer *m_AtmosphereVB;
	ID3D11Buffer *m_AtmosphereIB;

	XMFLOAT4X4 m_angleMatrix;
	XMFLOAT4X4 m_angleMatrixInv;
	XMFLOAT4X4 m_positionMatrixInv;
	XMFLOAT4X4 m_positionMatrix;
	XMFLOAT4X4 m_scalingMatrix;
};*/
