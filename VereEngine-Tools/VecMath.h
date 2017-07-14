#pragma once

#include <math.h>

struct INT2
{
	INT2();
	INT2(int itsX, int itsY);

	int x;
	int y;
};

struct Float2
{
	Float2();
	Float2(float itsX, float itsY);

	float x;
	float y;
};

struct Float3
{
	Float3();
	Float3(float itsX, float itsY, float itsZ);

	float x;
	float y;
	float z;
};

struct Float4
{
	Float4();
	Float4(float itsX, float itsY, float itsZ, float itsW);

	float x;
	float y;
	float z;
	float w;
};

class VMath
{
public:
	static float Distance(Float2 v0, Float2 v1);
	static float Distance(Float3 v0, Float3 v1);

	static float Dot(Float2 v0, Float2 v1);
	static float Dot(Float3 v0, Float3 v1);
	static Float3 Cross(Float3 v0, Float3 v1);
	static float Clamp(float i, float min, float max);
	static Float2 Normalize(Float2 v);
	static Float3 Normalize(Float3 v);
	static Float3 Reflection(Float3 d, Float3 n);
	static float Lerp(float s1, float s2, float i);
	static float Lerp2D(float s1, float s2, float s3, float s4, float i1, float i2);
};