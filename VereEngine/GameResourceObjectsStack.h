#include "pch.h"
#include "GameResourceObject.h"

#ifndef GAME_RESOURCE_OBJECTS_STACK
#define GAME_RESOURCE_OBJECTS_STACK

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

	IDStack *idStaticMeshStack;
	std::vector<GameResourceObject*> m_gameObjects;
};

#endif //GAME_RESOURCE_OBJECTS_STACK