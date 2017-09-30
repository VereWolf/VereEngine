#include "pch.h"
#include "GameBaseObjectsStack.h"

GameBaseObjectsStack::GameBaseObjectsStack()
{

};

void GameBaseObjectsStack::Init(DX::DeviceResources *resources)
{
	m_resources = resources;

	idObjectStack = new IDRegistr(16);
};

int GameBaseObjectsStack::CreateGameObject(GameBaseObject *object)
{
	int id = idObjectStack->TakeElement();
	if (id >= m_gameObjects.size())
	{
		m_gameObjects.resize(m_gameObjects.size() + 32, NULL);
	}
	m_gameObjects[id] = object;

	return id;
};

void GameBaseObjectsStack::DeleteGameObject(int id)
{
	if (id < 0) return;

	if (m_gameObjects[id] != NULL)
	{
		idObjectStack->ReturnElement(id);
		delete m_gameObjects[id];
		m_gameObjects[id] = NULL;
	}
};

GameBaseObject *GameBaseObjectsStack::GetGameObject(int id)
{
	if (id >= m_gameObjects.size())
	{
		return NULL;
	}

	return m_gameObjects.at(id);
};