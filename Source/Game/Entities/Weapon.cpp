#include "stdAfx.h"

#include "../GEntities.h"

CWeaponItem::CWeaponItem()
{
	//CEntityMain::CEntityMain();

	wi_sValue = 10;
	wi_wtWeaponType = WI_PISTOL;

	m_uiPhysicFlags = COLLISION_NPC;
	m_uiPhysicType = COLLISION_ITEM;

	m_ulID = 6;

	m_strClassName = "WeaponItem";
	m_strName = "Weapon";

	wi_uiWeapon = 0;

	m_ulFlags |= PHYS_ENTITY;

	wi_dStarted = 0.0;

	wi_bRespawn = true;
}

CWeaponItem::~CWeaponItem()
{
	CPhysicsEntity::End();
	wi_dStarted = 0;
}

void CWeaponItem::End()
{
	CPhysicsEntity::End();
	wi_dStarted = 0;
	entityManager.addEntityToDelete(this);
}

void CWeaponItem::Load(CFileStream& stream)
{
	CEntityMain::Load(stream);
	int type;
	stream.ReadFromFile(&wi_sValue, sizeof(short));
	stream.ReadFromFile(&type, sizeof(int));
	wi_wtWeaponType = EWeaponItem(type);
}

void CWeaponItem::Save(CFileStream& stream)
{
	CEntityMain::Save(stream);
	stream.WriteToFile(&wi_sValue, sizeof(short));
	stream.WriteToFile(&wi_wtWeaponType, sizeof(int));
}

void CWeaponItem::Initialize()
{
	switch (wi_wtWeaponType)
	{
	case WI_AR:

		m_strModel = "Data\\Models\\Weapons\\AR\\Body.mo";
		m_strTexture = "Data\\Models\\Weapons\\AR\\Body.te";
		wi_sValue = 50;
		wi_uiWeapon = WEAPON_AR;

		break;

	case WI_SHOTGUN:

		m_strModel = "Data\\Models\\Weapons\\Q3ShotGun.mo";
		m_strTexture = "Data\\Models\\Weapons\\Q3ShotGun.te";
		wi_sValue = 5;
		wi_uiWeapon = WEAPON_SHOTGUN;

		break;

	case WI_ROCKETGUN:

		m_strModel = "Data\\Models\\Weapons\\RocketGun\\q3Rocket.mo";
		m_strTexture = "Data\\Models\\Weapons\\RocketGun\\rocketl.te";
		wi_sValue = 5;
		wi_uiWeapon = WEAPON_ROCKETGUN;

		break;
	case WI_ENERGYGUN:

		m_strModel   = "Data\\Models\\Weapons\\EnergyRifle\\EnergyRifle.mo";
		m_strTexture = "Data\\Models\\Weapons\\EnergyRifle\\Body.te";
		wi_sValue    = 50;
		wi_uiWeapon  = WEAPON_ENERGYGUN;

		break;
	default:
		Throw("Unknown weapon type");
	}

	vCollisionStretch = VECTOR3(0.25f, 0.25f, 0.25f);

	m_anglRotaion.x = 0.f;
	m_anglRotaion.y = 45.f;
	m_anglRotaion.z = 0.f;

	InitAsModel();

	InitCollisionInfo();
}

void CWeaponItem::Update(void)
{
	if (m_bDead) return;

	if ((SDL_GetTicks64() * 0.001) >= wi_dStarted)
	{
		m_bCatch = false;
		m_ulFlags &= ~ENTITY_EDITOR;
		m_ulFlags |= ENTITY_MODEL;
	}

	if (m_bCatch && !wi_bRespawn) { End(); return; }

	m_matModel = glm::mat4(1.f);

	m_matModel = glm::translate(m_matModel, m_vPosition);
	m_matModel = glm::rotate(m_matModel, AngleToRad(m_anglRotaion.y) * float((SDL_GetTicks64() * 0.001)), VECTOR3(0, 1, 0));

}

void CWeaponItem::generateTouchEvent(touchHandle_t* handle)
{
	handle->th_touchType = TT_ITEM;
	handle->th_entityFrom = this;
	handle->th_entityLaunched = nullptr;

	handle->th_iValue = wi_sValue;
	handle->th_damageType = DMT_NONE;

	m_ulFlags &= ~ENTITY_MODEL;
	m_ulFlags |= ENTITY_EDITOR;
}