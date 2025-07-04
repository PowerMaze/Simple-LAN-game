#include "stdAfx.h"

#include "GWeapon.h"
#include "GEntities.h"
#include "GPlayer.h"
#include "GPlayerClient.h"

static Vector2D afShotGunBrass[7] =
{
	Vector2D(-0.375,-0.6875), Vector2D(0.375,-0.6875),
	
	Vector2D(-0.75,0), Vector2D(0.16f,0), Vector2D(0.75,0),
	
	Vector2D(0.375,-0.6875), Vector2D(0.375,0.6875),
};

CWeapons::CWeapons()
{
	wp_currentWeapon = WEAPON_ZERO;

	wp_weaponState   = WEAPON_STATE_READY;

	wp_player   = nullptr;
	wp_clPlayer = nullptr;
	wp_fWeaponBanking = 0.0f;
	wp_vWeaponBanking = glm::vec2(0.f);

	memset(&wp_ammo, 0, sizeof(AmmoWeapon_t));

//	wp_ammo.m_ARbullets = MAX_BULLETS;
//	wp_ammo.m_rockets = 50;
//	wp_ammo.m_cells = 100;
//	wp_ammo.m_shells = 40;

	wp_time = 0;
	wp_timeStarted = 0;

//	memset(wp_aWeaponsAmmo, 0, MAX_WEAPONS * sizeof(int));
}


CWeapons::~CWeapons() 
{
	wp_modelWeapon.Clear();

	wp_player   = nullptr;
	wp_clPlayer = nullptr;

//	memset(aWeaponsAmmo, 0, MAX_WEAPONS * sizeof(int));
}

void CWeapons::selectWeapon(unsigned int iweapon) 
{
	CFileName strModelFile;
	CFileName strTextureFile;

	switch (iweapon) 
	{

	case WEAPON_MELE:

		strModelFile   = "Data\\Models\\Weapons\\Mele\\Saw.mo";
		strTextureFile = "Data\\Models\\Weapons\\Mele\\Saw.te";

		break;
	case WEAPON_AR:

		strModelFile   = "Data\\Models\\Weapons\\AR\\Body.mo";
		strTextureFile = "Data\\Models\\Weapons\\AR\\Body.te";

		break;
	case WEAPON_SHOTGUN:

		strModelFile   = "Data\\Models\\Weapons\\Q3ShotGun.mo";
		strTextureFile = "Data\\Models\\Weapons\\Q3ShotGun.te";
		break;

	case WEAPON_ROCKETGUN:

		strModelFile   = "Data\\Models\\Weapons\\RocketGun\\q3Rocket.mo";
		strTextureFile = "Data\\Models\\Weapons\\RocketGun\\rocketl.te";
		
		break;
	case WEAPON_ENERGYGUN:

		strModelFile   = "Data\\Models\\Weapons\\EnergyRifle\\EnergyRifle.mo";
		strTextureFile = "Data\\Models\\Weapons\\EnergyRifle\\Body.te";

		break;
	default:

		PrintF("Cannot get model info: %d\n", iweapon);


		strModelFile = "Data\\Models\\Weapons\\Q3ShotGun.mo";
		strTextureFile = "Data\\Models\\Weapons\\Q3ShotGun.te";

		break;
	}

	wp_modelWeapon.Clear();

	wp_modelWeapon.Load(strModelFile);
	FileSetGlobalPatch(strTextureFile);
	wp_modelWeapon.SetTexture(strTextureFile);

	wp_modelWeapon.SetUpModel();

	wp_currentWeapon = iweapon;

	wp_modelWeapon.m_uiRenderingFlags |= MODEL_WEAPON;
}

