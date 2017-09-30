// VereEngine-Tools.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "RecalculationMapFromCoord.h"
#include "GenerateTrees.h"


int main()
{
	RecalculationMapFromCoord RC;
	RC.Generate();
	//RC.GenerateTiles();
	//RC.GenerateResize();
	//RC.ConvertNormalFromPNGtoRAW();

	//GenerateTrees GT;

	//GT.Generate(17983);

    return 0;
}

