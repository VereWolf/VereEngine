#pragma once

#include "pch.h"

#define PI 3.14159265359

struct VereMath
{
	inline static XMFLOAT3 ConvertToXMFLOAT3(btVector3 v) { return XMFLOAT3(v.getX(), v.getY(), v.getZ()); }
	inline static XMFLOAT3X3 ConvertToXMFLOAT3X3(btMatrix3x3 m)
	{
		return XMFLOAT3X3(
			m.getRow(0).getX(), m.getRow(1).getX(), m.getRow(2).getX(),
			m.getRow(0).getY(), m.getRow(1).getY(), m.getRow(2).getY(),
			m.getRow(0).getZ(), m.getRow(21).getZ(), m.getRow(2).getZ());
	}
	inline static XMFLOAT4X4 ConvertToXMFLOAT4X4(btTransform t)
	{
		return XMFLOAT4X4(
			t.getBasis().getRow(0).getX(), t.getBasis().getRow(1).getX(), t.getBasis().getRow(2).getX(), 0.0,
			t.getBasis().getRow(0).getY(), t.getBasis().getRow(1).getY(), t.getBasis().getRow(2).getY(), 0.0,
			t.getBasis().getRow(0).getZ(), t.getBasis().getRow(1).getZ(), t.getBasis().getRow(2).getZ(), 0.0,
			t.getOrigin().getX(), t.getOrigin().getY(), t.getOrigin().getZ(), 1.0);
	}

	inline static btVector3 ConvertTobtVector3(XMFLOAT3 v) { return btVector3(v.x, v.z, v.y); }
	inline static btMatrix3x3 ConvertTobtMatrix3x3(XMFLOAT3X3 m)
	{
		return btMatrix3x3(
			m._11, m._21, m._31,
			m._12, m._22, m._32,
			m._13, m._23, m._33);
	}
	inline static btTransform ConvertTobtTransform(XMFLOAT4X4 t)
	{
		return btTransform(btMatrix3x3(
			t._11, t._21, t._31,
			t._12, t._22, t._32,
			t._13, t._23, t._33),
			btVector3(t._41, t._42, t._43));
	}

	inline static int Min(int a, int b)
	{
		if (a < b)
		{
			return a;
		}
		return b;
	}

	inline static int Max(int a, int b)
	{
		if (a > b)
		{
			return a;
		}
		return b;
	}

	inline static btVector3 MinVector(btVector3 a, btVector3 b)
	{
		btVector3 c;

		if (a.getX() < b.getX())
		{
			c.setX(a.getX());
		}
		else
		{
			c.setX(b.getX());
		}
		if (a.getY() < b.getY())
		{
			c.setY(a.getY());
		}
		else
		{
			c.setY(b.getY());
		}
		if (a.getZ() < b.getZ())
		{
			c.setZ(a.getZ());
		}
		else
		{
			c.setZ(b.getZ());
		}

		return c;
	}

	inline static btVector3 MaxVector(btVector3 a, btVector3 b)
	{
		btVector3 c;

		if (a.getX() > b.getX())
		{
			c.setX(a.getX());
		}
		else
		{
			c.setX(b.getX());
		}
		if (a.getY() > b.getY())
		{
			c.setY(a.getY());
		}
		else
		{
			c.setY(b.getY());
		}
		if (a.getZ() > b.getZ())
		{
			c.setZ(a.getZ());
		}
		else
		{
			c.setZ(b.getZ());
		}

		return c;
	}

	inline static int Clamp(int i, int min,int max)
	{
		if (i < min)
		{
			return min;
		}
		else if (i > max)
		{
			return max;
		}

		return i;
	}


	/*inline static bool FrustrumCulling(btVector3 extendBox, btVector3 centerBox, btScalar farY, btScalar heightFar, btScalar aspect, btTransform transform)
	{
		btVector3 box[] =
		{
			btVector3(centerBox.getX() - centerBox.getX(),centerBox.getY() - centerBox.getY(),centerBox.getZ() - centerBox.getZ()) * transform.getBasis() + transform.getOrigin(),
			btVector3(centerBox.getX() + centerBox.getX(),centerBox.getY() - centerBox.getY(),centerBox.getZ() - centerBox.getZ()) * transform.getBasis() + transform.getOrigin(),
			btVector3(centerBox.getX() + centerBox.getX(),centerBox.getY() + centerBox.getY(),centerBox.getZ() - centerBox.getZ()) * transform.getBasis() + transform.getOrigin(),
			btVector3(centerBox.getX() - centerBox.getX(),centerBox.getY() + centerBox.getY(),centerBox.getZ() - centerBox.getZ()) * transform.getBasis() + transform.getOrigin(),
			btVector3(centerBox.getX() - centerBox.getX(),centerBox.getY() - centerBox.getY(),centerBox.getZ() + centerBox.getZ()) * transform.getBasis() + transform.getOrigin(),
			btVector3(centerBox.getX() + centerBox.getX(),centerBox.getY() - centerBox.getY(),centerBox.getZ() + centerBox.getZ()) * transform.getBasis() + transform.getOrigin(),
			btVector3(centerBox.getX() + centerBox.getX(),centerBox.getY() + centerBox.getY(),centerBox.getZ() + centerBox.getZ()) * transform.getBasis() + transform.getOrigin(),
			btVector3(centerBox.getX() - centerBox.getX(),centerBox.getY() + centerBox.getY(),centerBox.getZ() + centerBox.getZ()) * transform.getBasis() + transform.getOrigin()
		};

		btVector3 right = btVector3(1.0, 0.0, 0.0);
		btVector3 up = btVector3(0.0, 1.0, 0.0);

		btVector3 slopeR = btVector3(heightFar * aspect, 0.0, farY).normalize();
		btVector3 slopeL = btVector3(-heightFar * aspect, 0.0, farY).normalize();
		btVector3 slopeT = btVector3(0.0, heightFar, farY).normalize();
		btVector3 slopeB = btVector3(0.0, -heightFar, farY).normalize();

		btScalar cosR = slopeR.dot(right);
		btScalar cosL = slopeL.dot(right);
		btScalar cosT = slopeT.dot(up);
		btScalar cosB = slopeB.dot(up);

		return FrustrumCulling(box, cosR, cosL, cosT, cosB, 0);
	}*/
	
