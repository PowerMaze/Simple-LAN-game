#include "stdAfx.h"

#include "../GEntities.h"

//// make default apperance
//CPhysicsEntity::CPhysicsEntity() 
//{
//	CEntityMain::CEntityMain();
//
//	m_fAcceleration   = 320.f;
//	m_fDeAcceleration = 100.0f;
//
//	m_fSpeed      = 1.0f;
//	m_fMassObject = 1.0f;
//
//	vLastPose = VECTOR3_NONE;
//	vCollisionStretch = VECTOR3_NONE;
//
//	//m_motionState = nullptr;
//	//m_rigidBody = nullptr;
//	//m_boxShape = nullptr;
//}
//
//void CPhysicsEntity::getMatrixOrientation(void)
//{
//
//	//if (m_rigidBody == NULL) return;
//
//	//m_rigidBody->getMotionState()->getWorldTransform(m_startTransform);
//	//btVector3 pos = m_startTransform.getOrigin();
//	//btQuaternion rot = m_startTransform.getRotation();
//
//	//// Конвертація в матрицю OpenGL
//	//float modelMatrix[16];
//	//m_startTransform.getOpenGLMatrix(modelMatrix);
//	//
//	//memcpy(&m_matModel[0], modelMatrix, sizeof(modelMatrix));
//}
//
//
//void CPhysicsEntity::InitCollisionInfo()
//{
//	//m_boxShape = new btBoxShape(btVector3(vCollisionStretch.x, vCollisionStretch.y, vCollisionStretch.z));
//
//	//m_startTransform.setIdentity();
//
//	//m_startTransform.setOrigin(btVector3(m_vPosition.x, m_vPosition.y, m_vPosition.z));
//
//	//btVector3 localInertia(0, 0, 0);
//	//m_boxShape->calculateLocalInertia(m_fMassObject, localInertia);
//
//	//m_motionState = new btDefaultMotionState(m_startTransform);
//
//	//btRigidBody::btRigidBodyConstructionInfo rbInfo(m_fMassObject, m_motionState, m_boxShape, localInertia);
//
//	//m_rigidBody = new btRigidBody(rbInfo);
//
//	//m_pwo->gw_DynamicsWorld->addRigidBody(m_rigidBody);
//	//m_pwo->gw_allPhysicsObjects.push_back(m_rigidBody);
//}
//
//
//void CPhysicsEntity::End()
//{
//	CEntityMain::End();
//
//	//m_pwo->gw_DynamicsWorld->removeRigidBody(m_rigidBody);
//	//
//	//auto toDelete = std::find(m_pwo->gw_allPhysicsObjects.begin(), m_pwo->gw_allPhysicsObjects.end(), m_rigidBody);
//
//	//if (toDelete != m_pwo->gw_allPhysicsObjects.end())
//	//{
//	//	unsigned int index = toDelete - m_pwo->gw_allPhysicsObjects.begin();
//
//	//	delete m_pwo->gw_allPhysicsObjects[index]->getMotionState();
//
//	//	// delete entity
//	//	delete  m_pwo->gw_allPhysicsObjects[index];
//	//	m_pwo->gw_allPhysicsObjects[index] = NULL;
//
//	//	m_pwo->gw_allPhysicsObjects.erase(m_pwo->gw_allPhysicsObjects.begin() + index);
//	//}
//
//	//m_rigidBody = NULL;
//	//delete m_motionState;  m_motionState = NULL;
//	//delete m_boxShape; m_boxShape = NULL;
//}
