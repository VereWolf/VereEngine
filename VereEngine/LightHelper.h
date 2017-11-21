#pragma once

#include "pch.h"


struct AmbientLight
{
	AmbientLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;
	float Power;
	float Pad;
};

struct DirectionalLight
{
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Direction;
	float Power;
	float Pad;
};

struct PointLight
{
	PointLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;

	XMFLOAT3 Position;
	float Range;

	XMFLOAT3 Att;
	float Power;
	float Pad;
};

struct SpotLight
{
	SpotLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;

	XMFLOAT3 Position;
	float Range;

	XMFLOAT3 Direction;
	float Spot;

	XMFLOAT3 Att;
	float Power;
	float Pad;
};

struct Fog
{
	Fog() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 color;
	float minDistance;
	float rangeDistance;
	float minHeight;
	float rangeHeight;
};

struct Material
{
	Material() { ZeroMemory(this, sizeof(this)); }
	Material(Material &m)
	{
		Ambient = m.Ambient;
		Diffuse = m.Diffuse;
		Specular = m.Specular;
		Reflect = m.Reflect;
	}

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT4 Reflect;
};

struct Model
{
	Model()
	{

	}
	Model(Model &m)
	{
		material = m.material;
		faceStart = m.faceStart;
		faceCount = m.faceCount;
		sizeOfVertex = m.sizeOfVertex;
		topology = m.topology;
		idMeshBuffer = m.idMeshBuffer;
		idEffect = m.idEffect;
		idVertex = m.idVertex;
		idInputLayouts = m.idInputLayouts;
	}

	Material material;
	UINT faceStart;
	UINT faceCount;
	UINT sizeOfVertex;
	D3D11_PRIMITIVE_TOPOLOGY topology;
	//std::vector<UINT> idMap;
	int idMeshBuffer;
	int idEffect;
	int idVertex;
	int idInputLayouts;
};