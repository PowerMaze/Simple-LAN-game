#pragma once

#include "GWeapon.h"
#include "GCamera.h"

/*
///////////////////////////////////////////////

Client interface player

//////////////////////////////////////////////
*/
class CPlayerClient
{
public:

	Vector3D cl_vPosition; // position of camera

	Vector3D cl_vLastPosition; // for lerping

	Angle3D  cl_aRotView;  // view direction
	Angle3D  cl_aRotDir;   // view of camera

	CCamera*  cl_Camera;   // client camera, same in server player
	CWeapons* cl_Weapons;  // client weapons, with client mechanics

	unsigned int cl_Buttons; // placed buttons flags used between client players on client - server computers
	 
	unsigned int cl_CurrentWeapon; // current weapon for client

	int cl_Health;  // client ammo
	int cl_Armor;   // client armor

	unsigned short cl_sId; // client identificator

	char cl_kills; // num of client player frags

	byte cl_state; // state of player (active/death/win)

	unsigned int cl_uiCurrentTick; // client tick

public:

	CPlayerClient();
	~CPlayerClient();

public:

	void setPlacement(Vector3D& vPos, Angle3D aRot);

	void Clear(void);

	void setView(CView* pView);

	void doActions(void);

	void doPredictedActions(void);

	void applyActions(Network::CNetMSG* msg);
	void sendActionToServer(Network::CNetMSG* msg);
};