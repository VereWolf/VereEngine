#include "pch.h"
#include "GameObject.h"
#include "IDRegistr.h"
#include "GameObjectSpace.h"

GameObjectSpace::GameObjectSpace()
{

}

GameObjectSpace::GameObjectSpace(GameObjectSpace &other)
{

}

void GameObjectSpace::Init()
{
	m_IDRegistr = new IDRegistr(16);
};

void GameObjectSpace::Render(btTransform camOffset, XMMATRIX camView, XMMATRIX camProj,
	float camFarZ, btScalar heightFar, btScalar aspect,
	float camFarRangeMod, float camModifier)
{
}

void GameObjectSpace::Update()
{
	Rotation(btVector3(0.0, 1.0, 0.0), 0.02 * GetDeltaTime());
	Circuit(btVector3(0.0, 1.0, 0.0), 0.04 * GetDeltaTime());
}

int GameObjectSpace::CreateGameObject(int id)
{
	int id2 = m_IDRegistr->TakeElement();
	if (id2 >= m_objectIDs.size())
	{
		m_objectIDs.resize(m_objectIDs.size() + 32, -1);
	}
	m_objectIDs[id2] = id;

	GameObjectStackHandle->GetGameObjectByID(id)->SetMaster(this);

	return id2;
};

void GameObjectSpace::DeleteGameObject(int id)
{
	if (m_objectIDs[id] != NULL)
	{
		m_IDRegistr->ReturnElement(id);
		m_objectIDs[id] = -1;
	}
};

inline int GameObjectSpace::GetGameObjectID(int id)
{
	if (id >= m_objectIDs.size())
	{
		return -1;
	}

	return m_objectIDs.at(id);
};