#pragma once

#include "pch.h"
#include "GameBaseObject.h"

namespace Game2DRenderDevice
{
	class GameTextFormat : public GameBaseObject
	{
	public:
		GameTextFormat() {}
		~GameTextFormat() { ReleaseTextFormat(); }

		void SetTextFormat(IDWriteTextFormat* T) { m_gameTextFormat = T; }
		IDWriteTextFormat* GetTextFormat() { return m_gameTextFormat; }
		inline void ReleaseTextFormat() { ReleaseCOM(m_gameTextFormat); }
	private:
		IDWriteTextFormat* m_gameTextFormat;
	};
}
