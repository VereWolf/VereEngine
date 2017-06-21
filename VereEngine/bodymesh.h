
#ifndef BODY_MESH
#define BODY_MESH

#include "Vertex.h"
#include "LoadVMB.h"

class BodyMesh
{
public:
	BodyMesh();

	~BodyMesh(){};
	void Draw(DX::DeviceResources *resources, btTransform transform);

	bool CreateSphere(DX::DeviceResources *resources, float diameter, int numGridInRow);
	bool CreateObject(DX::DeviceResources *resources, std::string filePath, std::wstring texturePath, std::string nameFile);
	bool BuildVerticesBuffer(DX::DeviceResources *resources);
	bool BuildVerticesBuffer(DX::DeviceResources *resources, std::vector<Vertex::Body> *vertices);
	bool BuildIndicesBuffer(DX::DeviceResources *resources);
	bool BuildIndicesBuffer(DX::DeviceResources *resources, std::vector<UINT> *indices);
	std::vector<Vertex::Body> *GetVertices(){ return &mVertices; }
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

	XMFLOAT4X4 mMesh;

	std::vector<VMBMaterial> mVMBmat;

	std::vector<Vertex::Body> mVertices;
	std::vector<UINT> mIndices;
	XMFLOAT2 mTS;
};

#endif //BODY_MESH