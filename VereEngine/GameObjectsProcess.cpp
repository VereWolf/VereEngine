#include "pch.h"
#include "GameObjectsProcess.h"

int GameObjectsProcess::TakeObject()
{
	if (m_curentProcessObjects >= m_maxProcessObjects)
	{
		return -1;
	}

	return m_queue.TakeElement();
}