#include "stdAfx.h"
#include "../GEntities.h"

CBullet::CBullet() 
{
	bl_iDamage = 5;
	entityFrom = nullptr;
	m_strClassName = "Bullet";
	m_ulID = 6;

	bl_distance = DISTANCE_BULLET;

	m_ulFlags |= PHYS_ENTITY;

	m_uiPhysicFlags = COLLISION_NPC;
	m_uiPhysicType  = COLLISION_ITEM;
}

CBullet::~CBullet() 
{
	entityFrom = nullptr;
	CPhysicsEntity::End();
}

void CBullet::LaunchBullet()
{
	m_anglRotaion = glm::normalize(m_anglRotaion);

	Vector3D vTo = m_vPosition + m_anglRotaion * bl_distance;

	imitateBulletHitScanDummy(m_vPosition, vTo);
}

void CBullet::Initialize()
{
	InitAsEditorEntity();
	InitCollisionInfo();
}

CEntityMain* CBullet::getTarget(void) 
{
	return entityFrom;
}

void CBullet::generateTouchEvent(touchHandle_t* handle)
{
	(*handle).th_damageType = DMT_BULLET;
	(*handle).th_entityFrom = this;
	(*handle).th_entityLaunched = entityFrom;
	(*handle).th_iValue = bl_iDamage;
	(*handle).th_touchType = TT_PROJECTILE;
}


