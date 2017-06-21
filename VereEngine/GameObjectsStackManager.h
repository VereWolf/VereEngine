/*Create by Michal Cizek and it is stack for working with different objects*/

#ifndef GAME_OBJECT_STACK_MANAGER
#define GAME_OBJECT_STACK_MANAGER

#include "pch.h"
#include "DeviceResources.h"
#include "GameObject.h"
#include "GameObjectsProcess.h"
#include "GameBaseObjectsStack.h"
#include "GameObjectsIDStack.h"
#include "StaticMesh.h"
#include "IDStack.h"
#include "GameBaseObject.h"
#include "Planet.h"
#include "Camera.h"
#include "TestObject.h"
#include "GameObjectSpace.h"
#include "TextRenderDevice.h"
#include "RenderDevice.h"

#define REGISTR_CLASS(CLASS)	\
{	\
	GameObject *object = new CLASS;	\
	object->PreInit(m_resources->GetD3DDevice(), m_resources->GetD3DDeviceContext(), m_cam);	\
	object->Init();	\
	CreateObjectToReg(object);	\
}



class GameObjectsStackManager
{
public:
	GameObjectsStackManager();
	GameObjectsStackManager(DX::DeviceResources *resources);
	~GameObjectsStackManager();

	void Init(DX::DeviceResources *resources, UINT numObjects = 32);
	void Update();
	void DrawScene();

	Camera *GetMainCamera() { return m_camera; }
	void SeatMainCamera(Camera * cam);
	
	int CreateObjectToReg(GameObject *object);
	void DestroyObjectToRegByID(int id);
	void ObjectRegistryInit()
	{
		//REGISTR_CLASS(TestObject);
	}

	int CreateObject(int idReg, btVector3 position, btVector3 angle, btVector3 spacing, btScalar mass, btVector3 localInertia, btScalar rangeForFullProcess, btCollisionShape *collisionShape);
	void DestroyObjectByID(int id);
	void DestroyAllObjects();
	int GetCurrentObjectID() { return m_CurrentObjectID; }
	void SaveOrLoadObjects(std::string nameFile, std::string namePath, bool isSaveMod);

	AssetsStacks* GetAssetsStacks() { return &m_assetsStacks; }
	GameObject* GetGameObjectByIDReg(int id) { return m_gameObjectsReg[id]; }
	GameObject* GetGameObjectByID(int id) { return m_gameObjects[id]; }
	int GetGameObjectStackSize() { return m_gameObjects.size(); }

	void CalcOffset(XMINT2 inputCC, XMINT2 inputBC, int lenght, XMINT2 offset, XMINT2 *outputCC, XMINT2 *outputBC);
	void PerformInteractions(int id);
	void InteractionsOfTwoObject(int id1, int id2);
	USING GenerateContact();

public:
	double m_duration;
private:

	DX::DeviceResources* m_resources;

	Camera * m_camera;

	IDStack *idObjectStackReg;
	IDStack *idObjectStack;

	std::vector<GameObject*> m_gameObjectsReg;

	std::vector<GameObject*> m_gameObjects;

	int m_CurrentObjectID;

	USING m_maxContact;

	//GameObjectsProcess *m_gameObjectProcess;
	std::vector<GameObjectsProcess> m_gameObjectProcess;
	GameObjectsProcess m_gameBaseObjectProcess;

	AssetsStacks m_assetsStacks;

	btDefaultCollisionConfiguration* m_bulletCollisionConfiguration;

	btCollisionDispatcher* m_bulletDispatcher;

	btBroadphaseInterface* m_bulletOverlappingPairCache;

	btSequentialImpulseConstraintSolver* m_bulletSolver;

	btDiscreteDynamicsWorld* m_bulletDynamicsWorld;
};

__declspec(selectany) GameObjectsStackManager *GameObjectStackHandle = 0;

#endif //GAME_OBJECT_STACK_MANAGER