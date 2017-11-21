#include "stdafx.h"
#include "GenerateRiver.h"

GenerateRiver::GenerateRiver()
{
	SetFolderName(L"Calc_Rivers");
	m_numRiverNode = 0;

}

void GenerateRiver::GeneratePixelOfRiver(Float2 V1, Float2 V2, float d1, float d2, float id)
{
	Float2 V = Float2(V2.x - V1.x, V2.y - V1.y);
	float dH = d1 + 0.5f * (d2 - d1);

	float D = V.Length();

	if (D >= 1.41422)
	{
		D = 0.25f * D;
		float S = 0.1f * D;
		Float2 Vec = V.Normalize();

		Float2 M = Float2(V1.x + V.x * 0.5f, V1.y + V.y * 0.5f);
		Float2 M2 = M;

		float H = m_HMap[((int)(M.y + 0.5f)) * m_width + ((int)(M.x + 0.5f))];

		if (D >= 0.70711f)
		{
			for (float i = -D; i <= D; i += S)
			{
				Float2 M3 = Float2(M.x + i * Vec.y, M.y + i * Vec.x);

				if (m_HMap[((int)(M3.y + 0.5f)) * m_width + ((int)(M3.x + 0.5f))] < H)
				{
					M2 = M3;
				}
			}
		}
		if (m_Rivers[((int)(M2.y + 0.5f)) * m_width + ((int)(M2.x + 0.5f))].w == 0.0f)
		{
			m_Rivers[((int)(M2.y + 0.5f)) * m_width + ((int)(M2.x + 0.5f))] = Float4(dH, id, 0.0f, 1.0f);
		}

		Float2 V3[3] = { Float2((int)(V1.x + 0.5f),(int)(V1.y + 0.5f)), Float2((int)(M2.x + 0.5f),(int)(M2.y + 0.5f)), Float2((int)(V2.x + 0.5f),(int)(V2.y + 0.5f)) };

		GeneratePixelOfRiver(V3[0], V3[1], d1, dH, id);
		GeneratePixelOfRiver(V3[1], V3[2], dH, d2, id);
	}
}

void GenerateRiver::ClampRiver(int x, int y, float d, float id)
{
	float d2 = -1;

	float RiverStep = 0;

	int sx = x;
	int sy = y;

	INT2 P = INT2(x, y);
	float D = d;
	float ID = id;

	int j = y * m_width + x;

	float HR = m_HMap[j];

	bool B = true;

	while (B == true && sx >= 0)
	{
		d2 = -1;

		sx = -1;
		sy = -1;

		B = true;

		for (int dy = (y - 1); dy <= (y + 1); ++dy)
		{
			for (int dx = (x - 1); dx <= (x + 1); ++dx)
			{
				if (dx >= 0 && dx < m_width && dy >= 0 && dy < m_height)
				{
					int i = dy * m_width + dx;

					if (m_Rivers[i].w == 1.0f)
					{

						if (m_Rivers[i].y < id && m_Rivers[i].w != 0.0f)
						{
							sx = dx;
							sy = dy;
							d2 = m_Rivers[i].x;
							id = m_Rivers[i].y;
						}

						if (m_Rivers[i].y == id && m_Rivers[i].x > d && (d2 == -1 || d2 > m_Rivers[i].x))
						{
							sx = dx;
							sy = dy;
							d2 = m_Rivers[i].x;
						}
					}
				}
			}
		}

		if (sx >= 0)
		{
			m_Rivers2[sy * m_width + sx].x = m_Rivers[sy * m_width + sx].x;
			m_Rivers2[sy * m_width + sx].y = m_Rivers[sy * m_width + sx].y;

			x = sx;
			y = sy;
			d = d2;

			j = y * m_width + x;

			if (HR < m_HMap[j])
			{
				m_HMap[j] = HR;
				m_Rivers2[j].z = HR * 0.0004f;
			}

			if (m_Rivers2[j].w == 0)
			{
				m_Rivers2[j].w += 0.25f;
			}
			else
			{
				m_Rivers2[j].w += 0.08f;
			}

			m_Rivers2[j].w = VMath::Clamp(m_Rivers2[j].w, 0.0f, 1.0f);
		}
	}
}

