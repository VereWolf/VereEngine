#pragma once

#include "pch.h"
#include "GameBaseObject.h"

class GameTexture: public GameBaseObject
{
public:
	GameTexture() {}
	~GameTexture() { ReleaseTexture(); }

	void SetTexture(ID3D11ShaderResourceView* T) { m_gameTextureSRV = T; }
	ID3D11ShaderResourceView* GetTexture() { return m_gameTextureSRV; }
	inline void ReleaseTexture() { ReleaseCOM(m_gameTextureSRV); }
private:
	ID3D11ShaderResourceView* m_gameTextureSRV;
};
