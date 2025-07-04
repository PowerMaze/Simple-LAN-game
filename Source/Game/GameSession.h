#pragma once

#include "GPlayerClient.h"

typedef struct playerScore 
{

	char strPlayerName[64];
	int iIndex;
	int iKills;

	void Clear() 
	{
		iIndex = 0;
		iKills = -591;
		memset(strPlayerName, 0, sizeof(strPlayerName));
	}

}playerScore_t;

class CGameSession 
{
public:

	CFileName gs_strFileName; // world file name
	CGameWorld gs_world;      // world histance

	class CPlayer* gs_pePlayer; // local player

	std::unordered_map<unsigned short, CPlayer*> gs_listClientsPlayers; // list of connected player for fast lookup

	CPlayerClient* gs_clPlayer; // local client player
	int gs_iListClients;        // count of clients players

	int gs_maxFrag;

	playerScore_t gs_playerScore[8];

	double gs_dTimeWait;

public:

	CGameSession();
	~CGameSession();

	void LoadWorld(const CFileName& filename);
	POWER_INLINE int getClientsCount() { return gs_listClientsPlayers.size(); }

	POWER_INLINE void addPlayer(CPlayer* pla, unsigned short index) 
	{
		gs_listClientsPlayers[index] = pla;
	}

	POWER_INLINE CPlayer* getLocalPlayer(void)    {	return gs_pePlayer;	}

	POWER_INLINE CPlayerClient* getClientPlayer(void) { return gs_clPlayer; }

	POWER_INLINE CGameWorld* getWorld(void) { return &gs_world; };

	void rematchPlayer(void);

	void updatePlayerScores(void);

};

/* creates check sum of game session. Includes world file, texture data, model data */
void createCheckSumSession(unsigned int *crc);

extern CGameSession gameSession;