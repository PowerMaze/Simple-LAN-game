#include "stdAfx.h"

#include "EditorDoc.h"

CModelDocument::CModelDocument(void) 
{
	m_moModel = nullptr;
	m_teTexture = nullptr;
}
CModelDocument::~CModelDocument(void) 
{
	/*
	*  we must deattach global pointers of model and textures before we clossing document
	*/

	delete m_moModel; m_moModel = nullptr;
	delete m_teTexture; m_teTexture = nullptr;

	CEditorDocument::Clear();
}

void CModelDocument::openDocument(const CFileName& strFile) 
{

	if (m_moModel)
		delete m_moModel;

	if (m_teTexture)
		delete m_teTexture;

	m_moModel = nullptr;

	m_moModel   = new CModelObject;

	strFileName = strFile;

	m_typeDocument = DT_MODEL;

	FileDeleteAbsolutePatch(strFileName);

	m_moModel->Load(strFileName);

	m_moModel->PlayAnimation("ANIM_DEFAULT");
}
void CModelDocument::saveDocument(void)
{
	m_moModel->Save(strFileName);
}
void CModelDocument::closeDocument(void)
{

}

void CModelDocument::setTexture(CFileName& strFile)
{
	if (m_teTexture)
		delete m_teTexture;

	m_teTexture = new CTextureObject;
	m_teTexture->Load(strFile);
	m_teTexture->Prepare();
}

CModelObject* CModelDocument::getModel(void) {
	return m_moModel;
}
CTextureObject* CModelDocument::getTexture(void) {
	return m_teTexture;
}