/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2013 Erwin Coumans  http://bulletphysics.org

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

///This file was written by Erwin Coumans
#include <stdAfx.h>
#include "btMultiBodyFixedConstraint.h"
#include "btMultiBodyLinkCollider.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletDynamics/ConstraintSolver/btGeneric6DofSpring2Constraint.h"
#include "LinearMath/btIDebugDraw.h"

#define BTMBFIXEDCONSTRAINT_DIM 6

btMultiBodyFixedConstraint::btMultiBodyFixedConstraint(btMultiBody* body, int link, btRigidBody* bodyB, const btVector3& pivotInA, const btVector3& pivotInB, const btMatrix3x3& frameInA, const btMatrix3x3& frameInB)
	: btMultiBodyConstraint(body, 0, link, -1, BTMBFIXEDCONSTRAINT_DIM, false, MULTIBODY_CONSTRAINT_FIXED),
	  m_rigidBodyA(0),
	  m_rigidBodyB(bodyB),
	  m_pivotInA(pivotInA),
	  m_pivotInB(pivotInB),
	  m_frameInA(frameInA),
	  m_frameInB(frameInB)
{
	m_data.resize(BTMBFIXEDCONSTRAINT_DIM);  //at least store the applied impulses
}

btMultiBodyFixedConstraint::btMultiBodyFixedConstraint(btMultiBody* bodyA, int linkA, btMultiBody* bodyB, int linkB, const btVector3& pivotInA, const btVector3& pivotInB, const btMatrix3x3& frameInA, const btMatrix3x3& frameInB)
	: btMultiBodyConstraint(bodyA, bodyB, linkA, linkB, BTMBFIXEDCONSTRAINT_DIM, false, MULTIBODY_CONSTRAINT_FIXED),
	  m_rigidBodyA(0),
	  m_rigidBodyB(0),
	  m_pivotInA(pivotInA),
	  m_pivotInB(pivotInB),
	  m_frameInA(frameInA),
	  m_frameInB(frameInB)
{
	m_data.resize(BTMBFIXEDCONSTRAINT_DIM);  //at least store the applied impulses
}

void btMultiBodyFixedConstraint::finalizeMultiDof()
{
	//not implemented yet
	btAssert(0);
}

btMultiBodyFixedConstraint::~btMultiBodyFixedConstraint()
{
}

int btMultiBodyFixedConstraint::getIslandIdA() const
{
	if (m_rigidBodyA)
		return m_rigidBodyA->getIslandTag();

	if (m_bodyA)
	{
		if (m_linkA < 0)
		{
			btMultiBodyLinkCollider* col = m_bodyA->getBaseCollider();
			if (col)
				return col->getIslandTag();
		}
		else
		{
			if (m_bodyA->getLink(m_linkA).m_collider)
				return m_bodyA->getLink(m_linkA).m_collider->getIslandTag();
		}
	}
	return -1;
}

int btMultiBodyFixedConstraint::getIslandIdB() const
{
	if (m_rigidBodyB)
		return m_rigidBodyB->getIslandTag();
	if (m_bodyB)
	{
		if (m_linkB < 0)
		{
			btMultiBodyLinkCollider* col = m_bodyB->getBaseCollider();
			if (col)
				return col->getIslandTag();
		}
		else
		{
			if (m_bodyB->getLink(m_linkB).m_collider)
				return m_bodyB->getLink(m_linkB).m_collider->getIslandTag();
		}
	}
	return -1;
}

