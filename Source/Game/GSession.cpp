#include "stdAfx.h"

#include "GameSession.h"
#include "GClient.h"
#include "GPlayer.h"
#include "GEntities.h"
#include "GHUD.h"

static void resetPlayerButtons() 
{
	pConsoleDataBase->executeVar("bMoveLeft = 0");
	pConsoleDataBase->executeVar("bMoveRight = 0");
	pConsoleDataBase->executeVar("bMoveFront = 0");
	pConsoleDataBase->executeVar("bMoveBack =  0");
	pConsoleDataBase->executeVar("bFire = 0");
	pConsoleDataBase->executeVar("bJump = 0");
	pConsoleDataBase->executeVar("bUse = 0");
	pConsoleDataBase->executeVar("bCrouch = 0");
	pConsoleDataBase->executeVar("bPrevWeapon = 0");
	pConsoleDataBase->executeVar("bNextWeapon = 0");

	pConsoleDataBase->executeVar("mouseX = 0");
	pConsoleDataBase->executeVar("mouseY = 0");
}

CEntityManager entityManager;

static int comparePlayerScores(const void* a, const void* b) 
{
	playerScore_t* scA = (playerScore_t*)a;
	playerScore_t* scB = (playerScore_t*)b;

	return scB->iKills - scA->iKills;
}

static void getClientPosition()
{
	if (gameSession.gs_clPlayer == NULL) return;

	PrintF("Position: %f %f %f\n", gameSession.gs_clPlayer->cl_vPosition.x, gameSession.gs_clPlayer->cl_vPosition.y,
		gameSession.gs_clPlayer->cl_vPosition.z);

}

CGameSession::CGameSession() 
{
	gs_pePlayer = nullptr;

	gs_clPlayer = nullptr;

	gs_iListClients = 0;

	gs_maxFrag = 3;

	gs_dTimeWait = 0.0;

	pConsoleDataBase->addConsoleFunction("getClientPosition()", getClientPosition);

	memset(gs_playerScore, 0, sizeof(playerScore_t) * 8);
}

CGameSession::~CGameSession() 
{
	renderClearWorld(&gs_world);
	gs_strFileName.strFileName.clear();
	gs_pePlayer = nullptr;

	if (gs_clPlayer) delete gs_clPlayer;
	gs_clPlayer = nullptr;

	FOREACH_STD_UNORDERED_MAP(gs_listClientsPlayers, unsigned short, CPlayer*) 
	{
		iter->second = nullptr;
	}

	gs_listClientsPlayers.clear();

	gs_iListClients = 0;
}

void CGameSession::updatePlayerScores(void) 
{
	FOREACH_ARRAY(8) 
	{
		gs_playerScore[i].Clear();
	}

	gs_playerScore[0].iIndex = gs_pePlayer->pl_sID;
	gs_playerScore[0].iKills = gs_pePlayer->pl_uiNumKills;

	strcpy(gs_playerScore[0].strPlayerName, gs_pePlayer->pl_strPlayerName);

	int i = 1;

	FOREACH_STD_UNORDERED_MAP(gs_listClientsPlayers, unsigned short, CPlayer*) 
	{
		CPlayer* p = iter->second;

		gs_playerScore[i].iIndex = p->pl_sID;
		gs_playerScore[i].iKills = p->pl_uiNumKills;
		strcpy(gs_playerScore[i].strPlayerName, p->pl_strPlayerName);

		i++;
	}

	qsort(gs_playerScore, 8, sizeof(playerScore_t), comparePlayerScores);


	FOREACH_STD_UNORDERED_MAP(gs_listClientsPlayers, unsigned short, CPlayer*)
	{
		Network::CNetMSG msg(NET_MAX_BUFFER);

		msg.writeBlock((byte*)&gs_playerScore[0], sizeof(playerScore_t) * 8);
		msg.setCMD(NET_CMD_EVENT_CHANGE_SCORE);
		Network::sendToClient(&msg, iter->second->pl_sID);
	}


	if (gs_playerScore[0].iKills == gs_maxFrag)
	{
		resetPlayerButtons();

		Game->gaType = GAME_FINISHED_MATCH;
		hud_bShowScore = true;
		gs_pePlayer->pl_state = PL_STATE_FINISHED;
//		gs_pePlayer->pl_camera->Pitch = 0;
//		gs_pePlayer->pl_camera->Yaw = 0;
		gs_pePlayer->SetUpPlayerPosition(Vector3D(0, 15, 30));

		FOREACH_STD_UNORDERED_MAP(gs_listClientsPlayers, unsigned short, CPlayer*)
		{
			CPlayer* p = iter->second;
			p->pl_state = PL_STATE_FINISHED;
			p->m_vPosition = Vector3D(0, 15, 30);
//			p->pl_camera->Pitch = 0;
//			p->pl_camera->Yaw = 0;
	//		p->SetUpPlayerPosition(Vector3D(0, 15, 30));

		}

		gs_dTimeWait = (SDL_GetTicks64() * 0.001) + 5.0;
	}
}

