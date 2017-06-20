#include "pch.h"
#include "Vertex.h"
#include "Effects.h"
#include "GenerateMesh.h"
#include "Camera.h"
#include "LoadVMB.h"
#include "BodyMesh.h"

BodyMesh::BodyMesh()
{
	XMMATRIX I = XMMatrixIdentity();

	XMStoreFloat4x4(&mMesh, I);

	mTS.x = 0.0f;
	mTS.y = 0.01f;

	//mBodyBase = new BodyBase;
}

void BodyMesh::Draw(ID3D11Device* device, ID3D11DeviceContext* dc, Camera *cam, btTransform transform)
{
	if (mTS.x < 0.0f || mTS.x > 1.0f)
		mTS.y *= -1.0f;

	mTS.x += mTS.y;

	XMMATRIX view = cam->View();

	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dc->IASetInputLayout(InputLayouts::Body);

	UINT stride = sizeof(Vertex::Body);

	XMMATRIX M = XMLoadFloat4x4(&VereMath::ConvertToXMFLOAT4X4(transform));

	for (UINT n = 0; n < mVMBmat.size(); ++n)
	{
		UINT offset = 0;

		dc->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);

		dc->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

		ID3DX11EffectTechnique* activeTech = Effects::BodyFX->Light1Tech;

		D3DX11_TECHNIQUE_DESC techDesc;
		activeTech->GetDesc(&techDesc);

		XMFLOAT3 EyePos = { 0.0f, 0.0f, 0.0f };

		XMFLOAT3 dir = VereMath::ConvertToXMFLOAT3(transform.getOrigin().normalize());

		Effects::BodyFX->SetEyePosW(dir);
		Effects::BodyFX->SetMaterial(mVMBmat.at(n).material);
		Effects::BodyFX->SetViewProj(cam->ViewProj());
		Effects::BodyFX->SetWorld(M);
		Effects::BodyFX->SetWorldN(M);
		//Effects::BodyFX->SetDiffuseMap(mDiffuseMapSRV[n]);
		//Effects::BodyFX->SetSpecularMap(mSpecularMapSRV[n]);
		//Effects::BodyFX->SetNormalMap(mNormalMapSRV[n]);
		//Effects::BodyFX->SetIsDiffuseMap(isDiffuseMap.at(n));
		//Effects::BodyFX->SetIsSpecularMap(isSpecularMap.at(n));
		//Effects::BodyFX->SetIsNormalMap(isNormalMap.at(n));
		Effects::BodyFX->SetTS(mTS.x);

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			activeTech->GetPassByIndex(p)->Apply(0, dc);
			dc->DrawIndexed(3 * mVMBmat.at(n).faceCount, 3 * mVMBmat.at(n).faceStart, 0);
		}
	}
}

bool BodyMesh::CreateSphere(ID3D11Device* device, float diameter, int numGridInRow)
{
	/*mVMBmat.resize(1);
	mVMBmat.at(0).faceCount = 1200;
	mVMBmat.at(0).faceStart = 0;
	mVMBmat.at(0).d = 0.0f;
	mVMBmat.at(0).material.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mVMBmat.at(0).material.Diffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mVMBmat.at(0).material.Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mVMBmat.at(0).material.Reflect = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);

	GenerateMesh::GenerateSphere(diameter, numGridInRow, XMFLOAT3(0.0f, 0.0f, 0.0f), mVertices, mIndices);

	if (!BuildVerticesBuffer(device, &mVertices))
		return false;

	if (!BuildIndicesBuffer(device, &mIndices))
		return false;

	/mDiffuseMapSRV.resize(mVMBmat.size());
	mSpecularMapSRV.resize(mVMBmat.size());
	mNormalMapSRV.resize(mVMBmat.size());

	isDiffuseMap.resize(mVMBmat.size());
	isSpecularMap.resize(mVMBmat.size());
	isNormalMap.resize(mVMBmat.size());


	for (UINT i = 0; i < mVMBmat.size(); ++i)
	{
		isDiffuseMap.at(i) = false;
		isSpecularMap.at(i) = false;
		isNormalMap.at(i) = false;
	}*/

	return true;
}

