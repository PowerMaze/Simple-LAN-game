#include "stdAfx.h"

#include "../GEntities.h"


CArmor::CArmor()
{
	//CEntityMain::CEntityMain();

	ar_Value = 10;
	ar_atArmorType = AR_10;
	ar_bRespawn = false;

	m_ulID = 5;

	m_strClassName = "Armor";
	m_strName = "Armor01";

	m_ulFlags |= PHYS_ENTITY;

	ar_dStarted = 0.0;
}

CArmor::~CArmor()
{
	CPhysicsEntity::End();
	PrintF("^6CArmor::~CArmor()^8\n");
}

void CArmor::End()
{
	CPhysicsEntity::End();
	entityManager.addEntityToDelete(this);
}

void CArmor::Load(CFileStream& stream)
{
	CEntityMain::Load(stream);
	int type;
	stream.ReadFromFile(&ar_Value, sizeof(short));
	stream.ReadFromFile(&type, sizeof(int));
	ar_atArmorType = EArmorType(type);
	stream.ReadFromFile(&ar_bRespawn, sizeof(bool));
}

void CArmor::Save(CFileStream& stream)
{
	CEntityMain::Save(stream);
	stream.WriteToFile(&ar_Value, sizeof(short));
	stream.WriteToFile(&ar_atArmorType, sizeof(int));
	stream.WriteToFile(&ar_bRespawn, sizeof(bool));
}

void CArmor::Initialize()
{
	m_uiPhysicFlags = COLLISION_NPC;
	m_uiPhysicType = COLLISION_ITEM;

	m_anglRotaion.x = 0.f;
	m_anglRotaion.y = 45.f;
	m_anglRotaion.x = 0.f;

	m_strModel = "Data\\Models\\Items\\Armor\\Armor.mo";

	switch (ar_atArmorType)
	{
	case AR_10:

		m_strTexture = "Data\\Models\\Items\\Armor\\50.te";
		ar_Value = 10;
		break;

	case AR_50:

		m_strTexture = "Data\\Models\\Items\\Armor\\50.te";
		ar_Value = 50;
		break;

	case AR_100:

		m_strTexture = "Data\\Models\\Items\\Armor\\100.te";
		ar_Value = 100;
		break;

	default:
		Throw("Unknown health type");
	}

	vCollisionStretch = VECTOR3(0.25f, 0.25f, 0.25f);

	InitAsModel();

	InitCollisionInfo();
}

void CArmor::Update(void)
{
	if (m_bDead) return;

	if ((SDL_GetTicks64() * 0.001) >= ar_dStarted)
	{
		m_bCatch = false;
		m_ulFlags &= ~ENTITY_EDITOR;
		m_ulFlags |= ENTITY_MODEL;
	}

	if (m_bCatch && !ar_bRespawn) { End(); return; }

	m_matModel = glm::mat4(1.f);

	m_matModel = glm::translate(m_matModel, m_vPosition);
	m_matModel = glm::rotate(m_matModel, AngleToRad(m_anglRotaion.y) * float((SDL_GetTicks64() * 0.001)), VECTOR3(0, 1, 0));

}

void CArmor::generateTouchEvent(touchHandle_t* handle)
{
	handle->th_touchType = TT_ITEM;
	//strcpy(handle->th_strClassName, m_strClassName.c_str());

	handle->th_entityFrom = this;

	handle->th_iValue = ar_Value;
	handle->th_damageType = DMT_NONE;
	handle->th_entityLaunched = nullptr;


	m_ulFlags &= ~ENTITY_MODEL;
	m_ulFlags |= ENTITY_EDITOR;

	//he_dStarted = (SDL_GetTicks64() * 0.001) + ITEM_RESPAWN_DEFAULT;

}
