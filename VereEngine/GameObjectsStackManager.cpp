#include "pch.h"
#include "DeviceResources.h"
#include "StaticMesh.h"
#include "GameObjectsProcess.h"
#include "IDRegistr.h"
#include "TestObject.h"
#include "RenderDevice.h"
#include "Camera.h"
#include "GameObjectsStackManager.h"
#include "RenderDevice.h"

GameObjectsStackManager::GameObjectsStackManager()
{
	GameObjectStackHandle = this;

	idObjectStackReg = new IDRegistr(16);
	idObjectStack = new IDRegistr(16);

	m_bulletCollisionConfiguration = new btDefaultCollisionConfiguration();
	m_bulletDispatcher = new btCollisionDispatcher(m_bulletCollisionConfiguration);
	m_bulletOverlappingPairCache = new btDbvtBroadphase();
	m_bulletSolver = new btSequentialImpulseConstraintSolver();
	m_bulletDynamicsWorld = new btDiscreteDynamicsWorld(m_bulletDispatcher, m_bulletOverlappingPairCache, m_bulletSolver, m_bulletCollisionConfiguration);
}

GameObjectsStackManager::GameObjectsStackManager(DX::DeviceResources *resources)
{
	GameObjectStackHandle = this;

	idObjectStackReg = new IDRegistr(16);
	idObjectStack = new IDRegistr(16);

	m_bulletCollisionConfiguration = new btDefaultCollisionConfiguration();
	m_bulletDispatcher = new btCollisionDispatcher(m_bulletCollisionConfiguration);
	m_bulletOverlappingPairCache = new btDbvtBroadphase();
	m_bulletSolver = new btSequentialImpulseConstraintSolver();
	m_bulletDynamicsWorld = new btDiscreteDynamicsWorld(m_bulletDispatcher, m_bulletOverlappingPairCache, m_bulletSolver, m_bulletCollisionConfiguration);
	
	this->Init(resources);
}

GameObjectsStackManager::~GameObjectsStackManager()
{
	delete idObjectStackReg;
	delete idObjectStack;
	GameObjectStackHandle = NULL;
	delete m_bulletCollisionConfiguration;
	delete m_bulletDispatcher;
	delete m_bulletOverlappingPairCache;
	delete m_bulletSolver;
	delete m_bulletDynamicsWorld;
}

void GameObjectsStackManager::Init(DX::DeviceResources *resources, UINT numObjects)
{
	m_resources = resources;

	GetAssetsStacks()->m_gameCollisionShapes.Init(resources);
	GetAssetsStacks()->m_gameSounds.Init(resources);

	m_gameObjects.resize(numObjects, NULL);

	m_gameObjectProcess.resize(8);
	m_gameObjectProcess[0].SetMaxObjectsInFrame(600);
	m_gameObjectProcess[1].SetMaxObjectsInFrame(600);
	m_gameObjectProcess[2].SetMaxObjectsInFrame(600);
	m_gameObjectProcess[3].SetMaxObjectsInFrame(600);
	m_gameObjectProcess[4].SetMaxObjectsInFrame(600);
	m_gameObjectProcess[5].SetMaxObjectsInFrame(600);
	m_gameObjectProcess[6].SetMaxObjectsInFrame(600);
	m_gameObjectProcess[7].SetMaxObjectsInFrame(600);

	ObjectRegistryInit();

	m_CurrentObjectID = -1;

	Camera *cam = new Camera;
	int idCam = CreateObjectToReg(cam);
	CreateObject(idCam, btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0), btVector3(1.0, 1.0, 1.0), 0, btVector3(0.0, 0.0, 0.0), 100.0, nullptr);
	m_camera = ((Camera*)GetGameObjectByID(idCam));
	((Camera*)GetGameObjectByID(idCam))->SetMainCamera(true);
}

void GameObjectsStackManager::Update() 
{
	XMINT2 BC;
	XMINT2 CC;
	//VFloat2 NCC;

	//m_registry.UpdateForces(m_duration);
	
	std::vector<int> S(m_gameObjectProcess.size());
	
	for (int i = 0; i < S.size(); ++i)
	{
		S[i] = m_gameObjectProcess[i].GetSize();
	}

	for (int i = 0; i < m_gameObjectProcess.size(); ++i)
	{
		m_gameObjectProcess[i].NewFrame();
		for (int j = 0; j < m_gameObjectProcess.at(i).GetMaxObjectsInFrame() && j < S[i]; ++j)
		{
			int id = m_gameObjectProcess.at(i).TakeObject();
			if (id >= 0)
			{
				if (GetGameObjectByID(id)->IsDestroy())
				{
					idObjectStack->ReturnElement(id);
					delete m_gameObjects[id];
					m_gameObjects[id] = NULL;

					if (id >= m_CurrentObjectID)
					{
						for (int i = id; i >= 0; --i)
						{
							if (m_gameObjects[i] != NULL)
							{
								m_CurrentObjectID = i;
							}
							else
							{
								if (i == 0)
								{
									m_CurrentObjectID = -1;
								}
							}
						}
					}
				}
				else
				{
					GetGameObjectByID(id)->Tick();
					GetGameObjectByID(id)->Update();
					GetGameObjectByID(id)->Integrate();

					btVector3 P = GetGameObjectByID(id)->GetWorldPosition();
					btVector3 C = GameObjectStackHandle->GetMainCamera()->GetWorldPosition();
					btScalar D = (P - C).length();

					btScalar max = GetGameObjectByID(id)->GetRangeForFullProcess();
					int index = 1;

					while ((max * index * index) < D && index < m_gameObjectProcess.size())
					{
						++index;
					}

					index -= 1;

					m_gameObjectProcess[index].GiveObject(id);
				}
			}
		}
	}

	m_camera->Update();
}

