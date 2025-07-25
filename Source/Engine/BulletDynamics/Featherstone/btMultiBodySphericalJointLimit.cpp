/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2018 Erwin Coumans  http://bulletphysics.org

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
#include "btMultiBodySphericalJointLimit.h"
#include "btMultiBody.h"
#include "btMultiBodyLinkCollider.h"
#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "LinearMath/btTransformUtil.h"
#include "BulletDynamics/ConstraintSolver/btGeneric6DofSpring2Constraint.h"
#include "LinearMath/btIDebugDraw.h"

btMultiBodySphericalJointLimit::btMultiBodySphericalJointLimit(btMultiBody* body, int link, 
	btScalar swingxRange,
	btScalar swingyRange,
	btScalar twistRange,
	btScalar maxAppliedImpulse)
	: btMultiBodyConstraint(body, body, link, body->getLink(link).m_parent, 3, true, MULTIBODY_CONSTRAINT_SPHERICAL_LIMIT),
	m_desiredVelocity(0, 0, 0),
	m_desiredPosition(0,0,0,1),
	m_use_multi_dof_params(false),
	m_kd(1., 1., 1.),
	m_kp(0.2, 0.2, 0.2),
	m_erp(1),
	m_rhsClamp(SIMD_INFINITY),
	m_maxAppliedImpulseMultiDof(maxAppliedImpulse, maxAppliedImpulse, maxAppliedImpulse),
	m_pivotA(m_bodyA->getLink(link).m_eVector),
	m_pivotB(m_bodyB->getLink(link).m_eVector),
	m_swingxRange(swingxRange),
	m_swingyRange(swingyRange),
	m_twistRange(twistRange)

{

	m_maxAppliedImpulse = maxAppliedImpulse;
}


void btMultiBodySphericalJointLimit::finalizeMultiDof()
{
	allocateJacobiansMultiDof();
	// note: we rely on the fact that data.m_jacobians are
	// always initialized to zero by the Constraint ctor
	int linkDoF = 0;
	unsigned int offset = 6 + (m_bodyA->getLink(m_linkA).m_dofOffset + linkDoF);

	// row 0: the lower bound
	// row 0: the lower bound
	jacobianA(0)[offset] = 1;

	jacobianB(1)[offset] = -1;

	m_numDofsFinalized = m_jacSizeBoth;
}


btMultiBodySphericalJointLimit::~btMultiBodySphericalJointLimit()
{
}

int btMultiBodySphericalJointLimit::getIslandIdA() const
{
	if (this->m_linkA < 0)
	{
		btMultiBodyLinkCollider* col = m_bodyA->getBaseCollider();
		if (col)
			return col->getIslandTag();
	}
	else
	{
		if (m_bodyA->getLink(m_linkA).m_collider)
		{
			return m_bodyA->getLink(m_linkA).m_collider->getIslandTag();
		}
	}
	return -1;
}

int btMultiBodySphericalJointLimit::getIslandIdB() const
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
		{
			return m_bodyB->getLink(m_linkB).m_collider->getIslandTag();
		}
	}
	return -1;
}