void CWeapons::Precache(unsigned int iWeaponMask)
{

	CFileName strTexDummy;

	if (iWeaponMask & WEAPON_BIT(WEAPON_MELE))
	{
		pDataBase->loadModel("Data\\Models\\Weapons\\Mele\\Saw.mo");
		strTexDummy = "Data\\Models\\Weapons\\Mele\\Saw.te";
		FileSetGlobalPatch(strTexDummy);
		pDataBase->loadTexture(strTexDummy);
	}

	if (iWeaponMask & WEAPON_BIT(WEAPON_AR)) 
	{
		pDataBase->loadModel("Data\\Models\\Weapons\\AR\\Body.mo");
		strTexDummy = "Data\\Models\\Weapons\\AR\\Body.te";
		FileSetGlobalPatch(strTexDummy);
		pDataBase->loadTexture(strTexDummy);


	}
	if (iWeaponMask & WEAPON_BIT(WEAPON_ENERGYGUN))
	{	
		pDataBase->loadModel("Data\\Models\\Weapons\\EnergyRifle\\EnergyRifle.mo");
		strTexDummy = "Data\\Models\\Weapons\\EnergyRifle\\Body.te";
		FileSetGlobalPatch(strTexDummy);
		pDataBase->loadTexture(strTexDummy);
	}

	if (iWeaponMask & WEAPON_BIT(WEAPON_SHOTGUN))
	{
		pDataBase->loadModel("Data\\Models\\Weapons\\Q3ShotGun.mo");
		strTexDummy = "Data\\Models\\Weapons\\Q3ShotGun.te";
		FileSetGlobalPatch(strTexDummy);
		pDataBase->loadTexture(strTexDummy);
	}
	if (iWeaponMask & WEAPON_BIT(WEAPON_ROCKETGUN))
	{
		// precache main model and texture
		pDataBase->loadModel("Data\\Models\\Weapons\\RocketGun\\q3Rocket.mo");
		strTexDummy = "Data\\Models\\Weapons\\RocketGun\\rocketl.te";
		FileSetGlobalPatch(strTexDummy);
		pDataBase->loadTexture(strTexDummy);

		// precache projectile

		pDataBase->loadModel("Data\\Models\\Weapons\\EnergyRifle\\Projectile\\Cube.mo");
		strTexDummy = "Data\\Models\\Weapons\\EnergyRifle\\Projectile\\Cube.te";
		FileSetGlobalPatch(strTexDummy);
		pDataBase->loadTexture(strTexDummy);

	}
}

unsigned int CWeapons::nextWeapon(unsigned int& uiWeaponMask)
{
	// wait after shoot from weapon
	if (wp_timeStarted > (SDL_GetTicks64() * 0.001)) return wp_currentWeapon;

	int i = wp_currentWeapon + 1;

	while (1)
	{
		if (WEAPON_BIT(i) & uiWeaponMask) 
		{
	
			wp_currentWeapon = i;
			if (getAmmoByWeapon(wp_currentWeapon) > 0) return wp_currentWeapon;
		}

		if (i == WEAPON_LAST) {
			i = WEAPON_ZERO;
		}

		i++;
	}
}

unsigned int CWeapons::prevWeapon(unsigned int& uiWeaponMask)
{

	if (wp_timeStarted > (SDL_GetTicks64() * 0.001)) return wp_currentWeapon;

	int i = wp_currentWeapon - 1;

	while (1)
	{
		if (WEAPON_BIT(i) & uiWeaponMask)
		{
			//PrintF("wp_currentWeapon = %d\n", wp_currentWeapon);
			wp_currentWeapon = i;
			if (getAmmoByWeapon(wp_currentWeapon) > 0)	return wp_currentWeapon;
		}

		if (i <= WEAPON_ZERO) {
			i = WEAPON_LAST;
		}

		i--;
	}
}


void CWeapons::renderWeapon(void)
{
	CView ViewPort;
	Vector3D vPosView;
	Matrix4 mat;

	// setup view for weapon
	ViewPort.setUpView(pViewPort->GetWidth(), pViewPort->GetHeight(), weaponPositions[wp_currentWeapon].fClip, 2048.f, weaponPositions[wp_currentWeapon].fFOV);
	// attach to camera
	ViewPort.m_viewMatrix = glm::mat4(1.0f);
	ViewPort.m_ViewDir = pViewPort->m_ViewDir;

	// get pos from weapon info
	vPosView = VECTOR3(weaponPositions[wp_currentWeapon].vPos[0], weaponPositions[wp_currentWeapon].vPos[1], weaponPositions[wp_currentWeapon].vPos[2]);

	// weapon iddle
	float fFrac = sinf(SDL_GetTicks64() * 0.001);
	vPosView.x += fFrac * 0.01f;
	vPosView.y += fFrac * 0.01f;
	//vPosView.z += fFrac * 0.01f;


	unsigned int uiButttons = 0;

	if (wp_player != NULL) uiButttons = wp_player->pl_uiPlacedButtons;
	if (wp_clPlayer != NULL) uiButttons = wp_clPlayer->cl_Buttons;

	// bobbing cycle weapon
	if (uiButttons & BUTT_WALK_BACK || uiButttons & BUTT_WALK_FOR
		|| uiButttons & BUTT_WALK_LEFT || uiButttons & BUTT_WALK_RIGHT)
	{
		if (wp_fWeaponBanking > 1.0f) 	wp_fWeaponBanking = 1.0f;
		wp_fWeaponBanking += 0.15f;
	}
	else 
	{
		if (wp_fWeaponBanking > 0.0f) 
		{
			wp_fWeaponBanking *= 0.8f;
			if (wp_fWeaponBanking < 0.01f) 
			{
				wp_fWeaponBanking = 0.0f;
			}
		}
	}

	
	wp_vWeaponBanking.x = sin((SDL_GetTicks64() * 0.001) * 7.0f) * wp_fWeaponBanking * 0.02f;
	wp_vWeaponBanking.y = sin((SDL_GetTicks64() * 0.001) * 14.0f) * wp_fWeaponBanking * 0.01f;

	vPosView.x += wp_vWeaponBanking.x;
	vPosView.y += wp_vWeaponBanking.y;

	// set matrix
	mat = glm::mat4(1.f);
	mat = glm::translate(mat, vPosView);
	wp_modelWeapon.modelMatrix = mat;	

	// render model
	renderOneModel(&wp_modelWeapon, COLOR_WHITE, &ViewPort);
}

