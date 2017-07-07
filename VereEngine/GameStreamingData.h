#pragma once

#include "pch.h"
#include "GameBaseObject.h"

class GameStreamingData : public GameBaseObject
{
public:
	GameStreamingData() {}
	~GameStreamingData() { ReleaseStreamingData(); }

	void SetStreamingData(void *data, int size)
	{
		m_gameStreamingData = data;
		m_size = size;
	}
	void *GetStreamingData() { return m_gameStreamingData; }
	int GetSize() { return m_size; }
	inline void ReleaseStreamingData() { SafeDelete(m_gameStreamingData); }
private:
	void* m_gameStreamingData;
	int m_size;
};
