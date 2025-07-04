#include "stdAfx.h"

#include "GameSession.h"
#include "GServer.h"
#include "GClient.h"
#include "GConsole.h"
#include "GCamera.h"
#include "GEntities.h"
#include "GPlayerClient.h"
#include "GHUD.h"


//static std::vector<CModel*> entities;


struct EntityInfo 
{
	CModel* model;
	HEXColor color;
};

static std::vector <EntityInfo*> entities;

static void getPlayerPosition(void) 
{
	CPlayerClient* cl = gameSession.getClientPlayer();

	PrintF("Pos: %f %f %f\n", cl->cl_vPosition.x, cl->cl_vPosition.y, cl->cl_vPosition.z);

}


void clientDisconnect(Network::CNetMSG* msg)
{
	GameHUD::clear();

	renderClearWorld(&gameSession.gs_world);

	if (gameSession.gs_clPlayer)
		delete gameSession.gs_clPlayer;

	gameSession.gs_clPlayer = NULL;

	Network::closeClient();

	pConsoleDataBase->deleteConsoleFunction("getPlayerPosition()");

	clientClear();

	char strText[64];

	msg->readBlock((byte*)strText, msg->getLength());
	strText[msg->getLength()] = 0;

	PrintF("Stopped client, reason: %s\n", strText);

	pConsoleDataBase->executeFunc("Quit()");
}




void clientSetUp(const CFileName& strFileName)
{
	Network::startClient();
	gameSession.LoadWorld(strFileName);
	renderPrepareWorld(&gameSession.gs_world);

	pConsoleDataBase->addConsoleVariable(CVT_BOOL, "bMoveLeft", &bMoveLeft);
	pConsoleDataBase->addConsoleVariable(CVT_BOOL, "bMoveRight", &bMoveRight);
	pConsoleDataBase->addConsoleVariable(CVT_BOOL, "bMoveFront", &bMoveFront);
	pConsoleDataBase->addConsoleVariable(CVT_BOOL, "bMoveBack", &bMoveBack);
	pConsoleDataBase->addConsoleVariable(CVT_BOOL, "bFire", &bFire);
	pConsoleDataBase->addConsoleVariable(CVT_BOOL, "bJump", &bJump);
	pConsoleDataBase->addConsoleVariable(CVT_BOOL, "bUse", &bUse);
	pConsoleDataBase->addConsoleVariable(CVT_BOOL, "bCrouch", &bCrouch);

	pConsoleDataBase->addConsoleVariable(CVT_BOOL, "bNextWeapon", &bNextWeapon);
	pConsoleDataBase->addConsoleVariable(CVT_BOOL, "bPrevWeapon", &bPrevWeapon);

	pConsoleDataBase->addConsoleFunction("getPlayerPosition()", &getPlayerPosition);


	CFileName m_strTexture = "Data\\Models\\Players\\Test\\Box.te";

	FileSetGlobalPatch(m_strTexture);

	pDataBase->loadModel("Data\\Models\\Players\\Test\\Box.mo");
	pDataBase->loadTexture(m_strTexture);

	createCheckSumSession(&net_uiCRC);
}

void clientShutDown() 
{
	// if player null, this means we not in game here
	if (gameSession.gs_clPlayer == nullptr) return;

	GameHUD::clear();

	renderClearWorld(&gameSession.gs_world);

	Network::CNetMSG msg;
	msg.InitMessage(20);

	msg.write16(gameSession.gs_clPlayer->cl_sId);
	msg.setCMD(NET_CMD_DISCONNECT_FROM_SERVER);


	Network::sendToServer(&msg);

	if (gameSession.gs_clPlayer)
		delete gameSession.gs_clPlayer;

	gameSession.gs_clPlayer = NULL;

	Network::closeClient();

	pConsoleDataBase->deleteConsoleFunction("getPlayerPosition()");

	clientClear();
}

