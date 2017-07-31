#pragma once

#include "LightHelper.h"
#include "Vertex.h"

struct VertexOfTransformation
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	UINT index;
};

struct VMBMaterial
{
	VMBMaterial() { ZeroMemory(this, sizeof(this)); }
	Material material;
	float d;

	UINT faceStart;
	UINT faceCount;
	std::vector <std::wstring> paths;
};

class VMBLoader
{
public:
	VMBLoader() {};
	~VMBLoader() {};

	bool LoadVMB(std::string nameFile, std::vector<Vertex::Body> *vertices, std::vector<UINT> *indices, std::vector<VMBMaterial> *materials);

private:
	std::vector<Vertex::Body> mBaseVertex;
	std::vector<std::vector<VertexOfTransformation>> mTransformations;
};