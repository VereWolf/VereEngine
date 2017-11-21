
#include "pch.h"
#include "DeviceResources.h"
#include "GameObject.h"
#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(Camera & other)
{
}

void Camera::Init()
{
	mFord = btVector3(0.0, 0.0, 1.0);
	mRight = btVector3(1.0, 0.0, 0.0);
	mUp = btVector3(0.0, 1.0, 0.0);
	mLook = btVector3(0.0, 0.0, 1.0);
	mSpeed = 0.1;
	SetLens(0.5f*PI, 1.78f, 0.2f, 1000000000.0f, 0.0001f);

}

void Camera::Render()
{
}

void Camera::Update()
{
	btVector3 U = GetLocalPosition().normalize();
	btVector3 L = mLook.normalize();
	btVector3 R = U.cross(L).normalize();
	U = L.cross(R);

	btMatrix3x3 N;

	SetLocalTransform(btTransform(btMatrix3x3(R.getX(), R.getY(), R.getZ(), U.getX(), U.getY(), U.getZ(), L.getX(), L.getY(), L.getZ()), GetLocalPosition()));

	mOffset.getBasis().setIdentity();
	mOffset.getOrigin() = -GetWorldPosition();

	mRight = R;
	mUp = U;
	mLook = L;

	btMatrix3x3 M = btMatrix3x3::getIdentity();

	if (m_master)
	{
		M = m_master->GetWorldTransform().getBasis();
	}

	R = M * R;
	L = M * L;
	U = M * U;

	mView = btTransform(btMatrix3x3(R.getX(), R.getY(), R.getZ(), U.getX(), U.getY(), U.getZ(), L.getX(), L.getY(), L.getZ()), btVector3(0.0, 0.0, 0.0));
};

btVector3 Camera::GetRight()
{
	return mRight;
}

btVector3 Camera::GetUp()
{
	return mUp;
}

btVector3 Camera::GetLook()
{
	return mLook;
}

void Camera::SetUp(btVector3 Up)
{
	mUp = Up.normalize();
	mFord = mUp.cross(mRight).normalize();
	mRight = mFord.cross(mUp).normalize();
}

 btScalar Camera::GetSpeed()
{
	return mSpeed;
}

void Camera::SetSpeed(btScalar itsSpeed)
{
	mSpeed = itsSpeed;
}

float Camera::GetNearZ()
{
	return mNearZ;
}

float Camera::GetFarZ()
{
	return mFarZ;
}

float Camera::GetFarModifier()
{
	return mFarModifier;
}

float Camera::GetAspect()
{
	return mAspect;
}

float Camera::GetFovY()
{
	return mFovY;
}

float Camera::GetFovX()
{
	float halfWidth = 0.5f*GetNearWindowWidth();
	return 2.0f*atan(halfWidth / mNearZ);
}

float Camera::GetNearWindowWidth()
{
	return mAspect * mNearWindowHeight;
}

float Camera::GetNearWindowHeight()
{
	return mNearWindowHeight;
}

float Camera::GetFarWindowWidth()
{
	return mAspect * mFarWindowHeight;
}

float Camera::GetFarWindowHeight()
{
	return mFarWindowHeight;
}

float Camera::GetFarChange()
{
	return 0.5f * (mFarWindowHeight - mNearWindowHeight) / (mFarZ - mNearZ);
}

void Camera::SetLens(float fovY, float aspect, float zn, float zf, float mod)
{
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = log(mod * zn + 1.0f) / log(mod * zf + 1.0f) * zf;
	mFarZ = zf;
	mFarModifier = mod;


	mNearWindowHeight = 2.0f * mNearZ * tanf( 0.5f*mFovY );
	mFarWindowHeight  = 2.0f * mFarZ * tanf( 0.5f*mFovY );

	XMMATRIX P = XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	XMStoreFloat4x4(&mProj, P);

	RenderMessage::m_HeightFar = mFarWindowHeight;
	RenderMessage::m_Aspect = aspect;
	RenderMessage::m_FarModifier = mod;
	RenderMessage::m_FarZ = zf;
}

void Camera::LookAt( btVector3 pos, btVector3 target, btVector3 worldUp)
{

	btVector3 L = (target - pos).normalize();
	btVector3 R = worldUp.cross(L).normalize();
	btVector3 U = L.cross(R).normalize();

	SetLocalPosition(pos);
	mLook = L;
	mRight = R;
	mUp = U;
}

XMMATRIX Camera::Offset()
{
	return XMLoadFloat4x4(&VereMath::ConvertToXMFLOAT4X4(mOffset));
}

btTransform Camera::btOffset()
{
	return mOffset;
}

XMMATRIX Camera::View()
{
	return XMLoadFloat4x4(&VereMath::ConvertToXMFLOAT4X4(mView));
}

btTransform Camera::btView()
{
	return btTransform(mView);
}

XMMATRIX Camera::Proj()
{
	return XMLoadFloat4x4(&mProj);
}

XMMATRIX Camera::ViewProj()
{
	return View() * Proj();
}

XMMATRIX Camera::InvViewProj()
{
	XMMATRIX M = ViewProj();

	return XMMatrixInverse(&XMMatrixDeterminant(M), M);
}

XMMATRIX Camera::Zero()
{
	return XMMatrixSet(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

void Camera::FordWard(btScalar d)
{
	SetLocalPosition(GetLocalPosition() + mFord * d * mSpeed);
}

void Camera::Strafe(btScalar d)
{
	SetLocalPosition(GetLocalPosition() + mRight * d * mSpeed);
}

void Camera::Walk(btScalar d)
{
	SetLocalPosition(GetLocalPosition() + mLook * d * mSpeed);
}

void Camera::Pitch(btScalar angle)
{
	btQuaternion Q;
	Q.setRotation(mRight, angle);
	btMatrix3x3 R;
	R.setIdentity();
	R.setRotation(Q);

	mRight = (mRight * R).normalize();
	mFord = (mFord * R).normalize();
	mUp = (mUp * R).normalize();
	mLook = (mLook * R).normalize();
}

void Camera::RotateY(btScalar angle)
{
	btQuaternion Q;
	Q.setRotation(mUp, angle);
	btMatrix3x3 R;
	R.setIdentity();
	R.setRotation(Q);

	mRight = (mRight * R).normalize();
	mFord = (mFord * R).normalize();
	mUp = (mUp * R).normalize();
	mLook = (mLook * R).normalize();
}


