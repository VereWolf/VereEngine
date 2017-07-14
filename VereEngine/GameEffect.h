#pragma once

#include "pch.h"
#include "GameBaseObject.h"
#include "Effects.h"

class GameEffect : public GameBaseObject
{
public:
	GameEffect() {}
	~GameEffect() { ReleaseEffect(); }

	void SetEffect(BaseEffect* T) { m_gameEffect = T; }
	BaseEffect* GetEffect() { return m_gameEffect; }
	inline void ReleaseEffect() { SafeDelete(m_gameEffect); }
private:
	BaseEffect* m_gameEffect;
};