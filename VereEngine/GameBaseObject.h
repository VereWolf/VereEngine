#pragma once

#include "pch.h"
#include "DeviceResources.h"

class GameBaseObject
{
public:
	GameBaseObject();
	GameBaseObject(GameBaseObject &other);
	virtual ~GameBaseObject()
	{

	}

	virtual void PreInit(DX::DeviceResources *resources);

	void SetId(int id) { m_id = id; }
	int GetId() { return m_id; }
	void SetIdInRegistr(int id) { m_idInRegistr = id; }
	int GetIdInRegistr() { return m_idInRegistr; }

	//Persistence for saving data on disk
	void PerformancePersistence(bool onlyGetSize, bool isSaveMod); //Save/load all relevant data to/from buffer.
	int SaveOrLoadValue(void * ptr, int size); // for one variable, their mods  related with variables bool m_persistenceIsSaveMod and bool m_persistenceIsOnlyGetSizeMod
	int SaveOrLoadString(std::string *S); //as SaveOrLoadValue but for string
	int GetSizeOfBuffer() { return m_persistenceSize; }
	std::vector<char> *GetPersistentBuffer() { return &m_persistenceBuffer; }
	void SetPersistentBuffer(std::vector<char> * buffer, int index, int size)
	{
		memcpy(&m_persistenceBuffer[0], &buffer->at(index), size);
	}
	void SetSizeOfPersistenceBuffer(int size);

	void Destroy() { m_isDestroy = true; }
	bool IsDestroy() { return m_isDestroy; }

protected:
	int m_id;
	int m_idInRegistr;

	DX::DeviceResources* m_resources;

	bool m_persistenceIsSaveMod;
	bool m_persistenceIsOnlyGetSizeMod;
	int m_persistenceSize;
	int m_persistenceIndex;
	std::vector<char> m_persistenceBuffer;

	bool m_isDestroy;
};