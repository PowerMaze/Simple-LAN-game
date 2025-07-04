#include <stdAfx.h>

#include "FontData.h"

#include <Engine/Common/Console.h>
#include <Engine/Common/PrintMessages.h>
#include <Engine/Common/FileName.h>
#include <Engine/Common/FileIO.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Graphics/Draw.h>
#include <Engine/Graphics/Shader.h>

#include <Engine/Math/Functions.h>

//#define STR_TAB   '\t'
//#define STR_SPACE '\n'

void drawText(CFont* font, uint uX, uint uY, float fScale, const HEXColor col, const char* strText)
{
	if (font == NULL) return;

	if (strText == NULL) return;

	unsigned int strLen = strlen(strText);
	if (strLen == 0) return;

	Vector4D vColor = HEX_toOGL(col);

	fScale = Clamp(fScale, 0.1f, 64.f);

	shaText.use();

	shaText.shaderSetVector("textColor", glm::vec3(vColor.x, vColor.y, vColor.z));
	shaText.shaderSetMatrix("matProjection", pViewPort->m_OrtoMatrix);

	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	shaText.shaderSet("texture1", 0);

	glBindVertexArray(font->getVAO());

	// left top corner
	float xOffset = float(uX);
	float yOffset = float(uY);

	for (unsigned i = 0; i < strLen; i++)
	{	
		if (strText[i] == '^') 
		{
			i++;

			switch (strText[i]) 
			{
			case '0':
				vColor = HEX_toOGL(COLOR_WHITE);
				i++;
				break;
			case '1':
				vColor = HEX_toOGL(COLOR_BLACK);
				i++;
				break;
			case '2':
				vColor = HEX_toOGL(COLOR_RED);
				i++;
				break;
			case '3':
				vColor = HEX_toOGL(COLOR_YELLOW);
				i++;
				break;
			case '4':
				vColor = HEX_toOGL(COLOR_GREEN);
				i++;
				break;
			case '5':
				vColor = HEX_toOGL(COLOR_CYAN);
				i++;
				break;
			case '6':
				vColor = HEX_toOGL(COLOR_BLUE);
				i++;
				break;
			case '7':
				i++;
				vColor = HEX_toOGL(COLOR_MAGENTA);
				break;
			case '8':
				i++;
				vColor = HEX_toOGL(col);
				break;
			default:	
				i--;
				break;
			}
			shaText.shaderSetVector("textColor", glm::vec3(vColor.x, vColor.y, vColor.z));
		}

		CharacterData_t charData = font->characters[strText[i]];

		// right bottom
		charData.m_fData[0] *= fScale;
		charData.m_fData[1] *= fScale;
		// right top
		charData.m_fData[4] *= fScale;
		charData.m_fData[5] *= fScale;
		// left top
		//charData.m_fData[8] *= fScale;
		//charData.m_fData[9] *= fScale;
		// left bottom
		charData.m_fData[12] *= fScale;
		charData.m_fData[13] *= fScale;

        //---------------------------------------------

		//float ft = sin(double(SDL_GetTicks64()) * 0.001);
		//ft = fabs(ft);

		// right bottom
		charData.m_fData[0] += xOffset * fScale;
		charData.m_fData[1] += yOffset *fScale;
		// right top
		charData.m_fData[4] += xOffset * fScale;
		charData.m_fData[5] += yOffset *fScale;
		// left top
		charData.m_fData[8] += xOffset *fScale;
		charData.m_fData[9] += yOffset *fScale;
		// left bottom
		charData.m_fData[12] += xOffset * fScale;
		charData.m_fData[13] += yOffset * fScale;

		glBindTexture(GL_TEXTURE_2D, font->m_teData->texID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, font->getVBO());
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(charData), charData.m_fData);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		xOffset += float(font->GetSpace());
	}


	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ZERO);

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
}