void GenerateRiver::CreateRiverNode(int x, int y, float d, float id, int multiplicationForRiverNode)
{
	float d2 = -1;

	float RiverStep = 0;

	int sx = x;
	int sy = y;

	INT2 P = INT2(x, y);
	float D = d;
	float ID = id;

	int j = y * m_width + x;

	float HR = m_HMap[j];

	bool B = true;

	while (B == true && sx >= 0)
	{
		d2 = -1;

		sx = -1;
		sy = -1;

		B = true;

		for (int dy = (y - 1); dy <= (y + 1); ++dy)
		{
			for (int dx = (x - 1); dx <= (x + 1); ++dx)
			{
				if (dx >= 0 && dx < m_width && dy >= 0 && dy < m_height)
				{
					int i = dy * m_width + dx;

					if (m_Rivers[i].w == 1.0f)
					{

						if (m_Rivers[i].y < id) B = false;

						if (m_Rivers[i].y == id && m_Rivers[i].x > d && (d2 == -1 || d2 > m_Rivers[i].x))
						{
							sx = dx;
							sy = dy;
							d2 = m_Rivers[i].x;
						}
					}
				}
			}
		}

		if (B == true && sx >= 0)
		{
			x = sx;
			y = sy;
			d = d2;

			j = y * m_width + x;

			AddRiverNode(id, m_Rivers2[j].w, m_HMap[j], Float2(multiplicationForRiverNode * x, multiplicationForRiverNode * y));
		}
		else
		{
			Float2 pos = Float2(-1, -1);
			float DE = 2.0;

			for (int dy = (y - 1); dy <= (y + 1); ++dy)
			{
				for (int dx = (x - 1); dx <= (x + 1); ++dx)
				{
					if (dx >= 0 && dx < m_width && dy >= 0 && dy < m_height)
					{
						float DE2 = Float2(dx - x, dy - y).Length();
						int i = dy * m_width + dx;

						if (m_Rivers[i].w == 1.0f)
						{

							if (m_Rivers[i].y < id && DE2 < DE)
							{
								DE = DE2;
								pos = Float2(dx, dy);
							}
						}
					}
				}
			}

			if (pos.x != -1)
			{
				j = y * m_width + x;

				AddRiverNode(id, m_Rivers2[j].w, m_HMap[j], Float2(pos.x * multiplicationForRiverNode, pos.y * multiplicationForRiverNode));
			}
		}
	}
}

INT2 GenerateRiver::GetRiverOffset(Float2 V1, Float2 V2)
{
	Float2 V = Float2(V2.y - V1.y, V2.x - V1.x).Normalize();
	int k = V2.y * m_width + V2.x;
	float H = m_HMap[k];
	INT2 V3 = INT2(V2.x, V2.y);

	for (float i = -5.0f; i < -5.0f; i += 1.0f)
	{
		INT2 VO = INT2(V2.x + V.x * i + 0.5f, V2.y + V.y * i + 0.5f);

		int j = VO.y * m_width + VO.x;

		if (m_HMap[j] < H)
		{
			H = m_HMap[j];
			V3.x = VO.x;
			V3.y = VO.y;
		}
	}

	return V3;
}

void GenerateRiver::AddRiverNode(float id, float size, float height, Float2 pos)
{
	RiverNode *RN = new RiverNode;
	RN->id = id;
	RN->pos = pos;
	RN->size = size;
	RN->height = height;
	RN->prev = NULL;
	RN->next = NULL;

	if (m_numRiverNode == 0)
	{
		m_startRiverNode = RN;
		m_endRiverNode = RN;
		m_numRiverNode = 1;
	}
	else
	{
		m_endRiverNode->next = RN;
		RN->prev = m_endRiverNode;
		m_endRiverNode = RN;
		m_numRiverNode += 1;
	}
}

void GenerateRiver::InsertRiverNodeBetweenNodes(RiverNode *firstNode, float id, float size, float height, Float2 pos)
{
	RiverNode *N1 = firstNode;
	RiverNode *N2 = firstNode->next;
	RiverNode * NM = new RiverNode;
	NM->id = id;
	NM->pos = pos;
	NM->size = size;
	N1->next = NM;
	NM->prev = N1;
	NM->next = N2;
	N2->prev = NM;
}

void GenerateRiver::InsertRiverNodesRecurse(RiverNode *firstNode, float minLenght)
{
	if (firstNode->next && firstNode->id == firstNode->next->id)
	{
		Float2 PD = Float2(firstNode->pos.x - firstNode->next->pos.x, firstNode->pos.y - firstNode->next->pos.y);
		float L = pow(pow(PD.x, 2.0f) + pow(PD.y, 2.0f), 0.5f);

		if (L >= minLenght && (abs(PD.x) > 1.0f || abs(PD.y) > 1.0f))
		{
			m_numRiverNode += 1;
			InsertRiverNodeBetweenNodes(firstNode, firstNode->id, firstNode->size, 0.5f * (firstNode->height + firstNode->next->height), Float2(((int)(firstNode->pos.x + 0.5f * (firstNode->next->pos.x - firstNode->pos.x) + 0.5f)), ((int)firstNode->pos.y + 0.5f * (firstNode->next->pos.y - firstNode->pos.y) + 0.5f)));

			RiverNode *fm = firstNode->next;

			InsertRiverNodesRecurse(firstNode, minLenght);
			InsertRiverNodesRecurse(fm, minLenght);
		}
	}
}

