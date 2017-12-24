#include "stdafx.h"
#include "WaterErosion.h"

WaterErosion::WaterErosion()
{
	mHeight = 480;
	mWidth = 640;
	mImageFromFile.resize(480 * 640, Float4(0.2f, 0.2f, 0.2f, 1.0f));
	SetFolderName(L"Water_Erosion_Generation");
}
	
void WaterErosion::RainWrinkle(int itsX, int itsY, float itsLeft, float maxLeft)
{
	float min = -1;
	int sx, sy;

	//mImage.at((itsY) * mWidth + itsX).y += 1;

	float f = (maxLeft - itsLeft) / maxLeft;

	if (f > mImage.at((itsY)* mWidth + itsX).z)
	{
		mImage.at((itsY)* mWidth + itsX).z = f;
	}

	for (int y = -1; y <= 1; ++y)
	{
		for (int x = -1; x <= 1; ++x)
		{
			if ((y + itsY) >= 0 && (y + itsY) < mHeight && (x + itsX) >= 0 && (x + itsX) < mWidth)
			{
				if (min == -1.0f || min > mImage.at((y + itsY) * mWidth + x + itsX).y)
				{
					min = mImage.at((y + itsY) * mWidth + x + itsX).y;
					sx = x + itsX;
					sy = y + itsY;
				}
			}
		}
	}

	if (itsLeft > 0 && min != mImage.at((itsY)* mWidth + itsX).y)
	{
		RainWrinkle(sx, sy, itsLeft - 1.0f, maxLeft);
	}
}

void WaterErosion::RainWrinkleComplete(float itsLeft, float maxLeft)
{
	for (int i = 0; i < mSize; ++i)
	{
		mImage.at(i).z = 0.0f;
	}

	for (int y = 0; y < mHeight; ++y)
	{
		for (int x = 0; x < mWidth; ++x)
		{
			RainWrinkle(x, y, itsLeft, maxLeft);
		}
	}

	float m = 0;;
	float n = 0;

	std::vector<float> itsImg;
	itsImg.resize(mSize);

	for (int i = 0; i < mSize; ++i)
	{
		itsImg.at(i) = mImage.at(i).z;
	}

	for (int y = 0; y < mHeight; ++y)
	{
		for (int x = 0; x < mWidth; ++x)
		{
			m = 0;
			n = 0;

			for (int sy = -1; sy <= 1; ++sy)
			{
				for (int sx = -1; sx <= 1; ++sx)
				{
					if ((x + sx) >= 0 && (x + sx) < mWidth && (y + sy) >= 0 && (y + sy) < mHeight)
					{
						++n;
						m += itsImg.at((y + sy) * mWidth + x + sx);
					}
				}
			}
			m /= (float)n;

			m = m + 0.0f * (mImage.at(y * mWidth + x).z - m);

			mImage.at(y * mWidth + x).z = m;
		}
	}

	m = 0;

	for (int i = 0; i < mSize; ++i)
	{
		if (mImage.at(i).z > m)
		{
			m = mImage.at(i).z;
		}
	}

	for (int i = 0; i < mSize; ++i)
	{
		mImage.at(i).z /= m;
	}
}

void WaterErosion::Generate(int randomValue)
{
	std::vector<Float4> P;
	DataStreaming::LoadImageFromFile(GetWholeFilePatch(L"perlin.png"), &P, mHeight, mWidth);

	 std::vector<Float4> RT;
	DataStreaming::LoadImageFromFile(GetWholeFilePatch(L"rocky_terrain.png"), &RT, mHeight, mWidth);

	std::vector<Float4> WE;
	DataStreaming::LoadImageFromFile(GetWholeFilePatch(L"water_erosion.png"), &WE, mHeight, mWidth);

	mSize = mHeight * mWidth;

	std::vector<float> heightMap(mSize);

	DataStreaming::LoadFile(GetWholeFilePatch("LevelHeight.raw"),&heightMap[0], mHeight, mWidth, sizeof(float));

	mImage.resize(mSize);

	for (int i = 0; i < mSize; ++i)
	{
		float v = heightMap[i];
		mImage.at(i) = Float4(v, v, v, v);
	}

	RainWrinkleComplete(12.0f, 12.0f);

	float B = 0.20f;
	int C = 0;
	float S = 500.0f;
	float M = 0.66f;
	float IH = 1000.0f;

	std::vector <float > vect;

	for (int i = 0; i < mSize; ++i)
	{
		float v = B * (1.0f - heightMap[i]) * mImage.at(i).x + (1.0f - B) * mImage.at(i).x;
		v += RT[i].x * (VMath::Clamp(heightMap[i], M, 1.0f) - M) / (1.0f - M) * S * P.at(i).x * pow(1.0f - mImage.at(i).z, 2.0f);
		heightMap[i] = heightMap[i] * v;
	}

	mHeight = mHeight - C;
	mWidth = mWidth - C;
	mSize = mHeight * mWidth;

	DataStreaming::SaveFile(GetWholeFilePatch("output.raw"), &heightMap[0], mHeight, mWidth, sizeof(float));
	//SaveImage(L"Rocky.png");
}