#include "pch.h"
#include "Vertex.h"
#include "Effects.h"
#include "LoadVMS.h"
#include "GenerateMesh.h"
#include "DeviceResources.h"
#include "StaticMesh.h"

StaticMesh::StaticMesh()
{
}

void StaticMesh::Render(btTransform transform, btTransform scaling)
{
	XMMATRIX view = GameObjectStackHandle->GetMainCamera()->View();

	m_resources->GetD3DDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_resources->GetD3DDeviceContext()->IASetInputLayout(InputLayouts::PosNormalTexTan);

	UINT stride = sizeof(Vertex::PosNormalTexTan);

	for (UINT n = 0; n < mVMSmat.size(); ++n)
	{
		UINT offset = 0;

		m_resources->GetD3DDeviceContext()->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);

		m_resources->GetD3DDeviceContext()->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

		ID3DX11EffectTechnique* activeTech = Effects::PosNormalTexTanFX->Light1Tech;

		D3DX11_TECHNIQUE_DESC techDesc;
		activeTech->GetDesc(&techDesc);

		XMFLOAT3 EyePos = { 0.0f, 0.0f, 0.0f };

		XMFLOAT3 dir = XMFLOAT3(transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ());

		XMMATRIX mesh = XMLoadFloat4x4(&VereMath::ConvertToXMFLOAT4X4(transform * GameObjectStackHandle->GetMainCamera()->btOffset() * scaling));
		btTransform transformN = transform;
		transformN.getOrigin().setZero();
		
		XMMATRIX meshN = XMLoadFloat4x4(&VereMath::ConvertToXMFLOAT4X4(transformN.inverse()));

		Effects::PosNormalTexTanFX->SetEyePosW(dir);
		Effects::PosNormalTexTanFX->SetMaterial(mVMSmat.at(n).material);
		Effects::PosNormalTexTanFX->SetViewProj(GameObjectStackHandle->GetMainCamera()->ViewProj());
		Effects::PosNormalTexTanFX->SetWorld(mesh);
		Effects::PosNormalTexTanFX->SetWorldN(meshN);
		Effects::PosNormalTexTanFX->SetDiffuseMap(mDiffuseMapSRV[n]);
		Effects::PosNormalTexTanFX->SetSpecularMap(mSpecularMapSRV[n]);
		Effects::PosNormalTexTanFX->SetNormalMap(mNormalMapSRV[n]);
		Effects::PosNormalTexTanFX->SetIsDiffuseMap(isDiffuseMap.at(n));
		Effects::PosNormalTexTanFX->SetIsSpecularMap(isSpecularMap.at(n));
		Effects::PosNormalTexTanFX->SetIsNormalMap(isNormalMap.at(n));

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			activeTech->GetPassByIndex(p)->Apply(0, m_resources->GetD3DDeviceContext());
			m_resources->GetD3DDeviceContext()->DrawIndexed(3 * mVMSmat.at(n).faceCount, 3 * mVMSmat.at(n).faceStart, 0);
		}
	}
}

bool StaticMesh::CreateSphere(float diameter, int numGridInRow)
{
	mVMSmat.resize(1);
	mVMSmat.at(0).faceCount = numGridInRow * numGridInRow * 12;
	mVMSmat.at(0).faceStart = 0;
	mVMSmat.at(0).d = 0.0f;
	mVMSmat.at(0).material.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mVMSmat.at(0).material.Diffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mVMSmat.at(0).material.Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mVMSmat.at(0).material.Reflect = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);

	GenerateMesh::GenerateSphere(diameter, numGridInRow, XMFLOAT3(0.0f, 0.0f, 0.0f), mVertices, mIndices);

	if (!BuildVerticesBuffer(&mVertices))
		return false;

	if (!BuildIndicesBuffer(&mIndices))
		return false;

	mDiffuseMapSRV.resize(mVMSmat.size());
	mSpecularMapSRV.resize(mVMSmat.size());
	mNormalMapSRV.resize(mVMSmat.size());

	isDiffuseMap.resize(mVMSmat.size());
	isSpecularMap.resize(mVMSmat.size());
	isNormalMap.resize(mVMSmat.size());


	for (UINT i = 0; i < mVMSmat.size(); ++i)
	{
		isDiffuseMap.at(i) = false;
		isSpecularMap.at(i) = false;
		isNormalMap.at(i) = false;
	}

	return true;
}

