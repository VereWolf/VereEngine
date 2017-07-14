#pragma once

#include "stdafx.h"
#include <limits>
#include "BaseForTool.h"

using namespace std;

class WaterErosion :public BaseForTool
{
public:
	WaterErosion();
	~WaterErosion() {};
	void RainWrinkle(int itsX, int itsY, float itsLeft, float maxLeft);
	void RainWrinkleComplete(float itsLeft, float maxLeft);
	
	void Generate(int randomValue);
private:

	vector<Float4> mImage;
	float mSize;

	vector<Float4> mImageFromFile;
	int mHeight;
	int mWidth;

	std::vector<float> m_waterLevel;
	std::vector<int> m_isWater;
	float m_lowerWaterLevel;
	float m_originLowerWaterLevel;
	INT2 m_lowerWaterLvlCoord;
};