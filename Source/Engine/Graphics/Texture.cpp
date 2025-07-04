#include <stdAfx.h>
#include "stb_image.h"
#include "Texture.h"

#include <Engine/Common/Console.h>
#include <Engine/Common/FileName.h>
#include "Engine/Common/PrintMessages.h"


CTextureObject::CTextureObject()
{
	width = 0;
	height = 0;
	channels = 0;
	isAlpha = false;
	texID = 0;
	prepareOnLoads = true;
	textureData = nullptr;
	renderingFlags = 0;
	texCRC = 0;
}

CTextureObject::CTextureObject(CFileName& strFileName) 
{
	Load(strFileName);
}

CTextureObject::CTextureObject(const char* strFilename) 
{
	CFileName file = strFilename;
	Load(file);
}

CTextureObject::CTextureObject(const CTextureObject& texture) 
{
	channels = texture.channels;
	height = texture.height;
	width = texture.width;
	isAlpha = texture.isAlpha;
	texID = texture.texID;
	prepareOnLoads = texture.prepareOnLoads;
	renderingFlags = texture.renderingFlags;

	unsigned int imageLength = texture.channels * texture.height * texture.width;

//	if (textureData != nullptr)
//		free(textureData);
	textureData = nullptr;

	textureData = (unsigned char*)malloc(imageLength);

	memset(textureData, 0, imageLength);

	memcpy(textureData, texture.textureData, imageLength);
}

CTextureObject::CTextureObject(CTextureObject&& texture) noexcept 
{
	Clear();

	textureData = texture.textureData;
	channels = texture.channels;
	height = texture.height;
	width = texture.width;
	isAlpha = texture.isAlpha;
	texID = texture.texID;
	prepareOnLoads = texture.prepareOnLoads;
	renderingFlags = texture.renderingFlags;

	texture.textureData = nullptr;
	texture.renderingFlags = 0;
	texture.channels = 0;
	texture.height = 0;
	texture.width = 0;
	texture.isAlpha = false;
	texture.texID = 0;
	texture.prepareOnLoads = false;
}

void CTextureObject::loadFromImage(CFileName& strFileName)
{
	stbi_set_flip_vertically_on_load(true);
	Clear();
	textureData = stbi_load((const char*)strFileName, &width, &height, &channels, 0);

	if (textureData == NULL)
	{
		Throw("Cannot load texture: %s\n", strFileName.strFileName.c_str());
	}	

	channels == 4 ? isAlpha = true : isAlpha = false;	
}

void CTextureObject::Clear()
{
	width = height = 0;
	if (textureData != nullptr)
	{
		free(textureData);
	}
	textureData = nullptr;

	channels = 0;
	height = 0;
	width = 0;
	isAlpha = false;
	prepareOnLoads = false;
	glDeleteTextures(1, &texID);
	texID = 0;
}

CTextureObject::~CTextureObject() 
{
	Clear();
}

void CTextureObject::Load(CFileName& strFileName)
{
	Clear();

	FILE* file;
	unsigned int magic, imageLength;
	char strID[5];


	file = fopen(strFileName.strFileName.c_str(), "rb");

	if (file == NULL)
	{
		Error("Cannot load texture %s\n", strFileName.strFileName.c_str());
	}

	fread(strID, sizeof(char) * 4, 1, file);
	strID[4] = '\0';

	if (strcmp(strID, "TEXX") != 0) {
		PrintF("Invalid texture: %s\n", strFileName.strFileName.c_str());
		fclose(file);
		return;
	}

	fread(&magic, sizeof(unsigned int), 1, file);

	if (magic != TEXTURE_MAGIC) 
	{
		PrintF("Invalid texture version\n");
		fclose(file);
		return;
	}

	fread(&isAlpha, sizeof(bool), 1, file);

	fread(&width, sizeof(int), 1, file);
	fread(&height, sizeof(int), 1, file);
	fread(&channels, sizeof(int), 1, file);

	fread(&imageLength, sizeof(unsigned int), 1, file);

	textureData = (byte*)malloc(imageLength);

	fread(textureData, sizeof(unsigned char) * imageLength, 1, file);

	fclose(file);

	//if (prepareOnLoads)
	//	Prepare();
}


struct TE_Header_t
{
	char texID[4];
	unsigned int textureMagic;
	bool textureAlpha;
	int textureWidth;
	int textureHeight;
	int textureChannels;
	unsigned int imageLength;
};


