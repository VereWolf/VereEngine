#pragma once

#include "pch.h"
#include "DeviceResources.h"
#include "GameBaseObjectsStack.h"
#include "GameStreamingData.h"
#include "GameDataSet.h"


class StreamingDataManager
{
public:
	StreamingDataManager();
	StreamingDataManager(DX::DeviceResources *resources);
	~StreamingDataManager();

	void Init(DX::DeviceResources *resources);

	int LoadData(string nameFile);
	//bool SaveData(Platform::String ^nameFile, int id);
	bool SaveData(string nameFile, int id);

	/*int LoadDataSet(string nameFile);
	bool SaveDataSet(string nameFile, std::vector<int> *IDs);
	void DeleteDataInDataSet(int id);

	int CreateDataSet();
	void DeleteDataSet(int id);
	int GetDataSetSize(int id);
	void SetDataSetSize(int id, int size);
	void SetDataSet(int id, int index, int value);
	int GetDataSet(int id, int index);*/

	int CreateStreamingData(void *data, int size);
	void DeleteStreamingData(int id);
	void  *GetStreamingData(int id);
	int GetSizeOfStreamingData(int id);

	/*void CylinderMapToSquareMap(
		btScalar offsetOfMapinCylinderX, //ration
		btScalar offsetOfMapinCylinderY, //ratio
		btScalar ratioOfMapInCylinderX,
		btScalar ratioOfMapInCylinderY,
		btScalar squareSide,
		btScalar heightOfMap,
		btScalar widthOfMap,
		std::vector<int> idOfSideOfSquareMap,
		int idOfCylinderMap,
		float stride,
		btMatrix3x3 *blockMatrixs[12]);*/
private:
	DX::DeviceResources *m_resources;

	StreamingDataAssetsStacks m_streamingData;

	Windows::Foundation::IAsyncAction^ m_Worker;
};

__declspec(selectany) StreamingDataManager *GameStreamingDataHandle = 0;

