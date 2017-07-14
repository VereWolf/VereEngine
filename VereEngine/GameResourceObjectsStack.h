#pragma once

#include "pch.h"
#include "GameResourceObject.h"

class GameResourceObjectsStack
{
public:
	GameResourceObjectsStack();
	~GameResourceObjectsStack();

	void Init(DX::DeviceResources *resources);
	void Update();
	void Render(int index, btTransform transform, btTransform scaling);

	int CreateStaticMesh(GameResourceObject *object);
	void DestroyStaticMeshByID(int id);

	GameResourceObject* GetGameStaticMeshByID(int id) { return m_gameObjects[id]; }
private:

	DX::DeviceResources* m_resources;

	IDRegistr *idStaticMeshStack;
	std::vector<GameResourceObject*> m_gameObjects;
};