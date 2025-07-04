// dllmain.cpp : Defines the entry point for the DLL application.

#include "stdAfx.h"
#include "GMain.h"
#include "GCamera.h"
#include "GConsole.h"
#include "GEntities.h"
#include "GServer.h"
#include "GClient.h"
#include "GameSession.h"

//#include "GPlayer.h"

char plr_strPlayerName[64] = { 0 };
byte plr_colColor = 0;

/* Game object */
Game_t* Game;

static CTextureObject* pteCrosshair = nullptr;

static CTextureObject teLogoA;
static CTextureObject teLogoB;

bool hud_bShowScore = false;

objectDraw_t obj;

CGameWorld world;

CCamera* camera = NULL;

#define MOVE_NONE   0x000000UL
#define MOVE_BACK   (1UL << 0)
#define MOVE_FRONT  (1UL << 1)
#define MOVE_LEFT   (1UL << 2)
#define MOVE_RIGHT  (1UL << 3)
#define MOVE_UP     (1UL << 4)
#define MOVE_DOWN   (1UL << 5)

unsigned long b_MovementFlags = MOVE_NONE;
bool mouseAction = false;

void DoSomeThing(void)
{
    PrintF("Call from Game.dll33\n");
}


void ScreenShot();


void ReadKey(const float ftime)
{
    //PrintF("Call ReadKey\n");
}


void DrawCrosshair(void)
{
    //uint uiWidth  = pViewPort->GetWidth();
    //uint uiHeight = pViewPort->GetHeight();

    //uiWidth  /= 2;
    //uiHeight /= 2;

    //uint sizeW = 16 * pViewPort->GetWidth() / 1024;
    //uint sizeH = 16 * pViewPort->GetWidth() / 1024;

    //drawTextureToScreen(pteCrosshair, pViewPort, uiWidth, uiHeight, sizeW, sizeH, TEX_TRANSPARENT, COLOR_WHITE);
}


bool bLock = true;
bool bJmp = false;

static bool bTest = true;

static int mouseX = 0;
static int mouseY = 0;

void Input(const float time) 
{
    ProcessInputConsole();

    if (openConsole) return;

    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_MOUSEMOTION/* && Game->gaType != GAME_FINISHED_MATCH*/)
        { 
            mouseX = e.motion.xrel;
            mouseY = e.motion.yrel;
        }

        if (e.type == SDL_MOUSEWHEEL && Game->gaType != GAME_FINISHED_MATCH)
        {

            if (e.wheel.preciseY > 0.0f)
            {
                pConsoleDataBase->executeVar("bNextWeapon=1", false);
            }
            else 
            {
                pConsoleDataBase->executeVar("bPrevWeapon=1", false);
            }

        }

        if (e.type == SDL_MOUSEBUTTONDOWN && Game->gaType != GAME_FINISHED_MATCH)
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                pConsoleDataBase->executeVar("bFire = 1", false);
                /*Network::sendText("Secret text between server and clients");*/
            }
        }
        if (e.type == SDL_MOUSEBUTTONUP && Game->gaType != GAME_FINISHED_MATCH)
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                pConsoleDataBase->executeVar("bFire = 0", false);
            }
        }

        if (e.type == SDL_KEYDOWN)
        {

            int state = 0;

            Game->KeyDown = TRUE;
            Game->KeyUp = FALSE;

            Game->key = e.key.keysym.sym;

           // PrintF("key pressed %s\n", SDL_GetKeyName(Game->key));            

            if (e.key.keysym.sym == SDLK_w && Game->gaType != GAME_FINISHED_MATCH)
                pConsoleDataBase->executeVar("bMoveFront=1", false);
                
            if (e.key.keysym.sym == SDLK_s && Game->gaType != GAME_FINISHED_MATCH)
                pConsoleDataBase->executeVar("bMoveBack=1", false);
                
            if (e.key.keysym.sym == SDLK_a && Game->gaType != GAME_FINISHED_MATCH)
                pConsoleDataBase->executeVar("bMoveLeft=1", false);
                
            if (e.key.keysym.sym == SDLK_d && Game->gaType != GAME_FINISHED_MATCH)
                pConsoleDataBase->executeVar("bMoveRight=1", false);
                
            if (e.key.keysym.sym == SDLK_SPACE /* && !bJmp */ && Game->gaType != GAME_FINISHED_MATCH)
            {
           //     bJmp = true;
           ///     state = 1;
                pConsoleDataBase->executeVar("bJump = 1", false);
            }

           // if (e.key.keysym.sym == SDLK_SPACE && !state)
           // {
            //    bJmp = false;   
            //    pConsoleDataBase->executeVar("bJump = 0");
           // }

                //b_MovementFlags |= MOVE_UP;
           // if (e.key.keysym.sym == SDLK_RCTRL || e.key.keysym.sym == SDLK_LCTRL);
                //b_MovementFlags |= MOVE_DOWN;

            if (e.key.keysym.sym == SDLK_TAB)
            {
                hud_bShowScore = true;
            }

            if (e.key.keysym.sym == SDLK_BACKQUOTE && !openConsole)
            {
                openConsole = true;
                b_MovementFlags = MOVE_NONE;
                state = 1;
                //memset(&e, 0, sizeof(SDL_Event));
                //pConsoleDataBase->executeVar("bTest = 0");
                //PrintF("bTest = %d\n", bTest);

            }

            if (e.key.keysym.sym == SDLK_BACKQUOTE && openConsole && !state)
            {
                //pConsoleDataBase->executeVar("bTest = 1");
               // PrintF("bTest = %d\n", bTest);
                openConsole = false;
            }


        }

        if (e.type == SDL_KEYUP)
        {
            Game->KeyDown = FALSE;
            Game->KeyUp = TRUE;

            Game->key = e.key.keysym.sym;

            if (e.key.keysym.sym == SDLK_w && Game->gaType != GAME_FINISHED_MATCH)
                pConsoleDataBase->executeVar("bMoveFront=0", false);
                //b_MovementFlags &= ~MOVE_FRONT;
            if (e.key.keysym.sym == SDLK_s && Game->gaType != GAME_FINISHED_MATCH)
                pConsoleDataBase->executeVar("bMoveBack=0", false);
                //b_MovementFlags &= ~MOVE_BACK;
            if (e.key.keysym.sym == SDLK_a && Game->gaType != GAME_FINISHED_MATCH)
                pConsoleDataBase->executeVar("bMoveLeft=0", false);
                //b_MovementFlags &= ~MOVE_LEFT;
            if (e.key.keysym.sym == SDLK_d && Game->gaType != GAME_FINISHED_MATCH)
                pConsoleDataBase->executeVar("bMoveRight=0", false);


            if (e.key.keysym.sym == SDLK_SPACE /* && !bJmp */ && Game->gaType != GAME_FINISHED_MATCH)
            {
                //     bJmp = true;
                ///     state = 1;
                pConsoleDataBase->executeVar("bJump = 0", false);
            }

            if (e.key.keysym.sym == SDLK_TAB)
            {
                hud_bShowScore = false;
            }

               // b_MovementFlags &= ~MOVE_RIGHT;
           /* if (e.key.keysym.sym == SDLK_SPACE) {
            }*/
               // b_MovementFlags &= ~MOVE_UP;
            if (e.key.keysym.sym == SDLK_RCTRL || e.key.keysym.sym == SDLK_LCTRL);
               // b_MovementFlags &= ~MOVE_DOWN;

            if (e.key.keysym.sym == SDLK_r) 
            {
               // bLock = false;
               // bFire = false;
            }

            if (e.key.keysym.sym == SDLK_9)
                ScreenShot();
        }
		
    }   


    if (bFire/* && bLock*/ && !Game->enableMenu)
    {
       // PrintF("^2Cannot ^3You ^6Stop?\n");
       // entityManager.spawnProjectile(PRT_ENERGY_BALL, camera->m_position, camera->m_front);
       // GEntity::spawnProjectile(&world, PRT_ENERGY_BALL, camera->m_position, camera->m_front);
    }

    if (b_MovementFlags & MOVE_FRONT)
        camera->cameraMoveKeyboard(CAM_FRONT, time);

    if (b_MovementFlags & MOVE_BACK)
        camera->cameraMoveKeyboard(CAM_BACK, time);

    if (b_MovementFlags & MOVE_LEFT)
        camera->cameraMoveKeyboard(CAM_LEFT, time);

    if (b_MovementFlags & MOVE_RIGHT)
        camera->cameraMoveKeyboard(CAM_RIGHT, time);

    if (b_MovementFlags & MOVE_UP)
        camera->cameraMoveKeyboard(CAM_UP, time);

    if (b_MovementFlags & MOVE_DOWN)
        camera->cameraMoveKeyboard(CAM_DOWN, time);
    
}

