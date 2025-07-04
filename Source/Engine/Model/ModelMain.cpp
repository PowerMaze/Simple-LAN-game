#include <stdAfx.h>

#include "Model.h"
#include <Graphics/Texture.h>

#include <DataBase/DataBase.h>

CModel::CModel() 
{
	m_pmo = NULL;
	
	m_strFileToModel = "";

	m_bUsed = 0;
	
	m_uiTexID = 0;
	
	m_uiRenderingFlags = 0;

	modelMatrix = glm::mat4(1.0f);

	m_vScale = VECTOR3(1.0f, 1.0f, 1.0f);
	
	m_texCRC = 0;
}

void CModel::Clear() 
{
	// if model loaded from method CModel::Load(const CFileName& strFileName)
	//if (m_pmo && m_strFileToModel.strFileName.size() > 1)
	//{
	//	delete m_pmo;
	//}
	m_uiRenderingFlags = 0;
	m_pmo = NULL;

	m_strFileToModel = "";


	m_texCRC = 0;
}

void CModel::Load(const CFileName& strFileName) 
{
	Clear();

	m_pmo = pDataBase->loadModel(strFileName);

	m_strFileToModel = strFileName;
}

void CModel::SetModel(CModelObject* mo)
{
	Clear();

	m_pmo = mo;
}

void CModel::SetTexture(CTextureObject* tex) 
{
	if (tex == nullptr)  return;

	m_texCRC = tex->texCRC;
	m_uiTexID = tex->texID;
}

void CModel::SetTexture(unsigned int texID)
{
	m_uiTexID = texID;
}

void CModel::SetTexture(const CFileName& strFileName) 
{
	CTextureObject* tex = pDataBase->loadTexture(strFileName);
	//tex->Prepare();
	m_uiTexID = tex->texID;
	m_texCRC = tex->texCRC;
	m_pmo->textureId = tex->texID;
}

void CModel::SetUpModel()
{	
	if (m_pmo)	m_pmo->PrepareToRender();
}

void CModel::ScaleAll(float fRatio)
{
	m_vScale *= fRatio;
	//if (m_pmo == NULL) return;

	//Vector3D vec = glm::vec3(1.f);

	//vec *= fRatio;

	//modelMatrix = glm::scale(modelMatrix, vec);
}

void CModel::PlayAnimation(const char* strAnimName) 
{
	if (m_pmo)  m_pmo->PlayAnimation(strAnimName);
}