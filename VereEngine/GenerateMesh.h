#pragma once

#include "pch.h"
#include "vertex.h"

class GenerateMesh
{
public:
	static void GenerateBox(float length, UINT numVertexInRow, XMFLOAT3 offset, std::vector<Vertex::PosNormalTexTan> &vertices, std::vector<UINT> &indices)
	{
		XMFLOAT3 corners[8];
		corners[0].x = -0.5f;
		corners[0].y = -0.5f;
		corners[0].z = -0.5f;

		corners[1].x = 0.5f;
		corners[1].y = -0.5f;
		corners[1].z = -0.5f;

		corners[2].x = -0.5f;
		corners[2].y = 0.5f;
		corners[2].z = -0.5f;

		corners[3].x = 0.5f;
		corners[3].y = 0.5f;
		corners[3].z = -0.5f;

		corners[4].x = -0.5f;
		corners[4].y = -0.5f;
		corners[4].z = 0.5f;

		corners[5].x = 0.5f;
		corners[5].y = -0.5f;
		corners[5].z = 0.5f;

		corners[6].x = -0.5f;
		corners[6].y = 0.5f;
		corners[6].z = 0.5f;

		corners[7].x = 0.5f;
		corners[7].y = 0.5f;
		corners[7].z = 0.5f;

		vertices.resize(6 * pow(numVertexInRow, 2.0f));
		indices.resize(6 * 6 * pow(numVertexInRow - 1, 2.0f));

		UINT indicesForCorners[4];

		//XMVECTOR points[3];
		XMFLOAT3 points[3];

		//XMVECTOR O = XMLoadFloat3(&offset);

		XMFLOAT3 normal;
		int TD = 0;

		for (int i = 0; i < 6; ++i)
		{
			switch (i)
			{
			case 0:
				indicesForCorners[0] = 0;
				indicesForCorners[1] = 1;
				indicesForCorners[2] = 2;
				indicesForCorners[3] = 3;

				normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
				break;
			case 1:
				indicesForCorners[0] = 0;
				indicesForCorners[1] = 4;
				indicesForCorners[2] = 1;
				indicesForCorners[3] = 5;

				normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
				break;
			case 2:
				indicesForCorners[0] = 1;
				indicesForCorners[1] = 5;
				indicesForCorners[2] = 3;
				indicesForCorners[3] = 7;

				normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
				break;
			case 3:
				indicesForCorners[0] = 4;
				indicesForCorners[1] = 5;
				indicesForCorners[2] = 6;
				indicesForCorners[3] = 7;

				normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
				TD = 1;
				break;
			case 4:
				indicesForCorners[0] = 2;
				indicesForCorners[1] = 6;
				indicesForCorners[2] = 3;
				indicesForCorners[3] = 7;

				normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
				TD = 1;
				break;
			case 5:
				indicesForCorners[0] = 0;
				indicesForCorners[1] = 4;
				indicesForCorners[2] = 2;
				indicesForCorners[3] = 6;

				normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
				TD = 1;
				break;
			}

			for (int y = 0; y < numVertexInRow; ++y)
			{
				points[0].x = (corners[indicesForCorners[0]].x + offset.x) * length
					+ (float)y / (float)(numVertexInRow - 1) * ((corners[indicesForCorners[1]].x + offset.x) * length - (corners[indicesForCorners[0]].x + offset.x) * length);
				points[0].y = (corners[indicesForCorners[0]].y + offset.y) * length
					+ (float)y / (float)(numVertexInRow - 1) * ((corners[indicesForCorners[1]].y + offset.y) * length - (corners[indicesForCorners[0]].y + offset.y) * length);
				points[0].z = (corners[indicesForCorners[0]].z + offset.z) * length
					+ (float)y / (float)(numVertexInRow - 1) * ((corners[indicesForCorners[1]].z + offset.z) * length - (corners[indicesForCorners[0]].z + offset.z) * length);

				points[1].x = (corners[indicesForCorners[2]].x + offset.x) * length
					+ (float)y / (float)(numVertexInRow - 1) * ((corners[indicesForCorners[3]].x + offset.x) * length - (corners[indicesForCorners[2]].x + offset.x) * length);
				points[1].y = (corners[indicesForCorners[2]].y + offset.y) * length
					+ (float)y / (float)(numVertexInRow - 1) * ((corners[indicesForCorners[3]].y + offset.y) * length - (corners[indicesForCorners[2]].y + offset.y) * length);
				points[1].z = (corners[indicesForCorners[2]].z + offset.z) * length
					+ (float)y / (float)(numVertexInRow - 1) * ((corners[indicesForCorners[3]].z + offset.z) * length - (corners[indicesForCorners[2]].z + offset.z) * length);

				for (int x = 0; x < numVertexInRow; ++x)
				{
					points[2].x = points[0].x + (float)x / (float)(numVertexInRow - 1) * (points[1].x - points[0].x);
					points[2].y = points[0].y + (float)x / (float)(numVertexInRow - 1) * (points[1].y - points[0].y);
					points[2].z = points[0].z + (float)x / (float)(numVertexInRow - 1) * (points[1].z - points[0].z);

					vertices.at(i * pow(numVertexInRow, 2.0f) + y * numVertexInRow + x).Position = points[2];

					vertices.at(i * pow(numVertexInRow, 2.0f) + y * numVertexInRow + x).TexTess.x = (float)x / (float)(numVertexInRow - 1);
					vertices.at(i * pow(numVertexInRow, 2.0f) + y * numVertexInRow + x).TexTess.y = (float)y / (float)(numVertexInRow - 1);

					vertices.at(i * pow(numVertexInRow, 2.0f) + y * numVertexInRow + x).Normal = normal;
					
					XMFLOAT3 N = XMFLOAT3(normal.x + 1, normal.y + 1, normal.z + 1);
					float D = pow(N.x * N.x + N.y * N.y + N.z * N.z, 0.5f);
					N.x /= D;
					N.y /= D;
					N.z /= D;

					vertices.at(i * pow(numVertexInRow, 2.0f) + y * numVertexInRow + x).Tangent.x = normal.y * N.z - normal.z * N.y;
					vertices.at(i * pow(numVertexInRow, 2.0f) + y * numVertexInRow + x).Tangent.y = normal.z * N.x - normal.x * N.z;
					vertices.at(i * pow(numVertexInRow, 2.0f) + y * numVertexInRow + x).Tangent.z = normal.x * N.y - normal.y * N.x;
				}
			}

			for (int y = 0; y < numVertexInRow - 1; ++y)
			{
				for (int x = 0; x < numVertexInRow - 1; ++x)
				{
					if (TD == 0)
					{
						indices.at(i * 6 * pow(numVertexInRow - 1, 2.0f) + 6 * y * (numVertexInRow - 1) + 6 * x) = i * pow(numVertexInRow, 2.0f) + y *numVertexInRow + x;
						indices.at(i * 6 * pow(numVertexInRow - 1, 2.0f) + 6 * y * (numVertexInRow - 1) + 6 * x + 1) = i * pow(numVertexInRow, 2.0f) + y *numVertexInRow + x + 1;
						indices.at(i * 6 * pow(numVertexInRow - 1, 2.0f) + 6 * y * (numVertexInRow - 1) + 6 * x + 2) = i * pow(numVertexInRow, 2.0f) + (y + 1) *numVertexInRow + x + 1;
						indices.at(i * 6 * pow(numVertexInRow - 1, 2.0f) + 6 * y * (numVertexInRow - 1) + 6 * x + 3) = i * pow(numVertexInRow, 2.0f) + (y + 1) *numVertexInRow + x + 1;
						indices.at(i * 6 * pow(numVertexInRow - 1, 2.0f) + 6 * y * (numVertexInRow - 1) + 6 * x + 4) = i * pow(numVertexInRow, 2.0f) + (y + 1) *numVertexInRow + x;
						indices.at(i * 6 * pow(numVertexInRow - 1, 2.0f) + 6 * y * (numVertexInRow - 1) + 6 * x + 5) = i * pow(numVertexInRow, 2.0f) + y * numVertexInRow + x;
					}
					else
					{
						indices.at(i * 6 * pow(numVertexInRow - 1, 2.0f) + 6 * y * (numVertexInRow - 1) + 6 * x) = i * pow(numVertexInRow, 2.0f) + (y + 1) *numVertexInRow + x + 1;
						indices.at(i * 6 * pow(numVertexInRow - 1, 2.0f) + 6 * y * (numVertexInRow - 1) + 6 * x + 1) = i * pow(numVertexInRow, 2.0f) + y *numVertexInRow + x + 1;
						indices.at(i * 6 * pow(numVertexInRow - 1, 2.0f) + 6 * y * (numVertexInRow - 1) + 6 * x + 2) = i * pow(numVertexInRow, 2.0f) + y *numVertexInRow + x;
						indices.at(i * 6 * pow(numVertexInRow - 1, 2.0f) + 6 * y * (numVertexInRow - 1) + 6 * x + 3) = i * pow(numVertexInRow, 2.0f) + y * numVertexInRow + x;
						indices.at(i * 6 * pow(numVertexInRow - 1, 2.0f) + 6 * y * (numVertexInRow - 1) + 6 * x + 4) = i * pow(numVertexInRow, 2.0f) + (y + 1) *numVertexInRow + x;
						indices.at(i * 6 * pow(numVertexInRow - 1, 2.0f) + 6 * y * (numVertexInRow - 1) + 6 * x + 5) = i * pow(numVertexInRow, 2.0f) + (y + 1) *numVertexInRow + x + 1;
					}
				}
			}
		}
	}

