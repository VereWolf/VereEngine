#pragma once

#include "pch.h"
#include "GameObject.h"

class Camera : public GameObject
{
public:
	Camera();
	Camera(Camera & other);
	~Camera()
	{
	}

	void Init();
	void Render(btTransform camOffset, XMMATRIX camView, XMMATRIX camProj,
		float camFarZ, btScalar heightFar, btScalar aspect,
		float camFarRangeMod, float camModifier);
	void Update();

	GameObject* Clone() { return new Camera(*this); }

	void SetMainCamera(bool index) { mMainCamera = index; }
	bool GetMainCamera() { return mMainCamera; }

	//btTransform GetWorldTransform();

	btVector3 GetRight();
	btVector3 GetUp();
	btVector3 GetLook();

	void SetUp(btVector3 Up);

	btScalar GetSpeed();
	void SetSpeed(btScalar itsSpeed);

	float GetNearZ();
	float GetFarZ();
	float GetFarRangeMod() { return mFarZ / (mFarZ - mNearZ); }
	float GetAspect();
	float GetFovY();
	float GetFovX();
	float GetFarModifier();
	float GetFarChange();

	float GetNearWindowWidth();
	float GetNearWindowHeight();
	float GetFarWindowWidth();
	float GetFarWindowHeight();

	void SetLens(float fovY, float aspect, float zn, float zf, float mod);

	void LookAt( btVector3 pos, btVector3 target, btVector3 worldUp);

	XMMATRIX Offset();
	btTransform btOffset();
	XMMATRIX View();
	btTransform btView();
	XMMATRIX Proj();
	XMMATRIX ViewProj();
	XMMATRIX InvViewProj();
	XMMATRIX Zero();

	void Strafe(btScalar d);
	void FordWard(btScalar d);
	void Walk(btScalar d);

	void Pitch(btScalar angle);
	void RotateY(btScalar angle);
	

private:
	btVector3 mRight;
	btVector3 mFord;
	btVector3 mUp;
	btVector3 mLook;

	btScalar mSpeed;

	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;
	float mFarModifier;
	float mNearWindowHeight;
	float mFarWindowHeight;
	float mModifier;

	btTransform mOffset;
	btTransform mView;
	XMFLOAT4X4 mProj;

	bool mMainCamera;
};