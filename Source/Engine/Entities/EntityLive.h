#pragma once

//#pragma message(">> EntityLive.h is not present!!!")

/*
class CEntityPlayer : public CEntityMain
{
public:
	
	char* ep_strTeamName;
	char* ep_strPlayerName;
	Vector4D ep_colorName;

	unsigned int ep_uiButtons;

public:

	CEntityPlayer();


	virtual void End();
	virtual void Update();

protected:

	btCollisionShape* playerShape;


	btCollisionShape* playerShape;

	// 3. ��������� ������������
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0, 5, 0)); // ����� ���������

	// 4. Motion State
	btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);

	// 5. ��������� rigid body
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, playerShape, localInertia);
	btRigidBody* playerBody = new btRigidBody(rbInfo);

	// 6. ������ � ���
	dynamicsWorld->addRigidBody(playerBody);

	// ������ ��������� ������� X � Z
	playerBody->setAngularFactor(btVector3(0, 1, 0));


};

*/