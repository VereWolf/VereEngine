#pragma once

#include "stdafx.h"
#include "BaseForTool.h"

class RecalculationRiverMapFromCoord : public BaseForTool
{
public:
	RecalculationRiverMapFromCoord();
	Float2 GetReCalCoord(Float2 coord, std::vector<Float2> *coordMap, Float2 mapSize, Float2 sourceSize);
	void GenerateResize();
	void GenerateTiles();
	void ConvertNormalFromPNGtoRAW();
private:
};