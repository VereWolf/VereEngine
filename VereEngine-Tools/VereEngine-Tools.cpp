// VereEngine-Tools.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "RecalculationMapFromCoord.h"


int main()
{
	RecalculationMapFromCoord RC;
	RC.Generate();
	RC.GenerateTiles();
	//RC.GenerateResize();
	//RC.ConvertNormalFromPNGtoRAW();
    return 0;
}

