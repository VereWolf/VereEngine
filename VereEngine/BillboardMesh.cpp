#include "pch.h"
#include "BillboardMesh.h"

/*BillboardMesh::BillboardMesh()
{
	XMMATRIX I = XMMatrixIdentity();

	XMStoreFloat4x4(&mMesh, I);

}

void BillboardMesh::Init(ID3D11Device* device,
	ID3D11DeviceContext* dc,
	Camera *camera,
	ID3D11Buffer *treesLODVB,
	XMDOUBLE2 position,
	XMINT2 coord)
{
	m_device = device;
	m_deviceContext = dc;
	m_camera = camera;
	m_treesLODVB = treesLODVB;
	m_position = position;
	m_coord = coord;
}

void BillboardMesh::Draw(ID3D11Device* device,
	ID3D11DeviceContext* dc,
	Camera *camera)
{
	int NB = GameTerrainDataHandle->GetNumBlocksInRow();
	XMINT2 BC = XMINT2(m_coord.x + NB, m_coord.y + NB);

	XMMATRIX mesh = XMLoadFloat4x4(&mMesh);
	float S = GameTerrainDataHandle->GetScaling() * GameTerrainDataHandle->GetNumPointInRowInTreesCell();
	float S2 = GameTerrainDataHandle->GetScaling() * GameTerrainDataHandle->GetNumPointInRowInCell();

	mesh *= XMMatrixScaling(-S, -1.0f , -S);

	mesh *= XMMatrixTranslation(
		m_camera->GetPosition().x - m_position.x,
		m_camera->GetPosition().y,
		m_camera->GetPosition().z - m_position.y
		);

	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	dc->IASetInputLayout(InputLayouts::Billboard);

	UINT stride = sizeof(Vertex::Billboard);
	UINT offset = 0;

	dc->IASetVertexBuffers(0, 1, &m_treesLODVB, &stride, &offset);

	ID3DX11EffectTechnique* activeTech = Effects::BillboardFX->Light1Tech;

	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	Effects::BillboardFX->SetViewProj(m_camera->ViewProj());
	Effects::BillboardFX->SetWorld(mesh);
	Effects::BillboardFX->SetRenderStart(S2);
	Effects::BillboardFX->SetRenderEnd(S);
	Effects::BillboardFX->SetFogColor(GameTerrainDataHandle->GetFogColor());
	Effects::BillboardFX->SetFogStart(GameTerrainDataHandle->GetFogStart());
	Effects::BillboardFX->SetFogRange(GameTerrainDataHandle->GetFogRange());
	Effects::BillboardFX->SetHeightMap(GameTerrainDataHandle->GetBlock(BC)->GetHeightSRV());
	Effects::BillboardFX->SetNormalMap(GameTerrainDataHandle->GetBlock(BC)->GetNormalSRV());
	Effects::BillboardFX->SetTreesMap(GameTerrainDataHandle->GetBlock(BC)->GetTreesSRV());
	Effects::BillboardFX->SetSpacing(((float)GameTerrainDataHandle->GetNumPointInRowInTreesCell()) / GameTerrainDataHandle->GetNumPointInRowInBlock());
	Effects::BillboardFX->SetCoord(m_coord);
	/*Effects::BillboardFX->SetHeightTile_1(GameTerrainDataHandle->GetTileSRV()->GetHeightTileSRV_1());
	Effects::BillboardFX->SetNormalTile_1(GameTerrainDataHandle->GetTileSRV()->GetNormalTileSRV_1());*/
/*
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		activeTech->GetPassByIndex(p)->Apply(0, dc);
		dc->Draw(4 * 80 * 80, 0);
	}
}*/

void BillboardMesh::PlaceHolder() {};