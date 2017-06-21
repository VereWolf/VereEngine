#ifndef VERETEXTUE_FLOAT_H
#define VERETEXTURE_FLOAT_H

#include "pch.h"

class VereTextureFloat
{
public:
	VereTextureFloat() {}
	VereTextureFloat(const VereTextureFloat & other)
	{
		m_map = other.m_map;
		m_width = other.m_width;
		m_height = other.m_height;
	}

	~VereTextureFloat()
	{
		delete m_map;
	}

	void Init(std::vector <float> *map, int width, int height)
	{
		m_width = width;
		m_height = height;
		m_map = map;
	}

	float GetVariable(float x, float y)
	{
		if (m_map != NULL)
		{
			x = ((float)m_width - 1.0f) * x;
			y = ((float)m_height - 1.0f) * y;

			float lx = x - (float)((int)x);
			float ly = y - (float)((int)y);

			float x1y1;
			float x2y1;
			float x1y2;
			float x2y2;

			if (lx == 0.0f && ly == 0.0f)
			{
				x1y1 = m_map->at(((int)y) * m_width + ((int)x));
				x2y1 = m_map->at(((int)y) * m_width + ((int)x));
				x1y2 = m_map->at(((int)y) * m_width + ((int)x));
				x2y2 = m_map->at(((int)y) * m_width + ((int)x));
			}
			else if (lx == 0.0f)
			{
				x1y1 = m_map->at(((int)y) * m_width + ((int)x));
				x2y1 = m_map->at(((int)y) * m_width + ((int)x));
				x1y2 = m_map->at(((int)y + 1) * m_width + ((int)x));
				x2y2 = m_map->at(((int)y + 1) * m_width + ((int)x));
			}
			else if (ly == 0.0f)
			{
				x1y1 = m_map->at(((int)y) * m_width + ((int)x));
				x2y1 = m_map->at(((int)y) * m_width + ((int)x) + 1);
				x1y2 = m_map->at(((int)y) * m_width + ((int)x));
				x2y2 = m_map->at(((int)y) * m_width + ((int)x + 1));
			}
			else
			{
				x1y1 = m_map->at(((int)y) * m_width + ((int)x));
				x2y1 = m_map->at(((int)y) * m_width + ((int)x) + 1);
				x1y2 = m_map->at(((int)y + 1) * m_width + ((int)x));
				x2y2 = m_map->at(((int)y + 1) * m_width + ((int)x + 1));
			}

			float v1 = x1y1 + lx * (x2y1 - x1y1);
			float v2 = x1y2 + lx * (x2y2 - x1y2);
			v1 = v1 + ly * (v2 - v1);

			return v1;
		}

		return 0;
	}

	VereTextureFloat *CreateNewTextureFrom(float lvl, XMFLOAT2 offset, float height, float width)
	{
		float S = height * width;
		std::vector<float> *M = new std::vector<float>;

		M->resize(S);

		float L2 = 1.0 / pow(2.0, lvl);
		XMFLOAT2 OF = XMFLOAT2(offset.x * L2, offset.y * L2);
		XMFLOAT2 INC = XMFLOAT2(lvl / width, lvl / height);

		int H = m_height;
		int W = m_width;
		XMFLOAT2 I = XMFLOAT2(OF.x, OF.y);

		for (int y = 0; y < H; ++y)
		{
			for (int x = 0; x < W; ++x)
			{
				M->at(y * W + x) = GetVariable(I.x, I.y);

				I.x += INC.x;
			}

			I.x = OF.x;
			I.y += INC.y;
		}

		VereTextureFloat *T = new VereTextureFloat;
		T->Init(M, width, height);

		return T;
	}

	int GetHeight() { return m_height; }
	int GetWidth() { return m_width; }
private:
	std::vector<float> *m_map;
	int m_height;
	int m_width;
};

#endif // VERETEXTURE_FLOAT_H