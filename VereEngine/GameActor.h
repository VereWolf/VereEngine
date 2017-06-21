/*#ifndef GAME_ACTOR_H
#define GAME_ACTOR_H

#include "pch.h"
#include "GameObject.h"
#include "GameObjectsStackManager.h"
#include "StaticMesh.h"
#include "BodyMesh.h"

class GameActor : public GameObject
{
public:
	GameActor()
	{
		m_id = -1;
		//m_idInCell = -1;
		m_position = XMDOUBLE3(0.0, 0.0, 0.0);
		m_angle = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_scaling = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_acceleration = XMDOUBLE3(0.0, 0.0, 0.0);
		m_velocity = XMDOUBLE3(0.0, 0.0, 0.0);
		m_mass = 0.0;
		m_inverseMass = -1.0f;
		m_radiusOfCircleBox = 0.0f;
		m_damping = 1.0;
		ClearAccumulator();
		GameObject::InitialTime();
	}

	GameActor(GameActor & other)
	{
		m_id = -1;
		m_position = other.GetPosition();
		m_angle = other.GetAngle();
		m_scaling = other.GetScaling();
		m_acceleration = other.GetAcceleration();
		m_velocity = other.GetVelocity();
		m_mass = other.GetMass();
		m_inverseMass = other.GetInverseMass();
		m_radiusOfCircleBox = other.GetRadiusOfCircleBox();
		m_damping = other.GetDamping();
		m_forceAccum = other.m_forceAccum;
		m_torqueAccum = other.m_torqueAccum;
		GameObject::InitialTime();
	}

	GameObject *Clone() { return new GameActor(*this); }

	void Init()
	{
		m_position = XMDOUBLE3(0.0, 0.0, 0.0);

		m_up = XMDOUBLE3(1.0, 0.0, 0.0);
		m_right = XMDOUBLE3(0.0, 0.0, 1.0);
	}

	void Update()
	{

		if (GetAsyncKeyState(VK_UP) & 0x8000)
			Walk(100.0f*m_DeltaTime);

		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			Walk(-100.0f*m_DeltaTime);

		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			Strafe(-100.0f*m_DeltaTime);

		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			Strafe(100.0f*m_DeltaTime);
	}

	void Render()
	{
		RenderStaticMesh(0);
	}

	void Walk(double speed)
	{
		m_position = m_position + m_up * speed;
	}

	void Strafe(double speed)
	{
		m_position = m_position + m_right * speed;
	}

private:
	XMDOUBLE3 m_up;
	XMDOUBLE3 m_right;
	XMDOUBLE3 m_top;
};

#endif //GAME_ACTOR_H*/