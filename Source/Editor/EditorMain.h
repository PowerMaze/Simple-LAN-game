#pragma once


extern std::string strHelpTable[] =
{
	"In the Editor alpha version 0.1 by Dark-Head",
	" ",
	"N - creates file                       Worlds Documents Help",
	"O - opens file                         R - Add room",
	"S - saves file                         B - Add brush",
	"E - edit/fly mode                      W - Add entity",
	"H - helps                              P - Selects polygon",
	"-------------------",                   
	"Models Documents help",
	"T - open texture file",
	"A - create animations",
	"M - create LOD for model",
	"L - change light",
	"F - toggle floor on/off",
	"C - change color of model",
	"X - render models without shadow"
};







/*
#define MODE_EDIT byte(0)
#define MODE_FLY  byte(1)

extern byte editMode = MODE_EDIT;

// standard height of window
const int SCR_W = 1024;
// standard height of window
const int SCR_H = 768;
// tool bar caption
char* mainToolBar = NULL;

// current document
std::string strCurrentDoc = "";


static CFileName strFileToModel;
static CFileName strFileToModelTexture;

static HEXColor colBackgroud = COLOR_BLACK;
static HEXColor colFloor = COLOR_CYAN;


CEditorDocument* pEditorDocCurrent;


// pointer to model which will be edited
static CModelObject* moEditedModel = NULL;
// pointer to texture for model which will be edited
static CTextureObject* teEditedModelTexture = NULL;
// pointer to floor model
static CModelObject* moFloor = NULL;
// pointer to texture floor
static CTextureObject* teTextureFloor = NULL;
// pointer to world level which be edited
static CGameWorld* woEditedWorld = NULL;

CCamera* pEditorCamera = NULL;

typedef struct WindowCanvas_s
{
	int Wigth;
	int Height;
	SDL_Window* window;
	SDL_GLContext glcontext;

}WindowCanvas_t;
*/
//WindowCanvas_t window;