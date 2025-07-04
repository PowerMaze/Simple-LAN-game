#include "stdAfx.h"

#include "../GEntities.h"

CHealth::CHealth()
{
	//CEntityMain::CEntityMain();

	he_Value = 10;
	he_htHealthType = HP_10;
	he_bRespawn = false;

	m_ulID = 5;

	m_strClassName = "Health";
	m_strName = "Health01";

	m_ulFlags |= PHYS_ENTITY;

	he_dStarted = 0.0;
}

CHealth::~CHealth() 
{
	CPhysicsEntity::End();
	PrintF("^6CHealth::~CHealth()^8\n");
}

void CHealth::End()
{
	CPhysicsEntity::End();
	entityManager.addEntityToDelete(this);
}

void CHealth::Load(CFileStream& stream) 
{
	CEntityMain::Load(stream);
	int type;
	stream.ReadFromFile(&he_Value, sizeof(short));	
	stream.ReadFromFile(&type, sizeof(int));
	he_htHealthType = EHealthType(type);
	stream.ReadFromFile(&he_bRespawn, sizeof(bool));
}

void CHealth::Save(CFileStream& stream)
{
	CEntityMain::Save(stream);
	stream.WriteToFile(&he_Value, sizeof(short));
	stream.WriteToFile(&he_htHealthType, sizeof(int));
	stream.WriteToFile(&he_bRespawn, sizeof(bool));
}

void CHealth::Initialize() 
{
	m_uiPhysicFlags = COLLISION_NPC;
	m_uiPhysicType = COLLISION_ITEM;

	m_anglRotaion.x = 0.f;
	m_anglRotaion.y = 45.f;
	m_anglRotaion.x = 0.f;

	m_strModel = "Data\\Models\\Items\\Health\\Health.mo";

	switch (he_htHealthType) 
	{
	case HP_10:

		m_strTexture = "Data\\Models\\Items\\Health\\10.te";
		he_Value = 10;
		break;

	case HP_25:

		m_strTexture = "Data\\Models\\Items\\Health\\25.te";
		he_Value = 25;
		break;

	case HP_50:

		m_strTexture = "Data\\Models\\Items\\Health\\50.te";
		he_Value = 50;
		break;

	case HP_100:

		m_strTexture = "Data\\Models\\Items\\Health\\100.te";
		he_Value = 100;
		break;

	default:
		Throw("Unknown health type");
	}

	vCollisionStretch = VECTOR3(0.25f, 0.25f, 0.25f);

	InitAsModel();

	InitCollisionInfo();
}

void CHealth::Update(void)
{
	if (m_bDead) return;

	if ((SDL_GetTicks64() * 0.001) >= he_dStarted)
	{
		m_bCatch = false;
		m_ulFlags &= ~ENTITY_EDITOR;
		m_ulFlags |= ENTITY_MODEL;
	}

	if (m_bCatch && !he_bRespawn) {	End(); return;	}

	m_matModel = glm::mat4(1.f);

	m_matModel = glm::translate(m_matModel, m_vPosition);
	m_matModel = glm::rotate(m_matModel, AngleToRad(m_anglRotaion.y) * float((SDL_GetTicks64() * 0.001)), VECTOR3(0, 1, 0));

}

void CHealth::generateTouchEvent(touchHandle_t* handle) 
{
	handle->th_touchType = TT_ITEM;
	//strcpy(handle->th_strClassName, m_strClassName.c_str());

	handle->th_entityFrom = this;

	handle->th_iValue = he_Value;
	handle->th_damageType = DMT_NONE;
	handle->th_entityLaunched = nullptr;

	m_ulFlags &= ~ENTITY_MODEL;
	m_ulFlags |= ENTITY_EDITOR;

	//he_dStarted = (SDL_GetTicks64() * 0.001) + ITEM_RESPAWN_DEFAULT;

}