void GenerateRiver::AddRiverNodeToBlock(float id, float size, Float2 pos, int blockIndex)
{
	RiverNode *RN = new RiverNode;
	RN->id = id;
	RN->size = size;
	RN->pos = pos;
	RN->prev = NULL;
	RN->next = NULL;

	if (m_RiverBlocks[blockIndex].numNode == 0)
	{
		m_RiverBlocks[blockIndex].start = RN;
		m_RiverBlocks[blockIndex].end = RN;
		m_RiverBlocks[blockIndex].numNode = 1;
	}
	else
	{
		m_RiverBlocks[blockIndex].end->next = RN;
		RN->prev = m_RiverBlocks[blockIndex].end;
		m_RiverBlocks[blockIndex].end = RN;

		m_RiverBlocks[blockIndex].numNode += 1;
	}
}

void GenerateRiver::GenerateRiverNode(float minLenght)
{
	SetCurrentToStartRiverNode();

	while (m_currentRiverNode->next)
	{
		RiverNode *next = m_currentRiverNode->next;
		if (m_currentRiverNode->id == m_currentRiverNode->next->id)
		{
			InsertRiverNodesRecurse(m_currentRiverNode, minLenght);
		}

		m_currentRiverNode = next;
	}
}

void GenerateRiver::GenerateNodeInBlock(int widthBlockStack, int heightBlockStack, int sizeOfBlock)
{
	SetCurrentToStartRiverNode();

	while (m_currentRiverNode)
	{
		int XB = m_currentRiverNode->pos.x / sizeOfBlock;
		int YB = m_currentRiverNode->pos.y / sizeOfBlock;
		int XIB = m_currentRiverNode->pos.x - XB * sizeOfBlock;
		int YIB = m_currentRiverNode->pos.y - YB * sizeOfBlock;

		int i = YB * widthBlockStack + XB;

		AddRiverNodeToBlock(m_currentRiverNode->id, m_currentRiverNode->size, Float2(XIB, YIB), i);

		SetCurrentToNextRiverNode();
	}
}

void GenerateRiver::GenerateBlock(int widthOfMap, int heightOfMap, int numBlockInRow, int numBlockInColumn)
{
	m_RiverMap1.resize(widthOfMap * heightOfMap);
	m_RiverMap2.resize(widthOfMap * heightOfMap);

	for (int j = 0; j < numBlockInColumn; ++j)
	{
		for (int i = 0; i < numBlockInRow; ++i)
		{
			ZeroMemory(&m_RiverMap1[0], m_RiverMap1.size() * sizeof(BYTE4));
			ZeroMemory(&m_RiverMap2[0], m_RiverMap2.size() * sizeof(BYTE4));

			int k = j * numBlockInRow + i;

			m_RiverBlocks[k].current = m_RiverBlocks[k].start;

			if (m_RiverBlocks[k].numNode > 0)
			{
				int IM2 = false;

				while (m_RiverBlocks[k].current)
				{
					Float2 V1 = Float2(0.0f, 0.0f);
					Float2 V2 = Float2(0.0f, 0.0f);
					if (m_RiverBlocks[k].current->next)
					{
						V1 = Float2(m_RiverBlocks[k].current->next->pos.x - m_RiverBlocks[k].current->pos.x, m_RiverBlocks[k].current->next->pos.y - m_RiverBlocks[k].current->pos.y).Normalize();
					}
					if (m_RiverBlocks[k].current->prev)
					{
						V2 = Float2(m_RiverBlocks[k].current->pos.x - m_RiverBlocks[k].current->prev->pos.x, m_RiverBlocks[k].current->pos.y - m_RiverBlocks[k].current->prev->pos.y).Normalize();
					}

					Float2 V3 = Float2(V1.y, -V1.x);
					Float2 V4 = Float2(V2.y, -V2.x);

					INT2 coord = INT2(m_RiverBlocks[k].current->pos.x, m_RiverBlocks[k].current->pos.y);
					int S = 128 * m_RiverBlocks[k].current->size - 1;
					int S2 = 128 / S;
					int id = 255.0f * m_RiverBlocks[k].current->id;

					float DO1 = V1.Dot(V4);

					for (int y = -S; y <= S; ++y)
					{
						for (int x = -S; x <= S; ++x)
						{
							int D = pow(pow((float)(S2 * x), 2.0f) + pow((float)(S2 * y), 2.0f), 0.5f) + 0.5f;
							if (D < 128)
							{
								int sx = x + coord.x;
								int sy = y + coord.y;
								int z = sy * widthOfMap + sx;

								Float2 V5 = Float2(x, y).Normalize();


								BYTE L = 128 - D;

								if (sx > 0 && sx < widthOfMap && sy > 0 && sy < heightOfMap)
								{
									if (m_RiverMap1[z].w == 0 || id == m_RiverMap1[z].x)
									{
										BYTE L2 = m_RiverMap1[z].y;

										if (L2 < 128)
										{
											L2 = 128 - L2;
										}
										else
										{
											L2 = L2 - 128;
										}

										if (L2 > D || m_RiverMap1[z].y == 0)
										{
											m_RiverMap1[z].x = id;
											m_RiverMap1[z].y = L;
											m_RiverMap1[z].z = S;
											m_RiverMap1[z].w = 255;
										}
									}
									else if (m_RiverMap2[z].w == 0 || id == m_RiverMap2[z].x)
									{
										BYTE L2 = m_RiverMap2[z].y;

										if (L2 < 128)
										{
											L2 = 128 - L2;
										}
										else
										{
											L2 = L2 - 128;
										}

										if (L2 > D || m_RiverMap2[z].y == 0)
										{
											m_RiverMap2[z].x = id;
											m_RiverMap2[z].y = L;
											m_RiverMap2[z].z = 0;
											m_RiverMap2[z].w = 255;
										}

										IM2 = true;
									}
								}
							}
						}
					}

					m_RiverBlocks[k].current = m_RiverBlocks[k].current->next;
				}

				std::wstringstream wRM1;
				wRM1 << L"coord_4/" << L"RiverMap1_" << L"_" << i << L"_" << j << L".jpg";
				std::wstringstream wRM2;
				wRM2 << L"coord_4/" << L"RiverMap2_" << L"_" << i << L"_" << j << L".jpg";

				std::stringstream sRM1;
				sRM1 << "coord_4/" << "RiverMap1_" << "_" << i << "_" << j << ".raw";
				std::stringstream sRM2;
				sRM2 << "coord_4/" << "RiverMap2_" << "_" << i << "_" << j << ".raw";

				DataStreaming::SaveImage(GetWholeFilePatch(&wRM1.str()[0]), &m_RiverMap1, heightOfMap, widthOfMap);
				DataStreaming::SaveFile(GetWholeFilePatch(&sRM1.str()[0]), &m_RiverMap1[0], heightOfMap, widthOfMap, sizeof(BYTE4));

				if (IM2)
				{
					DataStreaming::SaveImage(GetWholeFilePatch(&wRM2.str()[0]), &m_RiverMap2, heightOfMap, widthOfMap);
					DataStreaming::SaveFile(GetWholeFilePatch(&sRM2.str()[0]), &m_RiverMap2[0], heightOfMap, widthOfMap, sizeof(BYTE4));
				}
			}
		}
	}
}