float change = 0.0f;
void drawSingleCharacterTest(CFont* font, const float fScale, const char c, const HEXColor col) 
{
	if (font == NULL) return;

	Vector4D vColor = HEX_toOGL(col);

	shaText.use();

	shaText.shaderSetVector("textColor", glm::vec3(vColor.x, vColor.y, vColor.z));
	shaText.shaderSetMatrix("matProjection", pViewPort->m_OrtoMatrix);
	glm::mat4 model = glm::mat4(1.0f);
	
	//model = glm::translate(model, glm::vec3(400.0f, 300.0f, 0.0f));
	
//	change += 0.0001f;
//	if (change > 1.f)
//		change = 0;

	shaText.shaderSet("change", change);
	shaText.shaderSetMatrix("matModel", model);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	shaText.shaderSet("texture1", 0);

	glBindVertexArray(font->getVAO());

	CharacterData_t charData = font->characters[c];

	charData.m_fData[0] *= fScale;
	charData.m_fData[1] *= fScale;
	charData.m_fData[4] *= fScale;
	charData.m_fData[5] *= fScale;
	charData.m_fData[8] *= fScale;
	charData.m_fData[9] *= fScale;
	charData.m_fData[12] *= fScale;
	charData.m_fData[13] *= fScale;

	FOREACH_ARRAY(16)
	{
		PrintF("%f ", charData.m_fData[i]);
		if (i == 3 || i == 7 || i == 11 || i == 15)
			PrintF("\n");
		
	}
	PrintF("------------------\n");
	glBindTexture(GL_TEXTURE_2D, font->m_teData->texID);
	// update content of VBO memory
	glBindBuffer(GL_ARRAY_BUFFER, font->getVBO());
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(charData), charData.m_fData);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// render quad
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


CFont::CFont(void)
{
	//numCharacters = 0;
	m_teData = NULL;
	m_bPrepareRender = false;
	charWidth = 0;
	charHeight = 0;
	m_ucSpaceBetweenChars = 1;
	VAO = 0;
	VBO = 0;
	EBO = 0;
}
CFont::CFont(CFileName& strFileName)
{
	Load(strFileName);
}

void CFont::SetSpace(byte space)
{
	m_ucSpaceBetweenChars = Clamp(space, byte(1), byte(255));
}
byte CFont::GetSpace(void) {
	return m_ucSpaceBetweenChars;
}

bool CFont::createFont(const char* charData, int w, int h, CFileName& strFileTexture)
{
	// clear data before we create new font
	Clear();

	// temp data contains uv and coordinates of symbol
	CharacterData_t chrDat;

	// load texture
	m_teData = new CTextureObject;
	m_teData->prepareOnLoads = false;
	m_teData->Load(strFileTexture);
	
	// remember texture file
	strTextureFileName = strFileTexture;

	FileDeleteAbsolutePatch(strTextureFileName);

	charWidth = w;
	charHeight = h;	

	// be sure it has alpha channel
	if (!m_teData->isAlpha)
	{
		Warning("Only 32-bit textures for font will be used\n");
		m_teData->Clear();
		delete m_teData;
		return false;
	}

	// get width and height from texture
	uint textureWidth = m_teData->width;
	uint textureHeight = m_teData->height;

	// we don't need texture, so we can delete this from memory
	delete m_teData;
	m_teData = NULL;

	// convert from char* to std::string
	std::string ASCIIdata = charData;

	// get position of width symbol
	float getXoffset = float(w) / float(textureWidth);
	// get position of height symbol. H - h/2, because openGL UV-coordinates starts on "left-bottom" corner
	float getYoffset = float(h) / float(textureHeight);

	float posX = 0.0f;
	float posY = 1.0f + getYoffset; // because left top starts with 0, 1 (uv)

	printf("%s\n", charData);

	// for each symbol in ascii data char
	for (unsigned int i = 0; i < ASCIIdata.size(); i++)
	{
		char c = ASCIIdata[i];

		if (c == '\n') 
		{
			posX = 0.0f;
			posY += getYoffset;
			continue;
		}

		// clear data array
		memset(chrDat.m_fData, 0, 16 * sizeof(float));

		float dataForSymbol[] =
		{
			 float(w),  float(h), posX+getXoffset, posY,
			 float(w),  float(0), posX+getXoffset, posY-getYoffset,
			 float(0),  float(0), posX, posY-getYoffset,
			 float(0),  float(h), posX, posY
		};
		
		// copy array to data characters
		memcpy(chrDat.m_fData, dataForSymbol, sizeof(dataForSymbol));
		// insert to map of all characters
		characters.insert(std::pair<char, CharacterData_t>(c, chrDat));
		// move to next line
		posX += getXoffset;
	}
		
	return true;

}