	static bool FrustrumCulling2(btVector3 center, btVector3 offset, btScalar farY, btScalar heightFar, btScalar aspect, btTransform transform)
	{
		heightFar *= 0.5;
		btVector3 min = (transform * (center - offset)).normalize();
		btVector3 max = (transform * (center + offset)).normalize();

		btVector3 right = btVector3(1.0, 0.0, 0.0);
		btVector3 up = btVector3(0.0, 1.0, 0.0);

		btVector3 slopeR = btVector3(heightFar * aspect, 0.0, farY).normalize();
		btVector3 slopeL = btVector3(-heightFar * aspect, 0.0, farY).normalize();
		btVector3 slopeT = btVector3(0.0, heightFar, farY).normalize();
		btVector3 slopeB = btVector3(0.0, -heightFar, farY).normalize();

		btScalar cosR = slopeR.dot(right);
		btScalar cosL = slopeL.dot(right);
		btScalar cosT = slopeT.dot(up);
		btScalar cosB = slopeB.dot(up);

		return FrustrumCulling(min, max, cosR, cosL, cosT, cosB, 0);
	}


	inline static bool FrustrumCulling(btVector3 min, btVector3 max, btScalar cosR, btScalar cosL, btScalar cosT, btScalar cosB, UINT level)
	{
		btVector3 right = btVector3(1.0, 0.0, 0.0);
		btVector3 up = btVector3(0.0, 1.0, 0.0);
		btVector3 front = btVector3(0.0, 0.0, 1.0);

		btVector3 P1;
		btVector3 P2;
		float D1;
		float D2;
		float D3;
		int B1 = 1;
		int B2 = 1;
		bool B3 = false;

		btVector3 box[8];

		box[0].setX(min.getX());
		box[0].setY(min.getY());
		box[0].setZ(min.getZ());
		box[1].setX(max.getX());
		box[1].setY(min.getY());
		box[1].setZ(min.getZ());
		box[2].setX(min.getX());
		box[2].setY(max.getY());
		box[2].setZ(min.getZ());
		box[3].setX(max.getX());
		box[3].setY(max.getY());
		box[3].setZ(min.getZ());
		box[4].setX(min.getX());
		box[4].setY(min.getY());
		box[4].setZ(max.getZ());
		box[5].setX(max.getX());
		box[5].setY(min.getY());
		box[5].setZ(max.getZ());
		box[6].setX(min.getX());
		box[6].setY(max.getY());
		box[6].setZ(max.getZ());
		box[7].setX(max.getX());
		box[7].setY(max.getY());
		box[7].setZ(max.getZ());

		for (int i = 0; i < 8; ++i)
		{
			P1.setX(box[i].getX());
			P1.setY(0.0);
			P1.setZ(box[i].getZ());

			D1 = P1.dot(right);

			P2.setX(0.0);
			P2.setY(box[i].getY());
			P2.setZ(box[i].getZ());

			D2 = P2.dot(up);

			D3 = box[i].normalize().dot(front);

			if (D3 > 0)
			{
				B3 = true;
			}

			if (D1 <= cosR && D1 >= cosL && D2 <= cosT && D2 >= cosB)
			{
				return true;
			}

			if (D1 <= cosR && D1 >= cosL)
			{
				B1 = 0;
			}

			if (D2 <= cosT && D2 >= cosB)
			{
				B2 = 0;
			}

			if (i == 0)
			{
				if (B1 != 0)
				{
					if (D1 > cosR)
					{
						B1 = 1;
					}
					else
					{
						B1 = -1;
					}
				}

				if (B2 != 0)
				{
					if (D2 > cosT)
					{
						B2 = 1;
					}
					else
					{
						B2 = -1;
					}
				}
			}
			else
			{
				if (D1 > cosR)
				{
					if (B1 == -1)
					{
						B1 = 0;
					}
				}
				else
				{
					if (B1 == 1)
					{
						B1 = 0;
					}
				}
				if (D2 > cosT)
				{
					if (B2 == -1)
					{
						B2 = 0;
					}
				}
				else
				{
					if (B2 == 1)
					{
						B2 = 0;
					}
				}
			}
		}

		if (B3 == false)
		{
			return false;
		}

		if ((B1 == 0 || B2 == 0) && level < 2)
		{
			return true;
			/*btVector3 box2[8];
			B3 = false;

			for (int j = 0; j < 8; ++j)
			{
				for (int i = 0; i < 8; ++i)
				{
					box2[i] = 0.5 * box[i] + 0.5 * box[j];
				}

				btVector3 min2 = box[0];
				btVector3 max2 = box[7];

				B3 = FrustrumCulling(min2, max2, cosR, cosL, cosT, cosB, level + 1);

				if (B3 == true)
				{
					return true;
				}
			}*/
		}

		return false;
	}
};