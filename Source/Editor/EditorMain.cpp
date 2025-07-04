#include "stdAfx.h"

//#include "ModelFloor.h"
#include "EditorMain.h"
#include "EditorCamera.h"
#include "EditorIO.h"
#include "EditorDoc.h"

#ifndef NDEGUG
	#define EDITOR_NAME "DEditor.log"
#else
	#define EDITOR_NAME "Editor.log"
#endif 

#define MODE_EDIT byte(0)
#define MODE_FLY  byte(1)

extern byte editMode = MODE_EDIT;

static bool bLockHelp = false;
static bool bRenderFloor = true;

//std::string strHelpTable[14];

const int SCR_W = 1024;
const int SCR_H = 768;
char* mainToolBar = NULL;

std::string strCurrentDoc = "NONE";

static CFileName strFileToModel;
static CFileName strFileToModelTexture;

static HEXColor colBackgroud = COLOR_BLACK;
static HEXColor colFloor = COLOR_CYAN;

//CView* portToolBar = NULL;
//static CModelFloor *mfFloor = NULL;

int currentDocType = DT_NONE;

CModelDocument *pModelDoc = NULL;

// pointer to model which will be edited
static CModelObject *moEditedModel    = NULL;
// pointer to texture for model which will be edited
static CTextureObject* teEditedModelTexture = NULL;
// pointer to floor model
static CModelObject* moFloor          = NULL;
// pointer to texture floor
static CTextureObject  *teTextureFloor      = NULL;
// pointer to world level which be edited
static CGameWorld   *woEditedWorld    = NULL;


std::vector <CEditorDocument* > _aAllDocuments;

CCamera *pEditorCamera = NULL;

typedef struct WindowCanvas_s
{
	int Wigth;
	int Height;
	SDL_Window* window;
	SDL_GLContext glcontext;
}WindowCanvas_t;

WindowCanvas_t window;

static int resizingEventWatcher(void* data, SDL_Event* event)
{
	if (event->type == SDL_WINDOWEVENT &&
		event->window.event == SDL_WINDOWEVENT_RESIZED)
	{
		SDL_Window* win = SDL_GetWindowFromID(event->window.windowID);
		if (win == (SDL_Window*)data)
		{
			int i = 0;
			int j = 0;
			SDL_GetWindowSize(win, &i, &j);

			//PrintF("%d %d\n", i, j);

			window.Wigth  = i;
			window.Height = j;

			pViewPort->setUpView(i, j, 0.1f, 1000.0f, 90.0f);
			setCurrentViewPort(pViewPort);
			//PrintF("RESIZE\n");
			
		}
	}
	return 0;
}


