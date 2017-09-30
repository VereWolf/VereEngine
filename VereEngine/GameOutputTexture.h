#pragma once

#include "pch.h"
#include "GameBaseObject.h"

class GameOutputTexture : public GameBaseObject
{
public:
	GameOutputTexture() {}
	~GameOutputTexture() { ReleaseTexture(); }

	void SetTexture(ID3D11ShaderResourceView* T, ID3D11UnorderedAccessView *U) { m_gameTextureSRV = T; m_gameTextureUAV = U; }
	ID3D11ShaderResourceView* GetTextureSRV() { return m_gameTextureSRV; }
	ID3D11UnorderedAccessView* GetTextureUAV() { return m_gameTextureUAV; }
	inline void ReleaseTexture() { ReleaseCOM(m_gameTextureUAV); }
private:
	ID3D11ShaderResourceView* m_gameTextureSRV;
	ID3D11UnorderedAccessView* m_gameTextureUAV;
};