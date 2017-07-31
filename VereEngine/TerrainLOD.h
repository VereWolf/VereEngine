#pragma once

#include "pch.h"

class TerrainRenderMessage : public RenderMessage
{
public:
	void Use();

	btVector3	m_Position;
	btTransform m_AngleMatrix;
	XMINT2 m_Coord;
	btScalar m_Spacing1;
	btScalar m_Spacing2;
	float m_Level;
	btVector3 m_Tangent;

	ID3D11ShaderResourceView * m_HeightSRV;
	ID3D11ShaderResourceView * m_NormalSRV;
};
