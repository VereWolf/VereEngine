#include "pch.h"
#include "DeviceResources.h"
#include "GameResourceObjectsStack.h"
#include "GameResourceObject.h"
#include "IDRegistr.h"
#include "GameObjectsProcess.h"

GameResourceObjectsStack::GameResourceObjectsStack()
{
}

GameResourceObjectsStack::~GameResourceObjectsStack()
{
	delete idStaticMeshStack;
}

void GameResourceObjectsStack::Init(DX::DeviceResources *resources)
{
	idStaticMeshStack = new IDRegistr(16);

	m_resources = resources;
}

void GameResourceObjectsStack::Update()
{
}

void GameResourceObjectsStack::Render(int index, btTransform transform, btTransform scaling)
{
	if (m_gameObjects.size() > index)
	{
		m_gameObjects.at(index)->Render(transform, scaling);
	}
}

int GameResourceObjectsStack::CreateStaticMesh(GameResourceObject *object)
{
	int id = idStaticMeshStack->TakeElement();
	if (id >= m_gameObjects.size())
	{
		m_gameObjects.resize(m_gameObjects.size() + 32, NULL);
	}
	m_gameObjects[id] = object;

	return id;
}

void GameResourceObjectsStack::DestroyStaticMeshByID(int id)
{
	idStaticMeshStack->ReturnElement(id);
	delete m_gameObjects[id];
	m_gameObjects[id] = NULL;
}