static void WindowInit(WindowCanvas_t* wnd)
{
	wnd->Height = SCR_H;
	wnd->Wigth = SCR_W;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		Error("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());

	wnd->window = SDL_CreateWindow("In the Editor - alpha v.0.1.", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, wnd->Wigth, wnd->Height, SDL_WINDOW_OPENGL);

	if (wnd->window == NULL)
		Error("Window could not be created! SDL_Error: %s\n", SDL_GetError());

	wnd->glcontext = SDL_GL_CreateContext(wnd->window);

	if (wnd->glcontext == NULL)
		Error("Cannot create OGL context!\nError: %s\n", SDL_GetError());

	SDL_SetWindowResizable(wnd->window, SDL_TRUE);

	SDL_AddEventWatch(resizingEventWatcher, wnd->window);

}

static void EditorEnd(void)
{
	delete pEditorCamera;  pEditorCamera  = NULL;
	delete moFloor;        moFloor        = NULL;
	delete teTextureFloor; teTextureFloor = NULL;
	/*delete teEditedModelTexture;*/ teEditedModelTexture = NULL;
	/*delete moEditedModel; */       moEditedModel        = NULL;


	pModelDoc = NULL;

	FOREACH_ARRAY(_aAllDocuments.size())
	{
		delete _aAllDocuments[i];
		_aAllDocuments[i] = NULL;
	}

	_aAllDocuments.clear();
}

static void EditorInit(void) 
{
	pEditorCamera = new CCamera;
	pEditorCamera->setCamera(glm::vec3(0.0f, 1.0f, 3.0f));

	moFloor = new CModelObject;
	//moEditedModel = new CModelObject;
	teTextureFloor = new CTextureObject;
	//teEditedModelTexture = new CTextureObject;

	CFileName filename = strGlobalPatch + std::string("Data\\Models\\Editor\\Floor.te");	

	moFloor->Load("Data\\Models\\Editor\\Floor.mo");
	//moEditedModel->Load("Data\\Models\\Misc\\TestAnimations\\Cat.mo");

	teTextureFloor->Load(filename);
	teTextureFloor->Prepare();

	//filename = strGlobalPatch + std::string("Data\\Models\\Misc\\TestAnimations\\Cat.te");

	//teEditedModelTexture->Load(filename);
	//teEditedModelTexture->Prepare();

	//moEditedModel->textureId = teEditedModelTexture->texID;


	//moFloor->PrepareToRender();
	
	RenderSetupOneModel(moFloor);
	//moFloor->

	moFloor->textureId = teTextureFloor->texID;
	moFloor->PlayAnimation("Anim_default");
	
	//moEditedModel->PlayAnimation("Anim_default");
}

void OpenTextureFileForModel(void)
{

	if (currentDocType == DT_WORLD || pModelDoc == NULL)
	return;

	OPENFILENAME ofn = { 0 };
	TCHAR szFile[260] = { 0 };
	// Initialize remaining fields of OPENFILENAME structure
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Textures \".TE\"\0*.TE\0All\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		_bstr_t wcharfile(ofn.lpstrFile);

		strFileToModelTexture = wcharfile;

		if (strFileToModelTexture.strFileName == "")
			return;

		//size_t find = strFileToModel.strFileName.find(strGlobalPatch);

		//if (find != std::string::npos) {
		//	strFileToModel.strFileName.erase(find, strGlobalPatch.length());
		//}

	}

	//teEditedModelTexture->Load(strFileToModelTexture);

	pModelDoc->setTexture(strFileToModelTexture);
	teEditedModelTexture = NULL;
	teEditedModelTexture = pModelDoc->getTexture();
	moEditedModel->textureId = teEditedModelTexture->texID;

	PrintF("%f %f %f\n", pEditorCamera->m_position.x, pEditorCamera->m_position.y, pEditorCamera->m_position.z);

}

void DocumentNew(void)
{
	OPENFILENAME ofn = { 0 };
	TCHAR szFile[260] = { 0 };
	// Initialize remaining fields of OPENFILENAME structure
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Wavefront OBJ \".OBJ\"\0*.obj\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		_bstr_t wcharfile(ofn.lpstrFile);

		strFileToModel = wcharfile;
	}

	if (strFileToModel.CompareEXT(".obj"))
	{

		CFileOBJ obj;

		CModelDocument* moDoc = new CModelDocument;

		try {
			moDoc->m_moModel = CreateModel(obj, strFileToModel);
		}
		catch (char* error) {
			Warning("%s\n", error);
			delete moDoc;
			return;
		}
		/*
		if (moDoc->m_moModel == nullptr)
		{
			Warning("We cannot create model\n");
			delete moDoc;
			return;
		}
		*/

		pModelDoc = NULL;
		pModelDoc = moDoc;

		moDoc->m_moModel->PlayAnimation("ANIM_DEFAULT");
		moDoc->m_typeDocument = DT_MODEL;
		moDoc->strFileName = "Unnamed.mo";
		moDoc->bNew = true;

		moEditedModel = NULL;
		moEditedModel = moDoc->getModel();

		pEditorCamera->setCamera(glm::vec3(0.0f, 1.0f, 3.0f));

		pEditorCamera->Yaw = 0.0f;
		pEditorCamera->Pitch = 0.0f;
		pEditorCamera->cameraUpDate();

		currentDocType = DT_MODEL;

		strCurrentDoc = strFileToModel.GetFileNameWitoutDir();

		_aAllDocuments.push_back(moDoc);
	}
	else if (strFileToModel.CompareEXT(".wo")) {
		PrintF("NOTHING\n");
	}
	else {
		Warning("Only Models \".MO\" or Levels \".WO\" can be supported\n");
	}
}

