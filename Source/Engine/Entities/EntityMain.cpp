#include <stdAfx.h>

#include "EntityMain.h"
#include <Engine\Common\Console.h>
#include <Engine\Common\FileName.h>
#include <Engine\Common\FileIO.h>
#include <Engine\Common\PrintMessages.h>

#include <Engine\Graphics\Texture.h>
#include <Engine\Level\Brush.h>
#include <Engine\Level\BaseWorld.h>
#include <Engine\DataBase\DataBase.h>

bool IsClass(CEntityMain* e, const char* strClass)
{
	if (e)	return e->m_strClassName == strClass;
	return false;
}

CEntityMain::CEntityMain() 
{
	m_ulID = 0;
	
	m_strName = "Unnamed";
	
	m_strClassName = "Entity";

	
	m_vPosition = VECTOR3_NONE;
	// rotation of entity
	m_anglRotaion = VECTOR3_NONE;

	// color of entity
	m_color = COLOR_WHITE;

	// view matrix
	m_matModel = glm::mat4(1.0f);

	// health of entity (-1 - is invincible)
	m_iHealth  = -1;
	// has physics
	m_bPhysics = false;

	m_bCatch = false;

	//m_pmo = NULL; // if this model
	//m_pte = NULL; // if this texture
	m_pbr = NULL; // if this brush

	m_pwo = NULL; // pointer to world

	m_numProperties = 1; // count of properties
	m_iBrushIndex = 0;
	m_ulFlags = 0;      // spawn flags

	m_bDead = false;
}

CEntityMain::~CEntityMain() 
{
	if (m_bDead) return;

	Clear();
}

void CEntityMain::Clear()
{
	End();
}

void CEntityMain::Load(CFileStream& file)
{
	m_strName = file.ReadStringFromFile();

	file.ReadFromFile(&m_ulID, sizeof(unsigned long));

	file.ReadFromFile(&m_color, sizeof(HEXColor));

	file.ReadFromFile(&m_vPosition.x, sizeof(float));
	file.ReadFromFile(&m_vPosition.y, sizeof(float));
	file.ReadFromFile(&m_vPosition.z, sizeof(float));

	file.ReadFromFile(&m_anglRotaion.x, sizeof(float));
	file.ReadFromFile(&m_anglRotaion.y, sizeof(float));
	file.ReadFromFile(&m_anglRotaion.z, sizeof(float));

	file.ReadFromFile(&m_iHealth, sizeof(int));
	file.ReadFromFile(&m_bPhysics, sizeof(bool));
	file.ReadFromFile(&m_numProperties, sizeof(unsigned int));

	file.ReadFromFile(&m_ulFlags, sizeof(unsigned long));

	
	float data[16];

	file.ReadFromFile(data, sizeof(float) * 16);

	memcpy(glm::value_ptr(m_matModel), data, sizeof(data));

	//FOREACH_ARRAY(m_matModel.length())
	//{
	//	file.ReadFromFile(&m_matModel[i], sizeof(float));
	//}

	if (file.checkID_Safe("MDL "))
	{
		m_strModel = file.ReadStringFromFile();
		m_strTexture = file.ReadStringFromFile();
	}
	else if (file.checkID_Safe("BRSH")) 
	{
		file.ReadFromFile(&m_iBrushIndex, sizeof(unsigned int));

		m_pbr = m_pwo->gw_brushes[m_iBrushIndex];
	}
	else {
		return;
	}
}

void CEntityMain::Save(CFileStream& stream) 
{
	stream.WriteString(m_strClassName);

	stream.WriteString(m_strName);

	stream.WriteToFile(&m_ulID, sizeof(unsigned long));

	stream.WriteToFile(&m_color, sizeof(HEXColor));

	stream.WriteToFile(&m_vPosition.x, sizeof(float));
	stream.WriteToFile(&m_vPosition.y, sizeof(float));
	stream.WriteToFile(&m_vPosition.z, sizeof(float));

	stream.WriteToFile(&m_anglRotaion.x, sizeof(float));
	stream.WriteToFile(&m_anglRotaion.y, sizeof(float));
	stream.WriteToFile(&m_anglRotaion.z, sizeof(float));

	stream.WriteToFile(&m_iHealth, sizeof(int));
	stream.WriteToFile(&m_bPhysics, sizeof(bool));
	stream.WriteToFile(&m_numProperties, sizeof(unsigned int));

	stream.WriteToFile(&m_ulFlags, sizeof(unsigned long));


	stream.WriteToFile(glm::value_ptr(m_matModel), sizeof(float) * 16);

	//Warning("L: %i\n", m_matModel.length());

	//FOREACH_ARRAY(16)
	//{
	//	stream.WriteToFile(&m_matModel[i], sizeof(float));
	//}

	if ( m_pbr ) {
		stream.writeID("BRSH");
		stream.WriteToFile(&m_iBrushIndex, sizeof(unsigned int));
	}
	else if (m_ulID & ENTITY_MODEL){
		stream.writeID("MDL ");

		stream.WriteString(m_strModel.strFileName);
		stream.WriteString(m_strTexture.strFileName);
	}
	else {
		return;//stream.writeID("EDIT");
	}
}

void CEntityMain::InitAsEditorEntity()
{
	m_ulFlags &= ~ENTITY_MODEL;
	m_ulFlags |= ENTITY_EDITOR;
}

void CEntityMain::InitAsModel()
{

	m_ulFlags |= ENTITY_MODEL;
	m_ulFlags &= ~ENTITY_EDITOR;

	m_pmo.Load(m_strModel);

	FileSetGlobalPatch(m_strTexture);

	m_pmo.SetTexture(m_strTexture);

	m_pmo.SetUpModel();

	m_pmo.m_vScale = glm::vec3(1);

	//m_pmo = new CModelObject;
	//m_pte = new CTextureObject;

	//m_pmo->Load(m_strModel);
	//m_pte->Load(m_strTexture);

	//m_pte->Prepare();

	//m_pmo->textureId = m_pte->texID;
}



void CEntityMain::End()
{
	if (m_pbr) {
		m_pbr = NULL;
	}

	m_pmo.Clear();

	m_strName.clear();

	m_strClassName.clear();

	m_pwo = NULL;

	m_bDead = true;
}