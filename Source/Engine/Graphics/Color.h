#pragma once

//#include <Engine/Common/DataTypes.h>
//#include <Engine/EMain.h>

// basic colors in hex
#define COLOR_BLACK     0x000000FFUL
#define COLOR_WHITE     0xFFFFFFFFUL
#define COLOR_RED       0xFF0000FFUL
#define COLOR_GREEN     0x00FF00FFUL
#define COLOR_BLUE      0x0000FFFFUL
#define COLOR_YELLOW    0xFFFF00FFUL
#define COLOR_MAGENTA   0xFF00FFFFUL
#define COLOR_CYAN      0x00FFFFFFUL
#define COLOR_GRAY      0x808080FFUL

// basic color in gl vector 4
#define GL_COLOR_BLACK   VECTOR4(0.0f, 0.0f, 0.0f, 1.0f)
#define GL_COLOR_WHITE   VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)
#define GL_COLOR_RED     VECTOR4(1.0f, 0.0f, 0.0f, 1.0f)
#define GL_COLOR_GREEN   VECTOR4(0.0f, 1.0f, 0.0f, 1.0f)
#define GL_COLOR_BLUE    VECTOR4(0.0f, 0.0f, 1.0f, 1.0f)
#define GL_COLOR_YELLOW  VECTOR4(1.0f, 1.0f, 0.0f, 1.0f)
#define GL_COLOR_MAGENTA VECTOR4(1.0f, 0.0f, 1.0f, 1.0f)
#define GL_COLOR_CYAN    VECTOR4(0.0f, 1.0f, 1.0f, 1.0f)
#define GL_COLOR_GRAY    VECTOR4(0.5f, 0.5f, 0.5f, 1.0f)

#define MASK_ALPHA 0xFFFFFF00UL

// adds alpha component to hex color
#define COLOR_ADD_ALPHA(col, alpha) (( col & MASK_ALPHA ) | (alpha))
// removes alpha component from hex color
#define COLOR_REMOVE_ALPHA(col) (( col & MASK_ALPHA ) | ( 0x000000FFUL))

// converts rgb to hex color
inline HEXColor RGB_toHEX(byte r, byte g, byte b) 
{
	return (r << 24) | (g << 16) | (b << 8) | 0xFF;
}
// converts rgba to hex color
inline HEXColor RGBA_toHEX(byte r, byte g, byte b, byte a) 
{
	return (r << 24) | (g << 16) | (b << 8) | (a << 0);
}
// converts hex color to rgba
inline void HEX_toRGBA(byte* r, byte* g, byte* b, byte* a, const HEXColor color_in)
{
	*r = (color_in & 0xFF000000UL) >> 24;
	*g = (color_in & 0x00FF0000UL) >> 16;
	*b = (color_in & 0x0000FF00UL) >> 8;
	*a = (color_in & 0x000000FFUL) >> 0;
}
// inverses hex color
inline void HEX_InverseColor(HEXColor* color)
{
	*color ^= 0xFFFFFFFFUL;
}
// inverses rgba color
inline void RGBA_InverseColor(byte* r, byte* g, byte* b, byte *a) 
{
	HEXColor col = RGBA_toHEX(*r, *g, *b, *a);
	HEX_InverseColor(&col);
	HEX_toRGBA(r, g, b, a, col);
}
// converts rgba to OpenGL color (vector 4D)
inline glm::vec4 RGBA_toOGL(byte r, byte g, byte b, byte a)
{
	return glm::vec4
	(
		float(r) / 255.0f,
		float(g) / 255.0f,
		float(b) / 255.0f,
		float(a) / 255.0f
	);
}
// converts hex color to OpenGL color (vector 4D)
inline glm::vec4 HEX_toOGL(const HEXColor color) 
{
	byte r, g, b, a;

	HEX_toRGBA(&r, &g, &b, &a, color);

	return RGBA_toOGL(r, g, b, a);
}
// converts OpenGL color to rgb color
inline void OGL_toRGB(byte* r, byte* g, byte* b, glm::vec4 colorgl)
{
	*r = (byte)(colorgl.x * 255.0f);
	*g = (byte)(colorgl.y * 255.0f);
	*b = (byte)(colorgl.z * 255.0f);
}
// converts OpenGL color to rgba color
inline void OGL_toRGBA(byte* r, byte* g, byte* b, byte* a, glm::vec4 colorgl)
{
	*r = (byte)(colorgl.x * 255.0f);
	*g = (byte)(colorgl.y * 255.0f);
	*b = (byte)(colorgl.z * 255.0f);
	*a = (byte)(colorgl.w * 255.0f);
}

inline HEXColor OGL_toHEX(const glm::vec4& colorgl)
{
	byte r, g, b, a;
	OGL_toRGBA(&r, &g, &b, &a, colorgl);
	return RGBA_toHEX(r, g, b, a);
}

inline HEXColor RGBA_to_ABGR(HEXColor col) { return _swap32(col); }
