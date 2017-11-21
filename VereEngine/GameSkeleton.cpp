#include "pch.h"
#include "GameSkeleton.h"


void GameSkeleton::SetBone(int index, btVector3 jim, int master)
{
	m_bones[index].jim = jim;
	m_bones[index].master = master;
	m_bones[index].transform.setIdentity();
}

btTransform GameSkeleton::GetTransformBone(int index)
{
	btTransform J(btMatrix3x3::getIdentity(), m_bones[index].jim);

	if (m_bones[index].master < 0)
	{
		return m_transform * m_bones[index].transform * J;
	}

	return GetTransformBone(m_bones[index].master) * m_bones[index].transform * J;
}