#include "stdAfx.h"

#include "EditorDoc.h"

CEditorDocument::CEditorDocument(void) 
{
	m_uID = 0;
	strFileName = "";
	bNew = false;
	bIsCurrent = false;
	m_typeDocument = DT_NONE;

	m_fYaw = -90.0f;
	m_fPitch = 0.0f;
	m_vPosition = glm::vec3(0.f);

}

CEditorDocument::~CEditorDocument(void) 
{
	Clear();
}
void CEditorDocument::Clear(void)
{
	strFileName.strFileName.clear();
}