void proccessConnectToServer(void) 
{
	char string[128];
	size_t sizestring;

	/* first we need connect to server */
	Network::CNetMSG msg;
	msg.InitMessage(64);	
	msg.writeBlock(getPublicKey(), 32 * sizeof(byte));
	msg.write32(net_uiCRC);
	msg.setCMD(NET_CMD_CONNECT);
	Network::sendToServer( &msg );
	msg.Clear();

	/* second we get from server info and add player */

	Network::getFromServer(&msg);

	if (msg.getData() == nullptr)
		Throw("Cannot get answer from server\n");

	if (msg.msg_netCMD == NET_CMD_BAD) 
	{
		strcpy(string, (const char*)msg.getData());
		Throw("%s", string);
	}

	gameSession.gs_clPlayer = new CPlayerClient;

	//uint iWbit = WEAPON_BIT(WEAPON_MELE) | WEAPON_BIT(WEAPON_AR) | WEAPON_BIT(WEAPON_ROCKETGUN) | WEAPON_BIT(WEAPON_SHOTGUN);

	//gameSession.gs_clPlayer->cl_Weapons->Precache(iWbit);
	//PrintF("1 ====================================================================\n");

	msg.read16(&gameSession.gs_clPlayer->cl_sId);
	msg.read32(&sizestring);

	//PrintF("id %d, size %d\n", gameSession.gs_clPlayer->cl_sId, sizestring);

	msg.readBlock((byte*)string, sizestring);

	PrintF("%s\n", string);

	msg.Clear();

	msg.InitMessage(1024);

	msg.write16(gameSession.gs_clPlayer->cl_sId);
	msg.write8(plr_colColor);
	msg.write32(strlen(plr_strPlayerName));
	msg.writeBlock((byte*)plr_strPlayerName, strlen(plr_strPlayerName));
	msg.setCMD(NET_CMD_ADD_PLAYER);

	//PrintF("2 ====================================================================\n");

	Network::sendToServer( &msg );

	/* done */

	GameHUD::setup();
}

void clientGetMsg(Network::CNetMSG* msg)
{
	Network::getFromServer(msg);
	
	// kill client if server has stopped match
	if (msg->msg_netCMD == NET_CMD_STOP_SERVER) clientDisconnect(msg);

	// if we change table on server side, just read structure and get action from server
	if (msg->msg_netCMD == NET_CMD_EVENT_CHANGE_SCORE) 
	{
		msg->readBlock((byte*)&gameSession.gs_playerScore[0], sizeof(playerScore_t) * 8);
		Network::getFromServer(msg);
	}

	if (msg->msg_netCMD != NET_CMD_ACTION) return;

	clientClear();

	// first we get info for player
	CPlayerClient* cl = gameSession.getClientPlayer();



	cl->applyActions(msg);

	unsigned int iModelCRC, iTextureCRC;

//	unsigned int color;

	// second we get info for entities
	while ( 1 )
	{
		if (msg->msg_uiCurrentPosition >= msg->msg_uiNumData) break;	

		msg->read32(&iModelCRC);
		msg->read32(&iTextureCRC);
		//msg->read32(&color);

		Vector3D vPos = VECTOR3_NONE;

		msg->readVector(&vPos);

		byte yaw;
		byte index;

		msg->read8(&yaw);
		msg->read8(&index);

		CModelObject*    model = pDataBase->loadModelCRC(iModelCRC); 
		CTextureObject* texture = pDataBase->loadTextureCRC(iTextureCRC);

		assert(model != NULL);
		assert(texture != NULL);

		CModel* mo = new CModel;

		mo->SetModel(model);
		mo->SetTexture(texture);
		mo->SetUpModel();	

		mo->modelMatrix = glm::mat4(1.f);	
		mo->modelMatrix = glm::translate(mo->modelMatrix, vPos);
		mo->modelMatrix = glm::rotate(mo->modelMatrix, AngleToRad(float(yaw)) * float((SDL_GetTicks64() * 0.001)), VECTOR3(0, 1, 0));
		//memcpy(glm::value_ptr(mo->modelMatrix), m, sizeof(m));


		EntityInfo* info = new EntityInfo;
		info->color = playerColorTables[index];
		info->model = mo;


		entities.push_back(info);
	}
}


void clientSentToServer(Network::CNetMSG* msg) 
{
	msg->InitMessage(512);
	msg->setCMD(NET_CMD_ACTION);

	CPlayerClient* plcl = gameSession.getClientPlayer();

	plcl->doActions();
	plcl->setView(pViewPort);
	plcl->sendActionToServer(msg);

	Network::sendToServer(msg);

}

void clientClear() 
{
	FOREACH_ARRAY(entities.size())
	{
		delete entities[i]->model;
		delete entities[i];
		entities[i] = NULL;
	}

	entities.clear();
}

void clientRun(double time)
{
	Network::CNetMSG msg;

	clientGetMsg( &msg );

	msg.Clear();

	clientSentToServer(&msg);

	renderView(gameSession.getWorld(), pViewPort);

	FOREACH_ARRAY(entities.size()) 
	{
		renderOneModel(entities[i]->model, entities[i]->color, pViewPort);
	}

	if (gameSession.getClientPlayer()->cl_state == PL_STATE_ACTIVE)
		gameSession.getClientPlayer()->cl_Weapons->renderWeapon();

	GameHUD::draw(pViewPort);

	DrawConsole();
}