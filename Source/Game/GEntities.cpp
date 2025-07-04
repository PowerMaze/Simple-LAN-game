#include "stdAfx.h"

#include "GEntities.h"
//#include "Entities/Entities.h"
//#include "Entities/TextAlert.h"
#include "GameSession.h"

#define CHECK_ENTITY_ADD 1


HEXColor playerColorTables[8] =
{
	COLOR_WHITE,
	COLOR_BLACK,
	COLOR_RED,
	COLOR_YELLOW,
	COLOR_GREEN,
	COLOR_CYAN,
	COLOR_BLUE,
	COLOR_MAGENTA,
};

static void getEntityInfo() 
{
	PrintF("CEntityManager info:\n");
	PrintF("Count all entities in world: %d\n", entityManager.em_allServerEntities.size());
	FOREACH_ARRAY(entityManager.em_allServerEntities.size()) 
	{
		PrintF(" Entity [%d], classname = %s\n", i, entityManager.em_allServerEntities[i]->m_strClassName.c_str());
	}
}


CEntityManager::CEntityManager()
{	
	srand(time(NULL));

//	em_Players = nullptr;
	pConsoleDataBase->addConsoleFunction("getEntityInfo()", getEntityInfo);
}

void CEntityManager::addEntityToDelete(CEntityMain* ent) 
{
	em_toDeleteEntities.push_back(ent);
}

void CEntityManager::parseEntities(CGameWorld* wo, CFileStream& stream) 
{
	Clear();

	if (!stream.checkID("ENTS")) 
	{
		Throw("World: %s does not have entities\n", wo->getName());
	}

		
	assert(wo->gw_entityWorld != NULL);
	em_allServerEntities.push_back(wo->gw_entityWorld);

		/****** parse entities from world file ******/

	while (!stream.checkID_Safe("EEND"))
	{
		std::string className = stream.ReadStringFromFile();

		if (className == "TextAlert")
		{
			CTextAlert* e = New<CTextAlert>();
			e->Load(stream);
			e->m_strClassName = className;
			e->m_pwo = wo;

			em_allServerEntities.push_back(e);

//			PrintF("Here\n");
		}
		else if (className == "Health") 
		{
			CHealth* hp = new CHealth;

			hp->Load(stream);				
			hp->m_pwo = wo;
			hp->m_strClassName = className;
			hp->Initialize();

			em_allServerEntities.push_back(hp);
		}
		else if (className == "Armor")
		{
			CArmor* ar = new CArmor;

			ar->Load(stream);
			ar->m_pwo = wo;
			ar->m_strClassName = className;
			ar->Initialize();

			em_allServerEntities.push_back(ar);
		}
		else if (className == "WeaponItem")
		{
			CWeaponItem* wp = new CWeaponItem;

			wp->Load(stream);
			wp->m_pwo = wo;
			wp->m_strClassName = className;
			wp->Initialize();

			em_allServerEntities.push_back(wp);
		}
		else if (className == "PlayerSpawn") 
		{
			CPlayerSpawn* sp = new CPlayerSpawn;
			sp->Load(stream);
			sp->m_pwo = wo;
			sp->m_strClassName = className;
			sp->Initialize();
			em_allServerEntities.push_back(sp);
		}
		else if (className == "ZoneHurt")
		{
			CZoneHurt* zh = new CZoneHurt;
			zh->Load(stream);
			zh->m_pwo = wo;
			zh->m_strClassName = className;
			zh->Initialize();
			em_allServerEntities.push_back(zh);
		}
		else
		{
			Throw("Entity class \"%s\" not found in game library (.dll)", className.c_str());
		}
	}
}

void CEntityManager::removeEntity(CEntityMain* ent)
{
	auto toDelete = std::find(em_allServerEntities.begin(), em_allServerEntities.end(), ent);

	if (toDelete != em_allServerEntities.end())
	{
		unsigned int index = toDelete - em_allServerEntities.begin();

			// delete entity
		Delete(  em_allServerEntities[index]);
		em_allServerEntities[index] = NULL;

		em_allServerEntities.erase(em_allServerEntities.begin() + index);
	}
}