bool StaticMesh::CreateObject(std::string filePath, std::wstring texturePath, std::string nameFile)
{
	std::vector<Vertex::PosNormalTexTan> itsVertices;
	std::vector<UINT> itsIndices;

	if (!VMSLoader::LoadVMS(nameFile.insert(0, filePath), itsVertices, itsIndices, mVMSmat))
		return false;

	if (!BuildVerticesBuffer(&itsVertices))
		return false;

	if (!BuildIndicesBuffer(&itsIndices))
		return false;

	mDiffuseMapSRV.resize(mVMSmat.size());
	mSpecularMapSRV.resize(mVMSmat.size());
	mNormalMapSRV.resize(mVMSmat.size());

	isDiffuseMap.resize(mVMSmat.size());
	isSpecularMap.resize(mVMSmat.size());
	isNormalMap.resize(mVMSmat.size());


	for (UINT i = 0; i < mVMSmat.size(); ++i)
	{
		mVMSmat.at(i).paths.at(1).insert(0, texturePath);
		mVMSmat.at(i).paths.at(1).replace(mVMSmat.at(i).paths.at(1).end() - 3, mVMSmat.at(i).paths.at(1).end(), L"dds");
		mVMSmat.at(i).paths.at(2) = mVMSmat.at(i).paths.at(1);
		mVMSmat.at(i).paths.at(2).replace(mVMSmat.at(i).paths.at(2).end() - 5, mVMSmat.at(i).paths.at(2).end() - 4, L"S");
		mVMSmat.at(i).paths.at(5) = mVMSmat.at(i).paths.at(1);
		mVMSmat.at(i).paths.at(5).replace(mVMSmat.at(i).paths.at(5).end() - 5, mVMSmat.at(i).paths.at(5).end() - 4, L"N");
		//if (FAILED(D3DX11CreateShaderResourceViewFromFile(m_resources->GetD3DDevice(), mVMSmat.at(i).paths.at(1).c_str(), 0, 0, &mDiffuseMapSRV.at(i), 0)))
		{
			isDiffuseMap.at(i) = false;
		}
		/*else
		{
			isDiffuseMap.at(i) = true;
		}*/

		//if (FAILED(D3DX11CreateShaderResourceViewFromFile(m_resources->GetD3DDevice(), mVMSmat.at(i).paths.at(2).c_str(), 0, 0, &mSpecularMapSRV.at(i), 0)))
		{
			isSpecularMap.at(i) = false;
		}
		/*else
		{
			isSpecularMap.at(i) = true;
		}*/
		
		//if (FAILED(D3DX11CreateShaderResourceViewFromFile(m_resources->GetD3DDevice(), mVMSmat.at(i).paths.at(5).c_str(), 0, 0, &mNormalMapSRV.at(i), 0)))
		{
			isNormalMap.at(i) = false;
		}
		/*else
		{
			isNormalMap.at(i) = true;
		}*/
	}

	return true;
}

bool StaticMesh::BuildVerticesBuffer()
{
	D3D11_BUFFER_DESC vertexBuffer;

	vertexBuffer.Usage = D3D11_USAGE_DEFAULT;
	vertexBuffer.ByteWidth = sizeof(Vertex::PosNormalTexTan) * mVertices.size();
	vertexBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBuffer.CPUAccessFlags = 0;
	vertexBuffer.MiscFlags = 0;
	vertexBuffer.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &mVertices.at(0);

	if (FAILED(m_resources->GetD3DDevice()->CreateBuffer(&vertexBuffer, &vinitData, &mVB)))
		return false;

	return true;
}

bool StaticMesh::BuildVerticesBuffer(std::vector<Vertex::PosNormalTexTan> *vertices)
{
	D3D11_BUFFER_DESC vertexBuffer;

	vertexBuffer.Usage = D3D11_USAGE_DEFAULT;
	vertexBuffer.ByteWidth = sizeof(Vertex::PosNormalTexTan) * vertices->size();
	vertexBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBuffer.CPUAccessFlags = 0;
	vertexBuffer.MiscFlags = 0;
	vertexBuffer.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices->at(0);

	if (FAILED(m_resources->GetD3DDevice()->CreateBuffer(&vertexBuffer, &vinitData, &mVB)))
		return false;

	return true;
}

bool StaticMesh::BuildIndicesBuffer()
{

	D3D11_BUFFER_DESC indexBuffer;

	indexBuffer.Usage = D3D11_USAGE_DEFAULT;
	indexBuffer.ByteWidth = sizeof(UINT) * mIndices.size();
	indexBuffer.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBuffer.CPUAccessFlags = 0;
	indexBuffer.MiscFlags = 0;
	indexBuffer.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &mIndices.at(0);

	if (FAILED(m_resources->GetD3DDevice()->CreateBuffer(&indexBuffer, &iinitData, &mIB)))
		return false;

	return true;
}

bool StaticMesh::BuildIndicesBuffer(std::vector<UINT> *indices)
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

	if (FAILED(m_resources->GetD3DDevice()->CreateBuffer(&indexBuffer, &iinitData, &mIB)))
		return false;

	return true;
}