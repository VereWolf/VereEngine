#pragma once

#include "pch.h"
#include "GameBaseObject.h"

struct DataSet
{
};

class GameDataSet : public GameBaseObject
{
public:
	GameDataSet() {}
	~GameDataSet() {}

	void SetDataSetSize(int size) { m_gameDataSet.resize(size); }
	int GetDataSetSize() { return m_gameDataSet.size(); }
	void SetDataSet(int index, int id) { m_gameDataSet[index] = id; }
	int GetDataSet(int index) { return m_gameDataSet[index]; }
private:
	std::vector<int> m_gameDataSet;
};