unsigned int CWeapons::getAmmoByWeapon(const unsigned int& iweapon) 
{
	switch (iweapon)
	{
	case WEAPON_MELE:       return 1;
	case WEAPON_AR:      	return wp_ammo.m_ARbullets;		
	case WEAPON_SHOTGUN:	return wp_ammo.m_shells;		
	case WEAPON_ROCKETGUN:	return wp_ammo.m_rockets;
	case WEAPON_ENERGYGUN:  return wp_ammo.m_cells;
	default:
		PrintF("Cannot set model info: %d\n", iweapon);
		return -1;
	}
}

unsigned int CWeapons::getMaxAmmoByWeapon(const unsigned int& iweapon)
{
	switch (iweapon)
	{
	case WEAPON_MELE:       return 1;
	case WEAPON_AR:      	return MAX_BULLETS;
	case WEAPON_SHOTGUN:	return MAX_SHELLS;
	case WEAPON_ROCKETGUN:	return MAX_ROCKETS;
	case WEAPON_ENERGYGUN:  return MAX_CELLS;
	default:
		PrintF("Cannot set model info: %d\n", iweapon);
		return -1;
	}
}

void CWeapons::pickUpWeapon(unsigned int iweapon, unsigned int ivalue)
{
	switch (iweapon)
	{
	case WEAPON_AR:
		wp_ammo.m_ARbullets = iClamp(wp_ammo.m_ARbullets + ivalue, MAX_BULLETS);
		break;
	case WEAPON_SHOTGUN:
		wp_ammo.m_shells = iClamp(wp_ammo.m_shells + ivalue, MAX_SHELLS);
		break;
	case WEAPON_ROCKETGUN:
		wp_ammo.m_rockets = iClamp(wp_ammo.m_rockets + ivalue, MAX_ROCKETS);
		break;
	case WEAPON_ENERGYGUN:
		wp_ammo.m_cells = iClamp(wp_ammo.m_cells + ivalue, MAX_CELLS);
		break;
	default:
		PrintF("Cannot set model info: %d\n", iweapon);
		break;
	}
}

void CWeapons::fireWeapon(unsigned int iweapon)
{
	if (wp_player == NULL) return;

	switch (iweapon)
	{
	case WEAPON_MELE:
		mele_Attack();
		break;
	case WEAPON_AR:
		AR_Attack();
		break;
	case WEAPON_SHOTGUN:
		shotgun_Attack();
		break;
	case WEAPON_ROCKETGUN:
		rocket_Attack();
		break;
	case WEAPON_ENERGYGUN:
		energyRifle_Attack();
		break;
	default:
		PrintF("Cannot execute weapon firing: %d\n", iweapon);
		break;
	}
}

void CWeapons::mele_Attack() 
{
	if (wp_timeStarted > (SDL_GetTicks64() * 0.001)) return;


	wp_timeStarted = (SDL_GetTicks64() * 0.001) + 0.5;

	//assert(wp_player != NULL);

	Vector3D vStart = VECTOR3_NONE;

	if (wp_player != NULL) vStart = wp_player->getCurrentPosition();

	//Vector3D vStart = wp_player->getCurrentPosition();

	vStart.y += wp_player->vCollisionStretch.y * 0.5f;

	// CBullet can have raycast, so we use this
	CBullet* bullet = new CBullet;
	bullet->m_pwo = wp_player->m_pwo;
	bullet->bl_iDamage = 50;
	bullet->bl_distance = 3.0f;
	bullet->entityFrom = wp_player;
	bullet->m_vPosition = vStart;
	bullet->m_anglRotaion = wp_player->pl_camera->m_front;
	bullet->Initialize();
	bullet->LaunchBullet();

	delete bullet;

}

