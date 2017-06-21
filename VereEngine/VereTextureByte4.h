#ifndef VERETEXTUE_BYTE4_H
#define VERETEXTURE_BYTE4_H

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
		delete m_map;
	}

	void Init(std::vector <VBYTE4> *map, int width, int height)
	{
		m_width = width;
		m_height = height;
		m_map = map;
	}

	VBYTE4 GetVariable(float x, float y)
	{
		x = (float)m_width * x;
		y = (float)m_height * y;

		float lx = x - (float)((int)x);
		float ly = y - (float)((int)y);

		VBYTE4 x1y1;
		VBYTE4 x2y1;
		VBYTE4 x1y2;
		VBYTE4 x2y2;

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

		VBYTE4 v1 = x1y1 + (x2y1 - x1y1) * lx;
		VBYTE4 v2 = x1y2 + (x2y2 - x1y2) * lx;
		v1 = v1 + (v2 - v1) * ly;

		return v1;
	}
	
	VereTextureBYTE4 *CreateNewTextureFrom(float lvl, XMFLOAT2 offset, float height, float width)
	{
		float S = height * width;
		std::vector<VBYTE4> *M = new std::vector<VBYTE4>;

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

		VereTextureBYTE4 *T = new VereTextureBYTE4;
		T->Init(M, width, height);

		return T;
	}

	int GetHeight() { return m_height; }
	int GetWidth() { return m_width; }
private:
	std::vector<VBYTE4> *m_map;
	int m_height;
	int m_width;
};

#endif // VERETEXTURE_BYTE4_H