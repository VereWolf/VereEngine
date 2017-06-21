#pragma once

#include "pch.h"
#include "GameBaseObject.h"

class GameStreamingData : public GameBaseObject
{
public:
	GameStreamingData() {}
	~GameStreamingData() { ReleaseStreamingData(); }

	void SetStreamingData(std::vector<char> *D) { m_gameStreamingData = D; }
	std::vector<char>* GetStreamingData() { return m_gameStreamingData; }
	inline void ReleaseStreamingData() { SafeDelete(m_gameStreamingData); }
private:
	std::vector<char> *m_gameStreamingData;
};
