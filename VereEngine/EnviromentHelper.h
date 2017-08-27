#pragma once

#include "pch.h"
#include "LightHelper.h"


struct Type
{
	Type() { ZeroMemory(this, sizeof(this)); }
	void SetMat(XMFLOAT4 itsAmbient, XMFLOAT4 itsDiffuse, XMFLOAT4 itsSpecular, XMFLOAT4 itsReflect, float itsFractaConst, float itsHorizontalFractaConst)
	{
		mat.Ambient = itsAmbient;
		mat.Diffuse = itsDiffuse;
		mat.Specular = itsSpecular;
		mat.Reflect = itsReflect;
		fractalConst = itsFractaConst;
		horizontalFractalConst = itsHorizontalFractaConst;
	}
	Material mat;
	float fractalConst;
	float horizontalFractalConst;
};

struct EnviromentalType
{
	EnviromentalType() { ZeroMemory(this, sizeof(this)); }
	void SetEnviromental(UINT base, UINT rock, float maxAngle, float minAngle)
	{
		mBase = base;
		mRock = rock;
		mMaxAngleForRock = maxAngle;
		mMinAngleForRock = minAngle;
	}
	UINT mBase;
	UINT mRock;
	float mMaxAngleForRock;
	float mMinAngleForRock;
};