void btMultiBodySphericalJointLimit::createConstraintRows(btMultiBodyConstraintArray& constraintRows,
												 btMultiBodyJacobianData& data,
												 const btContactSolverInfo& infoGlobal)
{
	// only positions need to be updated -- data.m_jacobians and force
	// directions were set in the ctor and never change.

	if (m_numDofsFinalized != m_jacSizeBoth)
	{
		finalizeMultiDof();
	}

	//don't crash
	if (m_numDofsFinalized != m_jacSizeBoth)
		return;
	

	if (m_maxAppliedImpulse == 0.f)
		return;

	const btScalar posError = 0;
	const btVector3 zero(0, 0, 0);

	
	btVector3 axis[3] = { btVector3(1, 0, 0), btVector3(0, 1, 0), btVector3(0, 0, 1) };
	
	btQuaternion currentQuat(m_bodyA->getJointPosMultiDof(m_linkA)[0],
		m_bodyA->getJointPosMultiDof(m_linkA)[1],
		m_bodyA->getJointPosMultiDof(m_linkA)[2],
		m_bodyA->getJointPosMultiDof(m_linkA)[3]);

	btQuaternion refQuat = m_desiredPosition;
	btVector3 vTwist(0,0,1);
	
	btVector3 vConeNoTwist = quatRotate(currentQuat, vTwist);
	vConeNoTwist.normalize();
	btQuaternion qABCone = shortestArcQuat(vTwist, vConeNoTwist);
	qABCone.normalize();
	btQuaternion qABTwist = qABCone.inverse() * currentQuat;
	qABTwist.normalize();
	btQuaternion desiredQuat = qABTwist;


	btQuaternion relRot = currentQuat.inverse() * desiredQuat;
	btVector3 angleDiff;
	btGeneric6DofSpring2Constraint::matrixToEulerXYZ(btMatrix3x3(relRot), angleDiff);
	
	btScalar limitRanges[3] = {m_swingxRange, m_swingyRange, m_twistRange};
	
	/// twist axis/angle
	btQuaternion qMinTwist = qABTwist;
	btScalar twistAngle = qABTwist.getAngle();

	if (twistAngle > SIMD_PI)  // long way around. flip quat and recalculate.
	{
		qMinTwist = -(qABTwist);
		twistAngle = qMinTwist.getAngle();
	}
	btVector3 vTwistAxis = btVector3(qMinTwist.x(), qMinTwist.y(), qMinTwist.z());
	if (twistAngle > SIMD_EPSILON)
		vTwistAxis.normalize();
	
	if (vTwistAxis.dot(vTwist)<0)
		twistAngle*=-1.;

	angleDiff[2] = twistAngle;


	for (int row = 0; row < getNumRows(); row++)
	{
		btScalar allowed = limitRanges[row];
		btScalar damp = 1;
		if((angleDiff[row]>-allowed)&&(angleDiff[row]<allowed))
		{
			angleDiff[row]=0;
			damp=0;

		} else
		{
			if (angleDiff[row]>allowed)
			{
				angleDiff[row]-=allowed;
				
			}
			if (angleDiff[row]<-allowed)
			{
				angleDiff[row]+=allowed;
				
			} 
		}
		

		int dof = row;
		
		btScalar currentVelocity = m_bodyA->getJointVelMultiDof(m_linkA)[dof];
		btScalar desiredVelocity = this->m_desiredVelocity[row];
		
		double kd = m_use_multi_dof_params ? m_kd[row % 3] : m_kd[0];
		btScalar velocityError = (desiredVelocity - currentVelocity) * kd;

		btMatrix3x3 frameAworld;
		frameAworld.setIdentity();
		frameAworld = m_bodyA->localFrameToWorld(m_linkA, frameAworld);
		btScalar posError = 0;
		{
			btAssert(m_bodyA->getLink(m_linkA).m_jointType == btMultibodyLink::eSpherical);
			switch (m_bodyA->getLink(m_linkA).m_jointType)
			{
				case btMultibodyLink::eSpherical:
				{
					btVector3 constraintNormalAng = frameAworld.getColumn(row % 3);
					double kp = m_use_multi_dof_params ? m_kp[row % 3] : m_kp[0];
					posError = kp*angleDiff[row % 3];
					double max_applied_impulse = m_use_multi_dof_params ? m_maxAppliedImpulseMultiDof[row % 3] : m_maxAppliedImpulse;
					//should multiply by time step
					//max_applied_impulse *= infoGlobal.m_timeStep

					double min_applied_impulse = -max_applied_impulse;
					

					if (posError>0)
						max_applied_impulse=0;
					else
						min_applied_impulse=0;

					if (btFabs(posError)>SIMD_EPSILON)
					{
						btMultiBodySolverConstraint& constraintRow = constraintRows.expandNonInitializing();
						fillMultiBodyConstraint(constraintRow, data, 0, 0, constraintNormalAng,
							zero, zero, zero,//pure angular, so zero out linear parts
							posError,
							infoGlobal,
							min_applied_impulse, max_applied_impulse, true,
							1.0, false, 0, 0,
							damp);
						constraintRow.m_orgConstraint = this;
						constraintRow.m_orgDofIndex = row;
					}
					break;
				}
				default:
				{
					btAssert(0);
				}
			};
		}
	}
}


void btMultiBodySphericalJointLimit::debugDraw(class btIDebugDraw* drawer)
{
	btTransform tr;
	tr.setIdentity();
	if (m_bodyB)
	{
		btVector3 pivotBworld = m_bodyB->localPosToWorld(m_linkB, m_pivotB);
		tr.setOrigin(pivotBworld);
		drawer->drawTransform(tr, 0.1);
	}
}
