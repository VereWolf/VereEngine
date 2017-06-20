#pragma once

#include "pch.h"
#include "GameBaseObject.h"

namespace Game2DRenderDevice
{
	class GameColorBrush : public GameBaseObject
	{
	public:
		GameColorBrush() {}
		~GameColorBrush() { ReleaseColorBrush(); }

		void SetColorBrush(ID2D1SolidColorBrush* T) { m_gameColorBrush = T; }
		ID2D1SolidColorBrush* GetColorBrush() { return m_gameColorBrush; }
		inline void ReleaseColorBrush() { ReleaseCOM(m_gameColorBrush); }
	private:
		ID2D1SolidColorBrush* m_gameColorBrush;
	};
}