void CTextureObject::Save(CFileName& strFileName)
{
	FILE* file;

	file = fopen(strFileName.strFileName.c_str(), "wb");

	if (file == NULL) 
	{
		Warning("Cannot save texture %s\n", strFileName.strFileName.c_str());
		return;
	}

	unsigned int magic = TEXTURE_MAGIC;

	fwrite("TEXX",		sizeof(char) * 4,		1,file);
	fwrite(&magic,		sizeof(unsigned int),	1, file);

	fwrite(&isAlpha,	sizeof(bool),			1,file);

	fwrite(&width,		sizeof(int),			1,file);
	fwrite(&height,		sizeof(int),			1,file);
	fwrite(&channels,	sizeof(int),			1,file);

	unsigned int imageLength = width * height * channels;

	fwrite(&imageLength, sizeof(unsigned int), 1, file);
	
	fwrite(textureData,	sizeof(unsigned char) * imageLength, 1, file);
	fwrite("TEXD", sizeof(char) * 4, 1, file);

	fclose(file);

}


CTextureObject& CTextureObject::operator=(CTextureObject& texture)
{
	if (this != &texture) 
	{
		Clear();

		if (texture.textureData) 
		{
			this->channels = texture.channels;
			this->height = texture.height;
			this->width = texture.width;
			this->isAlpha = texture.isAlpha;
			this->texID = texture.texID;
			this->prepareOnLoads = texture.prepareOnLoads;
			this->renderingFlags = texture.renderingFlags;

			unsigned int imageLength = texture.channels * texture.height * texture.width;

			if (this->textureData != NULL)
				free(this->textureData);

			this->textureData = NULL;

			this->textureData = (unsigned char*)malloc(imageLength);

			memset(this->textureData, 0, imageLength);
			memcpy(this->textureData, texture.textureData, imageLength);
		}
	}
	return *this;
}

CTextureObject& CTextureObject::operator=(CTextureObject&& texture) noexcept 
{
	if (this != &texture)
	{
		Clear();
		textureData = texture.textureData;
		channels = texture.channels;
		height = texture.height;
		width = texture.width;
		isAlpha = texture.isAlpha;
		texID = texture.texID;
		prepareOnLoads = texture.prepareOnLoads;
		renderingFlags = texture.renderingFlags;

		texture.textureData = nullptr;
		texture.channels = 0;
		texture.height = 0;
		texture.width = 0;
		texture.isAlpha = false;
		texture.texID = 0;
		texture.prepareOnLoads = true;
		texture.renderingFlags = 0;
	}
	return *this;
}



void CTextureObject::Prepare() 
{
	GLenum format = GL_RGB;
	if (channels == 1)
		format = GL_RED;
	else if (channels == 3)
		format = GL_RGB;
	else if (channels == 4)
		format = GL_RGBA;
	else
		format = GL_RED;  //FIX ME: this illegal? Idk

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	// set the texture wrapping/filtering options (on the currently bound texture object)

	if (renderingFlags & TEXTURE_CLAMP)
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else 
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}


	if (renderingFlags & TEXTURE_CONST)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, (unsigned char*)textureData);

	if (renderingFlags & TEXTURE_CONST) return;
		
	glGenerateMipmap(GL_TEXTURE_2D);
}


CTextureObject* initDefaultTexture(HEXColor color)
{
	CTextureObject* teObject = new CTextureObject;

	teObject->height = 64;
	teObject->width = 64;
	teObject->channels = 3;
	teObject->isAlpha = false;
	teObject->textureData = (byte*)malloc(teObject->width * teObject->height * teObject->channels);

	if (teObject->textureData == nullptr) {
		Error("Cannot create main global texture for In the Engine!\n");
	}

	makeBitMapDefault(teObject->textureData, teObject->width, teObject->height, teObject->channels, color);

	return teObject;
}

void makeBitMapDefault(byte* pbitmap, uint width, uint height, short int channels, HEXColor color) 
{
	if (pbitmap == NULL) return;

	for(uint i = 0; i < (width * height * channels); i += channels)
	{
		pbitmap[  i  ] = byte( ( color & 0xFF000000UL ) >> 24 );
		pbitmap[i + 1] = byte( ( color & 0x00FF0000UL ) >> 16 );
		pbitmap[i + 2] = byte( ( color & 0x0000FF00UL ) >> 8  );
		if (channels == 4)
			pbitmap[i + 3] = byte( ( color & 0x000000FFUL ) >> 0 );
	}
}

