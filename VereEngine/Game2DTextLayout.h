#pragma once

#include "pch.h"
#include "GameBaseObject.h"

namespace Game2DRenderDevice
{
	class GameTextLayout : public GameBaseObject
	{
	public:
		GameTextLayout() {}
		~GameTextLayout() { ReleaseTextLayout(); }

		void SetTextLayout(IDWriteTextLayout* T) { m_gameTextLayout = T; }
		IDWriteTextLayout* GetTextLayout() { return m_gameTextLayout; }
		inline void ReleaseTextLayout() { ReleaseCOM(m_gameTextLayout); }
	private:
		IDWriteTextLayout* m_gameTextLayout;
	};
}
