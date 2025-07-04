#include <stdAfx.h>
#include "DataBase.h"

#include <Engine/Common/Console.h>
#include <Engine/Common/PrintMessages.h>
#include <Engine/Common/FileName.h>
#include <Engine/Common/FileIO.h>
#include <Graphics/Texture.h>
#include <Model/Model.h>

void reportLoadedObjects(void)
{
	if (pDataBase == NULL) return;

	PrintF("Loaded data base information:\n");
	PrintF(" total textures: %d\n", pDataBase->allTextures.size());
	PrintF(" total models:   %d\n", pDataBase->allModels.size());
	PrintF(" total sounds:   %d\n", 0);

	PrintF("---------------------------------------------------------------\n");

}

void printLoadedObjects()
{
	PrintF("*********LOADED INFO*************\n");
	PrintF("Textures:\n");
	FOREACH_STD_UNORDERED_MAP(pDataBase->allTextures, std::string, CTextureObject*)
	{
		PrintF(" %s\n", iter->first.c_str());
	}
	PrintF("Models:\n");
	FOREACH_STD_UNORDERED_MAP(pDataBase->allModels, std::string, CModelObject*)
	{
		PrintF(" %s\n", iter->first.c_str());
	}

	PrintF("---------------------------------------------------------------\n");
}


CDataBase::CDataBase()
{
	pConsoleDataBase->addConsoleFunction("reportLoadedObjects()", reportLoadedObjects);
	pConsoleDataBase->addConsoleFunction("printLoadedObjects()",  printLoadedObjects);
}

CDataBase::~CDataBase() 
{
	Clear();
}

void CDataBase::Clear()
{
	FOREACH_STD_UNORDERED_MAP(allTextures, std::string, CTextureObject*) 
	{
		CTextureObject* te = iter->second;
		te->Clear();
		delete te;
	}
	allTextures.clear();

	FOREACH_STD_UNORDERED_MAP(allModels, std::string, CModelObject*)
	{
		CModelObject* mo = iter->second;
		delete mo;
	}

	allModels.clear();

}

//CModelObject* loadModel(CFileName& strFileName);
CTextureObject* CDataBase::loadTexture(const CFileName& strFileName) 
{
	auto iter = allTextures.find(strFileName.strFileName);

	if (iter != allTextures.end())
	{
		return iter->second;
	}

	CTextureObject* texture = nullptr;

	CFileName fileDummy = strFileName;

	CFileStream stream;

	
	FileDeleteAbsolutePatch(fileDummy);

	if (!stream.open(fileDummy, 0, 1))
	{
		Throw("Cannot write CRC32 for %s file", (const char*)fileDummy);
	}

	unsigned int CRC = stream.calculateFileCRC();

	stream.closeFile();

	FileSetGlobalPatch(fileDummy);

	CRCTextures[CRC] = strFileName.strFileName;

	texture = new CTextureObject;
	texture->Load(fileDummy);
	texture->Prepare();

	texture->texCRC = CRC;

	allTextures[strFileName.strFileName] = texture;

	return texture;
}

CModelObject* CDataBase::loadModel(const CFileName& strFileName)
{
	auto iter = allModels.find(strFileName.strFileName);

	if (iter != allModels.end())
	{
		return iter->second;
	}

	CModelObject* obj;

	obj = new CModelObject;
	obj->Load(strFileName);

	allModels[strFileName.strFileName] = obj;

	CFileStream stream;
	
	if (!stream.open(strFileName, 0, 1)) 
	{
		Throw("Cannot write CRC32 for %s file", (const char*)strFileName);
	}

	unsigned int CRC = stream.calculateFileCRC();

	stream.closeFile();

	CRCModels[CRC] = strFileName.strFileName;

	obj->mo_modCRC = CRC;

	return obj;
}

CModelObject* CDataBase::loadModelCRC(const unsigned int& CRC)
{
	auto iter = CRCModels.find(CRC);

	if (iter == CRCModels.end()) return nullptr;

	CFileName strFile = iter->second;
	return loadModel(strFile);
}

CTextureObject* CDataBase::loadTextureCRC(const unsigned int& CRC)
{
	auto iter = CRCTextures.find(CRC);

	if (iter == CRCTextures.end()) return nullptr;

	CFileName strFile = iter->second;
	return loadTexture(strFile);
}

unsigned long CDataBase::getUsedMemory(void)
{
	return 0;
}
