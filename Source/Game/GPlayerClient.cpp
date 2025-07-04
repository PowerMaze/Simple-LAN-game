#include "stdAfx.h"

#include "GPlayerClient.h"
#include "GPlayer.h"

#include "GHUD.h"

#include "GEntities.h"


CPlayerClient::CPlayerClient() 
{
	cl_vPosition = VECTOR3_NONE;
	cl_aRotView  = VECTOR3_NONE;
	cl_aRotDir = VECTOR3_NONE;

	cl_Camera  = new CCamera;
	cl_Weapons = new CWeapons;

	cl_Buttons = 0;

	cl_CurrentWeapon = WEAPON_MELE;

	cl_Health = 100;
	cl_Armor  = 0;

	cl_sId = 0;

	cl_kills = 0;

	cl_uiCurrentTick = 0;

	cl_Weapons->wp_clPlayer = this;
	cl_Weapons->Precache(WEAPON_BIT(WEAPON_MELE)| WEAPON_BIT(WEAPON_AR) | WEAPON_BIT(WEAPON_ENERGYGUN) | WEAPON_BIT(WEAPON_ROCKETGUN));
	cl_Weapons->selectWeapon(cl_CurrentWeapon);
}

CPlayerClient::~CPlayerClient() 
{
	Clear();
}

void CPlayerClient::Clear() 
{
	if (cl_Camera) delete cl_Camera;
	if (cl_Weapons) delete cl_Weapons;

	cl_Camera = nullptr;
	cl_Weapons = nullptr;

	cl_Buttons = 0;
	cl_CurrentWeapon = 0;

	cl_Health = 0;
	cl_Armor = 0;

	cl_uiCurrentTick = 0;

	cl_sId = 0;
}

void CPlayerClient::applyActions(Network::CNetMSG* msg)
{
	/*
	PlayerForClient cl;

	msg->readBlock((byte*)&cl, sizeof(PlayerForClient));
	
	cl_Health = cl.health;
	cl_Armor = cl.armor;
	cl_CurrentWeapon = cl.currentWeapon;

//	cl_vPosition = glm::vec3(cl.vPos[0], cl.vPos[1], cl.vPos[2]);
	
	cl_vPosition = cl.vPos;

	cl_Weapons->wp_ammo = cl.awAmmo;*/
	
	
	msg->readBlock((byte*)&cl_Health, sizeof(short));
	msg->readBlock((byte*)&cl_Armor, sizeof(short));

	byte w;

	msg->read8(&w);
	msg->read8((byte*)&cl_kills);
	msg->read8(&cl_state);

	hud_bShowScore = cl_state;

	cl_CurrentWeapon = w;

	//msg->read32(&cl_CurrentWeapon);
	msg->readVector(&cl_vPosition);

	msg->readBlock((byte*)&cl_Weapons->wp_ammo, sizeof(AmmoWeapon_t));
	
}

void CPlayerClient::sendActionToServer(Network::CNetMSG* msg)
{
	msg->write16(cl_sId);
	/*
	PlayerForServer sv;

	sv.health = cl_Health;
	sv.armor = cl_Armor;
	sv.buttons = cl_Buttons;

//	sv.vpos[0] = cl_vPosition.x;
//	sv.vpos[1] = cl_vPosition.y;
//	sv.vpos[2] = cl_vPosition.z;

	sv.vPos = cl_vPosition;

//	sv.arot[0] = cl_aRotDir.x;
//	sv.arot[1] = cl_aRotDir.y;
//	sv.arot[2] = cl_aRotDir.z;

	sv.aRot = cl_aRotDir;

	msg->writeBlock((byte*)&sv, sizeof(PlayerForServer));
	*/
	
	msg->writeBlock((byte*)&cl_Health, sizeof(int));
	msg->writeBlock((byte*)&cl_Armor, sizeof(int));

	msg->write32(cl_Buttons);

	msg->writeVector(cl_vPosition);
	msg->writeVector(cl_aRotView);
	
}

void CPlayerClient::doActions()
{
	cl_Weapons->selectWeapon(cl_CurrentWeapon);

	cl_Buttons = 0;

	if (bCrouch)    cl_Buttons |= BUTT_CROUCH;
	if (bJump)      cl_Buttons |= BUTT_JUMP;
	if (bMoveLeft)  cl_Buttons |= BUTT_WALK_LEFT;
	if (bMoveRight) cl_Buttons |= BUTT_WALK_RIGHT;
	if (bMoveFront) cl_Buttons |= BUTT_WALK_FOR;
	if (bMoveBack)  cl_Buttons |= BUTT_WALK_BACK;
	if (bFire)      cl_Buttons |= BUTT_FIRE;
	if (bNextWeapon)cl_Buttons |= BUTT_SELECT_WEAPON;
	if (bPrevWeapon)cl_Buttons |= BUTT_PREV_WEAPON;

	float x = pConsoleDataBase->getInt("mouseX");
	float y = pConsoleDataBase->getInt("mouseY");

	doPredictedActions();

	cl_Camera->cameraMoveMouse(x, y);

	cl_aRotView = cl_Camera->m_front;

	pConsoleDataBase->executeVar("mouseX = 0", false);
	pConsoleDataBase->executeVar("mouseY = 0", false);

	pConsoleDataBase->executeVar("bNextWeapon = 0", false);
	pConsoleDataBase->executeVar("bPrevWeapon = 0", false);


}

void CPlayerClient::doPredictedActions() 
{
	Vector3D vViewDir;

	vViewDir = glm::normalize(glm::vec3(cl_Camera->m_front.x, 0.0f, cl_Camera->m_front.z));

	if (cl_Buttons & BUTT_WALK_FOR)
	{
		cl_vPosition += VECTOR3(vViewDir.x, 0.0f, vViewDir.z) * plr_fMoveSpeed * float(Game->lastTime);
	}

	if (cl_Buttons & BUTT_WALK_BACK)
	{
		cl_vPosition -= VECTOR3(vViewDir.x, 0.0f, vViewDir.z) * plr_fMoveSpeed* float(Game->lastTime);
	}

	if (cl_Buttons & BUTT_WALK_RIGHT)
	{
		cl_vPosition += glm::normalize(glm::cross(vViewDir, cl_Camera->m_up)) * plr_fMoveSpeed * float(Game->lastTime);
	}

	if (cl_Buttons & BUTT_WALK_LEFT)
	{
		cl_vPosition -= glm::normalize(glm::cross(vViewDir, cl_Camera->m_up)) * plr_fMoveSpeed * float(Game->lastTime);
	}
	
	if (cl_vPosition.length() > 0.0f)
	{
		glm::normalize(cl_vPosition);
	}

}


void CPlayerClient::setView(CView* pView)
{
	cl_Camera->m_position = cl_vPosition;
	
	//printf("%f %f %f\n", cl_Camera->m_position.x, cl_Camera->m_position.y, cl_Camera->m_position.z);

	pView->m_viewMatrix = cl_Camera->getMatrixView();
	pView->m_ViewDir = cl_Camera->m_position;
}

