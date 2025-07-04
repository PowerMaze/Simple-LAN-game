#pragma once

#include "Room.h"

class CBrush
{
public:

	//unsigned int uiIndex;

	unsigned int m_uFlags;
	unsigned int m_uIndexRoom;
	unsigned int m_uPolygonsCount;
	bool m_bPrepared;
	//bool m_bEmptyBrush;

	std::vector<CPolygonWorld* > m_brushPolygons;

	CGameWorld* m_world;

	//CRoomBase* room;

public:

	ENGINE_API CBrush(void);
	ENGINE_API CBrush(CBrush& brushOther);
	ENGINE_API ~CBrush(void);

	ENGINE_API void Clear(void);

	ENGINE_API void Load(CFileStream& fileStream);
	ENGINE_API void Save(CFileStream& fileStream);

	ENGINE_API void createFromOBJ(CFileOBJ& obj);
	//void createFromMO(CModelObject* mo);

	void CSG_add(CBrush* brush);
	void CSG_sub(CBrush* brush);
	void CSG_div(CBrush* brush);
	void CSG_cross(CBrush* brush);

	void Prepare(void);
	void Render(CShader& sha);

};

/*
/////////////////////////////////////////////////////////
Create default brush cube

@param height - height of brush
@param width  - width of brush
@param length - length of brush
@param texture - texture of brush sides

@returns Brush object

/////////////////////////////////////////////////////////
*/
ENGINE_API extern CBrush* createBrushCube(const float height, const float width, const float length, const glm::vec3 &vPos, CTextureObject* defaultTexture);
CBrush* createBrushCylinder(const float height, const float radius, const int numSides, CTextureObject* defaultTexture);
CBrush* createBrush(const float height, int numSides, CTextureObject* defaultTexture);

