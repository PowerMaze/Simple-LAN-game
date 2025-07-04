#pragma once

/*
////////////////////////////////////////////

 Editor document contains many documents 
 of model file or world file

////////////////////////////////////////////
*/

#define MAX_DOCUMENTS 8

typedef enum EDocType 
{
	DT_NONE,
	DT_MODEL,
	DT_WORLD
}EDocumentType;

class CEditorDocument 
{
public:

	uint m_uID;
	CFileName strFileName;
	bool bNew;
	bool bIsCurrent;

	EDocumentType m_typeDocument;

	/* save location of camera */
	float m_fYaw;
	float m_fPitch;
	Vector3D m_vPosition;

public:
	CEditorDocument(void);
	~CEditorDocument(void);
	void Clear(void);

	virtual void openDocument(const CFileName& strFile) {};
	virtual void saveDocument(void) {};
	virtual void closeDocument(void) {};
};

class CModelDocument : public CEditorDocument 
{
public:

	CModelObject *m_moModel;
	CTextureObject     *m_teTexture;

public:

	CModelDocument(void);
	~CModelDocument(void);

	void openDocument (const CFileName& strFile) override;
	virtual void saveDocument(void) override;
	virtual void closeDocument(void);
	void setTexture(CFileName& strFile);

	CModelObject* getModel(void);
	CTextureObject* getTexture(void);
};

class CWorldDocument : public CEditorDocument 
{
public:
	CGameWorld* m_woGameWorld;
	HEXColor m_BackGroundColor;
	CSkyBox* m_skyBox;

	bool bRenderGame;

public:
	CWorldDocument();
	~CWorldDocument();

	CGameWorld* getWorld(void);

	CSkyBox* makeSkyBox(const char** strImages);

};

// global array of all documents, contains models documents and worlds
extern std::vector< CEditorDocument* > _aAllDocuments;