void CEntityManager::spawnPlayerFromSpawner(CPlayer* player) 
{
	assert(player != NULL);

	int length = em_allSpawners.size();
	/*
	* Note: its not good use rand() function games, expecially in network (LAN/Internet), but com'on,
	* this game doesn't have predition for clients players, all logic make server. So basically client its just
	* camera with buttons, so we can use this trash named rand()
	*/ 
	int whereToSpawn =  rand() % length;

	//int dummy = rand() % length;
    PrintF("Dummy spawn - %d\n", whereToSpawn);

	//assert(whereToSpawn >= length);

	((CPlayerSpawn&)*em_allSpawners[whereToSpawn]).SpawnPlayer(player);
}

void CEntityManager::Clear() 
{
	deleteKilledEntites();

//	em_Players = nullptr;

	FOREACH_ARRAY(em_allSpawners.size()) 
	{
		em_allSpawners[i] = NULL;
	}

	em_allSpawners.clear();

	FOREACH_ARRAY(em_allServerEntities.size())
	{
		if (em_allServerEntities[i]) {
			em_allServerEntities[i]->End();
			Delete(em_allServerEntities[i]);
				//delete em_allServerEntities[i];
		}
	}
	em_allServerEntities.clear();
}

void CEntityManager::spawnProjectile(enum EProjectileType type, Vector3D vPos, Angle3D aRot, CEntityMain* pWhoLaunced)
{
		/*
		CPushableModel* pushmodel = New<CPushableModel>();

		pushmodel->m_vPosition = vPos;
		pushmodel->m_pwo = gameSession.getWorld();
		pushmodel->m_anglRotaion = aRot;

		pushmodel->Initialize();
		pushmodel->m_pmo.getModel()->PlayAnimation("ANIM_MORPH");

		em_allServerEntities.push_back(pushmodel);
		*/

	CProjectile* proj = new CProjectile;
	proj->m_vPosition = vPos;
	proj->m_pwo = gameSession.getWorld();
	proj->m_anglRotaion = aRot;
	proj->m_eptType = type;
	proj->m_whoLaunched = pWhoLaunced;
	proj->Initialize();
	em_allServerEntities.push_back(proj);

#if CHECK_ENTITY_ADD
	PrintF("count entites: %d\n", em_allServerEntities.size());
#endif

}
	
void CEntityManager::deleteKilledEntites(void)
{
	FOREACH_ARRAY(em_toDeleteEntities.size())
	{
		removeEntity(em_toDeleteEntities[i]);
	}

	em_toDeleteEntities.clear();
}
/*
	void parseEntities(CGameWorld* wo, CFileStream& stream)
	{

		if (!stream.checkID("ENTS")) {
			throw "World does entities\n";
		}

		/****** parse entities from world file ******/
	/*
		while (!stream.checkID_Safe("EEND"))
		{

			std::string className = stream.ReadStringFromFile();

			if (className == "TextAlert")
			{
				CTextAlert* e = new CTextAlert;
				e->Load(stream);
				e->m_strClassName = className;
				e->m_pwo = wo;
				wo->gw_allEntities.push_back(e);
			}
			else
			{
				Throw("Class \"%s\" not found in game library (.dll)", className.c_str());
			}
		}

	}

	void spawnProjectile(CGameWorld* wo, enum EProjectileType type, Vector3D vPos, Angle3D aRot) 
	{
		if ( wo == NULL ) return;


		CPushableModel* pushmodel = new CPushableModel;

		pushmodel->m_vPosition = vPos;
		pushmodel->m_pwo = wo;
		pushmodel->m_anglRotaion = aRot;

		pushmodel->Initialize();

		wo->gw_allEntities.push_back(pushmodel);
		
		/*CProjectile* projectile = new CProjectile;

		projectile->m_eptType = type;
		projectile->m_vPosition = vPos;
		projectile->m_anglRotaion = aRot;
		projectile->m_pwo = wo;

		projectile->Initialize();		

		wo->gw_allEntities.push_back(projectile);
		
		PrintF("count entites: %d\n", wo->gw_allEntities.size());
	}

	void deleteOneEntity(CGameWorld* wo, CEntityMain* eMain)
	{
		if (wo == NULL) return;

		auto toDelete = std::find(wo->gw_allEntities.begin(), wo->gw_allEntities.end(), eMain);

		if (toDelete != wo->gw_allEntities.end())
		{
			unsigned int index = toDelete - wo->gw_allEntities.begin();

			// delete entity
			delete  wo->gw_allEntities[index];
			wo->gw_allEntities[index] = NULL;

			wo->gw_allEntities.erase(wo->gw_allEntities.begin() + index);
		}
	}
*/






