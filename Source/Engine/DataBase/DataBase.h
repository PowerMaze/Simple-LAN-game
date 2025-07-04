#pragma once

/*
///////////////////////////////////////////////////
Data base for loaded objects, used for precaching

Stores: Models, Textures and Sounds
///////////////////////////////////////////////////
*/
class CDataBase 
{
public:
	std::unordered_map<std::string, CModelObject*> allModels;
	std::unordered_map<std::string, CTextureObject*> allTextures;
//	std::unordered_map<std::string, CSound*> allSounds;
//	std::unordered_map<std::string, CEntity*> allEntities;


	std::unordered_map<unsigned int, std::string> CRCModels;
	std::unordered_map<unsigned int, std::string> CRCTextures;

public:
	CDataBase();
	~CDataBase();
	void Clear();

	ENGINE_API CModelObject* loadModel(const CFileName& strFileName);
	ENGINE_API CTextureObject* loadTexture(const CFileName& strFileName);

	ENGINE_API CModelObject* loadModelCRC(const unsigned int& CRC);
	ENGINE_API CTextureObject* loadTextureCRC(const unsigned int& CRC);

	void FillCRC(void);

//	void deleteModel(CModelObject* mo);
//	void deleteTexture(CTextureObject* te);

	//void releaseUnused(CModelObject* mo);
	//void releaseUnused(CTextureObject* te);

	//void clearAllDataBase(void);

	unsigned long getUsedMemory(void);
};

/* global object of data base for precaching and caching elements */
ENGINE_API extern CDataBase* pDataBase;

ENGINE_API extern void reportLoadedObjects(void);

//ENGINE_API extern void loadObjectsFromWorld(CFileStream& fileStream)

