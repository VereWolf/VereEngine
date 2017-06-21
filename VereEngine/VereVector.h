#ifndef VEREVECTOR_H
#define VEREVECTOR_H

#include "pch.h"

class VereFloat3
{
public:
	VereFloat3();
	VereFloat3(float itsX, float itsY, float itsZ);

	void operator= (const XMFLOAT3 &v);
	void operator*= (const float &s);
	VereFloat3 operator* (const float &s);
	float operator* (const VereFloat3 &v);

	float Dot(VereFloat3 v);
	VereFloat3 Cross(VereFloat3 v);
	VereFloat3 Normalize();
	void AddScaledVector(VereFloat3 v, float s);
public:
	float x;
	float y;
	float z;
};

class VereDouble3
{
public:
	VereDouble3();
	VereDouble3(double itsX, double itsY, double itsZ);

	VereFloat3 operator= (const XMFLOAT3 &v);
	VereDouble3 operator*= (const double &s);
	VereDouble3 operator* (const double &s);
	double operator* (const VereDouble3 &v);

	double Dot(VereDouble3 v);
	VereDouble3 Cross(VereDouble3 v);
	VereDouble3 Normalize();
	void AddScaledVector(VereDouble3 v, double s);
public:
	double x;
	double y;
	double z;
};

#endif //VEREVECTOR_H