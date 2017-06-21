#ifndef GAME_COLLISION_SHAPE_OBJECT
#define GAME_COLLISION_SHAPE_OBJECT

#include "pch.h"
#include "GameBaseObject.h"

class GameCollisionShapeObject : public GameBaseObject
{
public:
	GameCollisionShapeObject() {	}
	~GameCollisionShapeObject() { delete m_collisionShape; }
	void CreateCollisionShape(btCollisionShape * shape) { m_collisionShape = shape; }
	btCollisionShape * GetCollisionShape() { return m_collisionShape; }
private:
	btCollisionShape *m_collisionShape;
};

#endif