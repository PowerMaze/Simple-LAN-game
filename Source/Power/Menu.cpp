#include "stdAfx.h"

#include "Menu.h"

CMenuButton* mb_ButtonTest = NULL;

void menuStart() 
{
	mb_ButtonTest = new CMenuButton;

	mb_ButtonTest->m_strName		= stringCopy("butt_Create");
	mb_ButtonTest->m_strCaption		= stringCopy("load level");
	mb_ButtonTest->m_strDescription = stringCopy("loading level from folder");
	mb_ButtonTest->m_uiXpos = 50;
	mb_ButtonTest->m_uiXpos = 50;
}

void menuEnd() 
{
	delete mb_ButtonTest; mb_ButtonTest = nullptr;
}