void GameObjectsStackManager::DrawScene()
{
	GameTextRenderDeviceHandle->SetOffset(0.0f);

	GameRenderDeviceHandle->BindMainRenderTarget();
	GameRenderDeviceHandle->ClearMainRenderTarget();

	int S = GetGameObjectStackSize();

	PlanetLOD::m_onlyRenderText = false;
	RenderMessage::m_ViewPort = GameRenderDeviceHandle->GetMainViewPort();
	RenderMessage::m_CameraOffset = GetMainCamera()->btOffset();
	RenderMessage::m_View = GetMainCamera()->View();
	RenderMessage::m_Proj = GetMainCamera()->Proj();

	for (int i = 0; i < S; ++i)
	{
		if (GetGameObjectByID(i) != NULL && GetGameObjectByID(i)->GetRenderId() >= 0)
		{
			GetGameObjectByID(i)->Render();
		}
	}

	GameRenderDeviceHandle->RenderToScreen();

	/*TerrainLOD::m_onlyRenderText = true;

	for (int i = 0; i < S; ++i)
	{
		if (GetGameObjectByID(i) != NULL && GetGameObjectByID(i)->GetRenderId() >= 0)
		{
			GetGameObjectByID(i)->Render(GetMainCamera()->btOffset(), GetMainCamera()->View(), GetMainCamera()->Proj(),
				GetMainCamera()->GetFarZ(), GetMainCamera()->GetFarWindowHeight(), GetMainCamera()->GetAspect(),
				GetMainCamera()->GetFarRangeMod(), GetMainCamera()->GetFarModifier());
		}
	}*/
}

void GameObjectsStackManager::SeatMainCamera(Camera * cam)
{
	m_camera->SetMainCamera(false);
	m_camera = cam;
	m_camera->SetMainCamera(true);
}

int GameObjectsStackManager::CreateObjectToReg(GameObject *object)
{
	int id = idObjectStackReg->TakeElement();

	object->SetId(id);
	if (id >= m_gameObjectsReg.size())
	{
		m_gameObjectsReg.resize(m_gameObjectsReg.size() + 32, NULL);
	}
	object->SetIdInRegistr(id);
	m_gameObjectsReg[id] = object;

	return id;
}

void GameObjectsStackManager::DestroyObjectToRegByID(int id)
{
	GetGameObjectByIDReg(id)->Destroy();
}

int GameObjectsStackManager::CreateObject(int idReg, btVector3 position, btVector3 angle, btVector3 spacing, btScalar mass, btVector3 localInertia, btScalar rangeForFullProcess, btCollisionShape *collisionShape)
{
	int id = idObjectStack->TakeElement();

	if (id >= m_gameObjects.size())
	{
		m_gameObjects.resize(m_gameObjects.size() + 32, NULL);
	}

	m_gameObjects[id] = GetGameObjectByIDReg(idReg)->Clone();
	m_gameObjects[id]->PreInit(m_resources, position, angle, spacing, mass, localInertia, rangeForFullProcess, collisionShape);
	m_gameObjects[id]->SetId(id);
	m_gameObjects[id]->SetIdInRegistr(idReg);
	m_gameObjects[id]->Init();

	m_gameObjectProcess[0].GiveObject(id);

	m_CurrentObjectID = id;

	return id;
}

void GameObjectsStackManager::DestroyObjectByID(int id)
{
	GetGameObjectByID(id)->Destroy();
}


void  GameObjectsStackManager::DestroyAllObjects()
{
	for (int i = 0; i < m_gameObjects.size(); ++i)
	{
		if (m_gameObjects[i] != NULL)
		{
			m_gameObjects[i]->Destroy();
		}
	}

	Update();

	m_gameObjects.resize(0);
	m_CurrentObjectID = -1;
}

