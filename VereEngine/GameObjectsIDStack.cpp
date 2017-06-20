#include "pch.h"
#include "GameObjectsIDStack.h"

GameObjectsIDStack::GameObjectsIDStack()
{

};

void GameObjectsIDStack::Init(DX::DeviceResources *resources)
{
	m_resources = resources;

	idObjectStack = new IDStack;
};

int GameObjectsIDStack::CreateGameObject(int idInReg)
{
	int id = idObjectStack->GetElement();
	if (id >= m_gameObjects.size())
	{
		m_gameObjects.resize(m_gameObjects.size() + 32, -1);
	}
	m_gameObjects[id] = idInReg;

	return id;
};

void GameObjectsIDStack::DeleteGameObject(int id)
{
	if (m_gameObjects[id] >= 0)
	{
		idObjectStack->ReturnElement(id);
		m_gameObjects[id] = -1;
	}
};

int GameObjectsIDStack::GetGameObject(int id)
{
	if (id >= m_gameObjects.size())
	{
		return -1;
	}

	return m_gameObjects.at(id);
};