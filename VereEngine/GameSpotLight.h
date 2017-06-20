#pragma once

#include "pch.h"

class GameSpotLight : public GameBaseObject
{
public:
	GameSpotLight() {}
	~GameSpotLight() { ReleaseSpotLight(); }

	void SetSpotLight(SpotLight *L) { m_gameSpotLight = L; }
	SpotLight* GetSpotLight() { return m_gameSpotLight; }
	inline void ReleaseSpotLight() { SafeDelete(m_gameSpotLight); }
private:
	SpotLight *m_gameSpotLight;
};