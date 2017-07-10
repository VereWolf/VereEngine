#include "pch.h"
#include "GameObjectSpace.h"
#include "Camera.h"
#include "PlanetData.h"
#include "IDRegistr.h"

TerrainPlanetData::TerrainPlanetData()
{
}

void TerrainPlanetData::Init()
{
	SetLocalPosition(btVector3(0.0, 0.0, 0.0));
	SetLocalAngle(btVector3(0.0, 0.0, 0.0));

	btMatrix3x3 M1 = btMatrix3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);
	btMatrix3x3 M2 = btMatrix3x3(-1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);

	m_planetElementID = new IDRegistr(10);
	m_planetElements.resize(pow(2, 10), NULL);

	for (int i = 0; i < 12; ++i)
	{
		m_blockMatrixs[i].setIdentity();

		m_tang = btVector3(0.0, 1.0, 0.0);
		
		switch (i)
		{
		case 1:
			m_blockMatrixs[i].setEulerZYX(0.5 * PI, 0.0, 0.0);
			break;
		case 2:
			m_blockMatrixs[i].setEulerZYX(0.0, 0.0, 0.5 * PI);
			break;
		case 3:
			m_blockMatrixs[i].setValue(
				1.0, 0.0, 1.0,
				0.0, 1.0, 0.0,
				0.0, 0.0, -1.0);
			m_blockMatrixs[i].setEulerZYX(1.0 * PI, 0.0, 0.0);
			break;
		case 4:
			m_blockMatrixs[i].setEulerZYX(-0.5 * PI, 0.0, 0.0);
			break;
		case 5:
			m_blockMatrixs[i].setEulerZYX(0.0, 0.0, -0.5 * PI);
			break;
		case 7:
			m_blockMatrixs[i].setEulerZYX(0.5 * PI, 0.0, 0.0);
			m_blockMatrixs[i] = m_blockMatrixs[i] * M1;
			break;
		case 8:
			m_blockMatrixs[i].setEulerZYX(0.0, 0.0, 0.5 * PI);
			m_blockMatrixs[i] = m_blockMatrixs[i] * M2;
			break;
		case 9:
			m_blockMatrixs[i].setValue(
				1.0, 0.0, 1.0,
				0.0, 1.0, 0.0,
				0.0, 0.0, -1.0);
			m_blockMatrixs[i].setEulerZYX(1.0 * PI, 0.0, 0.0);
			break;
		case 10:
			m_blockMatrixs[i].setEulerZYX(-0.5 * PI, 0.0, 0.0);
			m_blockMatrixs[i] = m_blockMatrixs[i] * M1;
			break;
		case 11:
			m_blockMatrixs[i].setEulerZYX(0.0, 0.0, -0.5 * PI);
			m_blockMatrixs[i] = m_blockMatrixs[i] * M2;
			break;
		}

		btVector3 V2 = btVector3(0.0, 1.0, 0.0) * GetBlockAnglMatrix(i);

		if (abs(V2.getX()) > abs(V2.getY()) && abs(V2.getX()) > abs(V2.getZ()))
		{
			if (V2.getX() > 0.0)
			{
				m_posfix[i] = "+X";
			}
			else
			{
				m_posfix[i] = "-X";
			}
		}

		else if (abs(V2.getY()) > abs(V2.getZ()) && abs(V2.getY()) > abs(V2.getX()))
		{
			if (V2.getY() > 0.0)
			{
				m_posfix[i] = "+Y";
			}
			else
			{
				m_posfix[i] = "-Y";
			}
		}

		else if (abs(V2.getZ()) > abs(V2.getX()) && abs(V2.getZ()) > abs(V2.getY()))
		{
			if (V2.getZ() > 0.0)
			{
				m_posfix[i] = "+Z";
			}
			else
			{
				m_posfix[i] = "-Z";
			}
		}
	}

	SetRadius(GetScaling().getRow(0).getX());
}

