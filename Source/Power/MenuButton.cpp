#include "stdAfx.h"

#include "MenuButton.h"

CMenuButton::CMenuButton() 
{
	m_strName = NULL;
	m_strCaption = NULL;
	m_strDescription = NULL;

	m_uiXpos = 0;
	m_uiYpos = 0;
	m_iPosType = BPT_LEFT;

	memset(m_fCorners, 0, sizeof(m_fCorners));
}

CMenuButton::CMenuButton(const char* name, const char* caption, const char* description, unsigned short X, unsigned short Y) 
{
	m_strName = stringCopy(name);
	m_strCaption = stringCopy(caption);
	m_strDescription = stringCopy(description);

	m_uiXpos = X;
	m_uiYpos = Y;
	m_iPosType = 1;
}
CMenuButton::~CMenuButton(void) 
{
	Clear();
}
void CMenuButton::Clear(void) 
{

}
void CMenuButton::DrawButton(HEXColor color) 
{
	drawText(systemEngineFont, 200, 300, 1.0f, color, "dummy");
}