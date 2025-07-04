#pragma once

#include <Engine/EMain.h>

#include "Entities/Bullet.h"
#include "Entities/Light.h"
#include "Entities/PhysicsEntity.h"
#include "Entities/Projectile.h"
#include "Entities/PushableModel.h"
#include "Entities/TextAlert.h"
#include "Entities/Health.h"
#include "Entities/Weapon.h"
#include "Entities/PlayerSpawn.h"
#include "Entities/Explosion.h"
#include "Entities/Zone.h"
#include "Entities/Armor.h"

#include "GPlayer.h"

#pragma pack(push, 1)
typedef struct CL_Entity 
{	
	unsigned int modelCRC;
	unsigned int textureCRC;
	float fVpos[3];
	unsigned char pitch;
	byte colorIndex;
}CL_Entity_t;
#pragma pack(pop)


extern HEXColor playerColorTables[8];


class CEntityManager
{
public:

	std::vector< CEntityMain* > em_allServerEntities;
	std::vector< CEntityMain* > em_toDeleteEntities;

	std::vector< CEntityMain*> em_allSpawners;

//	CPlayer* em_Players;
//		std::vector< EntityClient_t* > cm_allClientEntities;

public:

		// basic interface
	CEntityManager();
	~CEntityManager() { Clear(); };

	void Clear();

		// server interface
	void parseEntities(CGameWorld* wo, CFileStream& stream);

//		void precacheEntities(void);

		//void addEntity(const char* entity);

	void addEntityToDelete(CEntityMain* ent);

	void deleteKilledEntites(void);

	void removeEntity(CEntityMain* ent);

	void spawnProjectile(enum EProjectileType type, Vector3D vPos, Angle3D aRot, CEntityMain* pWhoLaunced);

	POWER_INLINE void addSpawner(CEntityMain* eSpawn) 
	{
		em_allSpawners.push_back(eSpawn);
	}

	void spawnPlayerFromSpawner(class CPlayer* player);
};
	
/*
	// parse entites from world file ".wo"
	void parseEntities(CGameWorld* wo, CFileStream& stream);
	// spawn projectile to world (from gun)
	void spawnProjectile(CGameWorld* wo, enum EProjectileType type, Vector3D vPos, Angle3D aRot);

	void deleteOneEntity(CGameWorld* wo, CEntityMain* eMain);
*/

extern class CEntityManager entityManager;