void CWeapons::AR_Attack() 
{
	if (wp_timeStarted > (SDL_GetTicks64() * 0.001)) return;

	if (wp_ammo.m_ARbullets)
	{
		wp_ammo.m_ARbullets -= 1;
		wp_timeStarted = (SDL_GetTicks64() * 0.001) + 0.06;

		//assert(wp_player != NULL);

		Vector3D vStart = VECTOR3_NONE;

		if (wp_player != NULL) vStart = wp_player->getCurrentPosition();
		if (wp_clPlayer != NULL) vStart = wp_clPlayer->cl_vPosition;

		//Vector3D vStart = wp_player->getCurrentPosition();

		//vStart += wp_player->pl_camera->m_front * 1.5f;
		//vStart.y += weaponPositions[WEAPON_AR].fFirePosition[1];
		vStart.y += wp_player->vCollisionStretch.y * 0.5f;

		CBullet* bullet = new CBullet;
		bullet->m_pwo = wp_player->m_pwo;
		bullet->bl_iDamage = 5;
		bullet->entityFrom = wp_player;
		bullet->m_vPosition = vStart;
		bullet->m_anglRotaion = wp_player->pl_camera->m_front;
		bullet->Initialize();
		bullet->LaunchBullet();

		delete bullet;		
	}
	else {
		wp_player->pl_ulCurrentWeapon = nextWeapon(wp_player->pl_uiWeaponMask);
		selectWeapon(wp_player->pl_ulCurrentWeapon);
	}
}

void CWeapons::shotgun_Attack(void)
{
	if (wp_timeStarted > (SDL_GetTicks64() * 0.001)) return;

	if (wp_ammo.m_shells)
	{
		wp_ammo.m_shells -= 1;
		wp_timeStarted = (SDL_GetTicks64() * 0.001) + 1.00;

		assert(wp_player != NULL);

		Vector3D vStart = wp_player->getCurrentPosition();

		//vStart += wp_player->pl_camera->m_front * 1.5f;
		//vStart.y += weaponPositions[WEAPON_AR].fFirePosition[1];
		vStart.y += wp_player->vCollisionStretch.y * 0.5f;	

		FOREACH_ARRAY(7)
		{
			CBullet* bullet = new CBullet;
			bullet->m_pwo = wp_player->m_pwo;
			bullet->bl_iDamage = 10;
			bullet->entityFrom = wp_player;
			bullet->m_vPosition = vStart;
			bullet->m_anglRotaion = wp_player->pl_camera->m_front;

			bullet->m_anglRotaion.x += afShotGunBrass[i].x * 0.25f;
			bullet->m_anglRotaion.y += afShotGunBrass[i].y * 0.25f;

			bullet->Initialize();
			bullet->LaunchBullet();

			delete bullet;
		}		
	}
	else {
		wp_player->pl_ulCurrentWeapon = nextWeapon(wp_player->pl_uiWeaponMask);
		selectWeapon(wp_player->pl_ulCurrentWeapon);
	}
}

void CWeapons::rocket_Attack(void) 
{
	//wp_timeStarted = SDL_GetTicks64() * 0.001;

	if (wp_timeStarted > (SDL_GetTicks64() * 0.001)) return;

	if (wp_ammo.m_rockets) 
	{
		wp_ammo.m_rockets -= 1;
		wp_timeStarted = (SDL_GetTicks64() * 0.001) + 1.0;
		
		assert(wp_player != NULL);

		Vector3D vStart = wp_player->getCurrentPosition();

		vStart += wp_player->pl_camera->m_front * 1.5f;

		vStart.y += weaponPositions[WEAPON_ROCKETGUN].fFirePosition[1];

		entityManager.spawnProjectile(PRT_ROCKET, vStart, wp_player->pl_camera->m_front, wp_player);
	}
	else {
		wp_player->pl_ulCurrentWeapon = nextWeapon(wp_player->pl_uiWeaponMask);
		selectWeapon(wp_player->pl_ulCurrentWeapon);
	}
}

void CWeapons::energyRifle_Attack(void)
{
	if (wp_timeStarted > (SDL_GetTicks64() * 0.001)) return;

	if (wp_ammo.m_cells)
	{
		wp_ammo.m_cells -= 1;
		wp_timeStarted = (SDL_GetTicks64() * 0.001) + 0.1;

		assert(wp_player != NULL);

		Vector3D vStart = wp_player->getCurrentPosition();

		vStart += wp_player->pl_camera->m_front * 1.5f;

		vStart.y += wp_player->vCollisionStretch.y * 0.45f;//weaponPositions[WEAPON_ENERGYGUN].fFirePosition[1]*0.f;		

		entityManager.spawnProjectile(PRT_ENERGY_BALL, vStart, wp_player->pl_camera->m_front, wp_player);
	}
	else {
		wp_player->pl_ulCurrentWeapon = nextWeapon(wp_player->pl_uiWeaponMask);
		selectWeapon(wp_player->pl_ulCurrentWeapon);
	}
}