void DocumentOpen(void)
{
	OPENFILENAME ofn = { 0 };
	TCHAR szFile[260] = { 0 };
	// Initialize remaining fields of OPENFILENAME structure
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Levels \".WO\"\0*.WO\0Models \".MO\"\0*.MO\0All files\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		_bstr_t wcharfile(ofn.lpstrFile);

		strFileToModel = wcharfile;
	}
	else {
		return;
	}

	if (strFileToModel.CompareEXT(".mo"))
	{
		CModelDocument* moDoc = new CModelDocument;

		FileDeleteAbsolutePatch(strFileToModel);

		moDoc->openDocument(strFileToModel);

		if (_aAllDocuments.size() != 0)
		{
			unsigned int i = _aAllDocuments.size() - 1;

			_aAllDocuments[i]->m_fPitch = pEditorCamera->Pitch;
			_aAllDocuments[i]->m_fYaw = pEditorCamera->Yaw;
			_aAllDocuments[i]->m_vPosition = pEditorCamera->m_position;
		}

		_aAllDocuments.push_back(moDoc);

		pModelDoc = moDoc;

		moEditedModel = NULL;

		moEditedModel = pModelDoc->getModel();

		pEditorCamera->setCamera(glm::vec3(0.0f, 1.0f, -3.0f));

		pEditorCamera->Yaw = 0.0f;
		pEditorCamera->Pitch = 0.0f;
		pEditorCamera->cameraUpDate();

		currentDocType = DT_MODEL;

		strCurrentDoc = strFileToModel.GetFileNameWitoutDir();

	}
	else if (strFileToModel.CompareEXT(".wo")) {
		PrintF("NOTHING\n");
	}
	else {
		Warning("Only models \".MO\" or levels \".WO\" can be supported\n");
	}
}

void DocumentClose(void)
{

	if (_aAllDocuments.size() == 0)
		return;

	// first we find in array our document
	auto toClose = std::find(_aAllDocuments.begin(), _aAllDocuments.end(), pModelDoc);

	if (toClose != _aAllDocuments.end()) 
	{
		// get index of document
		unsigned int index = toClose - _aAllDocuments.begin();

		// deattach pointers
		moEditedModel = NULL;
		teEditedModelTexture = NULL;
		pModelDoc = NULL;

		// delete doc
		delete _aAllDocuments[index];
		_aAllDocuments[index] = NULL;

		_aAllDocuments.erase(_aAllDocuments.begin() + index);

		// move to last document for viewing
		index = _aAllDocuments.size() - 1;

		PrintF("index = %d\n", index);

		if (_aAllDocuments.size() != 0)
		{
			if (_aAllDocuments[index]->m_typeDocument == DT_MODEL)
			{
				pModelDoc = (CModelDocument*)_aAllDocuments[index];

				moEditedModel = pModelDoc->getModel();
				teEditedModelTexture = pModelDoc->getTexture();

				pEditorCamera->Yaw = pModelDoc->m_fYaw;
				pEditorCamera->Pitch = pModelDoc->m_fPitch;
				pEditorCamera->m_position = pModelDoc->m_vPosition;
				pEditorCamera->cameraUpDate();

				currentDocType = DT_MODEL;
			}
		}
		else {
			currentDocType = DT_NONE;
			strCurrentDoc = "NONE";
		}
	}
	else {
		PrintF("NOTHING TO CLOSE\n");
	}
}