	static void GenerateSphere(float diameter, UINT numVertexInRow, XMFLOAT3 offset, std::vector<Vertex::PosNormalTexTan> &vertices, std::vector<UINT> &indices)
	{
		GenerateBox(diameter, numVertexInRow, offset, vertices, indices);

		float distance;
		XMFLOAT3 normal;

		for (int i = 0; i < vertices.size(); ++i)
		{
			distance = pow(pow(vertices.at(i).Position.x - offset.x * diameter, 2.0f)
				+ pow(vertices.at(i).Position.y - offset.y * diameter, 2.0f)
				+ pow(vertices.at(i).Position.z - offset.z * diameter, 2.0f), 0.5f);

			distance = diameter / distance;

			vertices.at(i).Position.x = (vertices.at(i).Position.x - offset.x * diameter) * distance + offset.x * diameter;
			vertices.at(i).Position.y = (vertices.at(i).Position.y - offset.y * diameter) * distance + offset.y * diameter;
			vertices.at(i).Position.z = (vertices.at(i).Position.z - offset.z * diameter) * distance + offset.z * diameter;

			distance = 1.0f / diameter;

			normal.x = (vertices.at(i).Position.x - offset.x * diameter) * distance;
			normal.y = (vertices.at(i).Position.y - offset.x * diameter) * distance;
			normal.z = (vertices.at(i).Position.z - offset.x * diameter) * distance;

			vertices.at(i).Normal = normal;

			XMFLOAT3 N = XMFLOAT3(normal.x + 1, normal.y + 1, normal.z + 1);
			float D = pow(N.x * N.x + N.y * N.y + N.z * N.z, 0.5f);
			N.x /= D;
			N.y /= D;
			N.z /= D;

			vertices.at(i).Tangent.x = normal.y * N.z - normal.z * N.y;
			vertices.at(i).Tangent.y = normal.z * N.x - normal.x * N.z;
			vertices.at(i).Tangent.z = normal.x * N.y - normal.y * N.x;
		}
	}
};