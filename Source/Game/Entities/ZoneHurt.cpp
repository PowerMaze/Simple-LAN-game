#include "stdAfx.h"

#include "../GEntities.h"





CZone::CZone()
{
	zn_bAllowPlayers = true;
}
CZone::~CZone(void) 
{

}

void CZone::Load(CFileStream& stream) 
{
	CEntityMain::Load(stream);
	stream.ReadFromFile(&zn_bAllowPlayers, sizeof(bool));
}

void CZone::Save(CFileStream& stream) 
{
	CEntityMain::Save(stream);
	stream.WriteToFile(&zn_bAllowPlayers, sizeof(bool));
}



CZoneHurt::CZoneHurt() 
{
	CPhysicsEntity::CPhysicsEntity();

	m_strClassName = "ZoneHurt";
	m_ulFlags |= PHYS_ENTITY;
	m_ulID = 15;

	zh_iDamage = 1000;
	zh_dmt = DMT_VOID;

	m_uiPhysicFlags = COLLISION_NPC;
	m_uiPhysicType = COLLISION_ITEM;

	vCollisionStretch = VECTOR3(2, 2, 2);
}

CZoneHurt::~CZoneHurt()
{
	CPhysicsEntity::End();
}

void CZoneHurt::End() 
{
	CPhysicsEntity::End();
}

void CZoneHurt::Update(void)
{
//	m_matModel = glm::mat4(1.f);
//	m_matModel = glm::translate(m_matModel, m_vPosition);
//	m_matModel = glm::scale(m_matModel, vCollisionStretch);
}

void CZoneHurt::Load(CFileStream& stream) 
{
	CZone::Load(stream);

	stream.ReadFromFile(&zh_iDamage, sizeof(int));
	byte t;
	stream.ReadFromFile(&t, sizeof(byte));
	zh_dmt = EDamageType(t);
	stream.ReadFromFile(&vCollisionStretch, sizeof(Vector3D));
}
// load file
void CZoneHurt::Save(CFileStream& stream) 
{
	CZone::Save(stream);

	stream.WriteToFile(&zh_iDamage, sizeof(int));
	byte t = zh_dmt;
	stream.WriteToFile(&t, sizeof(byte));
	stream.WriteToFile(&vCollisionStretch, sizeof(Vector3D));
}
// init as default
void CZoneHurt::Initialize(void) 
{

//	m_strModel = "Data\\Models\\Misc\\TestAnimations\\MorphCube.mo";
//	m_strTexture = "Data\\Models\\Misc\\wood.te";

	InitAsEditorEntity();

//	m_pmo.m_vScale = vCollisionStretch;

	InitCollisionInfo();
}
// generates touch event
void CZoneHurt::generateTouchEvent(touchHandle_t* handle) 
{
	handle->th_damageType = zh_dmt;
	handle->th_touchType = TT_PROJECTILE;
	handle->th_entityFrom = this;
	handle->th_entityLaunched = nullptr;
	handle->th_iValue = zh_iDamage;
}




