#pragma once

/*
//////////////////////////////////////////////////////////////

Server interface, this code activate only in local client host

//////////////////////////////////////////////////////////////
*/

/*
///////////////////////////////////

EntityClient_t. This simple entity
where server send to client(s) to render screen.

Doesnt contains any logic, just data to render

///////////////////////////////////
*/
typedef struct EntityClient
{

	unsigned int uiCRCModel;
	unsigned int uiCRCTexture;

	byte bRender;

	Vector3D vPos;
	Angle3D  aRot;
}EntityClient_t;

/*
////////////////////////////////////////////////

ServerInfo. This send to client, if we search LAN games

////////////////////////////////////////////////
*/
typedef struct ServerInfo 
{
	CFileName strWorldFile;  

	char* strMapName;

	byte ubNumPlayers;
	byte ubMaxPlayers;
	byte ubPing;

	char* strAddress;

	struct ServerInfo* next;

}ServerInfo_t;

/*ServerInfo_t si_ListServersInfo;*/


typedef struct GameSetting_s
{
	byte gs_gameType;
	byte gs_ubMaxPlayers;

	//byte gs_bInfinityAmmo;
	//byte gs_bProtect;
}GameSetting_t;



void serverSetUP(const CFileName& strFileName);
void serverShutdown(void);
void serverRun(double ms);


void serverUpdate(double ms);
void serverSendToClients(void);


//extern CGameSession gameSession;
//void serverGetFromClient(int client, Network::CNetMSG* msg);
//void serverSendToClient(int client, Network::CNetMSG* msg);


