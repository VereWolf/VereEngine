#pragma once
#include "pch.h"
#include "GameObject.h"
#include "IDRegistr.h"

class GameObjectSpace : public GameObject
{
public:

	GameObjectSpace();
	GameObjectSpace(GameObjectSpace &other);
	~GameObjectSpace()
	{

	}

	void Init();
	void Render();
	void Update();

	GameObject* Clone() { return new GameObjectSpace(*this); }

	int CreateGameObject(int id);
	void DeleteGameObject(int id);
	inline int GetGameObjectID(int id);
private:
	IDRegistr *m_IDRegistr;
	std::vector<int> m_objectIDs;
};