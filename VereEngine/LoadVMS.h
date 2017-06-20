#ifndef LOAD_VMS
#define LOAD_VMS

#include "pch.h"
#include "vertex.h"

struct VMSMaterial
{
	VMSMaterial(){ ZeroMemory(this, sizeof(this));}
	Material material;
	float d;

	UINT faceStart;
	UINT faceCount;
	std::vector <std::wstring> paths;
};

class VMSLoader
{
public:

	static bool LoadVMS(std::string nameFile, std::vector<Vertex::PosNormalTexTan> &vertices, std::vector<UINT> &indices, std::vector<VMSMaterial> &materials)
	{
		UINT elmUINT;
		std::ifstream file(nameFile, std::ifstream::binary);

		if (!file)
		{
			return false;
		}

		file.read((char*)&elmUINT, sizeof(UINT));
		vertices.resize(elmUINT);

		for (UINT i = 0; i < vertices.size(); i++)
		{
			file.read((char*)&vertices.at(i).Position.x, sizeof(float));
			file.read((char*)&vertices.at(i).Position.y, sizeof(float));
			file.read((char*)&vertices.at(i).Position.z, sizeof(float));
			file.read((char*)&vertices.at(i).Normal.x, sizeof(float));
			file.read((char*)&vertices.at(i).Normal.y, sizeof(float));
			file.read((char*)&vertices.at(i).Normal.z, sizeof(float));

			vertices.at(i).Tangent = vertices.at(i).Normal;
			vertices.at(i).Tangent.x += 0.5f;
			vertices.at(i).Tangent.y += 1.0f;
			vertices.at(i).Tangent.z += 2.0f;

			vertices.at(i).Tangent.x *= vertices.at(i).Tangent.x;
			vertices.at(i).Tangent.y *= vertices.at(i).Tangent.y;
			vertices.at(i).Tangent.z *= vertices.at(i).Tangent.z;

			XMStoreFloat3(&vertices.at(i).Tangent, XMVector3Normalize(XMVector3Cross(
				XMLoadFloat3(&vertices.at(i).Normal),
				XMVector3Normalize(XMLoadFloat3(&vertices.at(i).Tangent)))));

			file.read((char*)&vertices.at(i).Tex.x, sizeof(float));
			file.read((char*)&vertices.at(i).Tex.y, sizeof(float));
		}

		file.read((char*)&elmUINT, sizeof(UINT));
		indices.resize(elmUINT);

		for (UINT i = 0; i < indices.size(); i++)
		{
			file.read((char*)&indices.at(i), sizeof(float));
		}

		file.read((char*)&elmUINT, sizeof(UINT));
		materials.resize(elmUINT);

		for (UINT i = 0; i < materials.size(); i++)
		{
			file.read((char*)&materials.at(i).material.Ambient.x, sizeof(float));
			file.read((char*)&materials.at(i).material.Ambient.y, sizeof(float));
			file.read((char*)&materials.at(i).material.Ambient.z, sizeof(float));
			materials.at(i).material.Ambient.w = 1.0f;
			file.read((char*)&materials.at(i).material.Diffuse.x, sizeof(float));
			file.read((char*)&materials.at(i).material.Diffuse.y, sizeof(float));
			file.read((char*)&materials.at(i).material.Diffuse.z, sizeof(float));
			materials.at(i).material.Diffuse.w = 1.0f;
			file.read((char*)&materials.at(i).material.Specular.x, sizeof(float));
			file.read((char*)&materials.at(i).material.Specular.y, sizeof(float));
			file.read((char*)&materials.at(i).material.Specular.z, sizeof(float));
			file.read((char*)&materials.at(i).material.Specular.w, sizeof(float));
			file.read((char*)&materials.at(i).d, sizeof(float));
			file.read((char*)&materials.at(i).faceStart, sizeof(UINT));
			file.read((char*)&materials.at(i).faceCount, sizeof(UINT));

			file.read((char*)&elmUINT, sizeof(UINT));
			materials.at(i).paths.resize(6);
			UINT lsize = elmUINT;
			for (UINT j = 0; j < lsize; j++)
			{
				file.read((char*)&elmUINT, sizeof(UINT));
				UINT k = elmUINT;

				file.read((char*)&elmUINT, sizeof(UINT));
				char ch = ' ';
				materials.at(i).paths.at(k).resize(elmUINT);

				for (UINT m = 0; m < elmUINT; m++)
				{
					file.read(&ch, sizeof(char));
					materials.at(i).paths.at(k).at(m) = ch;
				}
			}
		}

		file.close();

		return true;
	}
};


#endif//LOAD_VMS