#include "pch.h"
#include "GameObjectsIDRegistr.h"

GameObjectsIDRegistr::GameObjectsIDRegistr()
{

};

void GameObjectsIDRegistr::Init(DX::DeviceResources *resources)
{
	m_resources = resources;

	idObjectStack = new IDRegistr(16);
};

int GameObjectsIDRegistr::CreateGameObject(int idInReg)
{
	int id = idObjectStack->TakeElement();
	if (id >= m_gameObjects.size())
	{
		m_gameObjects.resize(m_gameObjects.size() + 32, -1);
	}
	m_gameObjects[id] = idInReg;

	return id;
};

void GameObjectsIDRegistr::DeleteGameObject(int id)
{
	if (m_gameObjects[id] >= 0)
	{
		idObjectStack->ReturnElement(id);
		m_gameObjects[id] = -1;
	}
};

int GameObjectsIDRegistr::GetGameObject(int id)
{
	if (id >= m_gameObjects.size())
	{
		return -1;
	}

	return m_gameObjects.at(id);
};