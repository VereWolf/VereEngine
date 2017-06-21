#pragma once

#include "pch.h"

class GameDirectionalLight : public GameBaseObject
{
public:
	GameDirectionalLight() {}
	~GameDirectionalLight() { ReleaseDirectionalLight(); }

	void SetDirectionalLight(DirectionalLight *L) { m_gameDirectionalLight = L; }
	DirectionalLight* GetDirectionalLight() { return m_gameDirectionalLight; }
	inline void ReleaseDirectionalLight() { SafeDelete(m_gameDirectionalLight); }
private:
	DirectionalLight *m_gameDirectionalLight;
};