void CGameSession::rematchPlayer() 
{
	hud_bShowScore = false;

	int i = 0;

	gs_pePlayer->pl_state = PL_STATE_ACTIVE;
	gs_pePlayer->pl_uiNumKills = 0;
	((CPlayerSpawn&)*entityManager.em_allSpawners[i]).SpawnPlayer(gs_pePlayer);

	i++;

	FOREACH_STD_UNORDERED_MAP(gs_listClientsPlayers, unsigned short, CPlayer*)
	{
		CPlayer* p = iter->second;
		p->pl_state = PL_STATE_ACTIVE;
		p->pl_uiNumKills = 0;
		((CPlayerSpawn&)*entityManager.em_allSpawners[i]).SpawnPlayer(p);
		i++;
	}

	updatePlayerScores();
}

void CGameSession::LoadWorld(const CFileName& filename) 
{
	// remember file name
	gs_strFileName = filename;

	CFileStream stream;

	if (!stream.open(gs_strFileName, FILE_BIN, FILE_READ)) 
	{
		Throw("Cannot load world: %s\nNo such file or directory!\n");
	}

	// loads world info, brushes, rooms
	gs_world.LoadWithoutEntities(stream);

	if (!stream.checkID("FILS"))
	{
		gs_world.Clear();
		Throw("Cannot read entity files from world");
	}

	// precache entity data
	uint iCountFiles;
	stream.ReadFromFile(&iCountFiles, sizeof(uint));

	PrintF("Precache data from world...\n");

	FOREACH_ARRAY(iCountFiles)
	{
		std::string string;

		stream.ReadStringFromFile(string);

		pDataBase->loadModel(string);

		stream.ReadStringFromFile(string);

		CFileName f = string;

		FileSetGlobalPatch(f);

		pDataBase->loadTexture(f);

	}

	// we only loads entities from world if we server
	if (Network::isServer()) 
	{
		PrintF("Loading entities...\n");

		gs_world.SetUpPhysics();		
		entityManager.parseEntities(&gs_world, stream);
		//printf("here\n");
	}

	stream.closeFile();
}

void createCheckSumSession(unsigned int* crc)
{
	CFileStream fstream;

	if (!fstream.open(gameSession.gs_strFileName, 0, 1))
	{
		Throw("Cannot open world file \"%s\" for crc32", gameSession.gs_strFileName);
	}


	CRC32::start(crc);

	fstream.calculateFileCRC(crc);

	for (unsigned int i = 0; i < gameSession.getWorld()->gw_numBrushes; i++) 
	{
		CBrush* br = gameSession.getWorld()->gw_brushes[i];

		for (unsigned int j = 0; j < br->m_uPolygonsCount; j++)
		{
			CFileStream file;

			FileDeleteAbsolutePatch(br->m_brushPolygons[j]->tex01FileName);

			bool bResult = file.open(br->m_brushPolygons[j]->tex01FileName, 0, 1);

			if (!bResult) {
				Throw("Cannot open brush texture file \"%s\" for crc32", br->m_brushPolygons[j]->tex01FileName);
			}

			file.calculateFileCRC(crc);

			file.closeFile();


		}
	}

	for (unsigned int i = 0; i < gameSession.getWorld()->gw_numRooms; i++)
	{
		CRoomBase* room = &gameSession.getWorld()->gw_rooms[i];

		for (unsigned int j = 0; j < room->ro_polygons.size(); j++)
		{
			CFileStream file;

			FileDeleteAbsolutePatch(room->ro_polygons[j].tex01FileName);

			bool bResult = file.open(room->ro_polygons[j].tex01FileName, 0, 1);

			if (!bResult) {
				Throw("Cannot open room texture file \"%s\" for crc32", room->ro_polygons[j].tex01FileName);
			}

			file.calculateFileCRC(crc);

			file.closeFile();
		}
	}

	CRC32::end(crc);

	PrintF("CRC session: %u\n", *crc);

	fstream.closeFile();
}