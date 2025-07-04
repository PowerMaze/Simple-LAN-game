#include <stdAfx.h>
#include "EMain.h"


/*** global objects ***/

//ConsoleLog_t* ConsoleMain           = NULL;
CConsole  *pConsoleMain              = NULL;
CConsoleDataBase *pConsoleDataBase   = NULL;
CView     *pViewPort                 = NULL;
CFont     *systemEngineFont          = NULL;
CFont     *pFontDisplay              = NULL;
CTextureObject  *teDefaultTextureForModels = NULL;
CDataBase *pDataBase                 = NULL;

simpleBox2D_t  *drawingShapeBox         = NULL;
simpleBox2D_t  *drawingTexturedShapeBox = NULL;


/*** global variables ***/
std::string strGlobalPatch;
//bool net_bCryptText;

/** shaders **/
CShader shaText;
CShader shaModel;
CShader shaTexture;
CShader shaFillColor;
CShader shaSkyBox;
CShader shaPolygon;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
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

static void compileBasicShaders(void) 
{
    shaText.Create_t("System\\Shaders\\TextShaders\\vertShader.shr",
                     "System\\Shaders\\TextShaders\\fragShader.shr");

    shaModel.Create_t("System\\Shaders\\Model_Test\\vertShader.shr",
                      "System\\Shaders\\Model_Test\\fragShader.shr");

    shaTexture.Create_t("System\\Shaders\\SimpleShaderShape\\vertShaderTexture.shr",
                        "System\\Shaders\\SimpleShaderShape\\fragShaderTexture.shr");

    shaFillColor.Create_t("System\\Shaders\\SimpleShaderShape\\vertShader.shr",
                          "System\\Shaders\\SimpleShaderShape\\fragShader.shr");

    shaPolygon.Create_t("System\\Shaders\\RoomShaders_\\vertShader.shr",
                        "System\\Shaders\\RoomShaders_\\fragShader.shr");

    shaSkyBox.Create_t("System\\Shaders\\SkyBoxShaders\\vertShader.shr",
                       "System\\Shaders\\SkyBoxShaders\\fragShader.shr");

}

static void releaseBasicShaders(void)
{
    shaText.end();
    shaModel.end();
    shaTexture.end();
    shaFillColor.end();
    shaPolygon.end();
    shaSkyBox.end();
}

static void InitConsoleSymbols();

static void InitGlobals(void) 
{
    
    // init viewport of game
    pViewPort = new CView;

    /* data base init */
    pDataBase = new CDataBase;  

    // setup system fonts for engine
    CFileName file = "Data\\Font\\Console1.fo";
    systemEngineFont = new CFont;
    systemEngineFont->Load(file);

    file = "Data\\Font\\Display1.fo";
    pFontDisplay = new CFont;
    pFontDisplay->Load(file);

    pFontDisplay->SetSpace(15);

    // setup default texture
    teDefaultTextureForModels = initDefaultTexture(COLOR_WHITE);
    teDefaultTextureForModels->Prepare();

    // init shape box for drawing elements
    drawingShapeBox = (struct simpleBox2D_s*)malloc(sizeof(struct simpleBox2D_s));

    // make sure memory is allocated as well
    if (drawingShapeBox == NULL)
        Error("Failed to init 2d box\n");
    // intit shape 
    simpleBoxInit(drawingShapeBox);

    drawingTexturedShapeBox = simpleBoxInitTextured();

    if (drawingTexturedShapeBox == NULL)
        Error("Failed to init textured 2d box\n");

    InitConsoleSymbols();

    strAddressString = stringCopy("127.0.0.1");
}

void getGPUDriverInfo(void) 
{
    std::string glVendor   = (const char*)glGetString(GL_VENDOR);
    std::string glRenderer = (const char*)glGetString(GL_RENDERER);
    std::string glVersion  = (const char*)glGetString(GL_VERSION);

   // PrintF("---------------------------------------------------------------\n");
    PrintF("OpenGL 4.0 created succesfull\n");
    PrintF(" %s %s %s\n", glVendor.c_str(), glRenderer.c_str(), glVersion.c_str());
    PrintF("---------------------------------------------------------------\n");
}

