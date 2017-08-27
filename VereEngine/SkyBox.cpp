#include "pch.h"
#include "Vertex.h"
#include "Effects.h"
#include "GenerateMesh.h"
#include "DeviceResources.h"
#include "SkyBox.h"

/*SkyBox::SkyBox()
{
	XMMATRIX I = XMMatrixIdentity();

	XMStoreFloat4x4(&m_mesh, I);

	m_sun.SetTurnMatrix(0.0f);
	m_sun.SetInclineMatrix(0.15f);
}

void SkyBox::Draw(DX::DeviceResources *resources, float time)
{

	VereFloat3 sun;
	sun.operator=(m_sun.GetNVectorOfSun(GameDataHandle->GetTime()));
	GameDataHandle->SetSunVector(XMFLOAT3(sun.x, sun.y, sun.z));
	VereFloat3 top = VereFloat3(0.0f, 1.0f, 0.0f);
	float dotSun = sun.Dot(top);
	GameDataHandle->SetLight(VereMath::Clamp(dotSun + 1.05f, 0.0f, 1.05f) * 0.952f);


	VereFloat3 blueColor(0.0f, 0.52f, 0.89f);
	VereFloat3  orangeColor(0.83f, 0.51f, 0.22f);
	VereFloat3  redColor(0.7f, 0.16f, 0.16f);
	VereFloat3  skyColor = blueColor * GameDataHandle->GetLight();


	float S = 20000;

	XMMATRIX mesh = XMLoadFloat4x4(&m_mesh);

	mesh *= XMMatrixScaling(-S, -S, -S);

	mesh *= XMMatrixTranslation(
		0.0f,
		0.0f,
		0.0f
		);

	XMMATRIX view = cam->View();

	resources->GetD3DDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	resources->GetD3DDeviceContext()->IASetInputLayout(InputLayouts::SkyBox);

	UINT stride = sizeof(Vertex::SkyBox);

	UINT offset = 0;

	resources->GetD3DDeviceContext()->IASetVertexBuffers(0, 1, &m_VB, &stride, &offset);

	resources->GetD3DDeviceContext()->IASetIndexBuffer(m_IB, DXGI_FORMAT_R32_UINT, 0);

	ID3DX11EffectTechnique* activeTech = Effects::SkyBoxFX->Light1Tech;

	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	Effects::SkyBoxFX->SetViewProj(cam->ViewProj());
	Effects::SkyBoxFX->SetWorld(mesh);
	Effects::SkyBoxFX->SetSkyColor(XMFLOAT4(skyColor.x, skyColor.y, skyColor.z, 1.0f));
	Effects::SkyBoxFX->SetSunVector(XMFLOAT3(sun.x, sun.y, sun.z));
	Effects::SkyBoxFX->SetLight(GameDataHandle->GetLight());

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		activeTech->GetPassByIndex(p)->Apply(0, resources->GetD3DDeviceContext());
		resources->GetD3DDeviceContext()->DrawIndexed(4356, 0, 0);
	}
}

bool SkyBox::CreateSkyBox(DX::DeviceResources *resources)
{
	std::vector<Vertex::PosNormalTexTan> verticesPNTT;
	std::vector<Vertex::SkyBox> vertices;
	std::vector<UINT> indices;

	GenerateMesh::GenerateBox(1.0f, 12, XMFLOAT3(0.0f, 0.0f, 0.0f), verticesPNTT, indices);

	vertices.resize(verticesPNTT.size());

	for (int i = 0; i < verticesPNTT.size(); ++i)
	{
		vertices.at(i).Pos = verticesPNTT.at(i).Position;
		vertices.at(i).Tex = verticesPNTT.at(i).Tex;
	}

	if (!BuildVerticesBuffer(resources->GetD3DDevice(), &vertices))
		return false;

	if (!BuildIndicesBuffer(resources->GetD3DDevice(), &indices))
		return false;

	return true;
}

bool SkyBox::BuildVerticesBuffer(DX::DeviceResources *resources, std::vector<Vertex::SkyBox> *vertices)
{
	D3D11_BUFFER_DESC vertexBuffer;

	vertexBuffer.Usage = D3D11_USAGE_DEFAULT;
	vertexBuffer.ByteWidth = sizeof(Vertex::SkyBox) * vertices->size();
	vertexBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBuffer.CPUAccessFlags = 0;
	vertexBuffer.MiscFlags = 0;
	vertexBuffer.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices->at(0);

	if (FAILED(resources->GetD3DDevice()->CreateBuffer(&vertexBuffer, &vinitData, &m_VB)))
		return false;

	return true;
}

bool SkyBox::BuildIndicesBuffer(DX::DeviceResources *resources, std::vector<UINT> *indices)
{

	D3D11_BUFFER_DESC indexBuffer;

	indexBuffer.Usage = D3D11_USAGE_DEFAULT;
	indexBuffer.ByteWidth = sizeof(UINT) * indices->size();
	indexBuffer.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBuffer.CPUAccessFlags = 0;
	indexBuffer.MiscFlags = 0;
	indexBuffer.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices->at(0);

	if (FAILED(resources->GetD3DDevice()->CreateBuffer(&indexBuffer, &iinitData, &m_IB)))
		return false;

	return true;
}*/