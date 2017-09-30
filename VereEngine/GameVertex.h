#pragma once

#include "pch.h"
#include "GameBaseObject.h"

using namespace std;

#define ElementsVertex vector<D3D11_INPUT_ELEMENT_DESC>

class GameVertex : public GameBaseObject
{
public:
	GameVertex() {}
	~GameVertex() { ReleaseVertex(); }

	void SetVertex(ElementsVertex* T) { m_gameVertex = T; }
	ElementsVertex* GetVertex() { return m_gameVertex; }
	inline void ReleaseVertex() { SafeDelete(m_gameVertex); }
private:
	ElementsVertex *m_gameVertex;
	UINT m_stride;
};