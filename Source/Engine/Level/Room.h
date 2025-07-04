#pragma once

#include <Engine\Common\FileName.h>
#include <Engine\Graphics\Texture.h>
#include <Engine\Graphics\Shader.h>


class CPortalRoom
{
public:
	unsigned int portalID;

	double origin;

};

/* Basic class vertex for room/brush rendering and collision */
class CVertex
{
public:
	/* for colission detect */
	glm::vec3 positionInWorld;
	/* for rendering room/brush */
	glm::vec3 positionInRoomOrBrush;
public:
	// default constructor
	CVertex();
	// load vertex data
	void ReadData(CFileStream& file);
	// save vertex data
	void WriteData(CFileStream& file);

};

/* Basic class for room and brush polygon */
class CPolygonWorld
{
public:

	unsigned long pr_flags; // physhics/rendering flags	
	unsigned long tag;      // tag for fast search

	std::vector<CVertex>verticies;    // verticies of polygon	
	std::vector<glm::vec2>textureUVs; // texture uv for polygon

	Vector3D vNormal;         // normal plane

	CTextureObject texture01; // base texture
	CFileName tex01FileName;  // base texture filename

	bool renderPrepared;      // check if prepared polygon
	bool notRender;           // check if render polygon

	CRoomBase* pr_room; // pointer to room
	CBrush*    pr_brush;   // pointer to brush

//	CTextureObject texture02; // texture layer 2
//	CFileName tex02FileName;  // texture layer 2 filename

//	CTextureObject texture03; // texture layer 3
//	CFileName tex03FileName;  // texture layer 3 filename

//	CTextureObject texture04; // texture layer 4
//	CFileName tex04FileName;  // texture layer 4 filename

	HEXColor pr_color; // texture color

public:
	// default constructor
	ENGINE_API CPolygonWorld();
	//constructor from verticies data
//	ENGINE_API CPolygonWorld(float* data);

	// write to file
	ENGINE_API void Save(CFileStream& file);
	// read from file
	ENGINE_API void Load(CFileStream& file);
	// build polygon data for rendering
	ENGINE_API void processData(void);
	// setup polygon for rendering
	ENGINE_API void Prepare(void);
	// render polygon
	ENGINE_API void Render(CShader &sha);
	// clear polygon object
	ENGINE_API void Clear(void);
	// copy polygon world
	ENGINE_API CPolygonWorld& operator=(CPolygonWorld& newFilename);

	ENGINE_API CGameWorld* getWorld(void);

private:
	// OpenGL stuff
	uint VAO, VBO, EBO;
	// vertex data (position, uv, normals)
	std::vector <float> RenderData;
	// indices of vertex data
	std::vector<unsigned int> indices;
};

/* Main class of room structure used for fast rendering (maybe call sector) */
class CRoomBase
{
public:
	unsigned long uRoomID;

	std::vector<CPolygonWorld> ro_polygons;

	CGameWorld* ro_world; // game world pointer
	//	std::vector<CVertex> ro_verticies;

	//	CShader shader;

		//std::vector <CEntity*> ro_entities;
		//std::vector <CLight> ro_lights;

		//std::vector<CPortalRoom> ro_portals;

	//	double origin[3];
public:
	ENGINE_API CRoomBase();


	ENGINE_API void RenderRoom(CShader &sha);
	ENGINE_API void Clear();


	ENGINE_API void Save(CFileStream& file);
	ENGINE_API void Load(CFileStream& file);

};


/*
General function to create room...
@param room - room to create
@param height - height of room
@param width - width of room
@param length - length of room
@param dimensions - dimensions, 3 - triangle, 4 - cube, 5 - pentagon, etc..
@param vPos - position of room in the world
*/
extern ENGINE_API void addRoom(CRoomBase* room,float height, float width, float length, int dimensions, unsigned long ulRoomTag, const glm::vec3& vPos);
// h - y, w - z, l - x
ENGINE_API extern void addPolygon(CRoomBase* room, float height, float width, float length);
//ENGINE_API extern CRoomBase* createRoomFromObj(COBJDecoder& obj, glm::mat4& mat_transform);

//ENGINE_API extern void connectRooms(CRoomBase* roomA, const CRoomBase* roomB, CPortalRoom& portal);

//ENGINE_API extern void cutPolygon(CPolygonWorld* room, float fline);