void UpdateLogic(void)
{
    int maxSubSteps = 10;

    world.UpdatePhysics(Game->lastTime);

    FOREACH_ARRAY(world.gw_allEntities.size()) 
    {
        world.gw_allEntities[i]->Update();
    }
}


bool NewGame(void)
{
    CFileName FileName = "Data\\Levels\\Multiplayer\\DM1.wo";    

    try 
    {
        serverSetUP(FileName);
    }
    catch (char* e) 
    {
        PrintF("Cannot create server\n%s\n", e);
        Network::closeServer();
        return false;
    }

    openConsole = false;

    return true;
}

bool JoinGame(void)
{
    CFileName FileName = "Data\\Levels\\Multiplayer\\DM1.wo";

    try 
    {
        clientSetUp(FileName);
        proccessConnectToServer();
    }
    catch (char* e) 
    {
        PrintF("Cannot join game:\n%s\n", e);
        Network::closeClient();
        return false;
    }

    openConsole = false;

    return true;
}

void ConMenu(void) 
{
    drawColorFull(COLOR_BLACK);


    //float fAdjuster = (4.f / 3.f) * (float(pViewPort->GetHeight()) / float(pViewPort->GetWidth()));

/*   uint uiSizeW = 256 * pViewPort->GetWidth() / 1024;
    uint uiSizeH = 64 * pViewPort->GetHeight() / 768;

    uiSizeW *= pViewPort->GetAdjuster();//fAdjuster;
    //uiSizeH *= fAdjuster;

    uint uiWidth =  ((pViewPort->GetWidth()) / 2) - uiSizeW;
    uint uiHeight = pViewPort->GetHeight() / 2;

    //drawSquare(uiWidth, uiHeight, uiSizeW, uiSizeH, COLOR_ADD_ALPHA(COLOR_BLUE, 192));
    drawTextureToScreen(&teLogoA, pViewPort, uiWidth, uiHeight, uiSizeW, uiSizeH, TEX_TRANSPARENT, COLOR_WHITE);

    uiWidth += uiSizeW*2;

   // drawSquare(uiWidth, uiHeight, uiSizeW, uiSizeH, COLOR_ADD_ALPHA(COLOR_YELLOW, 192));

    drawTextureToScreen(&teLogoB, pViewPort, uiWidth, uiHeight, uiSizeW, uiSizeH, TEX_TRANSPARENT, COLOR_WHITE);*/

    DrawConsole();
}


void RunGame(void)
{

    //Network::getPackets();

    UpdateLogic();

    pViewPort->m_viewMatrix = camera->getMatrixView();
    pViewPort->m_ViewDir    = camera->m_position;

    RenderWorld(&world);

    DrawCrosshair();

    DrawConsole();

    int w = 0, h = 440;

    w *= pViewPort->GetWidth() / 640;
    h = (pViewPort->GetHeight() * h) / 480;

   // float fSize = 1.0f;


   // fSize *= (3.f / 4.f) * (float(pViewPort->GetWidth()) / float(pViewPort->GetHeight()));
   // printf("fSize = %f\n", fSize);
   // drawCenteredText(pFontDisplay, pViewPort, 480, 1.0f, COLOR_GREEN, FALSE, "HP = 100");

    drawText(pFontDisplay, w, h, 1.0f, COLOR_GREEN, "HP = 100%");
}


