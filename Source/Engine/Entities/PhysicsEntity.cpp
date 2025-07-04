#include <stdAfx.h>

#include "EntityMain.h"
#include <Common/Console.h>
#include <Level/BaseWorld.h>

// make default apperance
CPhysicsEntity::CPhysicsEntity()
{
	CEntityMain::CEntityMain();

	m_fAcceleration = 320.f;
	m_fDeAcceleration = 100.0f;

	m_fSpeed = 1.0f;
	m_fMassObject = 1.0f;

	m_uiPhysicFlags = COLLISION_ALL;
	m_uiPhysicType  = COLLISION_STATIC;

	vCollisionStretch = VECTOR3_NONE;

	m_motionState = nullptr;
	m_rigidBody   = nullptr;
	m_boxShape    = nullptr;
	m_goObject    = nullptr;

	m_bPhysics = true;

	m_ulFlags |= PHYS_CINEMATIC;	

}

void CPhysicsEntity::getMatrixOrientation(void)
{

	if (m_goObject != NULL) 
	{
		m_startTransform = m_goObject->getWorldTransform();

		float modelMatrix[16];
		m_startTransform.getOpenGLMatrix(modelMatrix);

		memcpy(&m_matModel[0], modelMatrix, sizeof(modelMatrix));
		m_matModel = glm::scale(m_matModel, m_pmo.m_vScale);
	}

	if (m_rigidBody == NULL) return;

	m_rigidBody->getMotionState()->getWorldTransform(m_startTransform);
	//btVector3 pos = m_startTransform.getOrigin();
	//btQuaternion rot = m_startTransform.getRotation();

	// Конвертація в матрицю OpenGL
	float modelMatrix[16];
	m_startTransform.getOpenGLMatrix(modelMatrix);
	
	memcpy(&m_matModel[0], modelMatrix, sizeof(modelMatrix));

	
	m_matModel = glm::scale(m_matModel, m_pmo.m_vScale);
}

Vector3D CPhysicsEntity::getCurrentPosition(void) 
{
	btTransform trans;
	btVector3 position;

	if (m_rigidBody != NULL) {
		m_rigidBody->getMotionState()->getWorldTransform(trans);
		position = trans.getOrigin();

		return VECTOR3(position.x(), position.y(), position.z());
	}
	if (m_goObject != NULL)
	{
		//PrintF("Here explode\n");
		trans = m_goObject->getWorldTransform();
		position = trans.getOrigin();
	}

	return VECTOR3(position.x(), position.y(), position.z());
}

