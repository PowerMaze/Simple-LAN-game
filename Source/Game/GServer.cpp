#include "stdAfx.h"

#include "GMain.h"
#include "GServer.h"
#include "GameSession.h"
#include "GEntities.h"
#include "GCamera.h"
#include "GConsole.h"
#include "GPlayer.h"
#include "GHUD.h"

static bool sv_bGetMSGSize = false;

CGameSession gameSession;

static void ghostObjectsInfo() 
{
	gameSession.getWorld()->gw_collisionDispatcher.reportInfo();
}

static void spawnPlayer(void)
{
	CPlayer* pl = New<CPlayer>();
	pl->m_pwo = gameSession.getWorld();
	pl->pl_bLocal = true;

//	pl->m_vPosition = VECTOR3(0, 2.f, 0);

	if (pl->pl_strPlayerName)
		Free(pl->pl_strPlayerName);
	pl->pl_strPlayerName = stringCopy(plr_strPlayerName);

	pl->Initialize();

	entityManager.em_allServerEntities.push_back(pl);
	entityManager.spawnPlayerFromSpawner(pl);


	gameSession.gs_pePlayer = pl;

//	pl->SetUpPlayerPosition(VECTOR3(1, 2.f, 0));

	pl->pl_weapons->selectWeapon(WEAPON_MELE);

	gameSession.updatePlayerScores();
}

void serverSetUP(const CFileName& strFileName)
{
	Network::startServer();
		
	gameSession.LoadWorld(strFileName);	

	renderPrepareWorld(gameSession.getWorld());	


	/* add player to world */
	spawnPlayer();

//	gameSession.getWorld()->gw_collisionDispatcher.reportInfo();

	pConsoleDataBase->addConsoleFunction("ghostObjectsInfo()", ghostObjectsInfo);
	pConsoleDataBase->addConsoleVariable(CVT_BOOL, "sv_bGetMSGSize", &sv_bGetMSGSize);

	createCheckSumSession(&net_uiCRC);

	GameHUD::setup();
}

static void removePlayer(const unsigned short& s) 
{
	auto iter = gameSession.gs_listClientsPlayers.find(s);
	if (iter == gameSession.gs_listClientsPlayers.end()) return;

	entityManager.addEntityToDelete(iter->second);
	iter->second = nullptr;

	gameSession.gs_listClientsPlayers.erase(s);
	gameSession.gs_iListClients--;
}

static void applyClientsActions(Network::CNetMSG* msg) 
{
	unsigned short id;

	msg->read16(&id);

	auto iter = gameSession.gs_listClientsPlayers.find(id);

	if (iter == gameSession.gs_listClientsPlayers.end()) return;

	CPlayer* player = iter->second;
	
	player->getFromClient(msg);
}

void serverAnalyseMSG( Network::CNetMSGHolder* holder )
{
	//std::string strName;
	std::string strTeam;
	size_t stringLen;
	char strName[128];
	unsigned short s_Id = 0;
	byte color = 0;

	CPlayer* player = nullptr;

	for ( int i = 0 ; i < holder->msgh_uiNum; i++ ) 
	{
		Network::CNetMSG* msg = holder->getMSG(i);

		switch (msg->msg_netCMD) 
		{
		case NET_CMD_ADD_PLAYER:

			player = new CPlayer;

			msg->read16(&player->pl_sID);
			msg->read8(&color);
			msg->read32(&stringLen);
			msg->readBlock((byte*)strName, stringLen);
			strName[stringLen] = 0;
			
			PrintF("%d\n", player->pl_sID);

			player->pl_bLocal = false;
			player->m_vPosition = VECTOR3(0, 10.f, 0);
			player->pl_strPlayerName = stringCopy(strName);

			player->pl_colorIndex = color;
			player->m_color = playerColorTables[color];

			player->m_pwo = gameSession.getWorld();		

			//player->pl_strTeam       = stringCopy(strTeam.c_str());

			player->Initialize();

			entityManager.spawnPlayerFromSpawner(player);

			// add to entity list
			entityManager.em_allServerEntities.push_back(player);	
			// add to player list
			gameSession.addPlayer(player, player->pl_sID);
			gameSession.gs_iListClients++;
			gameSession.updatePlayerScores();

			PrintF("Player[%d] \"%s\" joined to us, count %d\n", i, strName, gameSession.getClientsCount());

			break;
		case NET_CMD_DISCONNECT_FROM_SERVER:

			msg->read16(&s_Id);

			removePlayer(s_Id);

			gameSession.updatePlayerScores();

			break;

		case NET_CMD_ACTION:

			applyClientsActions(msg);

			break;

		case NET_CMD_NONE:			

			break;
		default:
			break;
		}
	}
}


