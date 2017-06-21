#pragma once

#include "pch.h"
#include "GameBaseObject.h"

class GameModel: public GameBaseObject
{
public:
	GameModel() {}
	~GameModel() { ReleaseModel(); }

	void SetModel(Model *M) { m_gameModel = M; }
	Model* GetModel() { return m_gameModel; }
	inline void ReleaseModel() { SafeDelete(m_gameModel); }
private:
	Model *m_gameModel;
};