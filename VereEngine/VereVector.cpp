#include "pch.h"
#include "VereVector.h"

VereFloat3::VereFloat3()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

VereFloat3::VereFloat3(float itsX, float itsY, float itsZ)
{
	x = itsX;
	y = itsY;
	z = itsZ;
}

void VereFloat3::operator= (const XMFLOAT3 &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

void VereFloat3::operator *= (const float &s)
{
	x = x*s;
	y = y*s;
	z = z*s;
}

VereFloat3 VereFloat3::operator* (const float &s)
{
	return VereFloat3(x*s, y*s, z*s);
}

float VereFloat3::operator* (const VereFloat3 &v)
{
	return (x*v.x + y*v.y + z*v.z);
}

float VereFloat3::Dot(VereFloat3 v)
{
	return x*v.x + y*v.y + z*v.z;
}

VereFloat3 VereFloat3::Cross(VereFloat3 v)
{
	return VereFloat3(
		y*v.z - z*v.y,
		z*v.x - x*v.z,
		x*v.y - y*v.x
	);
}

VereFloat3 VereFloat3::Normalize()
{
	float d = 1.0 / pow(x*x + y*y + z*z, 0.5);

	return VereFloat3(x*d, y*d, z*d);
}

void VereFloat3::AddScaledVector(VereFloat3 v, float s)
{
	x += v.x*s;
	y += v.y*s;
	z += v.z*s;
}

VereDouble3::VereDouble3()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

VereDouble3::VereDouble3(double itsX, double itsY, double itsZ)
{
	x = itsX;
	y = itsY;
	z = itsZ;
}

VereFloat3 VereDouble3::operator = (const XMFLOAT3 &v)
{
	return VereFloat3(v.x, v.y, v.z);
}

VereDouble3 VereDouble3::operator *= (const double &s)
{
	return VereDouble3(x*s, y*s, z*s);
}

VereDouble3 VereDouble3::operator * (const double &s)
{
	return VereDouble3(x*s, y*s, z*s);
}

double VereDouble3::operator * (const VereDouble3 &v)
{
	return (x*v.x + y*v.y + z*v.z);
}

double VereDouble3::Dot(VereDouble3 v)
{
	return x*v.x + y*v.y + z*v.z;
}

VereDouble3 VereDouble3::Cross(VereDouble3 v)
{
	return VereDouble3(
		y*v.z - z*v.y,
		z*v.x - x*v.z,
		x*v.y - y*v.x
	);
}

VereDouble3 VereDouble3::Normalize()
{
	double d = 1.0 / pow(x*x + y*y + z*z, 0.5);

	return VereDouble3(x*d, y*d, z*d);
}

void VereDouble3::AddScaledVector(VereDouble3 v, double s)
{
	x += v.x*s;
	y += v.y*s;
	z += v.z*s;
}