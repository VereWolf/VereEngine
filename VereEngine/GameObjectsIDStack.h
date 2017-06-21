#ifndef GAME_OBJECTS_STACK_H
#define GAME_OBJECTS_STACK_H

#include "pch.h"
#include "IDStack.h"


class GameObjectsIDStack
{
public:
	GameObjectsIDStack();
	~GameObjectsIDStack()
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

	IDStack *idObjectStack;
	std::vector<int> m_gameObjects;
};

#endif //GAME_OBJECTS_STACK_H