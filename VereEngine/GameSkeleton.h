#pragma once
#include "pch.h"
#include "GameObject.h"
#include "IDRegistr.h"

class GameSkeleton : public GameObject
{
	struct Bone
	{
		btVector3 jim;
		btTransform transform;
		int master;
	};

	GameSkeleton() { m_transform.setIdentity(); }

	void SetNumBone(int n) { m_bones.resize(n); }
	void SetBone(int index, btVector3 jim, int master = -1);
	btTransform GetTransformBone(int index);
	void SetTransformBone(int index, btTransform T) { m_bones[index].transform = T; }

	btTransform GetTransform() { return m_transform; }
	void SetTransform(btTransform T) { m_transform = T; }

private:
	std::vector<Bone> m_bones;
	btTransform m_transform;
};