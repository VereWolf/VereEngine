#include "pch.h"
#include "StreamingDataManager.h"
#include "GameStreamingData.h"
#include "GameDataSet.h"

StreamingDataManager::StreamingDataManager()
{
	GameStreamingDataHandle = this;
}

StreamingDataManager::StreamingDataManager(DX::DeviceResources *resources)
{
	GameStreamingDataHandle = this;

	Init(resources);
}

StreamingDataManager::~StreamingDataManager()
{

}

void StreamingDataManager::Init(DX::DeviceResources *resources)
{
	m_resources = resources;

	m_streamingData.m_gameData.Init(resources);
	m_streamingData.m_gameDataSet.Init(resources);
}

int StreamingDataManager::LoadData(string nameFile)
{
	std::ifstream fin(nameFile, std::ios::binary);

	if (!fin)
	{
		return -1;
	}

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();

	if (size <= 0)
	{
		return -1;
	}

	std::vector<char> *dataBuffer = new std::vector<char>();

	dataBuffer->resize(size);

	fin.read(&dataBuffer->at(0), size);

	return CreateStreamingData(dataBuffer);
}

bool StreamingDataManager::SaveData(string nameFile, int id)
{
	std::ofstream fout(nameFile, std::ios::binary);

	if (!fout)
	{
		return false;
	}

	std::vector<char> dataBuffer;
	std::vector<char> *data = GetStreamingData(id);

	fout.write(&data->at(0), data->size());

	return true;
}

int StreamingDataManager::LoadDataSet(string nameFile)
{
	std::ifstream fin(nameFile, std::ios::binary);

	if (!fin)
	{
		return -1;
	}

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();

	if (size <= 0)
	{
		return -1;
	}

	std::vector<char> dataBuffer;

	dataBuffer.resize(size);

	fin.read(&dataBuffer[0], size);

	int numDataBlock;
	UINT currentData = 0;

	memcpy(&numDataBlock, &dataBuffer[0], sizeof(numDataBlock));
	currentData += sizeof(numDataBlock);

	int id = CreateDataSet();
	SetDataSetSize(id, numDataBlock);

	std::vector<UINT> S(numDataBlock);

	for (int i = 0; i < S.size(); ++i)
	{
		memcpy(&S[i], &dataBuffer[numDataBlock], sizeof(UINT));
		numDataBlock += sizeof(UINT);
	}

	for (int i = 0; i < S.size(); ++i)
	{
		std::vector<char> *data = new std::vector<char>;
		data->resize(S[i]);
		memcpy(&data[0], &dataBuffer[numDataBlock], S[i]);

		SetDataSet(id, i, CreateStreamingData(data));
	}

	return id;
}

bool StreamingDataManager::SaveDataSet(string nameFile, std::vector<int> *IDs)
{
	std::ofstream fout(nameFile, std::ios::binary);

	if (!fout)
	{
		return false;
	}

	std::vector<char> dataBuffer;
	int S = IDs->size();
	UINT dataSize = sizeof(int) + S * sizeof(UINT);

	for (int i = 0; i < S; ++i)
	{
		dataSize += GetStreamingData(IDs->at(i))->size();
	}

	dataBuffer.resize(dataSize);

	memcpy(&dataBuffer[0], &S, sizeof(S));
	dataSize = sizeof(S);

	UINT S2;

	for (int i = 0; i < S; ++i)
	{
		S2 = GetStreamingData(IDs->at(i))->size();
		memcpy(&dataBuffer[dataSize], &S2, sizeof(S2));
		dataSize += sizeof(S2);
	}

	for (int i = 0; i < S; ++i)
	{
		std::vector<char> *data = GetStreamingData(IDs->at(i));
		memcpy(&dataBuffer[dataSize], &data->at(0), data->size());
		dataSize += data->size();
	}

	fout.write(&dataBuffer[0], dataBuffer.size());

	return true;
}

void StreamingDataManager::DeleteDataInDataSet(int id)
{
	int S = GetDataSetSize(id);

	for (int i = 0; i < S; ++i)
	{
		DeleteStreamingData(GetDataSet(id, i));
	}
}

int StreamingDataManager::CreateDataSet()
{
	GameDataSet *gameDataSet = new GameDataSet;

	return m_streamingData.m_gameData.CreateGameObject(gameDataSet);
}

void StreamingDataManager::DeleteDataSet(int id)
{
	m_streamingData.m_gameDataSet.DeleteGameObject(id);
}

int StreamingDataManager::GetDataSetSize(int id)
{
	return ((GameDataSet*)m_streamingData.m_gameDataSet.GetGameObject(id))->GetDataSetSize();
}

void StreamingDataManager::SetDataSetSize(int id, int size)
{
	((GameDataSet*)m_streamingData.m_gameDataSet.GetGameObject(id))->SetDataSetSize(size);
}

void StreamingDataManager::SetDataSet(int id, int index, int value)
{
	((GameDataSet*)m_streamingData.m_gameDataSet.GetGameObject(id))->SetDataSet(index, value);
}

int StreamingDataManager::GetDataSet(int id, int index)
{
	return ((GameDataSet*)m_streamingData.m_gameDataSet.GetGameObject(id))->GetDataSet(index);
}

int  StreamingDataManager::CreateStreamingData(std::vector<char> *data)
{
	GameStreamingData *gameStreamingData = new GameStreamingData;
	gameStreamingData->PreInit(m_resources);
	gameStreamingData->SetStreamingData(data);

	return m_streamingData.m_gameData.CreateGameObject(gameStreamingData);
}

void StreamingDataManager::DeleteStreamingData(int id)
{
	m_streamingData.m_gameData.DeleteGameObject(id);
}

std::vector<char>* StreamingDataManager::GetStreamingData(int id)
{
	if (id >= 0)
	{
		return ((GameStreamingData*)m_streamingData.m_gameData.GetGameObject(id))->GetStreamingData();
	}

	return NULL;
}

void StreamingDataManager::CylinderMapToSquareMap(
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
	btMatrix3x3 *blockMatrixs[12])
{
	for (int i = 0; i < 6; ++i)
	{
		for (float y = 0; y < squareSide; y += 1.0f)
		{
			for (float x = 0; x < squareSide; x += 1.0f)
			{
				btVector3 coord = PlanetCordinateMat::GetCoordForCylinder((btVector3(x - 0.5 * squareSide, 0.5 * squareSide, y - 0.5 * squareSide) * *blockMatrixs[i]).normalize());
				coord = btVector3(((coord.getX() - offsetOfMapinCylinderX) / ratioOfMapInCylinderX) * widthOfMap * stride, ((coord.getY() - offsetOfMapinCylinderX) / ratioOfMapInCylinderY) * heightOfMap * stride, 0.0);

				memcpy(&GetStreamingData(idOfSideOfSquareMap[i])->at(y * pow(stride, 2) * squareSide + x* stride), &GetStreamingData(idOfCylinderMap)->at(coord.getY() * widthOfMap * stride + coord.getX()), stride);
			}
		}
	}
};