static void UnloadConsoleVariables(void) 
{
    pConsoleDataBase->deleteConsoleVariable("yaw");
    pConsoleDataBase->deleteConsoleVariable("pitch");
    pConsoleDataBase->deleteConsoleVariable("speed");
    pConsoleDataBase->deleteConsoleVariable("sensinivity");
    pConsoleDataBase->deleteConsoleVariable("zoom");
    pConsoleDataBase->deleteConsoleVariable("bTest");
    pConsoleDataBase->deleteConsoleVariable("fSpeedFactor");

    pConsoleDataBase->deleteConsoleVariable("bMoveLeft");
    pConsoleDataBase->deleteConsoleVariable("bMoveRight");
    pConsoleDataBase->deleteConsoleVariable("bMoveFront");
    pConsoleDataBase->deleteConsoleVariable("bMoveBack");
    pConsoleDataBase->deleteConsoleVariable("bFire");
    pConsoleDataBase->deleteConsoleVariable("bJump");
    pConsoleDataBase->deleteConsoleVariable("bUse");
    pConsoleDataBase->deleteConsoleVariable("bCrouch");
    pConsoleDataBase->deleteConsoleVariable("bPrevWeapon");
    pConsoleDataBase->deleteConsoleVariable("bNextWeapon");

    pConsoleDataBase->deleteConsoleVariable("mouseX");
    pConsoleDataBase->deleteConsoleVariable("mouseY");
    pConsoleDataBase->deleteConsoleVariable("plr_fMoveSpeed");

    pConsoleDataBase->deleteConsoleFunction("getEntityInfo()");

}

// new <<
void RunFrame() 
{
    if (Network::isServer())   
        serverRun(Game->lastTime);
    else                       
        clientRun(Game->lastTime);    
}



void StopGame(void) 
{
    PrintF("Call StopGame\n");

    if (camera != NULL)
    {
        delete camera;
        
    }
    camera = NULL;
//    RenderEND(&world);

 //   if (pteCrosshair)
 //       Delete(pteCrosshair);
       // delete pteCrosshair;

 //   pteCrosshair = NULL;

    // release all data from container
    UnloadConsoleVariables();

 //   Network::closeServer();

    if (Network::isServer())
        serverShutdown();
    else
        clientShutDown();


    teLogoA.Clear();
    teLogoB.Clear();
}


void EndGame(void) 
{
    PrintF("Call EndGame\n");

    if (camera != NULL)
    {
        delete camera;
        
    }
    camera = NULL;
    RenderEND(&world);

    serverShutdown();

    Network::closeServer();

 //   RenderEnd(&obj);
}


void sendDumpTextTest() 
{
    Network::sendText("Secret text between server and clients");
}

void initGameInstance(void) 
{
    camera = new CCamera;
    camera->setCamera(glm::vec3(0.0f, 2.0f, 0.0f));

    //pteCrosshair = new CTextureObject;
   // pteCrosshair = New<CTextureObject>();

   // CFileName strFile = "Data\\Textures\\HUD\\Crosshairs\\Crosshair01.te";
   // FileSetGlobalPatch(strFile);
   // pteCrosshair->Load(strFile);
   // pteCrosshair->Prepare();


    CFileName strFile = "Data\\Textures\\Logo\\PowerLogoA.te";
    FileSetGlobalPatch(strFile);

    teLogoA.Load(strFile);

    strFile = "Data\\Textures\\Logo\\PowerLogoB.te";
    FileSetGlobalPatch(strFile);

    teLogoB.Load(strFile);

    teLogoB.renderingFlags |= TEXTURE_CONST;
    teLogoA.renderingFlags |= TEXTURE_CONST;

    teLogoB.Prepare();
    teLogoA.Prepare();

    pConsoleDataBase->addConsoleVariable(CVT_BOOL, "bTest", &bTest);
    pConsoleDataBase->addConsoleVariable(CVT_INTEGER, "mouseX", &mouseX);
    pConsoleDataBase->addConsoleVariable(CVT_INTEGER, "mouseY", &mouseY);
    pConsoleDataBase->addConsoleFunction("sendDumpTextTest()", sendDumpTextTest);

    strFile = "Data\\PConfig.cfg";
    FileSetGlobalPatch(strFile);


    FILE* file = fopen((const char*)strFile, "r");

    if (file == NULL) 
    { 
        PrintF("Error\n"); 
        return;
    }

    fscanf(file, "PlayerName = %s\n", plr_strPlayerName);
    fscanf(file, "Color = %hhu", &plr_colColor);
    //PrintF("plr_colColor = %d\n", plr_colColor);

    fclose(file);

    // make color from player color

    char* str = stringCopyF("^%d%s^8", plr_colColor, plr_strPlayerName);

    PrintF("Player name = %s\n", str);

    strcpy(plr_strPlayerName, str);

    free(str);
//    Game->LoadLevel("Data\\Levels\\Multiplayer\\DM1.wo");
}


bool LoadWorld(const char* strName) 
{ 

    CFileName FileName("Data\\Levels\\Multiplayer\\DM1.wo");

    CFileStream file;

    if (!file.open(FileName, 0, 1))
    {
        Throw("Cannot load level %s, no file\n", (const char*)FileName);
    }

    world.LoadWithoutEntities(file);

 //  try {
 //       GEntity::parseEntities(&world, file);
 //   }
 //   catch (char* error) {
 //       Error("Fatal error: %s\n", error);
 //   }

  //  unsigned int iCRC = file.calculateFileCRC();

 //   PrintF("World: %s, CRC_32: %u (0x%X)\n", world.getName(), iCRC, iCRC);

    file.closeFile();

  ///  world.SetUpPhysics();

  //  RenderSetupWorld(&world);

    //try 
    //{
    //    Network::startServer();
    //}
    //catch (char* e) 
    //{
    //    PrintF("Cannot create server: %s\n", e);
    //}

   // GEntity::spawnProjectile(&world, PRT_ENERGY_BALL, VECTOR3(0, 1, 0), VECTOR3_NONE);
   // GEntity::spawnProjectile(&world, PRT_ENERGY_BALL, VECTOR3(0, 700, 0), VECTOR3_NONE);

    //GEntity::spawnProjectile(&world, PRT_ENERGY_BALL, VECTOR3(0, 1, 0), VECTOR3_NONE);

    return false;

}

