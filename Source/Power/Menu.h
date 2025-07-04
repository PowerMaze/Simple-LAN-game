#pragma once

#include "MenuButton.h"

//CMenuButton* mb_ButtonTest;

//std::vector<CMenuButton*> _avctAllButtons;


struct MainMenu_s
{
	CMenuButton* m_mbSinglePlayer;
	CMenuButton* m_mbNetwork;
	CMenuButton* m_mbOption;
	CMenuButton* m_mbAbout;
	CMenuButton* m_mbQuit;
};

struct NetworkMenu_s
{
	CMenuButton* m_mbCreateNew;
	CMenuButton* m_mbConnect;
	CMenuButton* m_mbOption;	
};

void menuStart(void);

void menuDraw(void);

void menuAction(void);

void menuMain(void);

void menuHideAll(void);

void menuEnd(void);