void getGlobalAppPath()
{
    char testEXE[256];
    GetModuleFileNameA(NULL, testEXE, sizeof(testEXE) - 1);
   // PrintF("Application patch: %s\n", testEXE);

    strGlobalPatch = testEXE;

    
    size_t posOfDot = strGlobalPatch.find_last_of("\\");

    std::string newString = "";

    for (int i = 0; i < posOfDot; i++)
    {
        newString += strGlobalPatch[i];
    }

    strGlobalPatch = newString + std::string("\\");

   // PrintF("Working In the Engine directory patch: %s\n", strGlobalPatch.c_str());

    PrintF("---------------------------------------------------------------\n");
}


void InTheEngine_Start(int argc, char** argv, const char* strGame) 
{
    initializeProtection();

    getGlobalAppPath();

    pConsoleMain = new CConsole;
    pConsoleMain->Init(strGame); 

    pConsoleDataBase = new CConsoleDataBase;

    PrintF("In the Engine version %d.%d\n", 1, 0);
    PrintF("Initialization OpenGL 4.0...\n");
    PrintF("---------------------------------------------------------------\n");

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        Error("Failed to initialize GLAD\n");
    }  

    getGPUDriverInfo();

    PrintF("Working In the Engine directory patch: %s\n", strGlobalPatch.c_str());

    PrintF("---------------------------------------------------------------\n");

    InitGlobals();

    PrintF("Compile shaders...\n");
    compileBasicShaders();
    PrintF("All shaders compiled succesful\n");
    PrintF("---------------------------------------------------------------\n");
}

void InTheEngine_RunFrame(unsigned long miliseconds)
{
   // global = miliseconds;
  //  PrintF("Var = %d\n", global);
    PrintF("In the Engine - Dummy\n");
}

void InTheEngine_Shutdown(void)
{

//    Network::closeClient();
//    Network::closeServer();

    PrintF("---------------------------------------------------------------\n");
    PrintF("Shutting down...\n");
    PrintF("Resource released\n");
    PrintF("---------------------------------------------------------------\n");
 //   PrintF("Value of variable 'global' is %d\n", global);
  //  PrintF("Thanks you)))\n");

    pConsoleDataBase->Save("System\\ConsoleSymbols.cfg");

    delete pConsoleDataBase;
    pConsoleDataBase = NULL;

    if (pViewPort != NULL) 
    {
        delete pViewPort;
        pViewPort = NULL;
    }

    if (pConsoleMain != NULL) 
    {
        pConsoleMain->End();
        delete pConsoleMain;
        pConsoleMain = NULL;
    }

    simpleBoxEnd(drawingShapeBox);

    if (drawingShapeBox != NULL) 
    {
        free(drawingShapeBox);
        drawingShapeBox = NULL;
    }

    simpleBoxEnd(drawingTexturedShapeBox);

    if (drawingTexturedShapeBox != nullptr) 
    {
        free(drawingTexturedShapeBox);
        drawingTexturedShapeBox = nullptr;
    }

    if (systemEngineFont)
    {
        delete systemEngineFont;
    }

    if (pFontDisplay) delete pFontDisplay;

    systemEngineFont = NULL;
    pFontDisplay = NULL;

    if (teDefaultTextureForModels)
        delete teDefaultTextureForModels;

    delete pDataBase; pDataBase = NULL;

    teDefaultTextureForModels = NULL;

    if (strAddressString != NULL)
        free(strAddressString);

    releaseBasicShaders();
}


void InitConsoleSymbols()
{
    //net_bCryptText = true;    
    pConsoleDataBase->addConsoleVariable(CVT_BOOL, "net_bCryptText", &net_bCryptText);
    pConsoleDataBase->addConsoleVariable(CVT_BOOL, "net_bDumpPacketToFile", &net_bDumpPacketToFile);
}