void LoadLevel(const char* name)
{
    CRoomBase room;
 //   addPolygon(&room, 10.5f, 15.0f, 10.5f);
    addRoom(&room, 64.0f, 40.0f, 40.0f, 4, 1, Vector3D(0.f, -32.f, 0.f));

    CFileName hroom = "Data\\Levels\\dummy.wo";

    CFileStream stream;
    stream.open(hroom, 0, 0);
    room.Save(stream);
    stream.closeFile();
    room.Clear(); 

    stream.open(hroom, 0, 1);
    room.Load(stream);
    stream.closeFile();


    //std::vector<std::string> skyNames =
    //{
    //    "Data\\Textures\\Background\\Day01\\DayBK.bmp",
    //    "Data\\Textures\\Background\\Day01\\DayFT.bmp",
    //    "Data\\Textures\\Background\\Day01\\DayUP.bmp",
    //    "Data\\Textures\\Background\\Day01\\DayDN.bmp",
    //    "Data\\Textures\\Background\\Day01\\DayRT.bmp",
    //    "Data\\Textures\\Background\\Day01\\DayLT.bmp",
    //};

    std::vector<std::string> skyNames =
    {
        "Data\\Textures\\Background\\Space\\nebulaft.bmp",
        "Data\\Textures\\Background\\Space\\nebulabk.bmp",
        "Data\\Textures\\Background\\Space\\nebulaup.bmp",
        "Data\\Textures\\Background\\Space\\nebuladn.bmp",
        "Data\\Textures\\Background\\Space\\nebulart.bmp",
        "Data\\Textures\\Background\\Space\\nebulalf.bmp",
    };


    //CBrush* MyBrush = createBrushCube(0.5f, 20.0f, 20.0f, Vector3D(0.f, 0.5f, 0.f), nullptr);

    //world.gw_brushes.push_back(MyBrush);
    //world.gw_numBrushes += 1;

    //CBrush* MyBrush2 = createBrushCube(0.5f, 42.0f, 10.0f, Vector3D(0.f, 1.5f, -16.f), nullptr);

    //world.gw_brushes.push_back(MyBrush2);
    //world.gw_numBrushes += 1;

    //CBrush* MyBrush3 = createBrushCube(0.5f, 42.0f, 10.0f, Vector3D(0.f, 1.5f, 16.f), nullptr);

    //world.gw_brushes.push_back(MyBrush3);
    //world.gw_numBrushes += 1;

    //CBrush* MyBrush4 = createBrushCube(0.5f, 10.0f, 22.0f, Vector3D(16.f, 1.5f, 0.f), nullptr);

    //world.gw_brushes.push_back(MyBrush4);
    //world.gw_numBrushes += 1;

    //CBrush* MyBrush5 = createBrushCube(0.5f, 10.0f, 22.0f, Vector3D(-16.f, 1.5f, 0.f), nullptr);

    //world.gw_brushes.push_back(MyBrush5);
    //world.gw_numBrushes += 1;

    //CBrush* MyBrush6 = createBrushCube(0.5f, 66.0f, 10.0f, Vector3D(0.f, 2.5f, -27.f), nullptr);

    //world.gw_brushes.push_back(MyBrush6);
    //world.gw_numBrushes += 1;

    //CBrush* MyBrush7 = createBrushCube(0.5f, 66.0f, 10.0f, Vector3D(0.f, 2.5f, 27.f), nullptr);

    //world.gw_brushes.push_back(MyBrush7);
    //world.gw_numBrushes += 1;

    //CBrush* MyBrush8 = createBrushCube(0.5f, 10.0f, 44.0f, Vector3D(-28.f, 2.5f, 0.f), nullptr);

    //world.gw_brushes.push_back(MyBrush8);
    //world.gw_numBrushes += 1;

    //CBrush* MyBrush9 = createBrushCube(0.5f, 10.0f, 44.0f, Vector3D(28.f, 2.5f, 0.f), nullptr);

    //world.gw_brushes.push_back(MyBrush9);
    //world.gw_numBrushes += 1;    

    CBrush* brushes[21];

    FOREACH_ARRAY(21) 
    {
        brushes[i] = new CBrush;
        char* strCopy = stringCopyF("Data\\Levels\\Multiplayer\\Brushes\\%d.br", i + 1);
        hroom = strCopy;

        stream.open(hroom, 0, 1);

        brushes[i]->Load(stream);
        world.gw_brushes.push_back(brushes[i]);
        world.gw_numBrushes += 1;
        free(strCopy);
        stream.closeFile();
    }
    /*
    CBrush* MyBrush10 = createBrushCube(5.0f, 1.0f, 30.0f, Vector3D(23.5f, 3.0f, 0.f), nullptr);

    world.gw_brushes.push_back(MyBrush10);
    world.gw_numBrushes += 1;

    hroom = "Data\\Levels\\Multiplayer\\Brushes\\10.br";
    stream.open(hroom, 0, 0);
    MyBrush10->Save(stream);
    stream.closeFile();

    CBrush* MyBrush11 = createBrushCube(5.0f, 1.0f, 30.0f, Vector3D(-23.5f, 3.0f, 0.f), nullptr);

    world.gw_brushes.push_back(MyBrush11);
    world.gw_numBrushes += 1;

    hroom = "Data\\Levels\\Multiplayer\\Brushes\\11.br";
    stream.open(hroom, 0, 0);
    MyBrush11->Save(stream);
    stream.closeFile();

    CBrush* MyBrush12 = createBrushCube(5.0f, 15.0f, 1.0f, Vector3D(0.f, 2.0f, -11.5f), nullptr);

    world.gw_brushes.push_back(MyBrush12);
    world.gw_numBrushes += 1;

    hroom = "Data\\Levels\\Multiplayer\\Brushes\\12.br";
    stream.open(hroom, 0, 0);
    MyBrush12->Save(stream);
    stream.closeFile();

    CBrush* MyBrush13 = createBrushCube(5.0f, 15.0f, 1.0f, Vector3D(0.f, 2.0f, 11.5f), nullptr);

    world.gw_brushes.push_back(MyBrush13);
    world.gw_numBrushes += 1;

    hroom = "Data\\Levels\\Multiplayer\\Brushes\\13.br";
    stream.open(hroom, 0, 0);
    MyBrush13->Save(stream);
    stream.closeFile();
    
    CBrush* MyBrush14 = createBrushCube(10.0f, 5.0f, 5.0f, Vector3D(25.5f, 3.0f, 24.5f), nullptr);

    world.gw_brushes.push_back(MyBrush14);
    world.gw_numBrushes += 1;

    hroom = "Data\\Levels\\Multiplayer\\Brushes\\14.br";
    stream.open(hroom, 0, 0);
    MyBrush14->Save(stream);
    stream.closeFile();


    CBrush* MyBrush15 = createBrushCube(10.0f, 5.0f, 5.0f, Vector3D(-25.5f, 3.0f, 24.5f), nullptr);

    world.gw_brushes.push_back(MyBrush15);
    world.gw_numBrushes += 1;

    hroom = "Data\\Levels\\Multiplayer\\Brushes\\15.br";
    stream.open(hroom, 0, 0);
    MyBrush15->Save(stream);
    stream.closeFile();

    CBrush* MyBrush16 = createBrushCube(10.0f, 5.0f, 5.0f, Vector3D(25.5f, 3.0f, -24.5f), nullptr);

    world.gw_brushes.push_back(MyBrush16);
    world.gw_numBrushes += 1;

    hroom = "Data\\Levels\\Multiplayer\\Brushes\\16.br";
    stream.open(hroom, 0, 0);
    MyBrush16->Save(stream);
    stream.closeFile();

    CBrush* MyBrush17 = createBrushCube(10.0f, 5.0f, 5.0f, Vector3D(-25.5f, 3.0f, -24.5f), nullptr);

    world.gw_brushes.push_back(MyBrush17);
    world.gw_numBrushes += 1;

    hroom = "Data\\Levels\\Multiplayer\\Brushes\\17.br";
    stream.open(hroom, 0, 0);
    MyBrush17->Save(stream);
    stream.closeFile();

    CBrush* MyBrush18 = createBrushCube(10.0f, 5.0f, 5.0f, Vector3D(13.5f, 2.0f, 13.5f), nullptr);

    world.gw_brushes.push_back(MyBrush18);
    world.gw_numBrushes += 1;

    hroom = "Data\\Levels\\Multiplayer\\Brushes\\18.br";
    stream.open(hroom, 0, 0);
    MyBrush18->Save(stream);
    stream.closeFile();

    CBrush* MyBrush19 = createBrushCube(10.0f, 5.0f, 5.0f, Vector3D(-13.5f, 2.0f, 13.5f), nullptr);

    world.gw_brushes.push_back(MyBrush19);
    world.gw_numBrushes += 1;

    hroom = "Data\\Levels\\Multiplayer\\Brushes\\19.br";
    stream.open(hroom, 0, 0);
    MyBrush19->Save(stream);
    stream.closeFile();

    CBrush* MyBrush20 = createBrushCube(10.0f, 5.0f, 5.0f, Vector3D(-13.5f, 2.0f, -13.5f), nullptr);

    world.gw_brushes.push_back(MyBrush20);
    world.gw_numBrushes += 1;

    hroom = "Data\\Levels\\Multiplayer\\Brushes\\20.br";
    stream.open(hroom, 0, 0);
    MyBrush20->Save(stream);
    stream.closeFile();

    CBrush* MyBrush21 = createBrushCube(10.0f, 5.0f, 5.0f, Vector3D(13.5f, 2.0f, -13.5f), nullptr);

    world.gw_brushes.push_back(MyBrush21);
    world.gw_numBrushes += 1;

    hroom = "Data\\Levels\\Multiplayer\\Brushes\\21.br";
    stream.open(hroom, 0, 0);
    MyBrush21->Save(stream);
    stream.closeFile();*/

    world.skybox = new CSkyBox;

    world.skybox->makeSkyBox(skyNames);
    world.skybox->strTexturesNameSkyBox = skyNames;
    world.gw_fGravity = 9.81f;

    world.gw_description = "Survie!!!)";
    world.gw_bckgColour = COLOR_BLUE;
    world.gw_nextRoomID = 1;
    world.gw_numRooms = 1;
    world.gw_strName = "Cosmic coliseum";

    world.gw_rooms.push_back(room);

    /*
    struct Sun sun 
    {
        0.5f,

        HEX_toOGL(COLOR_GRAY),
        HEX_toOGL(COLOR_WHITE),
        HEX_toOGL(COLOR_WHITE),

        VECTOR3(-0.5f, -0,5, 0),
        0
    };*/

    world.gw_Sun = new Sun_t;

    world.gw_Sun->su_fSize = 0.5f;
    world.gw_Sun->su_aDirection = VECTOR3(0.64f, -1, 1.f);
    world.gw_Sun->su_ColorAmbient  = HEX_toOGL(0xb4c4d4FF); // 0xD0D6C7FF
    world.gw_Sun->su_ColorDiffuse  = HEX_toOGL(0xC285FFFF); // 0xFFFFEDFF
    world.gw_Sun->su_ColorSpecular = HEX_toOGL(COLOR_WHITE);

    CFileName saveworld = name;
    //world.Save(saveworld);


    /*******************  ENTITIES START *************************/

    world.SetUpPhysics();

   // CTextAlert* entity = new CTextAlert;

   // entity->setText("Hello World");

   // world.gw_allEntities.push_back(entity);
   // world.gw_numEntities = world.gw_allEntities.size();

   // 
   // CHealth* h = new CHealth;
   //// h->he_Value = 10;
   // h->he_htHealthType = HP_10;
   // h->m_vPosition = VECTOR3(-3, 1, -4);
   // h->m_pwo = &world;
   // h->Initialize();
   // world.gw_allEntities.push_back(h);
   // world.gw_numEntities = world.gw_allEntities.size();
   // 
   // CHealth* h1 = new CHealth;
   // ///h1->he_Value = 10;
   // h1->he_htHealthType = HP_25;
   // h1->m_vPosition = VECTOR3(-1, 1, -4);
   // h1->m_pwo = &world;
   // h1->Initialize();
   // world.gw_allEntities.push_back(h1);
   // world.gw_numEntities = world.gw_allEntities.size();
   // 
   // CHealth* h2 = new CHealth;
   // h2->he_Value = 50;
   // h2->he_htHealthType = HP_50;
   // h2->m_vPosition = VECTOR3(1, 1, -4);
   // h2->m_pwo = &world;
   // h2->Initialize();
   // world.gw_allEntities.push_back(h2);
   // world.gw_numEntities = world.gw_allEntities.size();
   // 
   // CHealth* h3 = new CHealth;
   // h3->he_Value = 100;
   // h3->he_htHealthType = HP_100;
   // h3->m_vPosition = VECTOR3(0, 1, 0);
   // h3->m_pwo = &world;
   // h3->Initialize();
   // world.gw_allEntities.push_back(h3);
   // world.gw_numEntities = world.gw_allEntities.size();
    

    /*
    *  INIT SPAWNERS
    */
    CPlayerSpawn* sp01 = new CPlayerSpawn;
    sp01->m_anglRotaion = Vector3D(0.0f, 270.0f, 0.0f);
    sp01->m_vPosition = Vector3D(30.5f, 4.0f, 25.5f);
    sp01->psp_uiStartedWeaponPack = WEAPON_BIT(WEAPON_MELE);// | WEAPON_BIT(WEAPON_AR) | WEAPON_BIT(WEAPON_ENERGYGUN) | WEAPON_BIT(WEAPON_ROCKETGUN);
    sp01->Initialize();
    world.gw_allEntities.push_back(sp01);
    world.gw_numEntities = world.gw_allEntities.size();

    CPlayerSpawn* sp02 = new CPlayerSpawn;
    sp02->m_anglRotaion = Vector3D(0.0f, 180.0f, 0.0f);
    sp02->m_vPosition = Vector3D(25.5f, 4.0f, -30.0f);
    sp02->psp_uiStartedWeaponPack = WEAPON_BIT(WEAPON_MELE);// | WEAPON_BIT(WEAPON_AR) | WEAPON_BIT(WEAPON_ENERGYGUN) | WEAPON_BIT(WEAPON_ROCKETGUN);
    sp02->Initialize();
    world.gw_allEntities.push_back(sp02);
    world.gw_numEntities = world.gw_allEntities.size();

    CPlayerSpawn* sp03 = new CPlayerSpawn;
    sp03->m_anglRotaion = Vector3D(0.0f, 90.0f, 0.0f);
    sp03->m_vPosition = Vector3D(-30.0f, 4.0f, -25.5f);
    sp03->psp_uiStartedWeaponPack = WEAPON_BIT(WEAPON_MELE);// | WEAPON_BIT(WEAPON_AR) | WEAPON_BIT(WEAPON_ENERGYGUN) | WEAPON_BIT(WEAPON_ROCKETGUN);
    sp03->Initialize();
    world.gw_allEntities.push_back(sp03);
    world.gw_numEntities = world.gw_allEntities.size();

    CPlayerSpawn* sp04 = new CPlayerSpawn;
    sp04->m_anglRotaion = Vector3D(0.0f, 180.0f, 0.0f);
    sp04->m_vPosition = Vector3D(25.5f, 4.0f, -30.0f);
    sp04->psp_uiStartedWeaponPack = WEAPON_BIT(WEAPON_MELE);// | WEAPON_BIT(WEAPON_AR) | WEAPON_BIT(WEAPON_ENERGYGUN) | WEAPON_BIT(WEAPON_ROCKETGUN);
    sp04->Initialize();
    world.gw_allEntities.push_back(sp04);
    world.gw_numEntities = world.gw_allEntities.size();

    // next 4 spawners

    CPlayerSpawn* sp05 = new CPlayerSpawn;
    sp05->m_anglRotaion = Vector3D(0.0f, 180.0f, 0.0f);
    sp05->m_vPosition = Vector3D(-14.0f, 3.0f, 17.0f);
    sp05->psp_uiStartedWeaponPack = WEAPON_BIT(WEAPON_MELE);// | WEAPON_BIT(WEAPON_AR) | WEAPON_BIT(WEAPON_ENERGYGUN) | WEAPON_BIT(WEAPON_ROCKETGUN);
    sp05->Initialize();
    world.gw_allEntities.push_back(sp05);
    world.gw_numEntities = world.gw_allEntities.size();

    CPlayerSpawn* sp06 = new CPlayerSpawn;
    sp06->m_anglRotaion = Vector3D(0.0f, 0.0f, 0.0f);
    sp06->m_vPosition = Vector3D(14.0f, 3.0f, -17.0f);
    sp06->psp_uiStartedWeaponPack = WEAPON_BIT(WEAPON_MELE);// | WEAPON_BIT(WEAPON_ENERGYGUN) | WEAPON_BIT(WEAPON_ROCKETGUN);
    sp06->Initialize();
    world.gw_allEntities.push_back(sp06);
    world.gw_numEntities = world.gw_allEntities.size();

    CPlayerSpawn* sp07 = new CPlayerSpawn;
    sp07->m_anglRotaion = Vector3D(0.0f, 270.0f, 0.0f);
    sp07->m_vPosition = Vector3D(-17.0f, 3.0f, -14.0f);
    sp07->psp_uiStartedWeaponPack = WEAPON_BIT(WEAPON_MELE);// | WEAPON_BIT(WEAPON_ENERGYGUN) | WEAPON_BIT(WEAPON_ROCKETGUN);
    sp07->Initialize();
    world.gw_allEntities.push_back(sp07);
    world.gw_numEntities = world.gw_allEntities.size();

    CPlayerSpawn* sp08 = new CPlayerSpawn;
    sp08->m_anglRotaion = Vector3D(0.0f, 90.0f, 0.0f);
    sp08->m_vPosition = Vector3D(17.0f, 3.0f, 14.0f);
    sp08->psp_uiStartedWeaponPack = WEAPON_BIT(WEAPON_MELE);// | WEAPON_BIT(WEAPON_ENERGYGUN) | WEAPON_BIT(WEAPON_ROCKETGUN);
    sp08->Initialize();
    world.gw_allEntities.push_back(sp08);
    world.gw_numEntities = world.gw_allEntities.size();

    // health

    CHealth* h100 = new CHealth;
    h100->he_htHealthType = HP_100;
    h100->m_vPosition = VECTOR3(0, 1.5, 0);
    h100->m_pwo = &world;
    h100->he_bRespawn = true;
    h100->Initialize();
    world.gw_allEntities.push_back(h100);
    world.gw_numEntities = world.gw_allEntities.size();

    CHealth* h10_01 = new CHealth;
    h10_01->he_htHealthType = HP_10;
    h10_01->m_vPosition = VECTOR3(8, 1.5, 8);
    h10_01->m_pwo = &world;
    h10_01->he_bRespawn = true;
    h10_01->Initialize();
    world.gw_allEntities.push_back(h10_01);
    world.gw_numEntities = world.gw_allEntities.size();

    CHealth* h10_02 = new CHealth;
    h10_02->he_htHealthType = HP_10;
    h10_02->m_vPosition = VECTOR3(-8, 1.5, 8);
    h10_02->m_pwo = &world;
    h10_02->he_bRespawn = true;
    h10_02->Initialize();
    world.gw_allEntities.push_back(h10_02);
    world.gw_numEntities = world.gw_allEntities.size();

    CHealth* h10_03 = new CHealth;
    h10_03->he_htHealthType = HP_10;
    h10_03->m_vPosition = VECTOR3(8, 1.5, -8);
    h10_03->m_pwo = &world;
    h10_03->he_bRespawn = true;
    h10_03->Initialize();
    world.gw_allEntities.push_back(h10_03);
    world.gw_numEntities = world.gw_allEntities.size();

    CHealth* h10_04 = new CHealth;
    h10_04->he_htHealthType = HP_10;
    h10_04->m_vPosition = VECTOR3(-8, 1.5, -8);
    h10_04->m_pwo = &world;
    h10_04->he_bRespawn = true;
    h10_04->Initialize();
    world.gw_allEntities.push_back(h10_04);
    world.gw_numEntities = world.gw_allEntities.size();

    CHealth* h50_01 = new CHealth;
    h50_01->he_htHealthType = HP_50;
    h50_01->m_vPosition = VECTOR3(-31.5f, 3.5f, -30.5f);
    h50_01->m_pwo = &world;
    h50_01->he_bRespawn = true;
    h50_01->Initialize();
    world.gw_allEntities.push_back(h50_01);
    world.gw_numEntities = world.gw_allEntities.size();

    CHealth* h50_02 = new CHealth;
    h50_02->he_htHealthType = HP_50;
    h50_02->m_vPosition = VECTOR3(31.5f, 3.5f, 30.5f);
    h50_02->m_pwo = &world;
    h50_02->he_bRespawn = true;
    h50_02->Initialize();
    world.gw_allEntities.push_back(h50_02);
    world.gw_numEntities = world.gw_allEntities.size();

    // armor

    CArmor* ar50 = new CArmor;
    ar50->ar_atArmorType = AR_50;
    ar50->m_vPosition = VECTOR3(-31.5f, 3.5f, 30.5f);
    ar50->m_pwo = &world;
    ar50->ar_bRespawn = true;
    ar50->Initialize();
    world.gw_allEntities.push_back(ar50);
    world.gw_numEntities = world.gw_allEntities.size();

    CArmor* ar501 = new CArmor;
    ar501->ar_atArmorType = AR_50;
    ar501->m_vPosition = VECTOR3(31.5f, 3.5f, -30.5f);
    ar501->m_pwo = &world;
    ar501->ar_bRespawn = true;
    ar501->Initialize();
    world.gw_allEntities.push_back(ar501);
    world.gw_numEntities = world.gw_allEntities.size();

    // ammo

    // weapon

    CWeaponItem* wp = new CWeaponItem;

    wp->wi_wtWeaponType = WI_ENERGYGUN;
    wp->m_vPosition = VECTOR3(0, 20, 0);
    wp->m_pwo = &world;
    wp->Initialize();
    world.gw_allEntities.push_back(wp);
    world.gw_numEntities = world.gw_allEntities.size();

    CWeaponItem* wp_sh = new CWeaponItem;

    wp_sh->wi_wtWeaponType = WI_SHOTGUN;
    wp_sh->m_vPosition = VECTOR3(-16, 2.5, 0);
    wp_sh->m_pwo = &world;
    wp_sh->Initialize();
    world.gw_allEntities.push_back(wp_sh);
    world.gw_numEntities = world.gw_allEntities.size();

    CWeaponItem* wp_sh2 = new CWeaponItem;

    wp_sh2->wi_wtWeaponType = WI_SHOTGUN;
    wp_sh2->m_vPosition = VECTOR3(16, 2.5, 0);
    wp_sh2->m_pwo = &world;
    wp_sh2->Initialize();
    world.gw_allEntities.push_back(wp_sh2);
    world.gw_numEntities = world.gw_allEntities.size();


    CWeaponItem* wp_ar = new CWeaponItem;

    wp_ar->wi_wtWeaponType = WI_AR;
    wp_ar->m_vPosition = VECTOR3(27, 3.5, 0);
    wp_ar->m_pwo = &world;
    wp_ar->Initialize();
    world.gw_allEntities.push_back(wp_ar);
    world.gw_numEntities = world.gw_allEntities.size();

    CWeaponItem* wp_ar2 = new CWeaponItem;

    wp_ar2->wi_wtWeaponType = WI_AR;
    wp_ar2->m_vPosition = VECTOR3(-27, 3.5, 0);
    wp_ar2->m_pwo = &world;
    wp_ar2->Initialize();
    world.gw_allEntities.push_back(wp_ar2);
    world.gw_numEntities = world.gw_allEntities.size();

    CWeaponItem* wp_rk = new CWeaponItem;

    wp_rk->wi_wtWeaponType = WI_ROCKETGUN;
    wp_rk->m_vPosition = VECTOR3(0, 3.5, 28);
    wp_rk->m_pwo = &world;
    wp_rk->Initialize();
    world.gw_allEntities.push_back(wp_rk);
    world.gw_numEntities = world.gw_allEntities.size();

    CWeaponItem* wp_rk2 = new CWeaponItem;

    wp_rk2->wi_wtWeaponType = WI_ROCKETGUN;
    wp_rk2->m_vPosition = VECTOR3(0, 3.5, -28);
    wp_rk2->m_pwo = &world;
    wp_rk2->Initialize();
    world.gw_allEntities.push_back(wp_rk2);
    world.gw_numEntities = world.gw_allEntities.size();


    CZoneHurt* zh = new CZoneHurt;

    zh->m_vPosition = Vector3D(0, -20, 0);
    zh->vCollisionStretch = Vector3D(40, 2, 40);
    zh->m_pwo = &world;

    zh->Initialize();
    world.gw_allEntities.push_back(zh);
    world.gw_numEntities = world.gw_allEntities.size();

    stream.open(saveworld, 0, 0);
    world.SaveWithoutEntities(stream);

    stream.writeID("FILS");

    uint iCount = 0;

    FOREACH_ARRAY(world.gw_allEntities.size())
    {
        if (world.gw_allEntities[i]->m_ulFlags & ENTITY_MODEL)
            iCount++;
    }

    stream.WriteToFile(&iCount, sizeof(uint));

    for (unsigned int i = 0; i < world.gw_allEntities.size(); i++) 
    {
        if (world.gw_allEntities[i]->m_ulFlags & ENTITY_MODEL) 
        {
            stream.WriteString(world.gw_allEntities[i]->m_strModel.strFileName);
            FileDeleteAbsolutePatch(world.gw_allEntities[i]->m_strTexture);
            stream.WriteString(world.gw_allEntities[i]->m_strTexture.strFileName);
        }
    }

    stream.writeID("ENTS");

 /*   world.gw_allEntities[0]->Save(stream);
    CTextAlert* a = (CTextAlert*)world.gw_allEntities[0];
    stream.WriteToFile(&a->bOnce, sizeof(bool));
    stream.WriteString(a->strAlert);
    ((CHealth&)*world.gw_allEntities[1]).Save(stream);
    ((CHealth&)*world.gw_allEntities[2]).Save(stream);
    ((CHealth&)*world.gw_allEntities[3]).Save(stream);
    ((CHealth&)*world.gw_allEntities[4]).Save(stream);
    ((CWeaponItem&)*world.gw_allEntities[5]).Save(stream);*/

    sp01->Save(stream);
    sp02->Save(stream);
    sp03->Save(stream);
    sp04->Save(stream);

    sp05->Save(stream);
    sp06->Save(stream);
    sp07->Save(stream);
    sp08->Save(stream);

    h100->Save(stream);
    h10_01->Save(stream);
    h10_02->Save(stream);
    h10_03->Save(stream);
    h10_04->Save(stream);

    h50_01->Save(stream);
    h50_02->Save(stream);

    wp->Save(stream);
    wp_sh->Save(stream);
    wp_sh2->Save(stream);

    wp_ar->Save(stream);
    wp_ar2->Save(stream);
    wp_rk->Save(stream);
    wp_rk2->Save(stream);
    ar50->Save(stream);
    ar501->Save(stream);

    zh->Save(stream);

    stream.writeID("EEND");

    stream.closeFile();


    /*******************  ENTITIES END *************************/
    world.Clear();    
    skyNames.clear();


    PrintF("Call LoadLevel: %s\n", name);
    if (Game != NULL)
        printf("Game->ProccessId = %d\n", Game->ProccessId);
}

