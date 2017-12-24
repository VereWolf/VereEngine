// VereEngine-Tools.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "RecalculationMapFromCoord.h"
#include "GenerateTrees.h"
#include "GenerateRiver.h"
#include "WaterErosion.h"


int main()
{
	//RecalculationMapFromCoord RC;
	//RC.Generate();
	//RC.GenerateTiles();
	//RC.GenerateResize();
	//RC.ConvertNormalFromPNGtoRAW();

	//GenerateTrees GT;

	//GT.Generate(17983);

	//GenerateRiver RG;
	//RG.Generate(8192, 8192);

	WaterErosion WE;
	WE.Generate(174631);

    return 0;
}

