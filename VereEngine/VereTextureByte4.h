#pragma once

#include "pch.h"

struct VBYTE4
{
	VBYTE4()
	{
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}

	VBYTE4(BYTE itsX, BYTE itsY, BYTE itsZ, BYTE itsW)
	{
		x = itsX;
		y = itsY;
		z = itsZ;
		w = itsW;
	}

	inline VBYTE4 operator+ (const VBYTE4 &v)
	{
		return VBYTE4(x + v.x,
			y + v.y,
			z + v.z,
			w + v.w);
	}

	inline VBYTE4 operator- (const VBYTE4 &v)
	{
		return VBYTE4(x - v.x,
			y - v.y,
			z - v.z,
			w - v.w);
	}

	inline VBYTE4 operator* (const float &v)
	{
		return VBYTE4((float)x * v,
			(float)y * v,
			(float)z * v,
			(float)w * v);
	}

	inline VBYTE4 operator/ (const float &v)
	{
		return VBYTE4((float)x / v,
			(float)y / v,
			(float)z / v,
			(float)w / v);
	}

	BYTE x;
	BYTE y;
	BYTE z;
	BYTE w;
};

class VereTextureBYTE4
{
public:
	VereTextureBYTE4() {}
	VereTextureBYTE4(const VereTextureBYTE4 & other)
	{
		m_map = other.m_map;
		m_width = other.m_width;
		m_height = other.m_height;
	}
	~VereTextureBYTE4()
	{
	}

	void Init(void *map, int width, int height)
	{
		m_width = width;
		m_height = height;
		m_map.resize(width * height);
		memcpy(&m_map[0], map, sizeof(VBYTE4) * m_map.size());
	}

	void *GetTexture() { return &m_map[0]; }

	VBYTE4 GetVariable(float x, float y)
	{
		x = VereMath::Clamp(((float)m_width - 1.0f) * x, 0.0f, (float)m_width - 1.0f);
		y = VereMath::Clamp(((float)m_height - 1.0f) * y, 0.0f, (float)m_height - 1.0f);

		float lx = x - (float)((int)x);
		float ly = y - (float)((int)y);

		VBYTE4 x1y1;
		VBYTE4 x2y1;
		VBYTE4 x1y2;
		VBYTE4 x2y2;

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

		XMFLOAT4 v1;
		XMFLOAT4 v2;

		XMFLOAT4 x1y1F = XMFLOAT4(x1y1.x, x1y1.y, x1y1.z, x1y1.w);
		XMFLOAT4 x2y1F = XMFLOAT4(x2y1.x, x2y1.y, x2y1.z, x2y1.w);
		XMFLOAT4 x1y2F = XMFLOAT4(x1y2.x, x1y2.y, x1y2.z, x1y2.w);
		XMFLOAT4 x2y2F = XMFLOAT4(x2y2.x, x2y2.y, x2y2.z, x2y2.w);

		v1.x = x1y1F.x + lx * (x2y1F.x - x1y1F.x);
		v1.y = x1y1F.y + lx * (x2y1F.y - x1y1F.y);
		v1.z = x1y1F.z + lx * (x2y1F.z - x1y1F.z);
		v1.w = x1y1F.w + lx * (x2y1F.w - x1y1F.w);

		v2.x = x1y2F.x + lx * (x2y2F.x - x1y2F.x);
		v2.y = x1y2F.y + lx * (x2y2F.y - x1y2F.y);
		v2.z = x1y2F.z + lx * (x2y2F.z - x1y2F.z);
		v2.w = x1y2F.w + lx * (x2y2F.w - x1y2F.w);

		v1.x = v1.x + ly * (v2.x - v1.x);
		v1.y = v1.y + ly * (v2.y - v1.y);
		v1.z = v1.z + ly * (v2.z - v1.z);
		v1.w = v1.w + ly * (v2.w - v1.w);

		return VBYTE4(v1.x, v1.y, v1.z, v1.w);
	}

	void SetVariable(int x, int y, VBYTE4 V)
	{
		m_map[y * m_width + x] = V;
	}
	
	VereTextureBYTE4 *CreateNewTextureFrom(float lvl, XMFLOAT2 offset, float seam, float height, float width)
	{
		float S = (height + seam) * (width + seam);
		std::vector<VBYTE4> M(S);
		float L2 = 1.0f / pow(2, lvl);
		float H2 = 1.0f / (height + seam);
		float W2 = 1.0f / (width + seam);
		float OC = (height) / (height + seam);

		XMFLOAT2 OF = XMFLOAT2(offset.x * L2 * OC, offset.y * L2 * OC);
		XMFLOAT2 INC = XMFLOAT2(L2 * W2, L2 * H2);

		XMFLOAT2 I = XMFLOAT2(OF.x + W2 - INC.x, OF.y + H2 - INC.y);

		for (int y = 0; y < (height + seam); ++y)
		{
			for (int x = 0; x < (width + seam); ++x)
			{
				M[y * (width + seam) + x] = GetVariable(I.x, I.y);

				I.x += INC.x;
			}

			I.x = OF.x + H2 - INC.y;
			I.y += INC.y;
		}

		VereTextureBYTE4 *T = new VereTextureBYTE4;
		T->Init(&M[0], width + seam, height + seam);

		return T;
	}

	int GetHeight() { return m_height; }
	int GetWidth() { return m_width; }
private:
	std::vector<VBYTE4> m_map;
	int m_height;
	int m_width;
};