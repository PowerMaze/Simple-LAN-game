#pragma once


typedef struct simpleBox2D_s 
{
	unsigned int VBO; 
	unsigned int VAO;
	unsigned int EBO;
	unsigned int textureID;
}simpleBox2D_t;

ENGINE_API extern void simpleBoxInit(simpleBox2D_t* box2d);

extern simpleBox2D_t* simpleBoxInitTextured(void);
/*
* @brief Draw rectangle to screen
* 
* @param x - x coordinate of screen
* @param y - y coordinate of screen (0 - upper screen)
* @param width - width of shape
* @param height - height of shape
* @param HEXColor - color of shape
*/
ENGINE_API extern void drawSquare(uint x, uint y, uint width, uint height, HEXColor color);
/*
* @brief Draw texture to screen
* @param *te - pointer to texture object (can be NULL)
* @param *view - current viewport of screen
* @param x - x coordinate of screen
* @param y - y coordinate of screen (0 - upper screen)
* @param width - width of shape
* @param height - height of shape
* @param renderType - render texture (TEX_DEFAULT, TEX_ADD, TEX_INVERSE, TEX_TRANSLUCENT, TEX_TRANSPARENT)
* @param color - color of texture (defaults = white color)
*/
ENGINE_API extern void drawTextureToScreen(CTextureObject* te, CView* view, uint x, uint y, uint width, uint height, byte renderType, HEXColor color = COLOR_WHITE);

ENGINE_API extern void simpleBoxEnd(simpleBox2D_t* box2d);

/*
* Draw text to screen function
* 
* @param font - which font draw text
* @param uX - x coordinate of screen
* @param uY - y coordinate of screen (0 - top left corner)
* @param fScale - scale of font
* @param col - color of font
* @param strText - text to draw.
* 
*^0text - print 'text' in black color
* 
*^1text - print 'text' in white color
* 
*^2text - print 'text' in red color
* 
*^3text - print 'text' in yellow color
* 
*^4text - print 'text' in green color
* 
*^5text - print 'text' in cyan color
* 
*^6text - print 'text' in blue color
* 
*^7text - print 'text' in magenta color
* 
*^8text - print 'text' in original color (const HEXColor col)
* 
*/
ENGINE_API extern void drawText(CFont* font, uint uX, uint uY, float fScale, const HEXColor col, const char* strText);

ENGINE_API extern void drawCenteredText(CFont* font, CView* pview, uint uiY, float fScale, const HEXColor color, bool bAllowResolutionScreen, const char* strText);

ENGINE_API extern void drawSingleCharacterTest(CFont* font, const float fScale, const char c, const HEXColor col);

ENGINE_API extern void drawColorFull(HEXColor color);
//ENGINE_API void drawText(CFont* font, uint x, uint y, float scale, std::string &strText);

ENGINE_API extern simpleBox2D_t* drawingShapeBox;
ENGINE_API extern simpleBox2D_t* drawingTexturedShapeBox;