void CFont::prepare() 
{
	if (m_bPrepareRender) return;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	unsigned int indices[] = 
	{
		0, 1, 3,
		1, 2, 3
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 4, NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Атрибут текстурних координат
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_teData->renderingFlags |= TEXTURE_CLAMP;
	m_teData->renderingFlags |= TEXTURE_CONST;
	m_teData->Prepare();
	

	m_bPrepareRender = true;
}

void CFont::Load(CFileName& strFileName)
{
	Clear();

	CharacterData_t characterdata;

	CFileStream file;

	if (!file.open(strFileName, 0, 1))
	{
		Error("Error loading font file: %s\n", strFileName.getCharPointer());
	}

	if (!file.checkID("FONT")) 
	{
		Error("Invalid font format: %s\n", strFileName.getCharPointer());
	}

	strTextureFileName = file.ReadStringFromFile();
	file.ReadFromFile(&charWidth, sizeof(short));
	file.ReadFromFile(&charHeight, sizeof(short));

	m_teData = new CTextureObject;

	m_ucSpaceBetweenChars = charWidth;

	FileSetGlobalPatch(strTextureFileName);

	m_teData->Load(strTextureFileName);

	unsigned int numCharacters;
	file.ReadFromFile(&numCharacters, sizeof(unsigned int));

	FOREACH_ARRAY(numCharacters) 
	{
		char c;
		file.ReadFromFile(&c, sizeof(char));

		FOREACH_ARRAY(16) 
		{
			file.ReadFromFile(&characterdata.m_fData[i], sizeof(float));
		}	

		characters.insert(std::pair<char, CharacterData_t>(c, characterdata));
	}

	file.closeFile();

	prepare();

	/*
	std::string str = "Data\\" + strFileName.GetFileNameWitoutDir();

	strFileName = str;

	FileSetGlobalPatch(strFileName);	

	FILE* f = fopen((const char*)strFileName, "w");

	if (f == NULL)
	{
		PrintF("LOH\n");
		return;
	}

	FOREACH_STD_MAP(characters, char, CharacterData_t) 
	{
		CharacterData_t charData = iter->second;

		fprintf(f, "Symbol = %c\n", iter->first);

		fprintf(f, "Pos: %f %f\n", charData.m_fData[0], charData.m_fData[1]);
		fprintf(f, "Pos: %f %f\n", charData.m_fData[4], charData.m_fData[5]);

		fprintf(f, "Pos: %f %f\n", charData.m_fData[8], charData.m_fData[9]);
		fprintf(f, "Pos: %f %f\n", charData.m_fData[12], charData.m_fData[13]);

		fprintf(f, "----------------------------\n");
	}

	fclose(f);
	*/
}


void CFont::Save(CFileName& strFileName)
{
	CFileStream file;

	file.open(strFileName, 0, 0);

	file.WriteToFile("FONT", sizeof(char) * 4);
	file.WriteString(strTextureFileName.strFileName);
	file.WriteToFile(&charWidth, sizeof(short));
	file.WriteToFile(&charHeight, sizeof(short));
	unsigned int numCharacters = characters.size();

	file.WriteToFile(&numCharacters, sizeof(unsigned int));	

	std::map<char, CharacterData_t>::iterator itr;
	for (itr = characters.begin(); itr != characters.end(); itr++)
	{
		file.WriteToFile(&itr->first, sizeof(char));
		FOREACH_ARRAY(16) 
		{
			file.WriteToFile(&itr->second.m_fData[i], sizeof(float));
		}		
	}
	//file.WriteToFile("FEND", sizeof(char) * 4);

	file.closeFile();
}
void CFont::Clear(void)
{
	if (m_teData) {
		delete m_teData;
	}
	m_teData = NULL;
	characters.clear();

	if (m_bPrepareRender) 
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	m_bPrepareRender = false;
}