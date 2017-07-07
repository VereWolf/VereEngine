#include "stdafx.h"
#include "VecMath.h"

INT2::INT2()
{
	x = 0.0f;
	y = 0.0f;
}

INT2::INT2(int itsX, int itsY)
{
	x = itsX;
	y = itsY;
}

Float2::Float2()
{
	x = 0.0f;
	y = 0.0f;
}

Float2::Float2(float itsX, float itsY)
{
	x = itsX;
	y = itsY;
}

Float3::Float3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Float3::Float3(float itsX, float itsY, float itsZ)
{
	x = itsX;
	y = itsY;
	z = itsZ;
}

Float4::Float4()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

Float4::Float4(float itsX, float itsY, float itsZ, float itsW)
{
	x = itsX;
	y = itsY;
	z = itsZ;
	w = itsW;
}

float VMath::Distance(Float2 v0, Float2 v1)
{
	return pow(pow(v0.x - v1.x, 2.0f) + pow(v0.y - v1.y, 2.0f), 0.5f);
}

float VMath::Distance(Float3 v0, Float3 v1)
{
	return pow(pow(v0.x - v1.x, 2.0f) + pow(v0.y - v1.y, 2.0f) + pow(v0.z - v1.z, 2.0f), 0.5f);
}

float VMath::Dot(Float2 v0, Float2 v1)
{
	return (v0.x * v1.x + v0.y * v1.y);
}

float VMath::Dot(Float3 v0, Float3 v1)
{
	return (v0.x * v1.x + v0.y * v1.y + v0.z * v1.z);
}

Float3 VMath::Cross(Float3 v0, Float3 v1)
{
	return Float3(v0.y * v1.z - v0.z * v1.y, v0.z * v1.x - v0.x * v1.z, v0.x * v1.y - v0.y * v1.z);
}

float VMath::Clamp(float i, float min, float max)
{
	if (i < min)
		i = min;

	if (i > max)
		i = max;

	return i;
}

Float2 VMath::Normalize(Float2 v)
{
	float d = pow(pow(v.x, 2.0f) + pow(v.y, 2.0f), 0.5f);

	if (d == 0.0f)
		return Float2(0.0f, 0.0f);

	return Float2(v.x / d, v.y / d);
}

Float3 VMath::Normalize(Float3 v)
{
	float d = pow(pow(v.x, 2.0f) + pow(v.y, 2.0f) + pow(v.z, 2.0f), 0.5f);

	if (d == 0.0f)
		return Float3(0.0f, 0.0f, 0.0f);

	return Float3(v.x / d, v.y / d, v.z / d);
}

Float3 VMath::Reflection(Float3 d, Float3 n)
{
	d = VMath::Normalize(d);
	d = Float3(-d.x, -d.y, -d.z);
	n = VMath::Normalize(n);
	float D = 2 * VMath::Dot(d, n);
	return Float3(d.x - D * n.x, d.y - D * n.y, d.z - D * n.z);
}

float VMath::Lerp(float s1, float s2, float i)
{
	return s1 * (1.0f - i) + s2 * i;
}

float VMath::Lerp2D(float s1, float s2, float s3, float s4, float i1, float i2)
{
	float t1 = Lerp(s1, s2, i1);
	float t2 = Lerp(s3, s4, i1);

	return Lerp(t1, t2, i2);
}