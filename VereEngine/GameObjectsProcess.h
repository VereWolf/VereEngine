#ifndef GAME_OBJECTS_PROCESS_H
#define GAME_OBJECTS_PROCESS_H

#include "pch.h"

class GameObjectsProcess
{
public:
	void GiveObject(int id) { m_queue.GiveElement(id); }
	int TakeObject();
	void NewFrame() { m_curentProcessObjects = 0; }
	void SetMaxObjectsInFrame(int n) { m_maxProcessObjects = n; }
	int GetMaxObjectsInFrame() { return m_maxProcessObjects; }
	int GetSize() { return m_queue.GetSize(); }
private:
	VereQueue<int> m_queue;
	int m_maxProcessObjects;
	int m_curentProcessObjects;
};


#endif GAME_OBJECTS_PROCESS_H