void btMultiBodyFixedConstraint::createConstraintRows(btMultiBodyConstraintArray& constraintRows, btMultiBodyJacobianData& data, const btContactSolverInfo& infoGlobal)
{
	int numDim = BTMBFIXEDCONSTRAINT_DIM;
	for (int i = 0; i < numDim; i++)
	{
		btMultiBodySolverConstraint& constraintRow = constraintRows.expandNonInitializing();
		constraintRow.m_orgConstraint = this;
		constraintRow.m_orgDofIndex = i;
		constraintRow.m_relpos1CrossNormal.setValue(0, 0, 0);
		constraintRow.m_contactNormal1.setValue(0, 0, 0);
		constraintRow.m_relpos2CrossNormal.setValue(0, 0, 0);
		constraintRow.m_contactNormal2.setValue(0, 0, 0);
		constraintRow.m_angularComponentA.setValue(0, 0, 0);
		constraintRow.m_angularComponentB.setValue(0, 0, 0);

		constraintRow.m_solverBodyIdA = data.m_fixedBodyId;
		constraintRow.m_solverBodyIdB = data.m_fixedBodyId;

		// Convert local points back to world
		btVector3 pivotAworld = m_pivotInA;
		btMatrix3x3 frameAworld = m_frameInA;
		if (m_rigidBodyA)
		{
			constraintRow.m_solverBodyIdA = m_rigidBodyA->getCompanionId();
			pivotAworld = m_rigidBodyA->getCenterOfMassTransform() * m_pivotInA;
			frameAworld = frameAworld.transpose() * btMatrix3x3(m_rigidBodyA->getOrientation());
		}
		else
		{
			if (m_bodyA)
			{
				pivotAworld = m_bodyA->localPosToWorld(m_linkA, m_pivotInA);
				frameAworld = m_bodyA->localFrameToWorld(m_linkA, frameAworld);
			}
		}
		btVector3 pivotBworld = m_pivotInB;
		btMatrix3x3 frameBworld = m_frameInB;
		if (m_rigidBodyB)
		{
			constraintRow.m_solverBodyIdB = m_rigidBodyB->getCompanionId();
			pivotBworld = m_rigidBodyB->getCenterOfMassTransform() * m_pivotInB;
			frameBworld = frameBworld.transpose() * btMatrix3x3(m_rigidBodyB->getOrientation());
		}
		else
		{
			if (m_bodyB)
			{
				pivotBworld = m_bodyB->localPosToWorld(m_linkB, m_pivotInB);
				frameBworld = m_bodyB->localFrameToWorld(m_linkB, frameBworld);
			}
		}

		btMatrix3x3 relRot = frameAworld.inverse() * frameBworld;
		btVector3 angleDiff;
		btGeneric6DofSpring2Constraint::matrixToEulerXYZ(relRot, angleDiff);

		btVector3 constraintNormalLin(0, 0, 0);
		btVector3 constraintNormalAng(0, 0, 0);
		btScalar posError = 0.0;
		if (i < 3)
		{
			constraintNormalLin[i] = 1;
			posError = (pivotAworld - pivotBworld).dot(constraintNormalLin);
			fillMultiBodyConstraint(constraintRow, data, 0, 0, constraintNormalAng,
									constraintNormalLin, pivotAworld, pivotBworld,
									posError,
									infoGlobal,
									-m_maxAppliedImpulse, m_maxAppliedImpulse);
		}
		else
		{  //i>=3
			constraintNormalAng = frameAworld.getColumn(i % 3);
			posError = angleDiff[i % 3];
			fillMultiBodyConstraint(constraintRow, data, 0, 0, constraintNormalAng,
									constraintNormalLin, pivotAworld, pivotBworld,
									posError,
									infoGlobal,
									-m_maxAppliedImpulse, m_maxAppliedImpulse, true);
		}
	}
}

void btMultiBodyFixedConstraint::debugDraw(class btIDebugDraw* drawer)
{
	btTransform tr;
	tr.setIdentity();

	if (m_rigidBodyA)
	{
		btVector3 pivot = m_rigidBodyA->getCenterOfMassTransform() * m_pivotInA;
		tr.setOrigin(pivot);
		drawer->drawTransform(tr, 0.1);
	}
	if (m_bodyA)
	{
		btVector3 pivotAworld = m_bodyA->localPosToWorld(m_linkA, m_pivotInA);
		tr.setOrigin(pivotAworld);
		drawer->drawTransform(tr, 0.1);
	}
	if (m_rigidBodyB)
	{
		// that ideally should draw the same frame
		btVector3 pivot = m_rigidBodyB->getCenterOfMassTransform() * m_pivotInB;
		tr.setOrigin(pivot);
		drawer->drawTransform(tr, 0.1);
	}
	if (m_bodyB)
	{
		btVector3 pivotBworld = m_bodyB->localPosToWorld(m_linkB, m_pivotInB);
		tr.setOrigin(pivotBworld);
		drawer->drawTransform(tr, 0.1);
	}
}