void DocumentSave(void) 
{
	OPENFILENAME ofn = { 0 };

	TCHAR szFile[260] = { 0 };
	// Initialize remaining fields of OPENFILENAME structure
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Model \".MO\"\0*.MO\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetSaveFileName(&ofn) == TRUE)
	{
		_bstr_t wcharfile(ofn.lpstrFile);

		strFileToModel = wcharfile;

		if (strFileToModel.strFileName == "") {
			return;
		}

		FileDeleteAbsolutePatch(strFileToModel);
	}

	try {
		moEditedModel->Save(strFileToModel);
		pModelDoc->bNew = false;
		pModelDoc->strFileName.ChangeEXT(".mo");
	}
	catch (void* str) {
		Warning("Cannon save model file: %s\n", (const char*)str);
	}
}

void selectDocument(int index)
{
	if (_aAllDocuments.size() == 0 || _aAllDocuments.size() <= index)
	{
		return;
	}

	CEditorDocument* doc = _aAllDocuments[index];

	if (doc->strFileName.CompareEXT(".mo")) 
	{
		pModelDoc = (CModelDocument*)doc;

		moEditedModel = pModelDoc->getModel();
		teEditedModelTexture = pModelDoc->getTexture();

		strCurrentDoc = pModelDoc->strFileName.strFileName;

		currentDocType = DT_MODEL;
	}

}

static void End(void) 
{
	// release globals of editor application
	EditorEnd();
	// release engine
	InTheEngine_Shutdown();
	// delete context OpenGL
	SDL_GL_DeleteContext(window.glcontext);
	// delete window
	SDL_DestroyWindow(window.window);
	// release SDL library
	SDL_Quit();
	// free memory fo global tool bar text
	if (mainToolBar)
		free(mainToolBar);
	// exit from In the Editor
	exit(EXIT_SUCCESS);
}

static void drawMainToolBar() 
{
	drawSquare(0, 0, pViewPort->GetWidth(), 14, COLOR_ADD_ALPHA(COLOR_BLUE, 64UL) /* (COLOR_BLUE & MASK_ALPHA) | 64UL*/);

	mainToolBar = stringCopyF("| N - NEW | O - OPEN | C - CLOSE | E - EDIT | S - SAVE | D - SELECT DOCUMENT | H - HELP | Q - QUIT | current document = \"%s\"", strCurrentDoc.c_str());

	drawText(systemEngineFont, 0, 0, 1.0f, COLOR_CYAN, mainToolBar);

	free(mainToolBar);
	mainToolBar = NULL;
}

static void drawEditorHelp(void) 
{
	unsigned int iPosY = 28 + 14;

	drawSquare(0, iPosY + 98, pViewPort->GetWidth(), 98, COLOR_ADD_ALPHA(0x105723FFUL, 64UL));

	FOREACH_ARRAY(14) 
	{
		drawText(systemEngineFont, 0, iPosY, 1.0f, COLOR_GREEN, strHelpTable[i].c_str());
		iPosY += 14;
	}
}

static void drawDocumentsBar(void)
{
	std::string docNames = "";

	FOREACH_ARRAY(_aAllDocuments.size())
	{
		CEditorDocument* doc = _aAllDocuments[i];
		docNames += stringCopyF("[%d] ", i);
		docNames += doc->strFileName.GetFileNameWitoutDir();
		docNames += "; ";
	}

	drawSquare(0, 14+7, pViewPort->GetWidth(), 7, COLOR_ADD_ALPHA(0xA81414FFUL, 64UL));

	drawText(systemEngineFont, 0, 14, 1.0f, COLOR_WHITE, docNames.c_str());

}

