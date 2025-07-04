#pragma once

//#include <Engine/Common/FileName.h>

#define TEXTURE_MAGIC 0x20CU

/*
////////////////////////////////////////////////////////////////

In the Engine texture format. Uses raw bitmap data of image.

All pixel bits writes in order R (Red) G (Green) B (Blue) and
if image has alpha channel A (Alpha)

Textures also contains animations

////////////////////////////////////////////////////////////////
*/
class ENGINE_API CTextureObject 
{
public:
	int width; // texture width in pixels
	int height; // texture height in pixels
	int channels; // channels in texture 3 - RGB, 4 - RGBA

	bool isAlpha; // check if this alpha

	unsigned int texID; // texture id for OpenGL attach texture

	byte* textureData;  // bitmap array of image
	bool prepareOnLoads; // always prepare texture when loaded

	unsigned long renderingFlags; // rendering flags texture

	unsigned int texCRC;

public:
	// default constructor
	CTextureObject(void);
	// destructor
	~CTextureObject(void);
	// constructor from filename
	CTextureObject(CFileName& strFileName);
	// constructor from char*
	CTextureObject(const char* strFileName);
	// constructor from texture
	CTextureObject(const CTextureObject& texture);
	// constructof of move
	CTextureObject(CTextureObject&& texture) noexcept;
	// loaded from image
	void loadFromImage(CFileName& strFileName);

	// load file
	void Load(CFileName& strFileName);
	// save file
	void Save(CFileName& strFileName);
	// prepare texture for rendering
	void Prepare(void);
	// clear object
	void Clear();

	CTextureObject& operator=(CTextureObject& texture);
	CTextureObject& operator=(CTextureObject&& texture) noexcept;

};

typedef struct animationLump_s
{
	unsigned int uiStart; //offset to start frame
	unsigned int uiEnd;   //offset to end frame

	float fTimeAnimationSpeed; // animation speed sec/frames
	float fAnimTimeCurrent;
}animationLump_t;


class ENGINE_API CTexture
{
public:

	// texture data
	CTextureObject* te_toData;
	// anim control
	animationLump_t te_animData;
	// frames list
	//std::map<std::string, modelLump_t> te_AnimationInfos;
	// current animation
	const char* te_strAnimNameCurrent;
	// current frame
	int currentFrame;

public:

	CTexture(void);

	~CTexture(void);
	void Clear(void);

	void PlayAnimation(const char* strAnimName);

	void StopAnimation(const char* strAnimName);

	unsigned int getTextureID(void);

	void SetTexture(CTextureObject* obj);

	void Load(const CFileName& strFileName);

};


/* global texture for model rendering without texture loaded on disk */
ENGINE_API extern CTextureObject* teDefaultTextureForModels;

/* create standard texture and fill full color */
extern CTextureObject* initDefaultTexture(HEXColor color);

/* make bitmap */
extern void makeBitMapDefault(byte* pbitmap, uint width, uint height, short int channels, HEXColor color);