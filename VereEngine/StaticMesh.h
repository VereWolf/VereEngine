#pragma once

#include "Vertex.h"
#include "LoadVMB.h"
#include "LoadVMS.h"
#include "GameBaseObject.h"

class StaticMesh: public GameResourceObject
{
public:
	StaticMesh();

	~StaticMesh(){};

	void PreInit(DX::DeviceResources *resources)
	{
		GameResourceObject::PreInit(resources);
	}
	void Render(btTransform transform, btTransform scaling);

	void LoadResource() { CreateSphere(10.0f, 5); }

	bool CreateSphere(float diameter, int numGridInRow);
	bool CreateObject(std::string filePath, std::wstring texturePath, std::string nameFile);
	bool BuildVerticesBuffer();
	bool BuildVerticesBuffer(std::vector<Vertex::PosNormalTexTan> *vertices);
	bool BuildIndicesBuffer();
	bool BuildIndicesBuffer(std::vector<UINT> *indices);
	std::vector<Vertex::PosNormalTexTan> *GetVertices(){ return &mVertices; }
	std::vector<UINT> *GetIndices(){ return &mIndices; }

private:
	ID3D11Buffer *mVB;
	ID3D11Buffer *mIB;
	std::vector<ID3D11ShaderResourceView*> mDiffuseMapSRV;

	std::vector<ID3D11ShaderResourceView*> mSpecularMapSRV;
	std::vector<ID3D11ShaderResourceView*> mNormalMapSRV;

	std::vector<bool> isDiffuseMap;
	std::vector<bool> isSpecularMap;
	std::vector<bool> isNormalMap;

	std::vector<VMSMaterial> mVMSmat;

	std::vector<Vertex::PosNormalTexTan> mVertices;
	std::vector<UINT> mIndices;
};