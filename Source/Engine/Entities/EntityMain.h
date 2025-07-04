#pragma once

#include <Engine/Common/FileName.h>
#include <Engine/Model/Model.h>
#include <Engine/BulletCollision/CollisionDispatch/btGhostObject.h>

/*
//////////////////////////////////////////////////////
******************************************************
Structure used fo generate entities in main Engine
******************************************************
//////////////////////////////////////////////////////
*/

/*
property field type for entity
*/
enum fieldType
{
	FIELD_INT,
	FIELD_UNSIGNED_INT,
	FIELD_BYTE,
	FIELD_SHORT,
	FIELD_FLOAT,
	FIELD_DOUBLE,
	FIELD_STRING,
	FIELD_TARGET,
	FIELD_COLOR,
	FIELD_FILENAME,
	FIELD_ENUM
};

/*
property field of entity
*/
struct propertyField
{
	char* strFieldName;
	enum fieldType type;
	char* value;
};

/*
structure for reading entities between game.dll and In the Editor
*/
typedef struct importEntity_s 
{
	unsigned long e_ID;

	std::string EntityName;

	std::string EntityClassName;

	HEXColor    color;

	int iHealth;

	bool bPhysics;

	CFileName fileModel;
	CFileName fileTexture;

	CModelObject*       m_pmo;
	CTextureObject*     m_pte;
	CBrush*             m_pbr;

	unsigned int flags;

	int numProperties;
	propertyField* properties;

}importEntity_t;

/*
damage type
*/
enum EDamageType 
{
	DMT_VOID,      // falls into void
	DMT_EXPLOSION, // explosion from rockets/grenades
	DMT_DROWING,   // drowing in water
	DMT_TELEGRAPH, // teleport
	DMT_BULLET,    // bullet
	DMT_BURNING,   // burning in fire or lava
	DMT_FREEZING,  // freeze in nitrogen bomb or cold water
	DMT_FALLING,   // jump from high building for example
	DMT_SQUISHED,  // squished by doors, platforms or stones
	DMT_POWERBALL, // from power ball
	DMT_NONE = 666 // none, so you cannot be harm
};

/*
touch event type
*/
enum ETouchType 
{
	TT_ITEM,          // adds value to player (armor)
	TT_PROJECTILE,    // removes value from player (takes damage)
};


typedef struct touchHandle_s
{	
	int th_iValue;                 // value of touch
	enum ETouchType th_touchType;  // type of touch
	enum EDamageType th_damageType;// if we have damage	

	CEntityMain* th_entityFrom;     // entity who generate touch
	CEntityMain* th_entityLaunched; // entity who launched projectile

}touchHandle_t;

enum ERenderEntityType 
{
	RET_NULL,   // used for nothing, entity is nothing
	RET_VOID,   // used for editor entites (triggers, markers, etc..), and virtual entities 
	RET_MODEL,  // used for model renderig (ammos, healths, enemies, etc..)
	RET_BRUSH,  // used for brush entites (doors, moving plats, destroyable arhitecture, etc..)
//	RET_PORTAL, // used for dynamic portal
//	RET_ROOM    // used for room entites, note: unsuported
};

enum EPhysicsCollisionType
{
	PCT_COL_BAD,     // error
	PCT_COL_DYNAMIC, // players, npc, projectiles, etc..
	PCT_COL_STATIC   // ammo, key, health, etc..
};

/*
class entity for game logic

Note: This basic class for all entities. This not be called in editor or game.dll.
For adding new classes uses this ierarchy

CEntityMain
    |____CEntityA
*/
class CEntityMain
{
public:

	// unique id of entity
	unsigned long m_ulID;

	// name of entity
	std::string m_strName;
	// name of class
	std::string m_strClassName;

	// position of entity
	Vector3D m_vPosition;
	// rotation of entity
	Angle3D  m_anglRotaion;

	// color of entity
	HEXColor m_color;

	// view matrix
	Matrix4 m_matModel;

	// health of entity (-1 - is invincible)
	int m_iHealth;	
	// has physics
	bool m_bPhysics;

	
	CModel        m_pmo;       // if this model
	//CTextureObject*     m_pte; // if this texture
	CBrush*       m_pbr; // if this brush
	unsigned int  m_iBrushIndex;

	CGameWorld*   m_pwo; // pointer to world

	unsigned int m_numProperties; // count of properties

	unsigned long m_ulFlags;      // spawn flags

	CFileName m_strModel;
	CFileName m_strTexture;

	bool m_bDead;
	// this used for items
	bool m_bCatch;

	unsigned int roomID;

public:
	// default constructor
	ENGINE_API CEntityMain(void);
	// default destructor
	ENGINE_API virtual ~CEntityMain();
	// release class entity
	ENGINE_API virtual void End();
	// same
	ENGINE_API void Clear();