void SaveGame(string_t strFileName) 
{
    PrintF("Call SaveGame: %s\n", strFileName);
}


bool LoadGame(const char* strFileName) 
{

    camera = new CCamera;
    camera->setCamera(glm::vec3(0.0f, 2.0f, 0.0f));  
    
    pteCrosshair = new CTextureObject;

    CFileName strFile = "Data\\Textures\\HUD\\Crosshairs\\Crosshair01.te";

    FileSetGlobalPatch(strFile);

    pteCrosshair->Load(strFile);

    pteCrosshair->Prepare();
    
    CFileName FileName = "Data\\Levels\\Multiplayer\\DM1.wo";

    world.Load(FileName);

    RenderSetupWorld(&world);
//    RenderSetupScene(&obj);

    PrintF("---------------------------------------------------------------\n");
    PrintF("Loading level: %s\n", world.gw_strName.c_str());
    PrintF("---------------------------------------------------------------\n");

    PrintF("Call LoadGame:\n");

    return true;
}

void ScreenShot()
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    std::string strDataTime = "C:\\Games\\In-the-Power\\Data\\Screenshots\\shot_";

    char strFormated[17];

    sprintf(strFormated, "%d%d%d_%d%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);

    strDataTime += strFormated;

    CFileName fileName = strDataTime + ".png";

    int success = MakeScreenShoot(fileName);

    if (!success)
        PrintF("Error: %d, Cannot save screenshot: %s\n", success, fileName.getCharPointer());
    else
        PrintF("Take screenshot: %s\n", fileName.getCharPointer());

}

/* create game */
Game_t* GameCreate(void)
{
    Game = (Game_t*)malloc(sizeof(struct Game_s));

    if (Game == NULL)
        return NULL; 

    Game->ProccessId = 1;
    Game->KeyDown = FALSE;
    Game->KeyUp = FALSE;
    Game->mouseX = 0;
    Game->mouseY = 0;
    Game->key = 1;
    Game->enableMenu = TRUE;
    Game->lastTime = 0.0;

    Game->gaType  = GAME_RUN;

    Game->EndGame   = EndGame;
    Game->LoadGame  = LoadWorld;//LoadGame;
    Game->SaveGame  = SaveGame;
    Game->StopGame  = StopGame;
    Game->RunGame   = RunFrame; // RunGame;
    Game->LoadLevel = LoadLevel;
    Game->ReadKey   = ReadKey;
    Game->Input     = Input;
    Game->ScreenShot  = ScreenShot;
    Game->Initialize = initGameInstance;
    Game->ConMenu     = ConMenu;
    Game->NewGame     = NewGame;
    Game->JoinGame    = JoinGame;

    return Game;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}