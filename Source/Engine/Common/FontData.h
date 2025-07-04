#pragma once

#include <Engine/Common/FileName.h>
/*
Character data, contains size and texture coordinates
*/
typedef struct CharacterData_s 
{
	float m_fData[16]; //[24]
}CharacterData_t;


/*
////////////////////////////////////////////

			Main font class

////////////////////////////////////////////
*/
class CFont 
{
public:
	// map of characters
	std::map<char, CharacterData_t> characters;
	// file name for load texture
	CFileName strTextureFileName;
	// main texture
	CTextureObject* m_teData;
	// prepared rendered
	bool m_bPrepareRender;
	// info symbols
	short charWidth, charHeight;
public:
	// default constructor
	ENGINE_API CFont(void);
	// constructor from file name
	ENGINE_API CFont(CFileName& strFileName);
	// create font functions @returns success of creating font
	ENGINE_API bool createFont(const char* charData, int w, int h, CFileName& strFileTexture);
	// load font
	ENGINE_API void Load(CFileName& strFileName);
	// save font
	ENGINE_API void Save(CFileName& strFileName);
	ENGINE_API void SetSpace(byte space);
	ENGINE_API byte GetSpace(void);
	// clear font
	ENGINE_API void Clear(void);
	// get vbo
	__forceinline unsigned int getVBO(void) { return VBO; }
	// get vao
	__forceinline unsigned int getVAO(void) { return VAO; }
	// get ebo
	__forceinline unsigned int getEBO(void) { return EBO; }
private:
	void prepare(void);
private:
	unsigned int VBO, VAO, EBO;
	byte m_ucSpaceBetweenChars;
};


ENGINE_API extern CFont* systemEngineFont;
ENGINE_API extern CFont* pFontDisplay;

//ENGINE_API extern void drawText(CShader& sha, CFont* font, const char* strText, int x, int y, HEXColor color);