void GenerateRiver::GenerateRiverMap(int width, int height, int index)
{
	m_currentRiverNode = m_startRiverNode;

	while (m_currentRiverNode)
	{
		bool B = true;
		m_currentRiverNode->length = 0.0f;

		while (B)
		{
			if (m_currentRiverNode->next && m_currentRiverNode->id == m_currentRiverNode->next->id)
			{
				m_currentRiverNode->next->length = m_currentRiverNode->length + Float2(m_currentRiverNode->pos.x - m_currentRiverNode->next->pos.x, m_currentRiverNode->pos.y - m_currentRiverNode->next->pos.y).Length();
				m_currentRiverNode = m_currentRiverNode->next;
			}
			else
			{
				B = false;
			}
		}

		m_currentRiverNode = m_currentRiverNode->next;
	}

	int size = width * height;
	m_riverID.resize(size, INT2(-1, -1));
	m_riverLvl.resize(size, INT2(-1, -1));
	m_riverWidth1.resize(size, -9999.0f);
	m_riverWidth2.resize(size, -9999.0f);
	m_riverLength1.resize(size, -9999.0f);
	m_riverLength2.resize(size, -9999.0f);
	m_riverType1.resize(size, -9999.0f);
	m_riverType2.resize(size, -9999.0f);
	m_riverVector1.resize(size, Float2(0.0f, 0.0f));
	m_riverVector2.resize(size, Float2(0.0f, 0.0f));
	m_waterHeight1.resize(size, -9999.0f);
	m_waterHeight2.resize(size, -9999.0f);
	m_riverHeight.resize(size, 0.0f);

	m_currentRiverNode = m_startRiverNode;

	while (m_currentRiverNode)
	{
		int ID = m_currentRiverNode->id;
		Float2 P = m_currentRiverNode->pos;
		float S = m_currentRiverNode->size;
		float L = m_currentRiverNode->length;
		float H = m_currentRiverNode->height;
		Float2 V = Float2(0.0f, 0.0f);

		if (m_currentRiverNode->prev && m_currentRiverNode->id == m_currentRiverNode->prev->id)
		{
			Float2 VH = Float2(m_currentRiverNode->pos.x - m_currentRiverNode->prev->pos.x, m_currentRiverNode->pos.y - m_currentRiverNode->prev->pos.y).Normalize();
			V = VH;
		}

		if (m_currentRiverNode->next && m_currentRiverNode->id == m_currentRiverNode->next->id)
		{
			Float2 VH = Float2(m_currentRiverNode->next->pos.x - m_currentRiverNode->pos.x, m_currentRiverNode->next->pos.y - m_currentRiverNode->pos.y).Normalize();
			V = Float2(V.x + VH.x, V.y + VH.y).Normalize();
		}

		int i = (int)P.y * width + (int)P.x;

		m_riverHeight[i] = H;

		if (m_riverID[i].x < 0)
		{
			m_riverID[i].x = ID;
			m_riverLvl[i].x = 0;
			m_riverWidth1[i] = 0.0f;
			m_riverLength1[i] = L;
			m_riverType1[i] = S;
			m_riverVector1[i] = V;
		}
		else if (m_riverID[i].y < 0)
		{
			m_riverID[i].y = ID;
			m_riverLvl[i].y = 0;
			m_riverWidth2[i] = 0.0f;
			m_riverLength2[i] = L;
			m_riverType2[i] = S;
			m_riverVector2[i] = V;
		}

		m_currentRiverNode = m_currentRiverNode->next;
	}

	for (int i = 0; i < index; i += 1)
	{
		for (int y = 1; y < (height - 1); y += 1)
		{
			for (int x = 1; x < (width - 1); x += 1)
			{
				int z = y * width + x;

				if (m_riverID[z].x < 0.0f || m_riverID[z].y < 0.0f)
				{
					INT2 Le11 = INT2(-999, -999);
					INT2 Le12 = INT2(-999, -999);

					INT2 ID1;

					for (int py = -1; py <= 1; py += 1)
					{
						for (int px = -1; px <= 1; px += 1)
						{
							int ax = px + x;
							int ay = py + y;

							if (ax >= 0 && ax < width && ay >= 0 && ay < height)
							{
								int az = ay * width + ax;

								if (m_riverLvl[az].x == i)
								{
									if (Le11.x == -999)
									{
										Le11.x = px;
										Le11.y = py;

										ID1.x = m_riverID[az].x;
									}
									else
									{
										float d1 = pow(pow((float)ax, 2.0f) + pow((float)ay, 2.0f), 0.5f);
										float d2 = pow(pow((float)Le11.x, 2.0f) + pow((float)Le11.y, 2.0f), 0.5f);

										if (d2 < d1)
										{
											Le11.x = px;
											Le11.y = py;

											ID1.x = m_riverID[az].x;
										}
									}
								}

								if (m_riverLvl[az].y == i)
								{
									if (Le12.x == -999)
									{
										Le12.x = px;
										Le12.y = py;

										ID1.y = m_riverID[az].y;
									}
									else
									{
										float d1 = pow(pow((float)ax, 2.0f) + pow((float)ay, 2.0f), 0.5f);
										float d2 = pow(pow((float)Le12.x, 2.0f) + pow((float)Le12.y, 2.0f), 0.5f);

										if (d2 < d1)
										{
											Le12.x = px;
											Le12.y = py;

											ID1.y = m_riverID[az].y;
										}
									}
								}
							}
						}
					}

					INT2 Le21 = INT2(-999, -999);
					INT2 Le22 = INT2(-999, -999);

					if (Le11.x != -999 || Le12.x != -999)
					{
						for (int py = -1; py <= 1; py += 1)
						{
							for (int px = -1; px <= 1; px += 1)
							{
								int ax = px + x;
								int ay = py + y;

								if (ax >= 0 && ax < width && ay >= 0 && ay < height)
								{
									int az = ay * width + ax;

									if (m_riverLvl[az].x == i && m_riverID[az].x == ID1.x && Le11.x != px && Le11.y != py && m_riverID[z].x < 0.0f)
									{
										if (Le21.x == -999)
										{
											Le21.x = px;
											Le21.y = py;
										}
										else
										{
											float d1 = pow(pow((float)ax, 2.0f) + pow((float)ay, 2.0f), 0.5f);
											float d2 = pow(pow((float)Le21.x, 2.0f) + pow((float)Le21.y, 2.0f), 0.5f);

											if (d2 < d1)
											{
												Le21.x = px;
												Le21.y = py;
											}
										}
									}

									if (m_riverLvl[az].y == i && m_riverID[az].y == ID1.y && Le12.x != px && Le12.y != py && m_riverID[z].y < 0.0f)
									{
										if (Le22.x == -999)
										{
											Le22.x = px;
											Le22.y = py;
										}
										else
										{
											float d1 = pow(pow((float)ax, 2.0f) + pow((float)ay, 2.0f), 0.5f);
											float d2 = pow(pow((float)Le22.x, 2.0f) + pow((float)Le22.y, 2.0f), 0.5f);

											if (d2 < d1)
											{
												Le22.x = px;
												Le22.y = py;
											}
										}
									}
								}
							}
						}

						for (int j = 0; j < 2; ++j)
						{
							if ((j == 0 && Le11.x != -999 && m_riverID[z].x < 0.0f) || (j == 1 && Le12.x != -999 && m_riverID[z].y < 0.0f))
							{
								INT2 Le1 = INT2(-1, -1);
								INT2 Le2 = INT2(-1, -1);

								float LR1;
								float HWR1;
								float TR1;
								Float2 VE1;
								float RH1;

								if (j == 0)
								{
									Le1 = INT2(Le11.x + x, Le11.y + y);
									if (Le21.x != -999) Le2 = INT2(Le21.x + x, Le21.y + y);

									LR1 = m_riverLength1[Le1.y * width + Le1.x];
									HWR1 = m_waterHeight1[Le1.y * width + Le1.x];
									TR1 = m_riverType1[Le1.y * width + Le1.x];
									VE1 = m_riverVector1[Le1.y * width + Le1.x];
								}
								else
								{
									Le1 = INT2(Le12.x + x, Le12.y + y);
									if (Le22.x != -999) Le2 = INT2(Le22.x + x, Le22.y + y);

									LR1 = m_riverLength2[Le1.y * width + Le1.x];
									HWR1 = m_waterHeight2[Le1.y * width + Le1.x];
									TR1 = m_riverType2[Le1.y * width + Le1.x];
									VE1 = m_riverVector2[Le1.y * width + Le1.x];
								}

								RH1 = m_riverHeight[Le1.y * width + Le1.x];

								float WR;
								float LR;
								float HWR;
								float TR;
								float RH;
								Float2 VE;

								if (j == 0)
								{
									WR = abs(m_riverWidth1[Le1.y * width + Le1.x]);
								}
								else
								{
									WR = abs(m_riverWidth2[Le1.y * width + Le1.x]);
								}

								if (i == 0)VE = VE1;

								if (Le2.x < 0)
								{

									WR = Float2((float)x - (float)Le1.x, (float)y - (float)Le1.y).Length() + WR;

									if (i == 0)
									{
										if (Float2(VE.y, -VE.x).Dot(Float2((float)x - (float)Le1.x, (float)y - (float)Le1.y).Normalize()) < 0.0f) WR *= -1.0f;
									}
									else
									{
										if (j == 0)
										{
											if (m_riverWidth1[Le1.y * width + Le1.x] < 0.0f) WR *= -1;
										}
										else
										{
											if (m_riverWidth2[Le1.y * width + Le1.x] < 0.0f) WR *= -1;
										}
									}

									LR = LR1;
									HWR = HWR1;
									TR = TR1;
									RH = RH1;
								}
								else
								{
									Float2 Dir = Float2(Le2.x - Le1.x, Le2.y - Le1.y);
									float L = Dir.Length();
									Dir = Dir.Normalize();
									Float2 Dir2 = Float2(Dir.y, -Dir.x);

									float a = GetIndexOfRiver(Float2(Le1.x, Le1.y), Float2(x, y), Dir, Dir2);

									float WR2 = Float2((float)x - ((float)Le1.x + a * Dir.x), (float)y - ((float)Le1.y + a * Dir.y)).Length();

									float LR2;
									float HWR2;
									float TR2;

									if (j == 0)
									{
										LR2 = m_riverLength1[Le2.y * width + Le2.x];
										HWR2 = m_waterHeight1[Le2.y * width + Le2.x];
										TR2 = m_riverType1[Le2.y * width + Le2.x];
									}
									else
									{
										LR2 = m_riverLength2[Le2.y * width + Le2.x];
										HWR2 = m_waterHeight2[Le2.y * width + Le2.x];
										TR2 = m_riverType2[Le2.y * width + Le2.x];
									}

									float RH2 = m_riverHeight[Le2.y * width + Le2.x];

									a = a / L;

									if (j == 0)
									{
										WR = WR + a * (abs(m_riverWidth1[Le2.y * width + Le2.x]) - WR);
									}
									else
									{
										WR = WR + a * (abs(m_riverWidth2[Le2.y * width + Le2.x]) - WR);
									}

									WR = WR + WR2;

									if (i == 0)
									{
										if (Float2(VE.y, -VE.x).Dot(Float2((float)x - ((float)Le1.x + a * Dir.x), (float)y - ((float)Le1.y + a * Dir.y)).Normalize()) < 0.0f) WR *= -1.0f;
									}
									else
									{
										if (j == 0)
										{
											if (m_riverWidth1[Le1.y * width + Le1.x] < 0.0f) WR *= -1;
										}
										else
										{
											if (m_riverWidth2[Le1.y * width + Le1.x] < 0.0f) WR *= -1;
										}
									}

									LR = LR1 + a * (LR2 - LR1);
									HWR = HWR1 + a * (HWR2 - HWR1);
									TR = TR1 + a * (TR2 - TR1);
									RH = RH1 + a * (RH2 - RH1);
								}

								m_riverHeight[z] = RH;

								if (m_riverWidth1[z] < 0.0f)
								{
									m_riverWidth1[z] = WR;
									m_riverLength1[z] = LR;
									m_waterHeight1[z] = HWR;
									m_riverType1[z] = TR;
								}
								else if (m_riverWidth2[z] < 0.0f)
								{
									m_riverWidth2[z] = WR;
									m_riverLength2[z] = LR;
									m_waterHeight2[z] = HWR;
									m_riverType2[z] = TR;
								}

								m_riverHeight[z] = RH;
							}
						}
					}
				}
			}
		}
	}

	DataStreaming::SaveFile(GetWholeFilePatch("river_width1.raw"), &m_riverWidth1[0], height, width, sizeof(float));
	DataStreaming::SaveFile(GetWholeFilePatch("river_width2.raw"), &m_riverWidth2[0], height, width, sizeof(float));
	DataStreaming::SaveFile(GetWholeFilePatch("river_length1.raw"), &m_riverLength1[0], height, width, sizeof(float));
	DataStreaming::SaveFile(GetWholeFilePatch("river_length2.raw"), &m_riverLength2[0], height, width, sizeof(float));
	DataStreaming::SaveFile(GetWholeFilePatch("river_type1.raw"), &m_riverType1[0], height, width, sizeof(float));
	DataStreaming::SaveFile(GetWholeFilePatch("river_type2.raw"), &m_riverType2[0], height, width, sizeof(float));
	DataStreaming::SaveFile(GetWholeFilePatch("river_height.raw"), &m_riverHeight[0], height, width, sizeof(float));

	for (int i = 0; i < m_riverWidth1.size(); i += 1)
	{
		m_riverWidth1[i] += 3.0f;
		m_riverWidth2[i] += 3.0f;
	}

	DataStreaming::SaveImageNorm(GetWholeFilePatch(L"river_width1.png"), &m_riverWidth1, height, width);
	DataStreaming::SaveImageNorm(GetWholeFilePatch(L"river_lenght1.png"), &m_riverLength1, height, width);
	DataStreaming::SaveImageNorm(GetWholeFilePatch(L"river_type1.png"), &m_riverType1, height, width);
	DataStreaming::SaveImageNorm(GetWholeFilePatch(L"river_width2.png"), &m_riverWidth2, height, width);
	DataStreaming::SaveImageNorm(GetWholeFilePatch(L"river_lenght2.png"), &m_riverLength2, height, width);
	DataStreaming::SaveImageNorm(GetWholeFilePatch(L"river_type2.png"), &m_riverType2, height, width);
	DataStreaming::SaveImageNorm(GetWholeFilePatch(L"river_height.png"), &m_riverHeight, height, width);
}

