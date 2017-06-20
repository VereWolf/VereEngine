#pragma once

#include "pch.h"

class GameAmbientLight : public GameBaseObject
{
public:
	GameAmbientLight() {}
	~GameAmbientLight() { ReleaseAmbientLight(); }

	void SetAmbientLight(AmbientLight *L) { m_gameAmbientLight = L; }
	AmbientLight* GetAmbientLight() { return m_gameAmbientLight; }
	inline void ReleaseAmbientLight() { SafeDelete(m_gameAmbientLight); }
private:
	AmbientLight *m_gameAmbientLight;
};
