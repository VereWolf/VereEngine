#pragma once

#include "pch.h"
#include "DeviceResources.h"
#include "GameBaseObjectsStack.h"
#include "GameStreamingData.h"
#include "GameDataSet.h"

/*union UIntUnion
{
	char chars[sizeof(UINT)];
	UINT value;
};

union UFloatUnion
{
	char chars[sizeof(float)];
	float value;
};

union UDoubleUnion
{
	char chars[8];
	double value;
};

class StreamingData
{
public:
	StreamingData() {}
	~StreamingData() {}
	bool LoadFile(string nameFile)
	{
		std::ifstream fin(nameFile, std::ios::binary);

		if (!fin)
		{
			return false;
		}

		fin.seekg(0, std::ios_base::end);
		int size = (int)fin.tellg();

		if (size <= 0)
		{
			return false;
		}

		m_dataBuffer.resize(size);

		fin.read(&m_dataBuffer[0], size);

		index = 0;

		return true;
	}
	bool SaveFile(string nameFile)
	{
		std::ofstream fout(nameFile, std::ios::binary);

		if (!fout)
		{
			return false;
		}

		fout.write(&m_dataBuffer[0], m_dataBuffer.size());

		return true;
	}

	void GetData()
	{
		UINT size = GetUInt(index);
		index += sizeof(UINT);

		m_secondaryDataBuffer.resize(size);

		CopyMemory(&m_secondaryDataBuffer[0], &m_dataBuffer[index], m_secondaryDataBuffer.size());

		index += m_secondaryDataBuffer.size();
	}
	void GetDataWithZeroCompression()
	{
		BYTE isZeros;
		UINT size = GetUInt(index);
		index += sizeof(UINT);
		UINT endValue = index + size;

		m_secondaryDataBuffer.resize(size);

		UINT I = 0;
		while (index < m_dataBuffer.size())
		{
			isZeros = m_dataBuffer[index];

			++index;

			if (isZeros == 0)
			{
				size = GetUInt(index);

				index += sizeof(UINT);

				ZeroMemory(&m_secondaryDataBuffer[I], size);
				++index;
				I += size;
			}
			else if (isZeros == 1)
			{
				size = GetUInt(index);

				index += sizeof(UINT);

				CopyMemory(&m_secondaryDataBuffer[I], &m_dataBuffer[index], size);
				index += size;
				I += size;
			}
			else
			{
				break;
			}
		}


	}

	void SetData()
	{
		m_dataBuffer.resize(m_dataBuffer.size() + m_secondaryDataBuffer.size());

		SetUInt(index, m_secondaryDataBuffer.size());

		index += sizeof(UINT);

		CopyMemory(&m_dataBuffer[index], &m_secondaryDataBuffer[0], m_secondaryDataBuffer.size());

		index += m_secondaryDataBuffer.size();
	}

	void SetDataWithZeroCompression()
	{
		BYTE isZeroL;
		BYTE isZeroP;
		int I = 0;
		std::vector<char> buffer;
		buffer.resize(128);

		isZeroL = m_secondaryDataBuffer[0];
		for (int i = 0; i < m_secondaryDataBuffer.size(); i)
		{
			isZeroP = m_secondaryDataBuffer[i];
			if (isZeroL == isZeroP)
			{
				if (I >= buffer.size())
				{
					buffer.resize(buffer.size() + 128);
				}
				if (isZeroL == 0)
				{
					buffer[I] = 0;
				}
				else
				{
					buffer[I] = 1;
				}
				++I;
			}
			for (int j = i; j < m_secondaryDataBuffer.size(); ++j)
			{
				isZeroP = m_secondaryDataBuffer[j];
				if (isZeroL != isZeroP)
				{

				}
				isZeroL = isZeroP;

			}
		}
	}

	UINT GetUInt(int index)
	{
		UINT V;
		CopyMemory(&V, &m_dataBuffer[index], sizeof(UINT));

		return V;
	}

	void SetUInt(int index, UINT value)
	{
		CopyMemory(&m_secondaryDataBuffer[index], &value, sizeof(UINT));
	}

private:
	std::vector<char> m_dataBuffer;
	std::vector<char> m_secondaryDataBuffer;
	int index;
};*/

class StreamingDataManager
{
public:
	StreamingDataManager();
	StreamingDataManager(DX::DeviceResources *resources);
	~StreamingDataManager();

	void Init(DX::DeviceResources *resources);

	int LoadData(string nameFile);
	bool SaveData(string nameFile, int id);

	int LoadDataSet(string nameFile);
	bool SaveDataSet(string nameFile, std::vector<int> *IDs);
	void DeleteDataInDataSet(int id);

	int CreateDataSet();
	void DeleteDataSet(int id);
	int GetDataSetSize(int id);
	void SetDataSetSize(int id, int size);
	void SetDataSet(int id, int index, int value);
	int GetDataSet(int id, int index);

	int CreateStreamingData(std::vector<char> *data);
	void DeleteStreamingData(int id);
	std::vector<char> *GetStreamingData(int id);

	void CylinderMapToSquareMap(
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
		btMatrix3x3 *blockMatrixs[12]);
private:
	DX::DeviceResources *m_resources;

	StreamingDataAssetsStacks m_streamingData;
};

__declspec(selectany) StreamingDataManager *GameStreamingDataHandle = 0;

