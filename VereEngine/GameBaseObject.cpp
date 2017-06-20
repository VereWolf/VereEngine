#include "pch.h"
#include "GameBaseObject.h"
#include "DeviceResources.h"

GameBaseObject::GameBaseObject()
{
	m_id = -1;
	m_idInRegistr = -1;
}

GameBaseObject::GameBaseObject(GameBaseObject &other)
{
	m_id = -1;
	m_idInRegistr = -1;
	m_resources = other.m_resources;
}

void GameBaseObject::PreInit(DX::DeviceResources *resources)
{
	m_id = -1;
	m_idInRegistr = -1;
	m_resources = resources;
}

void GameBaseObject::PerformancePersistence(bool onlyGetSize, bool isSaveMod)
{
	m_persistenceIsOnlyGetSizeMod = onlyGetSize;

	//save object data to buffer mod == true or load object data from buffer == false
	m_persistenceIsSaveMod = isSaveMod;
	m_persistenceIndex = 0;
	m_persistenceSize = 0;
}

int GameBaseObject::SaveOrLoadValue(void * ptr, int size)
{
	if (m_persistenceIsOnlyGetSizeMod == false)
	{

		if (m_persistenceIsSaveMod == true)
		{
			memcpy(&m_persistenceBuffer[m_persistenceIndex], ptr, size);
			m_persistenceIndex += size;
		}
		else
		{
			memcpy(ptr, &m_persistenceBuffer[m_persistenceIndex], size);
			m_persistenceIndex += size;
		}
	}

	return size;
}

int GameBaseObject::SaveOrLoadString(std::string *S)
{
	int size = 0;
	if (m_persistenceIsOnlyGetSizeMod == false)
	{
		if (m_persistenceIsSaveMod == true)
		{
			size = S->size();
			memcpy(&m_persistenceBuffer[m_persistenceIndex], &size, sizeof(size));
			m_persistenceIndex += sizeof(size);
			memcpy(&m_persistenceBuffer[m_persistenceIndex], &S->at(0), size);
			m_persistenceIndex += size;
		}
		else
		{
			memcpy(&size, &m_persistenceBuffer[m_persistenceIndex], sizeof(size));
			m_persistenceIndex += sizeof(size);
			S->resize(size);
			memcpy(&S->at(0), &m_persistenceBuffer[m_persistenceIndex], size);
			m_persistenceIndex += size;
		}
	}

	return (sizeof(int) + S->size());
}

void GameBaseObject::SetSizeOfPersistenceBuffer(int size)
{
	m_persistenceBuffer.resize(size);
}