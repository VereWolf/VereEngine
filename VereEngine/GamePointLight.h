#pragma once

#include "pch.h"

class GamePointLight : public GameBaseObject
{
public:
	GamePointLight() {}
	~GamePointLight() { ReleasePointLight(); }

	void SetPointLight(PointLight *L) { m_gamePointLight = L; }
	PointLight* GetPointLight() { return m_gamePointLight; }
	inline void ReleasePointLight() { SafeDelete(m_gamePointLight); }
private:
	PointLight *m_gamePointLight;
};