#pragma once

#include "pch.h"
#include "GameBaseObject.h"

class GameEffect : public GameBaseObject
{
public:
	GameEffect() {}
	~GameEffect() {/*ReleaseEffect();*/ }

	void SetEffect(Effect* T) { m_gameEffect = T; }
	Effect* GetEffect() { return m_gameEffect; }
	//inline void ReleaseEffect() { SafeDelete(m_gameEffect); }
private:
	Effect* m_gameEffect;
};