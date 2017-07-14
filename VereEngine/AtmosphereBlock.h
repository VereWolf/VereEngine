#pragma once

#include "pch.h"
#include "AtmosphereData.h"

/*class AtmosphereBlock
{
public:
	void Init(AtmosphereData * data, int level, XMINT3 coord, XMDOUBLE3 position, double scaling)
	{
		m_data = data;
		m_level = level;
		m_coord = coord;
		m_scaling = scaling;
		m_position = position;

		float S = -scaling;
	}

	void Draw()
	{
		float S = -m_scaling;
		{
			XMMATRIX mesh = XMMatrixTranslationFromVector(XMLoadFloat3(&XMFLOAT3(m_position.x, m_position.y, m_position.z)));
			float distance;
			XMFLOAT3 BP;
			XMFLOAT3 CP = XMFLOAT3(
				m_data->GetPosition().x - GameObjectStackHandle->GetMainCamera()->GetPosition().x,
				m_data->GetPosition().y - GameObjectStackHandle->GetMainCamera()->GetPosition().y,
				m_data->GetPosition().z - GameObjectStackHandle->GetMainCamera()->GetPosition().z);

			XMMATRIX meshInv = XMLoadFloat4x4(&m_data->GetAngleMatrixInv());

			XMVECTOR blockV = XMVector3Transform(XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f)), mesh);
			XMVECTOR camV = XMVector3Transform(XMLoadFloat3(&XMFLOAT3(CP)), meshInv);

			XMVECTOR D = XMVector3Length(blockV - camV);

			XMStoreFloat(&distance, D);

			if (m_blocks.size() == 0 && distance < m_scaling && m_level < m_data->GetMaxLevel())
			{
				m_blocks.resize(8);

				XMDOUBLE3 pos[8];
				pos[0] = m_position;
				pos[0].x += 0.25 * m_scaling;
				pos[0].y += 0.25 * m_scaling;
				pos[0].z += 0.25 * m_scaling;
				pos[1] = m_position;
				pos[1].x += 0.25 * m_scaling;
				pos[1].y += 0.25 * m_scaling;
				pos[1].z -= 0.25 * m_scaling;
				pos[2] = m_position;
				pos[2].x += 0.25 * m_scaling;
				pos[2].y -= 0.25 * m_scaling;
				pos[2].z += 0.25 * m_scaling;
				pos[3] = m_position;
				pos[3].x += 0.25 * m_scaling;
				pos[3].y -= 0.25 * m_scaling;
				pos[3].z -= 0.25 * m_scaling;
				pos[4] = m_position;
				pos[4].x -= 0.25 * m_scaling;
				pos[4].y += 0.25 * m_scaling;
				pos[4].z += 0.25 * m_scaling;
				pos[5] = m_position;
				pos[5].x -= 0.25 * m_scaling;
				pos[5].y += 0.25 * m_scaling;
				pos[5].z -= 0.25 * m_scaling;
				pos[6] = m_position;
				pos[6].x -= 0.25 * m_scaling;
				pos[6].y -= 0.25 * m_scaling;
				pos[6].z += 0.25 * m_scaling;
				pos[7] = m_position;
				pos[7].x -= 0.25 * m_scaling;
				pos[7].y -= 0.25 * m_scaling;
				pos[7].z -= 0.25 * m_scaling;

				m_blocks[0].Init(m_data, m_level + 1, XMINT3(2 * m_coord.x, 2 * m_coord.y, 2 * m_coord.z), pos[0], 0.5 * m_scaling);
				m_blocks[1].Init(m_data, m_level + 1, XMINT3(2 * m_coord.x, 2 * m_coord.y, 2 * m_coord.z + 1), pos[1], 0.5 * m_scaling);
				m_blocks[2].Init(m_data, m_level + 1, XMINT3(2 * m_coord.x, 2 * m_coord.y + 1, 2 * m_coord.z), pos[2], 0.5 * m_scaling);
				m_blocks[3].Init(m_data, m_level + 1, XMINT3(2 * m_coord.x, 2 * m_coord.y + 1, 2 * m_coord.z + 1), pos[3], 0.5 * m_scaling);
				m_blocks[4].Init(m_data, m_level + 1, XMINT3(2 * m_coord.x + 1, 2 * m_coord.y, 2 * m_coord.z), pos[4], 0.5 * m_scaling);
				m_blocks[5].Init(m_data, m_level + 1, XMINT3(2 * m_coord.x + 1, 2 * m_coord.y, 2 * m_coord.z + 1), pos[5], 0.5 * m_scaling);
				m_blocks[6].Init(m_data, m_level + 1, XMINT3(2 * m_coord.x + 1, 2 * m_coord.y + 1, 2 * m_coord.z), pos[6], 0.5 * m_scaling);
				m_blocks[7].Init(m_data, m_level + 1, XMINT3(2 * m_coord.x + 1, 2 * m_coord.y + 1, 2 * m_coord.z + 1), pos[7], 0.5 * m_scaling);
			}
			else if (m_blocks.size() != 0 && distance > 1.1 * m_scaling)
			{
				m_blocks.resize(0);
			}

		}

		if (m_blocks.size() != 0)
		{
			m_blocks[0].Draw();
			m_blocks[1].Draw();
			m_blocks[2].Draw();
			m_blocks[3].Draw();
			m_blocks[4].Draw();
			m_blocks[5].Draw();
			m_blocks[6].Draw();
			m_blocks[7].Draw();
		}

		if (m_blocks.size() == 0)
		{
			XMMATRIX mesh = XMMatrixScaling(S, S, S);
			mesh *= XMMatrixTranslationFromVector(XMLoadFloat3(&XMFLOAT3(m_position.x, m_position.y, m_position.z)));
			mesh *= XMLoadFloat4x4(&m_data->GetAngleMatrix());
			mesh *= XMLoadFloat4x4(&m_data->GetPositionMatrix());

			XMMATRIX viewProj = GameObjectStackHandle->GetMainCamera()->ViewProj();

			m_data->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_data->GetDeviceContext()->IASetInputLayout(InputLayouts::Atmosphere);

			UINT stride = sizeof(Vertex::Atmosphere);


			UINT offset = 0;

			m_data->GetDeviceContext()->IASetVertexBuffers(0, 1, m_data->GetAtmosphereVB(), &stride, &offset);

			m_data->GetDeviceContext()->IASetIndexBuffer(m_data->GetAtmosphereIB(), DXGI_FORMAT_R32_UINT, 0);

			ID3DX11EffectTechnique* activeTech = Effects::AtmosphereFX->Light1Tech;

			D3DX11_TECHNIQUE_DESC techDesc;
			activeTech->GetDesc(&techDesc);

			XMFLOAT3 EyePos = { 0.0f, 0.0f, 0.0f };

			XMFLOAT3 C;
			C.x = GameObjectStackHandle->GetMainCamera()->GetPosition().x - m_data->GetPosition().x;
			C.y = GameObjectStackHandle->GetMainCamera()->GetPosition().y - m_data->GetPosition().y;
			C.z = GameObjectStackHandle->GetMainCamera()->GetPosition().z - m_data->GetPosition().z;

			XMFLOAT3 T = XMFLOAT3(0.0f, 1.0f, 0.0f);

			Effects::AtmosphereFX->SetViewProj(viewProj);
			Effects::AtmosphereFX->SetWorld(mesh);
			Effects::AtmosphereFX->SetCentrePos(C);
			Effects::AtmosphereFX->SetSpacing(m_data->GetScaling());
			Effects::AtmosphereFX->SetFogStart(5000.0);
			Effects::AtmosphereFX->SetFogRange(70000.0f);

			for (UINT p = 0; p < techDesc.Passes; ++p)
			{
				activeTech->GetPassByIndex(p)->Apply(0, m_data->GetDeviceContext());
				m_data->GetDeviceContext()->DrawIndexed(36 * pow(m_data->GetNumPointsInRow() - 1, 3), 0, 0);
			}
		}
	}
private:
	XMDOUBLE3 m_position;
	XMFLOAT3 m_angle;
	int m_level;
	XMINT3 m_coord;
	int m_scaling;

	std::vector<AtmosphereBlock> m_blocks;

	AtmosphereData *m_data;
};*/