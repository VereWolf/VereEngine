#include "pch.h"
#include "DeviceResources.h"
#include "GameObject.h"
#include "GameResourceObjectsStack.h"
#include "RenderDevice.h"
#include <ctime>

/*GameObject::GameObject()
{
}*/

/*GameObject::GameObject(GameObject& other)
{
}*/

void GameObject::InitialTime()
{
	m_SecondsPerCount = 0.0;
	m_DeltaTime = -1.0;
	m_PrevTime = 0;
	m_CurrTime = 0;

	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	m_SecondsPerCount = 1.0 / (double)countsPerSec;
}

void GameObject::PreInit(DX::DeviceResources *resources,
	btVector3 position = btVector3(0.0, 0.0, 0.0), btVector3 angle = btVector3(0.0, 0.0, 0.0), btVector3 spacing = btVector3(0.0, 0.0, 0.0),
	btScalar mass = 0.0, btVector3 localInertia = btVector3(0.0, 0.0, 0.0), btScalar rangeForFullProcces = 0, btCollisionShape *collisionShape = NULL)
{
	GameBaseObject::PreInit(resources);

	btTransform T;
	T.setIdentity();
	T.setOrigin(position);
	T.getBasis().setEulerZYX(angle.getX(), angle.getY(), angle.getZ());
	SetScaling(spacing);

	m_rangeForFullProcess = rangeForFullProcces;

	btDefaultMotionState *M = new btDefaultMotionState(T);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, M, collisionShape, localInertia);

	m_rigidBody = new btRigidBody(rbInfo);

	m_isDestroy = false;
	InitialTime();
	Tick();
}

double GameObject::Tick()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	m_CurrTime = currTime;
	m_DeltaTime = (m_CurrTime - m_PrevTime) * m_SecondsPerCount;

	m_PrevTime = m_CurrTime;

	if (m_DeltaTime < 0.0)
	{
		m_DeltaTime = 0.0;
	}

	return m_DeltaTime;
}

void GameObject::PerformancePersistence(bool onlyGetSize, bool isSaveMod)
{
	GameBaseObject::PerformancePersistence(onlyGetSize, isSaveMod);
}

void GameObject::Integrate()
{
	/*btScalar duration = m_DeltaTime;

	if (m_inverseMass < 0.0)
	{
		return;
	}

	if (duration <= 0.0)
	{
		return;
	}

	m_position = m_position + m_velocity * duration;

	btVector3 resultAcceleration = m_acceleration;
	resultAcceleration = resultAcceleration + m_forceAccum * m_inverseMass;

	m_velocity = m_velocity + resultAcceleration * duration;

	m_velocity = m_velocity * pow(m_damping, duration);

	/*XMVECTOR TA = XMLoadFloat3(&m_torqueAccum);
	XMMATRIX IITW = XMLoadFloat3x3(&m_inverseInertiaTensorWorld);
	XMVECTOR R = XMLoadFloat3(&m_rotation);
	//XMVECTOR RAA = (R + XMVector2Transform(TA, IITW) * duration) * pow(m_damping, duration);

	m_rotation = (m_rotation + (m_torqueAccum * m_inverseInertiaTensorWorld.getRotation()).);

	XMStoreFloat3(&m_rotation, RAA);*/

	/*m_position = m_position + m_velocity;

	ClearAccumulator();*/
}

btTransform GameObject::GetWorldTransform()
{
	if (m_master == NULL)
	{
		return m_rigidBody->getWorldTransform();
	}

	return(m_master->GetWorldTransform() * m_rigidBody->getWorldTransform());
}

btVector3  GameObject::GetLocalAngle()
{
	btScalar x, y, z;
	GetLocalTransform().getBasis().getEulerZYX(z, y, x);
	return btVector3(x, y, z);
}

void GameObject::Rotation(btVector3 axis, btScalar angle)
{
	m_rigidBody->getWorldTransform().setBasis(m_rigidBody->getWorldTransform().getBasis() * btMatrix3x3(btQuaternion(axis, angle)));
}

void GameObject::Circuit(btVector3 axis, btScalar angle)
{
	btTransform C = btTransform(btMatrix3x3(btQuaternion(axis, angle))) * m_rigidBody->getWorldTransform();
	m_rigidBody->getWorldTransform().setOrigin(C.getOrigin());
}