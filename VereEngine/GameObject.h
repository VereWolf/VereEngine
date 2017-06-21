
#ifndef GAME_OBJECT
#define GAME_OBJECT

#include "pch.h"
#include "GameBaseObject.h"
#include "GameBaseObjectsStack.h"
#include "GameTexture.h"
#include "GameEffect.h"
#include "GameVertex.h"
#include "GameMeshBuffer.h"
#include "DeviceResources.h"
#include "GameResourceObjectsStack.h"
#include "RenderDevice.h"
#include <ctime>
class GameObjectsStackManager;

class GameObject : public GameBaseObject
{
public:
	GameObject()
	{
		m_master = NULL;
		m_RenderId = -1;
	}
	/*GameObject(const btRigidBodyConstructionInfo& constructionInfo)
	{

	}*/

	GameObject(GameObject& other)
	{

	}
	virtual ~GameObject()
	{

	}
	void InitialTime();
	void PreInit(DX::DeviceResources *resources, btVector3 position, btVector3 angle,
		btVector3 spacing, btScalar mass, btVector3 localInertia, btScalar rangeForFullProcces, btCollisionShape *collisionShape);
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render(btTransform camOffset, XMMATRIX camView, XMMATRIX camProj,
		float camFarZ, btScalar heightFar, btScalar aspect,
		float camFarRangeMod, float camModifier) = 0;
	void Integrate();

	virtual GameObject* Clone() = 0;
	double Tick();
	double GetDeltaTime() { return m_DeltaTime; }

	int GetRenderId() { return m_RenderId; }

	GameObject *GetMaster() { return m_master; }
	void SetMaster(GameObject *M) { m_master = M; }

	//Persistence for saving data on disk
	void PerformancePersistence(bool onlyGetSize, bool isSaveMod);

	void SetLocalTransform(btTransform M) { m_rigidBody->getWorldTransform() = M; }
	btTransform GetLocalTransform() { return m_rigidBody->getWorldTransform(); }
	virtual btTransform GetWorldTransform();

	void  SetLocalPosition(btVector3 P) { m_rigidBody->setWorldTransform(btTransform(m_rigidBody->getWorldTransform().getBasis(), P)); }
	btVector3  GetWorldPosition() { return GetWorldTransform().getOrigin(); }
	btVector3  GetLocalPosition() { return GetLocalTransform().getOrigin(); }

	void  SetLocalAngle(btVector3 A) { GetLocalTransform().getBasis().setEulerZYX(A.getZ(), A.getY(), A.getX()); }
	btVector3  GetLocalAngle();
	void Rotation(btVector3 axis, btScalar angle);
	void Circuit(btVector3 axis, btScalar angle);
	void SetScaling(btMatrix3x3 scaling) { m_scaling = scaling; }
	void SetScaling(btVector3 scaling) { m_scaling.setValue(scaling.getX(), 0, 0, 0, scaling.getY(), 0, 0, 0, scaling.getZ()); }
	void SetScaling(btScalar scaling) { m_scaling.setValue(scaling, 0, 0, 0, scaling, 0, 0, 0, scaling); }
	btMatrix3x3 GetScaling() { return m_scaling; }

	void SetRangeForFullProcess(btScalar r) { m_rangeForFullProcess = r; }
	btScalar GetRangeForFullProcess() { return m_rangeForFullProcess; }

protected:
	btMatrix3x3 m_scaling;

	GameObject *m_master;

	btRigidBody *m_rigidBody;

	int m_RenderId;

	btScalar m_rangeForFullProcess;

	double  m_SecondsPerCount;
	double  m_DeltaTime;
	double m_TimePerFrame;

	bool m_isFrame;
	__int64 m_PrevTime;
	__int64 m_CurrTime;

};

#endif //GAME_OBJECT