#pragma once

#include "pch.h"
#include "GameBaseObject.h"
#include "Effects.h"
#include "Vertex.h"

class GameInputLayouts : public GameBaseObject
{
public:
	GameInputLayouts() {}
	~GameInputLayouts() { ReleaseInputLayouts(); }

	void SetInputLayouts(ID3D11InputLayout* T) { m_gameInputLayouts = T; }
	ID3D11InputLayout* GetInputLayouts() { return m_gameInputLayouts; }
	inline void ReleaseInputLayouts() { ReleaseCOM(m_gameInputLayouts); }
private:
	ID3D11InputLayout* m_gameInputLayouts;
};