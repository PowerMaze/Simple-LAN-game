//#include <SDL2/SDL.h>
#include "stdAfx.h"

#include <GMain.h>
#include "Menu.h"

int wnd_iScreenW = 640;
int wnd_iScreenH = 480;


/* Game states */
#define GAME_MENU		0
#define GAME_PAUSE		1
#define GAME_RUN		2
#define GAME_RUN_MENU	3
#define GAME_DEAD       4


// load main game list of function pointers
typedef Game_t* (*CREATE_GAME)(void);
// game pointer
extern Game_t* Game = NULL;


// Get command to Game Dlly
//Game_GetCmd Game_GetCommand;

HINSTANCE hGameDLL;


#ifdef _DEBUG 
	#define DLL_NAME "DGame.dll"
#else
	#define DLL_NAME "Game.dll"
#endif

/*
class CWindowCanvas 
{
public:
	int Wigth;
	int Height;
	SDL_Window* window;
	SDL_GLContext glcontext;
	HINSTANCE hGameDLL;

public:
	void WinInit();
	void Run();
};
*/

typedef struct WindowCanvas_s
{
	int Wigth;
	int Height;
	SDL_Window* window;
	SDL_GLContext glcontext;
}WindowCanvas_t;

WindowCanvas_t ScreenPower;


typedef struct VideoResolution_s
{
	const char* description;
	int width;
	int height;
}VideoResolution_t;


VideoResolution_t videoRes[3] =
{
	// 4:3
	{"800x600", 800, 600},
	// 16:9
	{"1280X720", 1280, 720},
    {"1920x1080", 1920, 1080 }
};


