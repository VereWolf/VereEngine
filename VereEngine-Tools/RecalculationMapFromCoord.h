#pragma once

#include "stdafx.h"
#include "BaseForTool.h"

class RecalculationMapFromCoord : public BaseForTool
{
public:
	RecalculationMapFromCoord();
<<<<<<< HEAD
	Float2 GetReCalCoord(Float2 coord, std::vector<Float2> coordMap, Float2 mapSize, Float2 sourceSize);
	void GenerateResize();
	void Generate();
	void GenerateTiles();
=======
	Float2 GetReCalCoord(Float2 coord, std::vector<Float2> *coordMap, Float2 mapSize, Float2 sourceSize);
	void GenerateResize();
	void Generate();
	void GenerateTiles();
	void ConvertNormalFromPNGtoRAW();
>>>>>>> VereEngine-Planet
private:
};