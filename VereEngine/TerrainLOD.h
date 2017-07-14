#pragma once

#include "pch.h"

class TerrainRenderMessage : public RenderMesage
{
public:
	void Use();

	btVector3	m_Position;
	btTransform m_AngleMatrix;
	XMINT2 m_Coord;
	btScalar m_Spacing1;
	btScalar m_Spacing2;
	btScalar m_Radius;
	float m_Level;
	btVector3 m_Tangent;

	ID3D11ShaderResourceView * m_HeightSRV;
	ID3D11ShaderResourceView * m_NormalSRV;
};
