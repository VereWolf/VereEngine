
#ifndef CLOTHING_MESH
#define CLOTHING_MESH

#include "pch.h"
#include "Vertex.h"
#include "Effects.h"
#include "LoadVMS.h"
#include "Camera.h"
#include "GenerateMesh.h"

#define CLOTHING_MESH_DLL_EXPORTS
#ifdef CLOTHING_MESH_DLL_EXPORTS
#define CLOTHING_MESH_API __declspec( dllexport ) 
#else
#define CLOTHING_MESH_API __declspec( dllimport )
#endif

class ClothingMesh
{
public:
	ClothingMesh()
	{
		XMMATRIX I = XMMatrixIdentity();

		XMStoreFloat4x4(&mMesh, I);
	}

	void Draw(ID3D11Device* device, ID3D11DeviceContext* dc, Camera *cam, XMDOUBLE3 pos, XMFLOAT3 angle)
	{
		XMMATRIX mesh = XMLoadFloat4x4(&mMesh);
		XMMATRIX meshN = XMLoadFloat4x4(&mMesh);


		mesh *= XMMatrixScaling(-1.0f, -1.0f, -1.0f);

		mesh *= XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

		meshN *= XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

		mesh *= XMMatrixTranslation(
			cam->GetPosition().x - pos.x,
			cam->GetPosition().y - pos.y,
			cam->GetPosition().z - pos.z
			);

		XMMATRIX view = cam->View();

		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		dc->IASetInputLayout(InputLayouts::PosNormalTexTan);

		UINT stride = sizeof(Vertex::PosNormalTexTan);

		for (UINT n = 0; n < mVMSmat.size(); ++n)
		{
			UINT offset = 0;

			dc->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);

			dc->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

			ID3DX11EffectTechnique* activeTech = Effects::PosNormalTexTanFX->Light1Tech;

			D3DX11_TECHNIQUE_DESC techDesc;
			activeTech->GetDesc(&techDesc);

			XMFLOAT3 EyePos = { 0.0f, 0.0f, 0.0f };

			XMFLOAT3 dir = XMFLOAT3(
				cam->GetPosition().x - pos.x,
				cam->GetPosition().y - pos.y,
				cam->GetPosition().z - pos.z);

			/*float dirAbs = pow(pow(dir.x, 2.0f) + pow(dir.y, 2.0f) + pow(dir.z, 2.0f), 0.5f);

			dir.x /= dirAbs;
			dir.y /= dirAbs;
			dir.z /= dirAbs;*/
			
			Effects::PosNormalTexTanFX->SetEyePosW(dir);
			Effects::PosNormalTexTanFX->SetMaterial(mVMSmat.at(n).material);
			Effects::PosNormalTexTanFX->SetViewProj(cam->ViewProj());
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
				activeTech->GetPassByIndex(p)->Apply(0, dc);
				dc->DrawIndexed(3 * mVMSmat.at(n).faceCount, 3 * mVMSmat.at(n).faceStart, 0);
			}
		}
	}

	bool CreateObject(ID3D11Device* device, std::string filePath, std::wstring texturePath, std::string nameFile)
	{
		std::vector<Vertex::PosNormalTexTan> itsVertices;
		std::vector<UINT> itsIndices;

		if (!VMSLoader::LoadVMS(nameFile.insert(0, filePath), itsVertices, itsIndices, mVMSmat))
			return false;

		if (!BuildVerticesBuffer(device, &itsVertices))
			return false;

		if (!BuildIndicesBuffer(device, &itsIndices))
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
			//if (FAILED(D3DX11CreateShaderResourceViewFromFile(device, mVMSmat.at(i).paths.at(1).c_str(), 0, 0, &mDiffuseMapSRV.at(i), 0)))
			{
				isDiffuseMap.at(i) = false;
			}
			/*/else
			{
				isDiffuseMap.at(i) = true;
			}*/

			//if (FAILED(D3DX11CreateShaderResourceViewFromFile(device, mVMSmat.at(i).paths.at(2).c_str(), 0, 0, &mSpecularMapSRV.at(i), 0)))
			{
				isSpecularMap.at(i) = false;
			}
			/*else
			{
				isSpecularMap.at(i) = true;
			}*/

			//if (FAILED(D3DX11CreateShaderResourceViewFromFile(device, mVMSmat.at(i).paths.at(5).c_str(), 0, 0, &mNormalMapSRV.at(i), 0)))
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

	/*void MakeFitClothing(std::vector<Vertex::PosNormalTexTan> * clothingVertices, std::vector<UINT> clothingIndices,
		std::vector<Vertex::Body> * sampleVertices, std::vector<UINT> sampleIndices)
	{
		for (UINT j = 0; j < sampleIndices.size(); j += 3)
		{
			for (UINT i = 0; i < clothingIndices.size(); i++)
			{
				if (VereMath::intersectTriangle(
					sampleVertices->at(sampleIndices.at(j)).Position,
					sampleVertices->at(sampleIndices.at(j + 1)).Position,
					sampleVertices->at(sampleIndices.at(j + 2)).Position,
					clothingVertices->at(clothingIndices.at(i)).Position,
					clothingVertices->at(clothingIndices.at(i)).Normal))
				{
					clothingVertices->at(clothingIndices.at(i)).Position = VereMath::PenetrationOfTriangle(
						sampleVertices->at(sampleIndices.at(j)).Position,
						sampleVertices->at(sampleIndices.at(j + 1)).Position,
						sampleVertices->at(sampleIndices.at(j + 2)).Position,
						clothingVertices->at(clothingIndices.at(i)).Position,
						clothingVertices->at(clothingIndices.at(i)).Normal);
				}
			}
		}

	}*/

	bool BuildVerticesBuffer(ID3D11Device* device, std::vector<Vertex::PosNormalTexTan> *vertices)
	{
		D3D11_BUFFER_DESC vertexBuffer;

		vertexBuffer.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBuffer.ByteWidth = sizeof(Vertex::PosNormalTexTan) * vertices->size();
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

	bool BuildIndicesBuffer(ID3D11Device* device, std::vector<UINT> *indices)
	{

		D3D11_BUFFER_DESC indexBuffer;

		indexBuffer.Usage = D3D11_USAGE_IMMUTABLE;
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

private:
	ID3D11Buffer *mVB;
	ID3D11Buffer *mIB;

	std::vector<ID3D11ShaderResourceView*> mDiffuseMapSRV;
	std::vector<ID3D11ShaderResourceView*> mSpecularMapSRV;
	std::vector<ID3D11ShaderResourceView*> mNormalMapSRV;

	std::vector<bool> isDiffuseMap;
	std::vector<bool> isSpecularMap;
	std::vector<bool> isNormalMap;

	XMFLOAT4X4 mMesh;

	std::vector<VMSMaterial> mVMSmat;
};

#endif //CLOTHING_MESH