static void WindowInit(WindowCanvas_t* wnd)
{
	wnd->Height = wnd_iScreenH;
	wnd->Wigth  = wnd_iScreenW;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		Error("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());			
	
	wnd->window = SDL_CreateWindow("LAN-Game - alpha v.0.1.", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, wnd->Wigth, wnd->Height, SDL_WINDOW_OPENGL);
	
	if (wnd->window == NULL)	
		Error("Window could not be created! SDL_Error: %s\n", SDL_GetError());
	
	wnd->glcontext = SDL_GL_CreateContext(wnd->window);

	if (wnd->glcontext == NULL) 	
		Error("Cannot create OGL context!\nError: %s\n", SDL_GetError());
	
}



void End(void)
{
	InTheEngine_Shutdown();
	SDL_GL_DeleteContext(ScreenPower.glcontext);
	SDL_DestroyWindow(ScreenPower.window);	
	SDL_Quit();
}

bool bFullScreen = false;

void SetupFullScreen() 
{
	if (bFullScreen) {
		bFullScreen = false;
		SDL_SetWindowFullscreen(ScreenPower.window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else {
		bFullScreen = true;
		SDL_SetWindowFullscreen(ScreenPower.window, 0); // Віконний режим
	}
}


bool ChangeResolutionWindow(VideoResolution_t* res/*, bool bFullScreen = false*/)
{

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int winPosX = (screenWidth - res->width) / 2;
	int winPosY = (screenHeight - res->height) / 2;

	ScreenPower.Height = res->height;
	ScreenPower.Wigth = res->width;

	SDL_SetWindowPosition(ScreenPower.window, winPosX, winPosY);

	SDL_SetWindowSize(ScreenPower.window, res->width, res->height);

	pViewPort->setUpView(ScreenPower.Wigth, ScreenPower.Height, 0.1f, 1000.0f, 90.f);	
	setCurrentViewPort(pViewPort);

	return 1;
}


void Quit(void)
{
	Game->StopGame();

	if (hGameDLL != NULL) 
	{
		FreeLibrary(hGameDLL);
	}
	if (Game != NULL) 
	{
		free(Game);
	}
	End();
	exit(EXIT_SUCCESS);
}

/*
bool isLibraryLoaded(const char* libraryName)
{
	HMODULE hMods[1024];
	DWORD cbNeeded;
	HANDLE hProcess = GetCurrentProcess();

	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
		int moduleCount = cbNeeded / sizeof(HMODULE);

		for (int i = 0; i < moduleCount; i++) {
			char moduleName[MAX_PATH];
			if (GetModuleBaseNameA(hProcess, hMods[i], moduleName, sizeof(moduleName) / sizeof(char))) {
				if (strcmp(moduleName, libraryName) == 0) {
					return true; // Знайдено завантажену бібліотеку
				}
			}
		}
	}
	return false; // Бібліотека не знайдена
}
*/

void LoadGameLibrary(const char* strDllName)
{
	PrintF("Loading game library: %s\n", strDllName);
	hGameDLL = LoadLibraryA(strDllName);

	if (hGameDLL == NULL)
	{
		/* error */
		End();
		//fprintf(stderr, "Error loading game library:\n%s\n", strDllName);
		Error("Error loading game library:\n%s\n", strDllName);
	}

	CREATE_GAME GameCreate = (CREATE_GAME)GetProcAddress(hGameDLL, "GameCreate");	

	if (GameCreate == NULL)
	{
		/* error */
		End();
		FreeLibrary(hGameDLL);
		fprintf(stderr, "Cannot get address \"GameCreate\" from library %s\n", strDllName);
		Error("Cannot get address \"GameCreate\" from library %s\n", strDllName);
	}


	Game = GameCreate();

	if (Game == NULL)
	{
		/* error */
		End();
		FreeLibrary(hGameDLL);
		fprintf(stderr, "Fatal error: failed to create game from %s library!", strDllName);
		Error("Fatal error: failed to create game from %s library!", strDllName);
	}
	PrintF("Game library: \"%s\" loaded succesfull\n", strDllName);


	Game->Initialize();
	//PrintF("Game library: '0x%X' loaded succesfull\n", hGameDLL);
}

bool ssucesful = false;

static void ProcessInput(const double time)
{

	Game->Input(time);
	
	if (Game->key == SDLK_ESCAPE) {
		Quit();
	}

	if (Game->KeyDown) 
	{

	//	if (ssucesful)
	//		return;


		if (Game->key == SDLK_1)
		{
			ssucesful = ChangeResolutionWindow(&videoRes[0]);
		}

		if (Game->key == SDLK_2)
		{
			ssucesful = ChangeResolutionWindow(&videoRes[1]);
		}

		if (Game->key == SDLK_3)
		{
			ssucesful = ChangeResolutionWindow(&videoRes[2]);
		}

		if (Game->key == SDLK_4)
		{
			SetupFullScreen();
		}
	}

}

bool prepareScene = FALSE;

bool bStartGame = false;
bool bJoinGame = false;

int setWorld;

static void startGame()
{
	if (prepareScene) return;
	//bStartGame = true;

	prepareScene = Game->NewGame();

	bStartGame = prepareScene;
	if (bStartGame)
		Game->enableMenu = false;
}

static void joinGame()
{
	if (prepareScene) return;

	prepareScene = Game->JoinGame();

	bJoinGame = prepareScene;

	if (bJoinGame)
		Game->enableMenu = false;

}


static void ParseCommandLine(int ac, char** av) 
{
	if (ac == 2) 
	{
		if (strcmp(av[1], "+start") == 0) 
		{
			startGame();
		}
		if (strcmp(av[1], "+gamename") == 0)
		{

		}
		if (strcmp(av[1], "+mapname") == 0)
		{

		}
	}
}



void BeginFrame() 
{

	if (!bStartGame && !bJoinGame)
	{
		Game->ConMenu();
	}
	else {
		Game->RunGame();
	}
//	if (!prepareScene) 
//	{
//		//Game->LoadLevel("Data\\Levels\\Multiplayer\\DM1.wo");
//		prepareScene = Game->LoadGame("");
//	}

}


static void helpConsole() 
{


	const char* strText =
		"/********** CONSOLE HELP ***********/\n"
		" startGame() - start game as server\n"
		" searchGame() - search avaliable session\n"
		" /connect x.x.x.x - join to LAN game, example\n"
		"/connect 192.168.0.6\n"
		" getConsoleLastInputs() - gets whatever you pront in console\n"
		" Quit() - Quit the game\n";


	PrintF(strText);

}



void MainLoop(void)
{
	// setup view of main canvas game
	pViewPort->setUpView(ScreenPower.Wigth, ScreenPower.Height, 0.1f, 1000.0f, 90.f);
	// set as current view, because we have only one view of game
	setCurrentViewPort(pViewPort);
	// time between current and next frame
	double deltaTime = 0.0;	
	// time of last frame
	double lastFrame = 0.0;

	// setup correct mouse input
	SDL_SetRelativeMouseMode(SDL_TRUE);

	// enable menu 
	Game->enableMenu = true;

	// print dummy console style menu
	PrintF("****** Dummy console type menu ******\n");
	PrintF("to create new game, print in console: startGame()\n");
	PrintF("to join local host, print in console: joinGame()\n");
	PrintF("to join a lan game use command: /connect x.x.x.x\n");
	PrintF("to help for console commands, print: helpConsole()\n");

	// adds function to console database 
	pConsoleDataBase->addConsoleFunction("startGame()", startGame);
	pConsoleDataBase->addConsoleFunction("joinGame()", joinGame);
	pConsoleDataBase->addConsoleFunction("Quit()", Quit);

	/* main loop game */
	while (1)
	{	
		// calc time
		double currentFrame = (double)SDL_GetTicks64() * 0.001;		
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// puts time
		Game->lastTime = deltaTime;

		// input logic from keyboard / mouse
		ProcessInput(deltaTime);
		// render view
		BeginFrame();
		// swap buffers
		SDL_GL_SetSwapInterval(1);
    	SDL_GL_SwapWindow(ScreenPower.window);
	}
}

#ifdef NDEBUG
	#define GAME_NAME "Power.log"
#else
	#define GAME_NAME "DPower.log"
#endif

int main(int argc, char** argv)
{		
	/******** init window ********/
	WindowInit(&ScreenPower);

	/******** init engine ********/
	InTheEngine_Start(argc, argv, GAME_NAME);

	/******** load game library ********/
	LoadGameLibrary(DLL_NAME);

	/******** parse command line ********/
	ParseCommandLine(argc, argv);

	/******** main loop game ********/
	MainLoop();
	
	/******** never gets here ********/
	return 1;
}

/*/
int FilterException(unsigned int code, struct _EXCEPTION_POINTERS* ep)
{
	int type = 0;
	char* strMessage = NULL;

	switch (code)
	{
	case EXCEPTION_ACCESS_VIOLATION:
		strMessage = stringCopyF("EXCEPTION_ACCESS_VIOLATION (0x%X)", EXCEPTION_ACCESS_VIOLATION);
		break;
	case EXCEPTION_DATATYPE_MISALIGNMENT:
		strMessage = stringCopyF("EXCEPTION_DATATYPE_MISALIGNMENT (0x%X)", EXCEPTION_DATATYPE_MISALIGNMENT);
		break;
	case EXCEPTION_BREAKPOINT:
		strMessage = stringCopyF("EXCEPTION_BREAKPOINT (0x%X)", EXCEPTION_BREAKPOINT);
		break;
	case EXCEPTION_SINGLE_STEP:
		strMessage = stringCopyF("EXCEPTION_SINGLE_STEP (0x%X)", EXCEPTION_SINGLE_STEP);
		break;
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		strMessage = stringCopyF("EXCEPTION_ARRAY_BOUNDS_EXCEEDED (0x%X)", EXCEPTION_ARRAY_BOUNDS_EXCEEDED);
		break;
	case EXCEPTION_FLT_DENORMAL_OPERAND:
		strMessage = stringCopyF("EXCEPTION_FLT_DENORMAL_OPERAND (0x%X)", EXCEPTION_FLT_DENORMAL_OPERAND);
		break;
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		strMessage = stringCopyF("EXCEPTION_FLT_DIVIDE_BY_ZERO (0x%X)", EXCEPTION_FLT_DIVIDE_BY_ZERO);
		break;
	case EXCEPTION_FLT_INEXACT_RESULT:
		strMessage = stringCopyF("EXCEPTION_FLT_INEXACT_RESULT (0x%X)", EXCEPTION_FLT_INEXACT_RESULT);
		break;
	case EXCEPTION_FLT_INVALID_OPERATION:
		strMessage = stringCopyF("EXCEPTION_FLT_INVALID_OPERATION (0x%X)", EXCEPTION_FLT_INVALID_OPERATION);
		break;
	case EXCEPTION_FLT_OVERFLOW:
		strMessage = stringCopyF("EXCEPTION_FLT_OVERFLOW (0x%X)", EXCEPTION_FLT_OVERFLOW);
		break;
	case EXCEPTION_FLT_STACK_CHECK:
		strMessage = stringCopyF("EXCEPTION_FLT_STACK_CHECK (0x%X)", EXCEPTION_FLT_STACK_CHECK);
		break;
	case EXCEPTION_FLT_UNDERFLOW:
		strMessage = stringCopyF("EXCEPTION_FLT_UNDERFLOW (0x%X)", EXCEPTION_FLT_UNDERFLOW);
		break;
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		strMessage = stringCopyF("EXCEPTION_INT_DIVIDE_BY_ZERO (0x%X)", EXCEPTION_INT_DIVIDE_BY_ZERO);
		break;
	case EXCEPTION_INT_OVERFLOW:
		strMessage = stringCopyF("EXCEPTION_INT_OVERFLOW (0x%X)", EXCEPTION_INT_OVERFLOW);
		break;
	case EXCEPTION_PRIV_INSTRUCTION:
		strMessage = stringCopyF("EXCEPTION_PRIV_INSTRUCTION (0x%X)", EXCEPTION_PRIV_INSTRUCTION);
		break;
	case EXCEPTION_IN_PAGE_ERROR:
		strMessage = stringCopyF("EXCEPTION_IN_PAGE_ERROR (0x%X)", EXCEPTION_IN_PAGE_ERROR);
		break;
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		strMessage = stringCopyF("EXCEPTION_ILLEGAL_INSTRUCTION (0x%X)", EXCEPTION_ILLEGAL_INSTRUCTION);
		break;
	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		strMessage = stringCopyF("EXCEPTION_NONCONTINUABLE_EXCEPTION (0x%X)", EXCEPTION_NONCONTINUABLE_EXCEPTION);
		break;
	case EXCEPTION_STACK_OVERFLOW:
		strMessage = stringCopyF("EXCEPTION_STACK_OVERFLOW (0x%X)", EXCEPTION_STACK_OVERFLOW);
		break;
	case EXCEPTION_INVALID_DISPOSITION:
		strMessage = stringCopyF("EXCEPTION_INVALID_DISPOSITION (0x%X)", EXCEPTION_INVALID_DISPOSITION);
		break;
	case EXCEPTION_GUARD_PAGE:
		strMessage = stringCopyF("EXCEPTION_GUARD_PAGE (0x%X)", EXCEPTION_GUARD_PAGE);
		break;
	case EXCEPTION_INVALID_HANDLE:
		strMessage = stringCopyF("EXCEPTION_INVALID_HANDLE (0x%X)", EXCEPTION_INVALID_HANDLE);
		break;
		//	 case EXCEPTION_POSSIBLE_DEADLOCK:
	default:
		strMessage = stringCopy("NONE");
		type = 1;
		break;
	}

	std::string strError = "Hard error detected:\n";
	if (strMessage) strError += strMessage;

	MessageBoxA(NULL, strError.c_str(), "In the Engine - hard ERROR", MB_ICONERROR | MB_OK);
	PrintF("---------------------------------------------------------------\n");
	PrintF("%s\n", strError.c_str());
	PrintF("---------------------------------------------------------------\n");
	if (strMessage) free(strMessage);

	if (type == 1) return EXCEPTION_CONTINUE_SEARCH;
	if (type == 0) return EXCEPTION_EXECUTE_HANDLER;

	return EXCEPTION_EXECUTE_HANDLER;
}
*/
