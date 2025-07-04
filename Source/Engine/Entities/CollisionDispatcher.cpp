#include <stdAfx.h>

#include "CollisionDispatcher.h"

#include <Engine/Common/Console.h>
#include <Engine/Level/BaseWorld.h>


CCollisionDispatcher::CCollisionDispatcher()
{
	cd_world = nullptr;
}

void CCollisionDispatcher::reportInfo() 
{
	FOREACH_ARRAY(cd_CollisionObjects.size())
	{
		CPhysicsEntity* e = (CPhysicsEntity*)cd_CollisionObjects[i]->getUserPointer();

		if (e == nullptr) PrintF("e == NULL\n");
		if (e != nullptr) 
		{
			PrintF("className = %s\n", e->m_strClassName.c_str());
		}
	}
}


void CCollisionDispatcher::Clear() 
{
	cd_world = nullptr;

	FOREACH_ARRAY(cd_CollisionObjects.size())
	{
		cd_CollisionObjects[i] = nullptr; // because we delete btGhostObjects in CPhysicsEntity::End()
	}
	cd_CollisionObjects.clear();
}

void CCollisionDispatcher::handleCollision() 
{

	FOREACH_ARRAY(cd_CollisionObjects.size())
	{
		int iCount = cd_CollisionObjects[i]->getNumOverlappingObjects();

		for (unsigned int j = 0u; j < iCount; j++)
		{
			btCollisionObject* other = cd_CollisionObjects[i]->getOverlappingObject(j);

			if (other == nullptr) continue;

			CPhysicsEntity* ent = (CPhysicsEntity*)other->getUserPointer();

			if (IsClass(ent, "Player")) 
			{
				touchHandle_t handle;

				CEntityMain* e = (CEntityMain*)cd_CollisionObjects[i]->getUserPointer();

				if (e->m_bCatch) continue;

				e->generateTouchEvent(&handle);
				ent->applyTouchEvent(&handle);
			}
			
			if (IsClass(ent, "World")) 
			{
				// used for projectiles
				CEntityMain* e = (CEntityMain*)cd_CollisionObjects[i]->getUserPointer();
				//touchHandle_t handle;
				//e->m_bCatch = true;
				PrintF("%s\n", e->m_strClassName.c_str());
			}
		}	
	}
}

void CCollisionDispatcher::removeObject(btGhostObject* obj)
{
	auto toDelete = std::find(cd_CollisionObjects.begin(), cd_CollisionObjects.end(), obj);

	if (toDelete != cd_CollisionObjects.end())
	{
		unsigned int index = toDelete - cd_CollisionObjects.begin();
		// delete entity
		cd_CollisionObjects[index] = NULL;
		cd_CollisionObjects.erase(cd_CollisionObjects.begin() + index);
	}
}

bool collisionRigidContact(	btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0,const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
{
	const btCollisionObject* objA = colObj0Wrap->getCollisionObject();
	const btCollisionObject* objB = colObj1Wrap->getCollisionObject();

	CEntityMain* ea = (CEntityMain*)objA->getUserPointer();
	CEntityMain* eb = (CEntityMain*)objB->getUserPointer();



	// Наприклад, якщо один із них — проджектайл
	if (ea && IsClass(ea, "Projectile"))
	{

		if (eb && IsClass(eb, "Player")) 
		{
			touchHandle_t touch;
			ea->generateTouchEvent(&touch);
			eb->applyTouchEvent(&touch);
			ea->m_bCatch = true;
		}

		if (eb && IsClass(eb, "World"))
		{;
			ea->m_bCatch = true;
		}		

		return true;
	}
	else if (eb && IsClass(eb, "Projectile"))
	{
		if (ea && IsClass(ea, "Player"))
		{
			touchHandle_t touch;
			eb->generateTouchEvent(&touch);
			ea->applyTouchEvent(&touch);
			//eb->m_bCatch = true;
		}

		if (ea && IsClass(ea, "World"))
		{			
			eb->m_bCatch = true;
		}
		return true;
	}

	return false; 
}