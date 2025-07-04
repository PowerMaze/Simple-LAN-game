#pragma once

#include <Engine/BulletCollision/CollisionDispatch/btGhostObject.h>

class CCollisionDispatcher 
{
public:

	CGameWorld* cd_world;
	std::vector< btGhostObject* > cd_CollisionObjects;

public:

	CCollisionDispatcher();
	inline ~CCollisionDispatcher() { Clear(); }

	void Clear();

public:

	inline void setWorld(CGameWorld* wo) { cd_world = wo; }

	void handleCollision(void);

	ENGINE_API void reportInfo(void);

	void removeObject(btGhostObject* obj);
//	void addCollisionInfo(CPhysicsEntity* p);

};


extern bool collisionRigidContact(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1);