	//ENGINE_API virtual void Copy(CEntityMain* e);
	// this can be used for load game
	ENGINE_API void Load(CFileStream& file);
	// this can be used for save game
	ENGINE_API void Save(CFileStream& file);
	// check if entity is valid
	ENGINE_API bool IsValid() {	return m_pwo != NULL; }
	// send event????
	ENGINE_API virtual void SendEvent(unsigned long cmd) {};
	// rotate entity? wtf
	ENGINE_API void RotateEntity(const float P, const float Y, const float R);
	// initialize entity class, can override
	ENGINE_API virtual void Initialize(void) {};
	// render view from entity, unused
//	ENGINE_API virtual void RenderFromEntity(CView* pvView) {};
	// precache component class
	ENGINE_API virtual void Precache(void) {};
	// gets touch event for entity
	ENGINE_API virtual void applyTouchEvent(touchHandle_t* handle) {};
	// generate touch event for entity
	ENGINE_API virtual void generateTouchEvent(touchHandle_t* handle) {};
	// gets entity target for triggers
	ENGINE_API virtual CEntityMain* getTarget(void) { return nullptr; }

	// main loop entity logic, call every frame tick on server
	ENGINE_API virtual void Update(void) {};

protected:

	// init as model, is visible in game or In the Editor (render mode)
	ENGINE_API void InitAsModel(void);
	// init as brush entity
	ENGINE_API void InitAsBrush(void);
	// init as editor entity. Visible only in In the Editor (edit mode)
	ENGINE_API void InitAsEditorEntity(void);

};

/*
///////////////////////////////////////////////////

Main abstract entity, which can move, can have gravity, has collision etc.

NOTE: this cannot be visible by In the Editor

///////////////////////////////////////////////////
*/
class CPhysicsEntity : public CEntityMain
{
public:

	float m_fAcceleration;
	float m_fDeAcceleration;

	float m_fSpeed;
	float m_fMassObject;

//	Vector3D vLastPose;
	unsigned int m_uiPhysicFlags;
	unsigned int m_uiPhysicType;

	Vector3D vCollisionStretch;

	// motion state of object
	btDefaultMotionState* m_motionState;
	// dynamic body
	btRigidBody* m_rigidBody;
	// transform object
	btTransform m_startTransform;
	// collision shape type
	btCollisionShape* m_boxShape;
	// ghost object for fast lookup
	btGhostObject* m_goObject;
	// ray callback
	//btCollisionWorld::ClosestRayResultCallback rayCallback;

public:

	ENGINE_API CPhysicsEntity(void);

	ENGINE_API ~CPhysicsEntity(void) {};

	ENGINE_API virtual void End(void);

	ENGINE_API void InitCollisionInfo(void);

	ENGINE_API void getMatrixOrientation(void);

	ENGINE_API Vector3D getCurrentPosition(void);

	ENGINE_API bool testIfEntityTouchesGround(void);

	// while we don't have a class for rayCast, we will use this
	ENGINE_API void imitateBulletHitScanDummy(const Vector3D &vFrom, const Vector3D &vTo);

	inline ENGINE_API Vector3D getLinearVel(void) 
	{	
		btVector3 v = m_rigidBody->getLinearVelocity();
		return VECTOR3(v.x(), v.y(), v.z());
	}

	inline ENGINE_API void setLinearVel(Vector3D& v)
	{
		btVector3 btv(v.x, v.y, v.z);
		m_rigidBody->setLinearVelocity(btv);
	}

	inline ENGINE_API void setGhostVel(const Vector3D& v)
	{
		assert(m_goObject != NULL);

		m_startTransform.setIdentity();
		m_startTransform.setOrigin(btVector3(v.x, v.y, v.z));
		m_goObject->setWorldTransform(m_startTransform);
	}

	inline ENGINE_API void applyImpulse(Vector3D& vImpulse)
	{
		btVector3 btv(vImpulse.x, vImpulse.y, vImpulse.z);
		m_rigidBody->applyCentralImpulse(btv);
	}

	inline ENGINE_API void activateEntityMovement(void) 
	{
		m_rigidBody->activate();
	}

	inline ENGINE_API void setPosition(const Vector3D &vPos) 
	{
		m_startTransform.setIdentity();
		m_startTransform.setOrigin(btVector3(vPos.x, vPos.y, vPos.z));

		m_rigidBody->setWorldTransform(m_startTransform);
	}

	ENGINE_API bool castRay(const Vector3D& from, const Vector3D& vPos, CEntityMain* entFilter);

//	ENGINE_API bool rayHit(CPhysicsEntity* entFilter);

	ENGINE_API virtual void Update() {};
};


ENGINE_API extern bool IsClass(CEntityMain* e, const char* strClass);

ENGINE_API extern inline bool IsSameEntity(CEntityMain* ea, CEntityMain* eb) 
{
	return ea == eb;
}

/*
ENGINE_API extern void RegisterModel(CModelObject* obj);
ENGINE_API extern void RegisterTexture(CTextureObject* te);
ENGINE_API extern void RegisterSound(CSound* snd);
ENGINE_API extern void RegisterEntity(CEntityMain* ent);
*/


