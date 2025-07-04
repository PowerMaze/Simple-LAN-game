#include "stdAfx.h"

#include "../GEntities.h"

CExplosion::CExplosion()
{
	CPhysicsEntity::CPhysicsEntity();

	ex_fRadius = 7.f;   
	ex_iMaxDamage = 100;  

	ex_iOnce = 0;    

	ex_entityReason = nullptr;

	ex_vDirection = glm::vec3(0.f);

	ex_fForce = 0.f;


}

CExplosion::~CExplosion() 
{
	CPhysicsEntity::End();

	ex_entityReason = nullptr;
}

void CExplosion::End() 
{
	CPhysicsEntity::End();
	entityManager.addEntityToDelete(this);
}

void CExplosion::Initialize()
{
	m_uiPhysicFlags = COLLISION_NPC;
	m_uiPhysicType = COLLISION_ITEM;

	vCollisionStretch.x = ex_fRadius;
	
	m_ulFlags |= PHYS_SPHERE | PHYS_ENTITY;

	m_ulID = 6;

	m_strClassName = "Explosion";	

	InitAsEditorEntity();
	InitCollisionInfo();
}

void CExplosion::Update(void)
{
	ex_iOnce++;
//	PrintF("Once\n");
	if (m_bCatch) { End(); return; }
	if (ex_iOnce > 1) End();
	
}

void CExplosion::generateTouchEvent(touchHandle_t* handle)
{
	handle->th_entityFrom = this;
	handle->th_damageType = DMT_EXPLOSION;
	handle->th_touchType = TT_PROJECTILE;
	handle->th_entityLaunched = ex_entityReason;
	handle->th_iValue = 0;

	// generate damage from explosion

	Vector3D vPos = ((CPhysicsEntity&)*ex_entityReason).getCurrentPosition();
	Vector3D vExp = getCurrentPosition();

	ex_vDirection = vPos - vExp;

	float fDist = glm::length(ex_vDirection);

	if (fDist > vCollisionStretch.x || fDist < SIMD_EPSILON)
	{
//		PrintF("To far\n");
		handle->th_entityFrom = nullptr;
		return;
	}
	//
	//if (!castRay(getCurrentPosition(), vPos, ex_entityReason)) 
	//{
	//	PrintF("Wall\n");
	//	handle->th_entityFrom = nullptr;
	//	return;
	//}

	float falloff = 1.0f - Clamp((fDist / vCollisionStretch.x), 0.0f, 1.0f);
	float damage = ex_iMaxDamage * falloff;

	ex_fForce = 30.f * falloff;
	ex_vDirection = glm::normalize(ex_vDirection);

	handle->th_iValue = damage;

//	PrintF("%d\n", handle->th_iValue);

	//m_bCatch = true;
}