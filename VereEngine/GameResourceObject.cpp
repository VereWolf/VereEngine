#include "pch.h"
#include "GameResourceObject.h"

void GameResourceObject::PerformancePersistence(bool onlyGetSize, bool isSaveMod)
{
	GameBaseObject::PerformancePersistence(onlyGetSize, isSaveMod);

	m_persistenceSize += SaveOrLoadString(&m_nameFile);
	m_persistenceSize += SaveOrLoadString(&m_pathFile);
}