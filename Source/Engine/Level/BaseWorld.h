#pragma once

#include <Engine/Common/FileName.h>
#include <Engine/Common/FileIO.h>
#include <Engine/Level/Room.h>
#include <Engine/Level/Brush.h>

#include <Engine/Entities/EntityMain.h>
#include <Engine/Entities/CollisionDispatcher.h>

enum ERoomType
{
	R_BLOCK,
	R_CONUS,
	R_SPHERE,
	R_GEODOME,
	R_TERRAIN,
	R_ARC
};

#define MAX_ENTITIES 4096
#define MAX_ROOMS    1024
#define MAX_POLYGONS 0x7FFFFFFF

// needed by pack float to short
#define MAX_DISTANCE_IN_WORLD 2047.f
#define MIN_DISTANCE_IN_WORLD -2047.f

typedef struct Sun 
{
	float su_fSize;	

	Vector4D su_ColorAmbient;
	Vector4D su_ColorDiffuse;
	Vector4D su_ColorSpecular;

	Angle3D  su_aDirection;

	byte su_type;
}Sun_t;

ENGINE_API inline short packFloat(const float& f) 
{
	float fNormal = (f - MIN_DISTANCE_IN_WORLD) / (MAX_DISTANCE_IN_WORLD - MIN_DISTANCE_IN_WORLD);
	short s = short(fNormal * 32767.0f);
	return s;
}

ENGINE_API inline float unpackFloat(const short& s) 
{
	float fNormal = float(s) / 32767.0f;
	float fValue = MIN_DISTANCE_IN_WORLD + fNormal * (MAX_DISTANCE_IN_WORLD - MIN_DISTANCE_IN_WORLD);
	return fValue;
}

/*

@brief basic world of all game, includes brushes, rooms, dll entites and editor entites

*/
class CGameWorld
{
public:

	/*** world info (view on Game.dll, Power.exe, Engine.dll) ***/

	std::string gw_strName;     // name world (Beyound)
	std::string gw_description; // description (description in start)

	unsigned int gw_flags;      // spawn flags

	unsigned int gw_numRooms;   // count of rooms in the world
	unsigned int gw_numBrushes; // count of brushes in the world
	unsigned int gw_numEntities;// count of entites in the world

	unsigned int gw_nextEntityID; // entity ID
	unsigned int gw_nextRoomID;   // room ID

//	unsigned int gw_entityOffsetPos; // where start block of entities

	float gw_fGravity; // gravity in whole world (maybe CRoomBase have own gravity factors, idk)

	HEXColor gw_bckgColour; // backround color, if sky box not present

	std::vector < CEntityMain* > gw_allEntities;            // all entities in world, included deleted (used for game)
	std::vector < importEntity_t* > gw_allImportedEntities; // all entities in world (used for editor)

	std::vector < CRoomBase > gw_rooms; // all rooms in world
	std::vector < CBrush *> gw_brushes; // all brushes in world

	//Light_t* gw_lightSources;

	CSkyBox* skybox;      // skybox (this can be nullptr)

	Sun_t* gw_Sun;        // if sun light is avaliable

	bool gw_openInEditor; // sets true in editor, but always false in game

public:

	/** World physics and collision detection [Bullet engine] (View only for Engine.dll) **/
	/** This used only on server side **/

//	CPhysicsEntity                   *gw_entityWorld;
	CEntityMain                      *gw_entityWorld;

	btDefaultCollisionConfiguration  *gw_CollisionConfiguration;
	btCollisionDispatcher            *gw_ColDispatcher;
	btDiscreteDynamicsWorld          *gw_DynamicsWorld;

	btDbvtBroadphase                 *gw_Broadphase;
	btSequentialImpulseConstraintSolver *gw_Csolver;

	btTriangleMesh* gw_WorldTriangleMesh;


	btBvhTriangleMeshShape* gw_LevelShape;

	/* this used only for world collision, static rooms and brushes */
	btRigidBody* gw_LevelBody;

	std::vector<btRigidBody*> gw_allPhysicsObjects;
	
	//btRigidBody* gw_player;

	class CCollisionDispatcher gw_collisionDispatcher;
	

public:
	// default constructor
	ENGINE_API CGameWorld();
	// constructor from file name
	ENGINE_API CGameWorld(CFileName& strFileName);
	// destructor
	ENGINE_API ~CGameWorld();
	// load entire world (used only in editor)
	ENGINE_API void Load(CFileName& strFileName);
	// load entire world but without entites (used only in game)                                
	ENGINE_API void LoadWithoutEntities(CFileStream& file);
	// save world (Editor only!)
	ENGINE_API void Save(CFileName& strFileName);
	// save world (Game save only!)
	ENGINE_API void SaveWithoutEntities(CFileStream& file);
	// set up physics and collisions
	ENGINE_API void SetUpPhysics(void);
	// release physics
	ENGINE_API void EndPysics(void);
	// destroys one entity from list
	void deleteOneEntity(CEntityMain* entity);
	// clear world
	ENGINE_API void Clear(void);

	ENGINE_API void UpdatePhysics(double fTime);

	ENGINE_API inline const char* getName() { return gw_strName.c_str();}
};