void CPhysicsEntity::InitCollisionInfo()
{

	if (m_ulFlags & PHYS_CHARACTER)
		m_boxShape = new btCapsuleShape(vCollisionStretch.x, vCollisionStretch.y - vCollisionStretch.x * 2);
		//m_boxShape = new btCapsuleShape(vCollisionStretch.x, vCollisionStretch.y + 2 * vCollisionStretch.x);
	else if (m_ulFlags & PHYS_SPHERE)
		m_boxShape = new btSphereShape(vCollisionStretch.x);
	else
		m_boxShape = new btBoxShape(btVector3(vCollisionStretch.x, vCollisionStretch.y, vCollisionStretch.z));

	m_startTransform.setIdentity();
	m_startTransform.setOrigin(btVector3(m_vPosition.x, m_vPosition.y, m_vPosition.z));

	if (m_ulFlags & PHYS_ENTITY)
	{
		m_goObject = new btGhostObject;

		m_goObject->setCollisionShape(m_boxShape);
		m_goObject->setWorldTransform(m_startTransform);
		m_goObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
		m_goObject->setUserPointer((void*)this);

		m_pwo->gw_collisionDispatcher.cd_CollisionObjects.push_back(m_goObject);
		m_pwo->gw_DynamicsWorld->addCollisionObject(m_goObject, m_uiPhysicType, m_uiPhysicFlags);

		return;
	}


	btVector3 localInertia(0, 0, 0);
	m_boxShape->calculateLocalInertia(m_fMassObject, localInertia);

	m_motionState = new btDefaultMotionState(m_startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(m_fMassObject, m_motionState, m_boxShape, localInertia);

	m_rigidBody = new btRigidBody(rbInfo);
	
	if (m_ulFlags & PHYS_STATIC || m_ulFlags & PHYS_CHARACTER) 
	{
		m_rigidBody->setAngularFactor(btVector3(0, 0, 0));
		// adds only if we have characters
		m_rigidBody->setUserPointer((void*)this);

		int flags = m_rigidBody->getCollisionFlags();
		flags &= ~btCollisionObject::CF_KINEMATIC_OBJECT;
		m_rigidBody->setCollisionFlags(flags);

	//	m_rigidBody->setGravity(btVector3(0, 0, 0));
	}

	m_rigidBody->setFriction(0.0f);
	m_rigidBody->setDamping(0.2f, 0.5f);


	if (m_uiPhysicType == COLLISION_PROJECTILE) {

		m_rigidBody->setGravity(btVector3(0, 0, 0));
		m_rigidBody->setFriction(0.0f);
		m_rigidBody->setRestitution(0.0f);
		m_rigidBody->setDamping(0, 0);
		m_rigidBody->setLinearFactor(btVector3(1, 1, 1));
		m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
		
	}

	//m_rigidBody->setCcdMotionThreshold(0.01f);
	//m_rigidBody->setCcdSweptSphereRadius(0.2f); 

	m_pwo->gw_DynamicsWorld->addRigidBody(m_rigidBody, m_uiPhysicType, m_uiPhysicFlags);
	m_pwo->gw_allPhysicsObjects.push_back(m_rigidBody);
}

bool CPhysicsEntity::testIfEntityTouchesGround(void) 
{
	/*
	btVector3 velocity = m_rigidBody->getLinearVelocity();
	bool isOnGround = fabs(velocity.y()) < 0.1f;
	return isOnGround;*/
	
	bool isOnGround = false;

	// Перевірка по швидкості
	btVector3 velocity = m_rigidBody->getLinearVelocity();
	if (fabs(velocity.y()) < 0.01f)
	{
		isOnGround = true;
	}

	// Перевірка по raycast
	btTransform playerTransform;
	m_rigidBody->getMotionState()->getWorldTransform(playerTransform);
	btVector3 playerPos = playerTransform.getOrigin();

	btVector3 start = playerPos;
	btVector3 end = playerPos - btVector3(0, 0.5f + 0.05f, 0);

	btCollisionWorld::ClosestRayResultCallback rayCallback(start, end);
	rayCallback.m_collisionFilterMask = btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter;

	m_pwo->gw_DynamicsWorld->rayTest(start, end, rayCallback);

	if (rayCallback.hasHit())
	{
		return rayCallback.hasHit();
	}
	return isOnGround;
	//return isOnGround;
	//return rayCallback.hasHit();
}

bool CPhysicsEntity::castRay(const Vector3D& from, const Vector3D& vPos, CEntityMain* entFilter)
{
	btCollisionWorld::ClosestRayResultCallback rayCallback(btVector3(from.x, from.y, from.z), btVector3(vPos.x, vPos.y, vPos.z));
	m_pwo->gw_DynamicsWorld->rayTest(btVector3(from.x, from.y, from.z), btVector3(vPos.x, vPos.y, vPos.z), rayCallback);



	return rayCallback.hasHit() && ((CPhysicsEntity&)*entFilter).m_rigidBody != rayCallback.m_collisionObject;
}
/*/
bool CPhysicsEntity::rayHit(CPhysicsEntity* entFilter)
{
	return rayCallback.hasHit() && entFilter->m_rigidBody != rayCallback.m_collisionObject;
}
*/

void CPhysicsEntity::imitateBulletHitScanDummy(const Vector3D& vFrom, const Vector3D& vTo)
{
	

	if (m_goObject == NULL) return;

	CEntityMain* emain = (CEntityMain *)m_goObject->getUserPointer();

	btCollisionWorld::IgnoreUserPointerRayCallback ray(btVector3(vFrom.x, vFrom.y, vFrom.z), btVector3(vTo.x, vTo.y, vTo.z), emain->getTarget());
	
	assert(m_pwo != nullptr);

	//ray.needsCollision

	m_pwo->gw_DynamicsWorld->rayTest(btVector3(vFrom.x, vFrom.y, vFrom.z), btVector3(vTo.x, vTo.y, vTo.z), ray);

	if (!ray.hasHit()) return;

//	PrintF("Here\n");

	const btCollisionObject* hitObj = ray.m_collisionObject;
	void* userPtr = hitObj->getUserPointer();

	if (userPtr == nullptr) return;

	CPhysicsEntity* ent = (CPhysicsEntity*)userPtr;

	if (!IsClass(ent, "Player")) return;

	touchHandle_t handle;

	CPhysicsEntity* pBullet = (CPhysicsEntity*)m_goObject->getUserPointer();

	pBullet->generateTouchEvent(&handle);
	/*
	handle.th_damageType = DMT_BULLET;
	handle.th_entityFrom = nullptr;
	handle.th_entityLaunched = nullptr;
	handle.th_iValue = iDamage;
	handle.th_touchType = TT_PROJECTILE;
	*/
	ent->applyTouchEvent(&handle);
}


void CPhysicsEntity::End()
{
	if (m_bDead) return;

//	PrintF(";)\n");
	if (m_rigidBody != NULL) 
	{
		m_rigidBody->setUserPointer(nullptr);
		m_pwo->gw_DynamicsWorld->removeRigidBody(m_rigidBody);
	}

	if (m_goObject != NULL) 
	{
		m_pwo->gw_collisionDispatcher.removeObject(m_goObject);
		m_goObject->setUserPointer(nullptr);
		m_pwo->gw_DynamicsWorld->removeCollisionObject(m_goObject);
		m_goObject = NULL;
	}
	
	auto toDelete = std::find(m_pwo->gw_allPhysicsObjects.begin(), m_pwo->gw_allPhysicsObjects.end(), m_rigidBody);

	if (toDelete != m_pwo->gw_allPhysicsObjects.end())
	{
		unsigned int index = toDelete - m_pwo->gw_allPhysicsObjects.begin();
		
		m_motionState = (btDefaultMotionState*)m_pwo->gw_allPhysicsObjects[index]->getMotionState();
		delete m_motionState;
		m_motionState = NULL;

		// delete entity
		delete  m_pwo->gw_allPhysicsObjects[index];
		m_pwo->gw_allPhysicsObjects[index] = NULL;

		m_pwo->gw_allPhysicsObjects.erase(m_pwo->gw_allPhysicsObjects.begin() + index);
	}

	m_rigidBody = NULL;

	if (m_boxShape != NULL)
	{
		delete m_boxShape;
		m_boxShape = NULL;
	}

	if (m_motionState != NULL) 
	{ 
		delete m_motionState;  
		m_motionState = NULL; 
	}
	/*
	if (m_goObject != NULL) 
	{
		delete m_goObject;
		m_goObject = NULL;
	}
	*/
	CEntityMain::End();
}