int TerrainPlanetData::BuildLODBuffers(DX::DeviceResources *resources, UINT &sizeOfVertex, UINT &indicesCount)
{
	int32_t PIC = GetNumPointInRowInCell();
	float W = 1.0f / GetNumPointInRowInCell();
	UINT verticesCount = pow(PIC + 2, 2);

	std::vector <Vertex::TerrainLOD> vertices(verticesCount);

	for (int j = 0; j < (PIC + 2); ++j)
	{
		for (int i = 0; i < (PIC + 2); ++i)
		{
			if (i == 0 || i == PIC + 1 || j == 0 || j == PIC + 1)
			{
				vertices[j * (PIC + 2) + i].Pos.y = -0.2f;
			}
			else
			{
				vertices[j * (PIC + 2) + i].Pos.y = 0.0f;
			}
			vertices[j * (PIC + 2) + i].Pos.x = -0.5f + static_cast<float>(i - 1) / (PIC - 1.0f);
			vertices[j * (PIC + 2) + i].Pos.z = -0.5f + static_cast<float>(j - 1) / (PIC - 1.0f);

			vertices[j * (PIC + 2) + i].TexTess.x = static_cast<float>(i) / (PIC + 1.0f);
			vertices[j * (PIC + 2) + i].TexTess.y = static_cast<float>(j) / (PIC + 1.0f);
		}
	}

	PIC = GetNumPointInRowInCell() + 2;
	indicesCount = 4 * pow(PIC - 1, 2);

	std::vector<UINT> indices(indicesCount);

	UINT n = 0;

	for (UINT j = 0; j < (PIC - 1); ++j)
	{
		for (UINT i = 0; i < (PIC - 1); ++i)
		{
			indices[n] = j * PIC + i;
			indices[n + 1] = j * PIC + i + 1;
			indices[n + 3] = (j + 1) * PIC + i + 1;
			indices[n + 2] = (j + 1) * PIC + i;

			n += 4;
		}
	}

	sizeOfVertex = sizeof(Vertex::TerrainLOD);

	return GameRenderDeviceHandle->CreateMeshBuffer(&vertices[0], sizeOfVertex, verticesCount, &indices);
}

void TerrainPlanetData::GenerateCoord(float height, float width, float level)
{
	int PL = pow(2, level);
	btScalar ofs = 1.0 / PL;
	btScalar HI = ofs / (height - 1);
	btScalar WI = ofs / (width - 1);
	btScalar HE1 = 1.0 / (m_numPointsInRowInCell * PL);
	btScalar WE1 = 1.0 / (m_numPointsInRowInCell * PL);
	int HE2 = height / m_numPointsInRowInCell;
	int WE2 = width / m_numPointsInRowInCell;
	btScalar H, W = 0.0f;
	btVector3 V = btVector3(0.0, 0.0, 0.0);
	btVector3 V2 = btVector3(0.0, 0.0, 0.0);
	std::vector<float> map(2 * (height + 2 * HE2) * (width + 2 * WE2));

	//for (int i = 0; i < 6; ++i)
	{
		int i = 2;
		for (int m = 0; m < PL; ++m)
		{
			for (int n = 0; n < PL; ++n)
			{
				H = -0.5 - HE1 + ofs;
				for (int y = 0; y < ((int)height + 2 * HE2); y += 1)
				{
					W = -0.5 - WE1 + ofs;
					for (int x = 0; x < ((int)width + 2 * WE2); x += 1)
					{
						V = PlanetCordinateMat::GetCoordForCylinder(btVector3(btVector3(W, 0.5, H) * GetBlockAnglMatrix(i + 6)).normalize()); // convert from cylinder map to cube side map
						map.at(2 * (y * (width + 2 * WE2) + x)) = V.getX();
						map.at(2 * (y * (width + 2 * WE2) + x) + 1) = V.getY();
						W += WI;
					}
					H += HI;
				}
				int id = GameStreamingDataHandle->CreateStreamingData(&map.at(0), sizeof(float) * map.size());

				//Platform::String ^str;

				stringstream str;

				/*if (level == 0)
				{
					str = "coord_" + m_posfix[i] + ".txt";
				}
				else
				{
					str = "coord_" + m_posfix[i] + "_" + level + "_" + n + "_" + m + ".txt";
				}*/
				
				if (level == 0)
				{
					str << "coord_" << GetPosFix(i) << ".txt";
				}
				else
				{
					str << "coord_" << GetPosFix(i) << "_" << to_string((int)level) << "_" + to_string(n) << "_" + to_string(m) << ".txt";
				}

				GameStreamingDataHandle->SaveData(str.str(), id);
				GameStreamingDataHandle->DeleteStreamingData(id);
			}
		}
	}
}