void GameObjectsStackManager::SaveOrLoadObjects(std::string nameFile, std::string namePath, bool isSaveMod)
{
	nameFile.insert(0, namePath);

	if (isSaveMod == true)
	{
		ULONG index = 0;
		int size = 0;
		int regId;
		UINT numberObjects = m_gameObjects.size();
		std::vector<char> buffer(sizeof(numberObjects));
		memcpy(&buffer[index], &numberObjects, sizeof(numberObjects));
		index += sizeof(numberObjects);
		size += sizeof(numberObjects);

		for (UINT i = 0; i < numberObjects; ++i)
		{
			if (m_gameObjects.at(i) == NULL)
			{
				size += sizeof(size);
			}
			else
			{
				size += sizeof(size) + sizeof(regId);
				m_gameObjects.at(i)->PerformancePersistence(true, false);
				size += m_gameObjects.at(i)->GetSizeOfBuffer();
				m_gameObjects.at(i)->SetSizeOfPersistenceBuffer(m_gameObjects.at(i)->GetSizeOfBuffer());
			}
		}

		buffer.resize(size);

		for (UINT i = 0; i < numberObjects; ++i)
		{
			if (m_gameObjects.at(i) == NULL)
			{
				size = 0;
				memcpy(&buffer[index], &size, sizeof(size));
				index += sizeof(size);
			}
			else
			{
				size = m_gameObjects.at(i)->GetSizeOfBuffer();
				memcpy(&buffer[index], &size, sizeof(size));
				index += sizeof(size);
				regId = m_gameObjects.at(i)->GetIdInRegistr();
				memcpy(&buffer[index], &regId, sizeof(regId));
				index += sizeof(regId);
				m_gameObjects.at(i)->PerformancePersistence(false, true);

				memcpy(&buffer[index], &m_gameObjects.at(i)->GetPersistentBuffer()->at(0), size);

				index += size;
			}
		}

		std::ofstream outFile(nameFile, std::ofstream::binary);

		outFile.write(&buffer[0], buffer.size());
		outFile.close();
	}
	else
	{
		DestroyAllObjects();

		std::ifstream inFile(nameFile, std::ofstream::binary);

		std::vector<char> buffer;

		inFile.seekg(0, std::ios_base::end);
		int size = (int)inFile.tellg();
		inFile.seekg(0, std::ios_base::beg);

		buffer.resize(size);

		inFile.read(&buffer[0], size);
		inFile.close();

		UINT numberObjects;
		ULONG index = 0;
		int id;
		int regId;
		size = 0;
		memcpy(&numberObjects, &buffer[index], sizeof(numberObjects));
		index += sizeof(numberObjects);

		m_gameObjects.resize(numberObjects, NULL);

		for (int i = 0; i < numberObjects; ++i)
		{
			memcpy(&size, &buffer[index], sizeof(size));
			index += sizeof(int);

			if (size != 0)
			{
				memcpy(&regId, &buffer[index], sizeof(regId));
				index += sizeof(regId);

				/*id = CreateObject(regId);

				m_gameObjects[id]->SetIdInRegistr(regId);
				m_gameObjects[id]->SetSizeOfPersistenceBuffer(size);
				m_gameObjects[id]->SetPersistentBuffer(&buffer, index, size);
				m_gameObjects[id]->PerformancePersistence(false, false);*/
				index += size;
			}
		}
	}
}

void GameObjectsStackManager::CalcOffset(XMINT2 inputCC, XMINT2 inputBC, int lenght, XMINT2 offset, XMINT2 *outputCC, XMINT2 *outputBC)
{
	*outputCC = inputCC;
	outputCC->x = offset.x;
	outputCC->y = offset.y;
	*outputBC = inputBC;

	if (outputCC->x >= lenght)
	{
		outputCC->x -= lenght;
		outputBC->x += 1;
	}
	else if (outputCC->x < 0)
	{
		outputCC->x = lenght + outputCC->x;
		outputBC->x -= 1;
	}

	if (outputCC->y >= lenght)
	{
		outputCC->y -= lenght;
		outputBC->y += 1;
	}
	else if (outputCC->y < 0)
	{
		outputCC->y = lenght + outputCC->y;
		outputBC->y -= 1;
	}
}

void GameObjectsStackManager::PerformInteractions(int id)
{
}

void GameObjectsStackManager::InteractionsOfTwoObject(int id1, int id2)
{
	if (id1 != id2)
	{
	}
}
USING GameObjectsStackManager::GenerateContact()
{
	USING limit = m_maxContact;
	/*ObjectContact * nextContact = m_contact;

	for (ContactGenerators::iterator g = m_contactGenerators.begin(); g != m_contactGenerators.end(); g++)
	{
		USING used = g->AddContact(nextContact, limit);
		limit -= used;
		nextContact += used;

		if (limit <= 0)
		{
			break;
		}
	}*/

	return m_maxContact - limit;
}