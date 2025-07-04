#ifndef GMAIN_H
#define GMAIN_H

/*
====================================================
*                  Game state
====================================================
*/
enum e_gameStateType
{
	GAME_NONE,  // on init game (loads Game.dll)
	GAME_START, // start game (load level, or save game)
	GAME_RUN,   // run game logic
	GAME_PAUSE, // pause game
	GAME_FINISHED_MATCH, // when we finnish match
	GAME_STOP,  // stop game, unload level
	GAME_MAIN_MENU, // main menu
	GAME_IN_GAME_MENU, // when game runs, but activated menu
	GAME_CONSOLE, // when user calls console in game
	GAME_CONNECT, // when connect to server
	GAME_DISSCONECT // when disconnect from server
};


enum EGameType
{
	GT_SINGLE,
	GT_COOP,
	GT_DEADMATH,
	GT_TEAMMATCH,
	GT_CTF,
	GT_DEFRAG
};


struct GameSession 
{
	CGameWorld gs_gwWorld;

	unsigned int gs_numPlayers;
	bool gs_bPaused;

	enum EGameType gs_eType;

	int gs_iMaxFrags;

	double gw_dTimeLeft;
};

/*
/////////////////////////////////////////////////////////////

Main game structure of function pointers. This only way
to get access to game.dll functions

//////////////////////////////////////////////////////////////
*/
struct Game_s 
{
	/* PID process of application */
	int ProccessId;

	/* keyboard key detect */
	int key;
	bool KeyDown, KeyUp;
	/* mouse coordinates */
	int mouseX, mouseY;

	/* check if enable game menu */
	bool enableMenu;

	/* check if we server */
	bool isServer;

	/* states of game */
	enum e_gameStateType gaType;

	/* last drawn time uses for lerping */
	double lastTime;

	void ( * ConMenu )(void);

	/* read key from keyboard */
	void ( * ReadKey )(const float ftime);
	/* initialize game instance */
	void (*  Initialize )(void);
	/* begin game */
	void ( * RunGame )(void);
	/* stop game, paused or change level */
	void ( * StopGame)(void);
	/* end entire game */
	void ( * EndGame )(void);
	/* input of game */
	void ( * Input)(const float time);
	/* loading level data to play world */
	void ( * LoadLevel)(const char* name);
	/* write game */
	void ( * SaveGame )(string_t strFileName);
	/* read game data and run game */
	bool ( * LoadGame )(const char* strFileName);
	/* make screenshot */
	void ( * ScreenShot)(void);
	/* start new game from level */
	bool ( * NewGame )(void);
	bool ( * JoinGame)(void);
	/* join to game network (starts as client) */
	//void ( *JoinToGame )(const char* addr, unsigned short port);
};

typedef struct Game_s Game_t;

extern Game_t* Game;


/*
///////////////////////////////////////////////////////

@brief 
This function create game
and call once by Power.exe

DO NOT CHANGE THIS!!!

///////////////////////////////////////////////////////
*/
extern "C" __declspec(dllexport) Game_t * GameCreate(void);
//__declspec(dllexport) void GameEND(void);
//__declspec(dllexport) void Game_GetCommand(unsigned int iCmd, const string_t Params);

#endif