float GenerateRiver::GetIndexOfRiver(Float2 pos1, Float2 pos2, Float2 dir1, Float2 dir2)
{
	float d1 = abs(Float2(pos2.x - (pos1.x + dir1.x), pos2.y - (pos1.y + dir1.y)).Normalize().Dot(dir2));
	float d2 = abs(Float2(pos2.x - (pos1.x - dir1.x), pos2.y - (pos1.y - dir1.y)).Normalize().Dot(dir2));
	float d3 = 0.0f;

	float l = 1.0f;
	float d = d1;

	if (d1 < d2)
	{
		l = -1.0f;
		d = d2;
	}
	else if (d1 == d2)
	{
		return 0.0f;
	}

	bool B = true;

	while (B)
	{
		d1 = abs(Float2(pos2.x - (pos1.x + 2.0f * l * dir1.x), pos2.y - (pos1.y + 2.0f * l * dir1.y)).Normalize().Dot(dir2));

		if (d1 > d)
		{
			l *= 2.0f;
			d = d1;
		}
		else
		{
			B = false;
		}
	}

	B = true;

	float iL = abs(0.5f * l);

	while (B)
	{
		Float2 dr1 = Float2(pos2.x - (pos1.x + l * dir1.x), pos2.y - (pos1.y + l * dir1.y));
		Float2 dr2 = Float2(pos2.x - (pos1.x + (l + iL) * dir1.x), pos2.y - (pos1.y + (l + iL) * dir1.y));
		Float2 dr3 = Float2(pos2.x - (pos1.x + (l - iL) * dir1.x), pos2.y - (pos1.y + (l - iL) * dir1.y));

		if (dr1.Length() == 0.0f || iL == 0.0f)
		{
			return 0.0f;
		}

		d1 = abs(dr1.Normalize().Dot(dir2));
		d2 = abs(dr2.Normalize().Dot(dir2));
		d3 = abs(dr3.Normalize().Dot(dir2));

		d = d1;

		if (d2 > d1 && d2 > d3)
		{
			l = l + iL;
			d = d2;
		}
		else if (d3 > d1)
		{
			l = l - iL;
			d = d3;
		}

		iL = 0.5f * iL;

		if (d > 0.996f)
		{
			B = false;
		}
	}

	return l;
}

