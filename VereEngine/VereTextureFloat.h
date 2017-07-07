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
	}

	void Init(void *map, int width, int height)
	{
		m_width = width;
		m_height = height;
		m_map.resize(width * height);
		memcpy(&m_map[0], map, sizeof(float) * m_map.size());
	}

	void *GetTexture() { return &m_map[0]; }

	float GetVariable(float x, float y)
	{
		if (m_map.size() != 0)
		{
			x = VereMath::Clamp(((float)m_width - 1.0f) * x, 0.0f, m_width - 1);
			y = VereMath::Clamp(((float)m_height - 1.0f) * y, 0.0f, m_height - 1);

			float lx = x - (float)((int)x);
			float ly = y - (float)((int)y);

			float x1y1;
			float x2y1;
			float x1y2;
			float x2y2;

			if (lx == 0.0f && ly == 0.0f)
			{
				x1y1 = m_map.at(((int)y) * m_width + ((int)x));
				x2y1 = m_map.at(((int)y) * m_width + ((int)x));
				x1y2 = m_map.at(((int)y) * m_width + ((int)x));
				x2y2 = m_map.at(((int)y) * m_width + ((int)x));
			}
			else if (lx == 0.0f)
			{
				x1y1 = m_map.at(((int)y) * m_width + ((int)x));
				x2y1 = m_map.at(((int)y) * m_width + ((int)x));
				x1y2 = m_map.at(VereMath::Clamp(((int)y) + 1, 0, m_height - 1) * m_width + ((int)x));
				x2y2 = m_map.at(VereMath::Clamp(((int)y) + 1, 0, m_height - 1) * m_width + ((int)x));
			}
			else if (ly == 0.0f)
			{
				x1y1 = m_map.at(((int)y) * m_width + ((int)x));
				x2y1 = m_map.at(((int)y) * m_width + VereMath::Clamp(((int)x) + 1, 0, m_width - 1));
				x1y2 = m_map.at(((int)y) * m_width + ((int)x));
				x2y2 = m_map.at(((int)y) * m_width + VereMath::Clamp(((int)x) + 1, 0, m_width - 1));
			}
			else
			{
				x1y1 = m_map.at(((int)y) * m_width + ((int)x));
				x2y1 = m_map.at(((int)y) * m_width + VereMath::Clamp(((int)x) + 1, 0, m_width - 1));
				x1y2 = m_map.at(VereMath::Clamp(((int)y) + 1, 0, m_height - 1) * m_width + ((int)x));
				x2y2 = m_map.at(VereMath::Clamp(((int)y) + 1, 0, m_height - 1) *m_width + VereMath::Clamp(((int)x) + 1, 0, m_width - 1));
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
		float S = (height + 2) * (width + 2);
		std::vector<float> M(S);
		float L2 = 1.0f / pow(2, lvl);
		float H2 = 1.0f / (height + 2);
		float W2 = 1.0f / (width + 2);
		float OC = (height) / (height + 2.0f);

		XMFLOAT2 OF = XMFLOAT2(offset.x * L2 * OC, offset.y * L2 * OC);
		XMFLOAT2 INC = XMFLOAT2(L2 * W2, L2 * H2);

		XMFLOAT2 I = XMFLOAT2(OF.x + W2 - INC.x, OF.y + H2 - INC.y);

		for (int y = 0; y < (height + 2); ++y)
		{
			for (int x = 0; x < (width + 2); ++x)
			{
				M[y * (width + 2) + x] = GetVariable(I.x, I.y);

				I.x += INC.x;
			}

			I.x = OF.x + H2 - INC.y;
			I.y += INC.y;
		}

		VereTextureFloat *T = new VereTextureFloat;
		T->Init(&M[0], width + 2, height + 2);

		return T;
	}

	int GetHeight() { return m_height; }
	int GetWidth() { return m_width; }
private:
	std::vector<float> m_map;
	int m_height;
	int m_width;
};

#endif // VERETEXTURE_FLOAT_H