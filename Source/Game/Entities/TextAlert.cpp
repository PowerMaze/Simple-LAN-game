#include "stdAfx.h"

#include "../GEntities.h"

CTextAlert::CTextAlert(void)
{

	m_ulID = 0;

	m_strClassName = "TextAlert";
	m_strName      = "TextAlert";

	bOnce = true;
	bLoop = true;
	strAlert = "";	
}


void CTextAlert::setText(const char* strText) 
{
	strAlert = strText;
}

void CTextAlert::Load(CFileStream& stream) 
{
	CEntityMain::Load(stream);

	stream.ReadFromFile(&bOnce, sizeof(bool));
	stream.ReadStringFromFile(strAlert);
}


void CTextAlert::Update(void)
{
	CEntityMain::Update();


	if (bOnce && bLoop) 
	{
		PrintF("%s\n", strAlert.c_str());
		bLoop = false;
	}
}

void CTextAlert::Initialize() 
{

}

