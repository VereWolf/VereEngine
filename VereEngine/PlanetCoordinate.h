#pragma once

#include "pch.h"

enum cubeside
{
	e_cubeTop,
	e_cubeBottom,
	e_cubeRight,
	e_cubeLeft,
	e_cubeFront,
	e_cubeBack
};

class PlanetCordinateMat
{
public:
	inline static int GetSide(btVector3 dir, btVector3 up = btVector3(0.0, 1.0, 0.0), btVector3 right = btVector3(1.0, 0.0, 0.0), btVector3 front = btVector3(0.0, 0.0, 1.0))
	{
		btVector3 R = right.normalize();
		btVector3 L = front.normalize();
		btVector3 U = L.cross(R).normalize();
		R = U.cross(L);

		btScalar DR = dir.dot(R);
		btScalar DL = dir.dot(-R);
		btScalar DT = dir.dot(U);
		btScalar DBO = dir.dot(-U);
		btScalar DF = dir.dot(L);
		btScalar DBA = dir.dot(-L);

		int A, B, C;
		btScalar D1, D2, D3;

		if (DR > DL)
		{
			A = e_cubeRight;
			D1 = DR;
		}
		else
		{
			A = e_cubeLeft;
			D1 = DL;
		}

		if (DT > DBO)
		{
			A = e_cubeTop;
			D2 = DT;
		}
		else
		{
			A = e_cubeBottom;
			D2 = DBO;
		}

		if (DF > DBA)
		{
			A = e_cubeFront;
			D3 = DF;
		}
		else
		{
			A = e_cubeBack;
			D3 = DBA;
		}

		if (D1 >= D2 && D1 >= D3)
		{
			return A;
		}

		if (D2 >= D3 && D2 >= D1)
		{
			return B;
		}

		if (D3 >= D1 && D3 >= D2)
		{
			return C;
		}

		return -1;
	}

	inline static btVector3 GetCoordForCube(btVector3 dir, btTransform sideMatrix, btVector3 up = btVector3(0.0, 1.0, 0.0), btVector3 right = btVector3(1.0, 0.0, 0.0), btVector3 front = btVector3(0.0, 0.0, 1.0))
	{
		btVector3 U = up;
		btVector3 R = right;
		btVector3 F = front;

		dir = (sideMatrix.inverse() * dir).normalize();

		if (dir.getY() <= 0.0)
		{
			return btVector3(0.0, 0.0, -1.0);
		}

		dir.setX(0.5 * dir.getX() / dir.getY());
		dir.setZ(0.5 * dir.getZ() / dir.getY());

		return btVector3(dir.getX(), dir.getZ(), 0.0);
	}

	inline static btVector3 GetDirFromCubeCoord(btVector3 coord, btTransform sideMatrixInv, btVector3 up = btVector3(0.0, 1.0, 0.0), btVector3 right = btVector3(1.0, 0.0, 0.0), btVector3 front = btVector3(0.0, 0.0, 1.0))
	{
		return sideMatrixInv * btVector3(coord.getX() - 0.5, 0.25, coord.getY() - 0.5).normalize();
	}

	inline static btVector3 GetCoordForCylinder(btVector3 dir, btVector3 up = btVector3(0.0, 1.0, 0.0), btVector3 right = btVector3(1.0, 0.0, 0.0), btVector3 front = btVector3(0.0, 0.0, 1.0))
	{
		btScalar DXZ = 0.0;
		btScalar DZ = 0.0;

		if (dir.getX() != 0.0 || dir.getZ() != 0.0)
		{
			btVector3 dirXZ = btVector3(dir.getX(), 0.0, dir.getZ()).normalize();
			DXZ = dirXZ.dot(right);
			DZ = dirXZ.dot(front);
		}

		btScalar DY = dir.dot(up);

		btScalar X = 0.318309886 * acos(DY);
		btScalar Y = 0.159154943 * acos(DXZ);

		if (DZ < 0.0)
		{
			Y = 1.0 - Y;
		}

		while (X < 0.0) X += 1.0;
		while (X >= 1.0) X -= 1.0;
		while (Y < 0.0) Y += 1.0;
		while (Y >= 1.0) Y -= 1.0;

		return btVector3(X, Y, 0.0);
	}

	inline static btVector3 GetDirFromCylinderCoord(btVector3 coord, btVector3 up = btVector3(0.0, 1.0, 0.0), btVector3 right = btVector3(1.0, 0.0, 0.0), btVector3 front = btVector3(0.0, 0.0, 1.0))
	{
		return btMatrix3x3(btQuaternion(up, 12.5663706 * coord.getY())) * (btMatrix3x3(btQuaternion(front, 6.2831853 * coord.getX())) * up);
	}

	inline static void CalcCoordForTile(btVector3 planetSideCoord, btScalar level, XMINT2 &outWhichTileCoord, XMFLOAT2 &outTileCoord)
	{
		btScalar PL = pow(2.0, level);
		btVector3 T = planetSideCoord * PL;
		btVector3 TC = (T - btVector3(((int)T.getX()), ((int)T.getY()), ((int)T.getZ())));

		return;

		outWhichTileCoord.x = T.getX();
		outWhichTileCoord.y = T.getY();

		outTileCoord.x = TC.getX();
		outTileCoord.y = TC.getY();

		return;
	}

	inline static btVector3 CalcPlanetSideCoordFromTile(btScalar level, XMINT2 WhichTileCoord, XMINT2 TileCoord)
	{
		btScalar PL = 1.0 / pow(2.0, level);

		return (btVector3(WhichTileCoord.x, WhichTileCoord.y, 0.0) + btVector3(TileCoord.x, TileCoord.y, 0.0)) * PL;
	}
};