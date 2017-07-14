#pragma once

#include "pch.h"

class AtmosphereRenderMessage : public RenderMesage
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
};