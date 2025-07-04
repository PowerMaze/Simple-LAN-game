#include "stdAfx.h"

#include "Light.h"

CLight::CLight() 
{
	CEntityMain::CEntityMain();
}

CLight::~CLight() 
{
	End ( );
}

void CLight::End() 
{
	CEntityMain::End();
}

void CLight::Initialize()
{
	m_ulID = 10;

	m_strClassName = "Light";
	m_strName = "Light";

	SetAmbient(COLOR_GRAY);
	SetDiffuse(COLOR_WHITE);
	SetSpecular(COLOR_WHITE);
}

void CLight::SetAmbient(HEXColor color) 
{
	lg_colAmbient = HEX_toOGL(color);
}
void CLight::SetDiffuse(HEXColor color) 
{
	lg_colDiffuse = HEX_toOGL(color);
}
void CLight::SetSpecular(HEXColor color) 
{
	lg_colSpecular = HEX_toOGL(color);
}

void CLight::Save(CFileStream& stream) 
{
	HEXColor color;
	color = OGL_toHEX(lg_colAmbient);
	stream.WriteToFile(&color, sizeof(unsigned long));
	color = OGL_toHEX(lg_colDiffuse);
	stream.WriteToFile(&color, sizeof(unsigned long));
	color = OGL_toHEX(lg_colSpecular);
	stream.WriteToFile(&color, sizeof(unsigned long));
}

void CLight::Load(CFileStream& stream) 
{
	HEXColor colA, colD, colS;

	stream.ReadFromFile(&colA, sizeof(unsigned long));
	stream.ReadFromFile(&colD, sizeof(unsigned long));
	stream.ReadFromFile(&colS, sizeof(unsigned long));

	SetAmbient(colA);
	SetDiffuse(colD);
	SetSpecular(colS);
}

void CLight::Update(void) 
{

}