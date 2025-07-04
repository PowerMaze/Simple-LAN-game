#include "stdAfx.h"
#include "../GEntities.h"

CPlayerSpawn::CPlayerSpawn() 
{
	CEntityMain::CEntityMain();

	psp_uiStartedWeaponPack = START_WEAPONS_MASK;

	psp_iHealth = 100;
	psp_iArmor = 0;

	m_ulID = 8;

	m_strName = "Player-Spawn01";

	m_strClassName = "PlayerSpawn";

	psp_bAllowed = false; 

	entityManager.addSpawner(this);
}



CPlayerSpawn::~CPlayerSpawn() 
{
	CEntityMain::End();
}

void CPlayerSpawn::SpawnPlayer(CPlayer* ePlayer)
{
	assert(ePlayer != NULL);

	ePlayer->m_iHealth = psp_iHealth;
	ePlayer->m_iArmor = psp_iArmor;
	ePlayer->pl_uiWeaponMask = psp_uiStartedWeaponPack;
	ePlayer->pl_state = PL_STATE_ACTIVE;
	ePlayer->pl_ulCurrentWeapon = WEAPON_MELE;
	ePlayer->pl_weapons->selectWeapon(WEAPON_MELE);
	ePlayer->setPosition(m_vPosition);
	ePlayer->pl_camera->Yaw = m_anglRotaion.y;
	ePlayer->pl_camera->Pitch = 0.f;
	ePlayer->pl_camera->setCamera(m_vPosition);
}

void CPlayerSpawn::Save(CFileStream& file)
{
	CEntityMain::Save(file);

	file.WriteToFile(&psp_bAllowed, sizeof(bool));
	file.WriteToFile(&psp_iArmor, sizeof(int));
	file.WriteToFile(&psp_iHealth, sizeof(int));
	file.WriteToFile(&psp_uiStartedWeaponPack, sizeof(unsigned int));


}

void CPlayerSpawn::Load(CFileStream& file)
{
	CEntityMain::Load(file);

	file.ReadFromFile(&psp_bAllowed, sizeof(bool));
	file.ReadFromFile(&psp_iArmor, sizeof(int));
	file.ReadFromFile(&psp_iHealth, sizeof(int));
	file.ReadFromFile(&psp_uiStartedWeaponPack, sizeof(unsigned int));
}

void CPlayerSpawn::Initialize()
{
	InitAsEditorEntity();
}







