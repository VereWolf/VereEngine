#pragma once

#include "pch.h"
#include "IDRegistr.h"


class GameObjectsIDRegistr
{
public:
	GameObjectsIDRegistr();
	~GameObjectsIDRegistr()
	{
		delete idObjectStack;

		for (int i = 0; i < m_gameObjects.size(); ++i)
		{
			DeleteGameObject(i);
		}
	}

	void Init(DX::DeviceResources *resources);

	int CreateGameObject(int idInReg);
	void DeleteGameObject(int id);
	int GetGameObject(int id);
private:
	DX::DeviceResources *m_resources;

	IDRegistr *idObjectStack;
	std::vector<int> m_gameObjects;
};