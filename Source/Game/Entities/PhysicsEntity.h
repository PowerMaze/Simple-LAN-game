#pragma once

///*
/////////////////////////////////////////////////////
//
//Main abstract entity, which can move, can have gravity, etc.
//
//NOTE: this cannot be visible by In the Editor
//
/////////////////////////////////////////////////////
//*/
//class CPhysicsEntity : public CEntityMain
//{
//public:
//
//	float m_fAcceleration;
//	float m_fDeAcceleration;
//
//	float m_fSpeed;
//	float m_fMassObject;
//
//	Vector3D vLastPose;
//
//	Vector3D vCollisionStretch;
//
//	//btDefaultMotionState* m_motionState;
//	//btRigidBody* m_rigidBody;
//
//	//btTransform m_startTransform;
//
//	//btCollisionShape* m_boxShape;
//
//public:
//
//	CPhysicsEntity(void);
//
//	~CPhysicsEntity(void) {};
//
//	virtual void End(void);
//
//	void InitCollisionInfo(void);
//
//	void getMatrixOrientation(void);
//
//	virtual void Update() {};
//};