static void writeEntitiesToData(Network::CNetMSG* msg) 
{
	msg->InitMessage(NET_MAX_BUFFER);

	int iNumModelEntities = 0;

	FOREACH_ARRAY(entityManager.em_allServerEntities.size()) 
	{
		CEntityMain* e = entityManager.em_allServerEntities[i];

		// if we have model entity and if this alive
		if (e->IsValid() && !e->m_bDead && e->m_ulFlags & ENTITY_MODEL/* && !IsClass(e, "Player")*/)
		{	

			/*
			For a network game, it's quite strange to have 64 entities, but our network
			game model doesn't support data compression, expecially delta compression,
			so we're using this for now(
			*/
			if (iNumModelEntities > 64)
			{
				PrintF("^2Warning: entities overflow!!! Count over 64!\n");
				continue;
			}

			msg->write32(e->m_pmo.getModel()->getCRC());
			msg->write32(e->m_pmo.getCRCTexture());
//			msg->write32(e->m_color);
			Vector3D vPos = ((CPhysicsEntity&)*e).getCurrentPosition();

			//short s[3] = { packFloat(vPos.x), packFloat(vPos.y) ,packFloat(vPos.z) };			
			//msg->writeBlock((byte*)s, sizeof(short) * 3);

			msg->writeVector(vPos);

			byte yaw = e->m_anglRotaion.y;
			msg->write8(yaw);

			
			if (IsClass(e, "Player"))
			{
				msg->write8(((CPlayer&)*e).pl_colorIndex);
			}
			else 
			{
				msg->write8(0);
			}

			iNumModelEntities++;
		}
	}
}



static void updateGameLogic(double msc, Network::CNetMSG * msg)
{
	CGameWorld* wo = gameSession.getWorld();

	wo->UpdatePhysics(msc);


	if (Game->gaType == GAME_FINISHED_MATCH && (gameSession.gs_dTimeWait <= (SDL_GetTicks64() * 0.001))) 
	{
		gameSession.rematchPlayer();
		Game->gaType = GAME_RUN;
	}

	FOREACH_ARRAY(entityManager.em_allServerEntities.size())
	{
		entityManager.em_allServerEntities[i]->Update();
	}

	entityManager.deleteKilledEntites();

	writeEntitiesToData(msg);
}

/*
////////////////////////////////////////////////

Writes to client informations

It means thats server send information about player and 
entities in world

////////////////////////////////////////////////
*/
static void writeToClients(Network::CNetMSGHolder* msgHolder, Network::CNetMSG* msg)
{
	msgHolder->Clear();

	Network::CNetMSG msgTmp;

	for (auto& iter : gameSession.gs_listClientsPlayers)
	{
	    msgTmp.InitMessage(NET_MAX_BUFFER);

		//msgTmp.write16(iter.second->pl_sID);

		iter.second->makeForClient(&msgTmp);

		msgTmp.writeBlock(msg->getData(), msg->getLength());
		msgTmp.setCMD(NET_CMD_ACTION);

		if (sv_bGetMSGSize)
		{
			PrintF("NETWORK_MSG_SIZE = %d\n", msgTmp.getLength());
			sv_bGetMSGSize = false;
		}

		Network::sendToClient(&msgTmp, iter.second->pl_sID);
	}

	//Network::sendToClients(msgHolder);
}

void serverRun(double msc)
{
	Network::CNetMSG msg;
	Network::CNetMSGHolder msgHolder;

	// get message from connected clients
	Network::listenClients( &msgHolder );

	// analyse message from connected clients
	serverAnalyseMSG( &msgHolder );	

	// update entity logic
	updateGameLogic(msc, &msg);
	
	// write data from game simulation to clients
	writeToClients( &msgHolder, &msg );

	CPlayer* pl = gameSession.gs_pePlayer;

	// get view matrix from player camera
	if (Game->gaType == GAME_FINISHED_MATCH) 
	{		
		pViewPort->m_viewMatrix = pl->pl_camera->getMatrixView();
		pViewPort->m_ViewDir    = pl->pl_camera->m_position;
	}
	else 
	{
		pl->getPlayerView(pViewPort);
	}

	// render world room, skybox, brushes
	renderView(&gameSession.gs_world, pViewPort);

	// render entities
	renderEntities(entityManager.em_allServerEntities, pViewPort, pl);

	// if we alive, render weapon
	if (pl->pl_state == PL_STATE_ACTIVE) {
		// render model weapon
		pl->pl_weapons->renderWeapon();
	}
	// draw hud to player
	GameHUD::draw(pViewPort);

	// draw console, if needed
	DrawConsole();
}

void serverShutdown() 
{
	Network::CNetMSG msg;

	const char* strEndText = "Server shutdown";	

	msg.InitMessage(strlen(strEndText) + 1);
	msg.writeBlock((byte*)strEndText, strlen(strEndText));
	msg.setCMD(NET_CMD_STOP_SERVER);

	//Network::sendToClients(&msg);

	pConsoleDataBase->deleteConsoleFunction("ghostObjectsInfo()");
	pConsoleDataBase->deleteConsoleVariable("sv_bGetMSGSize");


	for (auto& iter : gameSession.gs_listClientsPlayers)
	{
		Network::sendToClient(&msg, iter.second->pl_sID);
	}


	Network::closeServer();

	/* очищення ресурсів */

	GameHUD::clear();

	entityManager.Clear();

	gameSession.gs_world.Clear();
	//renderClearWorld(&gameSession.gs_world);

	
}