bool BodyMesh::CreateObject(ID3D11Device* device, std::string filePath, std::wstring texturePath, std::string nameFile)
{
	std::vector<Vertex::Body> itsVertices;
	std::vector<UINT> itsIndices;

	VMBLoader mBodyBase;

	if (!mBodyBase.LoadVMB(nameFile.insert(0, filePath), &itsVertices, &itsIndices, &mVMBmat))
		return false;

	if (!BuildVerticesBuffer(device, &itsVertices))
		return false;

	if (!BuildIndicesBuffer(device, &itsIndices))
		return false;

	mDiffuseMapSRV.resize(mVMBmat.size());
	mSpecularMapSRV.resize(mVMBmat.size());
	mNormalMapSRV.resize(mVMBmat.size());

	isDiffuseMap.resize(mVMBmat.size());
	isSpecularMap.resize(mVMBmat.size());
	isNormalMap.resize(mVMBmat.size());

	for (UINT i = 0; i < mVMBmat.size(); ++i)
	{
		mVMBmat.at(i).paths.at(1).insert(0, texturePath);
		mVMBmat.at(i).paths.at(1).replace(mVMBmat.at(i).paths.at(1).end() - 3, mVMBmat.at(i).paths.at(1).end(), L"dds");
		mVMBmat.at(i).paths.at(2) = mVMBmat.at(i).paths.at(1);
		mVMBmat.at(i).paths.at(2).replace(mVMBmat.at(i).paths.at(2).end() - 5, mVMBmat.at(i).paths.at(2).end() - 4, L"S");
		mVMBmat.at(i).paths.at(5) = mVMBmat.at(i).paths.at(1);
		mVMBmat.at(i).paths.at(5).replace(mVMBmat.at(i).paths.at(5).end() - 5, mVMBmat.at(i).paths.at(5).end() - 4, L"N");
		//if (FAILED(D3DX11CreateShaderResourceViewFromFile(device, mVMBmat.at(i).paths.at(1).c_str(), 0, 0, &mDiffuseMapSRV.at(i), 0)))
		{
			isDiffuseMap.at(i) = false;
		}
		/*else
		{
			isDiffuseMap.at(i) = true;
		}*/

		//if (FAILED(D3DX11CreateShaderResourceViewFromFile(device, mVMBmat.at(i).paths.at(2).c_str(), 0, 0, &mSpecularMapSRV.at(i), 0)))
		{
			isSpecularMap.at(i) = false;
		}
		/*else
		{
			isSpecularMap.at(i) = true;
		}*/

		//if (FAILED(D3DX11CreateShaderResourceViewFromFile(device, mVMBmat.at(i).paths.at(5).c_str(), 0, 0, &mNormalMapSRV.at(i), 0)))
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

bool BodyMesh::BuildVerticesBuffer(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vertexBuffer;

	vertexBuffer.Usage = D3D11_USAGE_DEFAULT;
	vertexBuffer.ByteWidth = sizeof(Vertex::Body) * mVertices.size();
	vertexBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBuffer.CPUAccessFlags = 0;
	vertexBuffer.MiscFlags = 0;
	vertexBuffer.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &mVertices.at(0);

	if (FAILED(device->CreateBuffer(&vertexBuffer, &vinitData, &mVB)))
		return false;

	return true;
}

bool BodyMesh::BuildVerticesBuffer(ID3D11Device* device, std::vector<Vertex::Body> *vertices)
{
	D3D11_BUFFER_DESC vertexBuffer;

	vertexBuffer.Usage = D3D11_USAGE_DEFAULT;
	vertexBuffer.ByteWidth = sizeof(Vertex::Body) * vertices->size();
	vertexBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBuffer.CPUAccessFlags = 0;
	vertexBuffer.MiscFlags = 0;
	vertexBuffer.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices->at(0);

	if (FAILED(device->CreateBuffer(&vertexBuffer, &vinitData, &mVB)))
		return false;

	return true;
}

bool BodyMesh::BuildIndicesBuffer(ID3D11Device* device)
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

	if (FAILED(device->CreateBuffer(&indexBuffer, &iinitData, &mIB)))
		return false;

	return true;
}

bool BodyMesh::BuildIndicesBuffer(ID3D11Device* device, std::vector<UINT> *indices)
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

	if (FAILED(device->CreateBuffer(&indexBuffer, &iinitData, &mIB)))
		return false;

	return true;
}