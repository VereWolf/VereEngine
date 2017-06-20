#ifndef GAME_EFFECT_H
#define GAME_EFFECT_H

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

#endif //GAME_EFFECT_H