static void drawModeType() 
{
	std::string mode;
	HEXColor color = COLOR_RED;

	if (editMode == MODE_FLY) {
		mode = "MODE: FLY"; color = COLOR_GREEN;
	}
	else if (editMode == MODE_EDIT) {
		mode = "MODE: EDIT"; color = COLOR_YELLOW;
	}
	else {
		mode = "MODE: WTF (59.1)";
	}
	
	drawText(systemEngineFont, 0, 28, 1.0f, color, mode.c_str());
}




static void drawFloor(void) 
{
	if (!bRenderFloor) return;
	RenderSetupOneModel(moFloor);	
	RenderOneModel(moFloor, colFloor);
}

static void drawModelFromDocument(void) 
{
	RenderSetupOneModel(moEditedModel);
	RenderOneModel(moEditedModel, COLOR_WHITE);
}

static void ProccesInput(const double time) 
{
	SDL_Event e;


	if (!pEditorCamera->bActive) {
		editMode = MODE_EDIT;
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}

	if (editMode == MODE_FLY) 
	{
		cameraInput(time);
		return;
	}

	while (SDL_PollEvent(&e))
	{
		

		if (e.type == SDL_KEYDOWN)
		{

			byte helpState = 0;
			byte floorState = 0;

			if (e.key.keysym.sym == SDLK_e)
			{
				editMode = MODE_FLY;
				pEditorCamera->bActive = true;
				SDL_SetRelativeMouseMode(SDL_TRUE);
			}

			if (e.key.keysym.sym == SDLK_o) {
				//OpenModelFile();
				DocumentOpen();
			}

			if (e.key.keysym.sym == SDLK_t) {
				OpenTextureFileForModel();
			}

			if (e.key.keysym.sym == SDLK_n) {
				DocumentNew();
			}

			if (e.key.keysym.sym == SDLK_s) {
				DocumentSave();
			}

			if (e.key.keysym.sym == SDLK_c) {
				DocumentClose();
			}

			if (e.key.keysym.sym == SDLK_h && !bLockHelp) {
				bLockHelp = true;
				helpState = 1;
			//	PrintF("Here true\n");
			}

			if (e.key.keysym.sym == SDLK_h && bLockHelp && !helpState) {
				bLockHelp = false;
			//	PrintF("Here false\n");
			}


			if (e.key.keysym.sym == SDLK_f && !bRenderFloor) {
				bRenderFloor = true;
				floorState = 1;
			}

			if (e.key.keysym.sym == SDLK_f && bRenderFloor && !floorState) {
				bRenderFloor = false;
			}

		}

		if (e.type == SDL_KEYUP)
		{
			if (e.key.keysym.sym == SDLK_q) {
				End();
			}
		}

	}
}

static void MainLoop(void)
{
	double deltaTime = 0;
	double lastFrame = 0;

	LOOP
	{
		double currentFrame = (double)SDL_GetTicks64() / 1000.0;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProccesInput(deltaTime);

		pViewPort->m_viewMatrix = pEditorCamera->getMatrixView();
		// fill backround color
		drawColorFull(colBackgroud);

		if (currentDocType == DT_MODEL) {
			// draw floor for modeler
			drawFloor();
			// draw model will be edited
			drawModelFromDocument();
		}
		// draw tool bar
		drawMainToolBar();

		drawDocumentsBar();
		// draw edit mode type
		drawModeType();

		if (bLockHelp)
			drawEditorHelp();
	
		SDL_GL_SetSwapInterval(1);

		SDL_GL_SwapWindow(window.window);
	}


}

int main(int argc, char** argv) 
{
	/* init window canvas */
	WindowInit(&window);

	/* setup engine */
	InTheEngine_Start(argc, argv, EDITOR_NAME);

	// setup view of main canvas game
	pViewPort->setUpView(window.Wigth, window.Height, 0.1f, 1000.0f, 90.f);

	// set as current view, because we have only one view of game
	setCurrentViewPort(pViewPort);

	/* setup editor globals and pointers */
	EditorInit();

	/* main loop of program */
	MainLoop();

	/* never gets here */
	return 0;
}