void GenerateRiver::Generate(int height, int width)
{
	std::vector<BYTE4> HM2;
	std::vector<BYTE4> RM;
	DataStreaming::LoadImageFromFile(GetWholeFilePatch(L"rivers.png"), &RM, m_height, m_width);

	m_HMap.resize(RM.size());
	HM2.resize(RM.size(), BYTE4(0, 0, 0, 0));
	DataStreaming::LoadFile(GetWholeFilePatch("LevelHeight.raw"), &m_HMap[0], m_height, m_width, sizeof(float));

	int NR = 0;

	for (int i = 0; i < RM.size(); ++i)
	{
		RM[i] = BYTE4(RM[i].x, RM[i].y, RM[i].z, RM[i].w);

		if (RM[i].z == 255)
		{
			if (NR < RM[i].y) NR = RM[i].y;
		}
	}

	NR += 1;

	std::vector<int> LRS(NR, 0.0f);

	for (int i = 0; i < RM.size(); ++i)
	{
		if (RM[i].z == 255)
		{
			if (LRS[RM[i].y] < RM[i].x) LRS[RM[i].y] = RM[i].x;
		}
	}

	std::vector<std::vector<INT2>> RS(LRS.size());

	for (int i = 0; i < LRS.size(); ++i)
	{
		LRS[i] += 1;
		RS[i].resize(LRS[i]);
	}

	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			int j = y * m_width + x;

			if (RM[j].z == 255)
			{
				for (int i = 0; i < LRS[RM[j].y]; ++i)
				{
					RS[RM[j].y][RM[j].x] = INT2(x, y);
				}
			}
		}
	}

	m_Rivers.resize(RM.size(), Float4(0.0f, 0.0f, 0.0f, 0.0f));

	for (int i = 0; i < RS.size(); ++i)
	{
		float s = 1.0f / (RS[i].size());

		for (int j = 0; j < (RS[i].size() - 1); ++j)
		{
			Float2 V1 = Float2(RS[i][j].x, RS[i][j].y);
			Float2 V2 = Float2(RS[i][j + 1].x, RS[i][j + 1].y);

			RS[i][j + 1] = GetRiverOffset(V1, V2);
			V2 = Float2(RS[i][j + 1].x, RS[i][j + 1].y);

			float id = (float)i / ((float)RS.size() - 1.0f);

			m_Rivers[((int)(V1.y + 0.5f)) * m_width + ((int)(V1.x + 0.5f))] = Float4(s * (float)j, id, 1.0f, 1.0f);
			m_Rivers[((int)(V2.y + 0.5f)) * m_width + ((int)(V2.x + 0.5f))] = Float4(s * ((float)j + 1.0f), id, 1.0f, 1.0f);


			GeneratePixelOfRiver(V1, V2, s * (float)j, s * ((float)j + 1.0f), id);

			float D = -1.0f;

			if (j == (RS[i].size() - 2))
			{
				int dx = -1;
				int dy = -1;

				for (int y = -10; y <= 10; ++y)
				{
					for (int x = -10; x <= 10; ++x)
					{
						float D2 = pow(pow((float)x, 2.0f) + pow((float)y, 2.0f), 0.5f);

						int sx = (V2.x + x);
						int sy = (V2.y + y);

						if (sx >= 0 && sx < m_width && sy >= 0 && sy < m_height)
						{
							int k = sy * m_width + sx;

							if (D == -1.0f || D2 < D)
							{
								if (m_Rivers[k].w != 0.0f && m_Rivers[k].y != id)
								{
									D = D2;
									dx = sx;
									dy = sy;
								}
							}
						}
					}
				}

				if (dx != -1)
				{
					GeneratePixelOfRiver(V2, Float2(dx, dy), s * ((float)j + 1.0f), s * ((float)j + 2.0f), id);
				}
			}
		}
	}

	m_Rivers2.resize(RM.size(), Float4(0.0f, 0.0f, 0.0f, 0.0f));

	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			int i = y * m_width + x;
			if (m_Rivers[i].w == 1.0f && m_Rivers[i].x == 0.0f)
			{
				m_Rivers2[i] = m_Rivers[i];

				ClampRiver(x, y, m_Rivers[i].x, m_Rivers[i].y);
			}
		}
	}

	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			int i = y * m_width + x;
			if (m_Rivers[i].w == 1.0f && m_Rivers[i].x == 0.0f)
			{
				m_Rivers2[i] = m_Rivers[i];

				CreateRiverNode(x, y, m_Rivers[i].x, m_Rivers[i].y, width / m_width);
			}


			BYTE H = (BYTE)(0.04f *(m_HMap[i] + 2200.0f));

			HM2[i] = BYTE4(H, H, H, 255);
		}
	}

	DataStreaming::SaveImage(GetWholeFilePatch(L"output.png"), &m_Rivers2, m_height, m_width);
	DataStreaming::SaveImage(GetWholeFilePatch(L"outputHeight.png"), &HM2, m_height, m_width);

	GenerateRiverNode(1.42f);

	GenerateRiverMap(width, height, 3);
}