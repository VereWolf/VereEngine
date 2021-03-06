#pragma once

#include "pch.h"

class WaterRenderMessage : public RenderMessage
{
public:
	void Use();

	btVector3	m_Position;
	btTransform m_AngleMatrix;
	XMINT2 m_Coord;
	btScalar m_Spacing1;
	btScalar m_Spacing2;
	float m_Level;
	bool m_IsHeightMap;

	ID3D11ShaderResourceView * m_HeightSRV;
};