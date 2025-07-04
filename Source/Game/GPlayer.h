#pragma once

#include "GWeapon.h"
#include "GCamera.h"

/*
//////////////////////////////////////////////////

Special entity class for server player. It also contains
clients player, but only

//////////////////////////////////////////////////
*/

// name of player
extern char plr_strPlayerName[64];
// color of player
extern byte plr_colColor;
// name of player's team
//extern char plr_strTeamName[64];

extern bool bMoveLeft;
extern bool bMoveRight;
extern bool bMoveFront;
extern bool bMoveBack;
extern bool bFire;
extern bool bJump;
extern bool bUse;
extern bool bFly;
extern bool bCrouch;
extern bool bNextWeapon;
extern bool bPrevWeapon;
extern float plr_fMoveSpeed;

#define PLAYER_ALIVE 0x01
#define PLAYER_DEAD

#define PLAYER_EYES 1.850f

#define MAX_HEALTH 200
#define MAX_ARMOR  200

#define TOP_HEALTH 100
#define TOP_ARMOR  100


/*
* Buttons flags for player action control communication betwenn server - client
*/

#define BUTT_WALK_LEFT     ( 1u << 0 )
#define BUTT_WALK_RIGHT    ( 1u << 1 )
#define BUTT_WALK_FOR      ( 1u << 2 )
#define BUTT_WALK_BACK     ( 1u << 3 )
#define BUTT_JUMP          ( 1u << 4 )
#define BUTT_CROUCH        ( 1u << 5 )
#define BUTT_FIRE          ( 1u << 6 )
#define BUTT_SELECT_WEAPON ( 1u << 7 )
#define BUTT_PREV_WEAPON   ( 1u << 8 )

// cheat code for recover player
POWER_INLINE void PowerRefresh(int* h, int* a)
{
	*h = MAX_HEALTH;
	*a = MAX_ARMOR;
}

// structure sending from client to server
struct PlayerForServer
{
	int health;
	int armor;
	unsigned int buttons;
//	float vpos[3];
//	float arot[3];

	Vector3D vPos;
	Angle3D aRot;

};

// structure sending from server to client
struct PlayerForClient
{
	int health;
	int armor;
	int currentWeapon;
	Vector3D vPos;
//	float vPos[3];
	AmmoWeapon_t awAmmo;
};


#define PL_STATE_ACTIVE   0   // player active
#define PL_STATE_DEATH    1   // player death
#define PL_STATE_FINISHED 2   // player has finished match


/*
Basic player entity
*/

class CPlayer : public CPhysicsEntity
{
public: 	
	char* pl_strPlayerName;	// player name
	char* pl_strTeam;       // player team
	int m_iArmor;           // armor
	//Vector4D pl_ColorName;        // player color name

	byte pl_colorIndex;     // color index of player, see GEntities.cpp -> HEXColor playerColorTables[8]

	unsigned int pl_uiPlacedButtons; // buttons (used for CRC and Network)

	bool pl_bLocal;                 // if this true, we control player from own computer

	short pl_uiNumDeaths;               // count how many player wa killed
	char  pl_uiNumKills;                // count how many player kills other players
	short pl_uiNumPing;                 // clients ping

	unsigned short pl_sID;      // player id (for clients player)
	 
	bool pl_bRespawn;                   // should respawn
	bool pl_bSpectate;                  // player spectate

	CWeapons* pl_weapons;            // player weapons
	unsigned int pl_uiWeaponMask;    // avaliable weapons for player
	unsigned int pl_ulCurrentWeapon; // current weapon for player

	double pl_dDeathTime;            // death time to next recover

	byte pl_state;                   // state of player (alive/death)

	CCamera* pl_camera; // view camera
public:
	// constructor
	CPlayer();
	// destructor
	~CPlayer();
	// initialize class
	void Initialize();
	// set new player name
	void SetPlayerName(const char* strName);
	// set player team
	void SetPlayerTeam(const char* strName);
	// delete player class
	void End() override;
	// respawn player
	void Respawn();
	// action from keyboard/client message
	void doActions();
	// handle touch event (damage/item)
	void applyTouchEvent(touchHandle_t* handle) override;
	// ????
	void RenderView(void);
	// set player view
	void SetUpPlayerPosition(const Vector3D& vPos);
	// setup view matrix from player camera
	void getPlayerView(CView* viewPort);
	// death of player
	void Death(void);
	// update server/client player entity
	void Update() override;
public:

	// gets setting from client computer and checks
	void getFromClient(Network::CNetMSG* msg);
	// make update setting and send to client
	void makeForClient(Network::CNetMSG* msg);

public:

	// block all player actions, needed by finished game
//	void blockPlayer(void);
//	// rematch player
//	void rematchPlayer(void);
};
