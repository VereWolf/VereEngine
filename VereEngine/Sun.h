#pragma once

#include "pch.h"

/*class Sun
{
public:
	Sun()
	{
		m_baseVector = XMFLOAT3(1.0f, 0.0f, 0.0f);
	}
	XMFLOAT3 GetNVectorOfSun(float time)
	{
		SetTimeAtDay(time);

		XMMATRIX D = XMLoadFloat3x3(&m_timeAtDayMatrix);
		XMMATRIX T = XMLoadFloat3x3(&m_turnMatrix);
		XMMATRIX I = XMLoadFloat3x3(&m_inclineMatrix);
		XMVECTOR B = XMLoadFloat3(&m_baseVector);

		XMVECTOR V = XMVector3Normalize(XMVector3Transform(B, (T*I*D)));

		XMFLOAT3 F;
		XMStoreFloat3(&F, V);

		return F;
	}

	XMFLOAT3 GetTurnVector()
	{
		XMMATRIX T = XMLoadFloat3x3(&m_turnMatrix);
		XMVECTOR B = XMLoadFloat3(&m_baseVector);

		XMVECTOR V = XMVector3Normalize(XMVector3Transform(B, (T)));

		XMFLOAT3 F;
		XMStoreFloat3(&F, V);

		return F;
	}

	void SetTimeAtDay(float a)
	{
		a *= 2.0f * PI;
		m_timeAtDayMatrix._11 = cos(a);
		m_timeAtDayMatrix._12 = -sin(a);
		m_timeAtDayMatrix._13 = 0.0f;
		m_timeAtDayMatrix._21 = -m_turnMatrix._12;
		m_timeAtDayMatrix._22 = m_turnMatrix._11;
		m_timeAtDayMatrix._23 = 0.0f;
		m_timeAtDayMatrix._31 = 0.0f;
		m_timeAtDayMatrix._32 = 0.0f;
		m_timeAtDayMatrix._33 = 1.0f;
	}

	void SetTurnMatrix(float a)
	{
		a *= 2.0f * PI;
		m_turnMatrix._11 = cos(a);
		m_turnMatrix._12 = 0.0f;
		m_turnMatrix._13 = sin(a);
		m_turnMatrix._21 = 0.0f;
		m_turnMatrix._22 = 1.0f;
		m_turnMatrix._23 = 0.0f;
		m_turnMatrix._31 = -m_turnMatrix._13;
		m_turnMatrix._32 = 0.0f;
		m_turnMatrix._33 = m_turnMatrix._11;
	}

	void SetInclineMatrix(float a)
	{
		a *= 2.0f * PI;
		m_inclineMatrix._11 = 1.0f;
		m_inclineMatrix._12 = 0.0f;
		m_inclineMatrix._13 = 0.0f;
		m_inclineMatrix._21 = 0.0f;
		m_inclineMatrix._22 = cos(a);
		m_inclineMatrix._23 = -sin(a);
		m_inclineMatrix._31 = 0.0f;
		m_inclineMatrix._32 = -m_turnMatrix._23;
		m_inclineMatrix._33 = m_turnMatrix._22;
	}

private:
	XMFLOAT3X3 m_turnMatrix;
	XMFLOAT3X3 m_inclineMatrix;
	XMFLOAT3X3 m_timeAtDayMatrix;

	XMFLOAT3 m_baseVector;
};*/