#include "stdAfx.h"

#include "GPlayer.h"
#include "GEntities.h"
#include "GameSession.h"
#include "GHUD.h"

/******* player actions ********/
bool bMoveLeft = false;
bool bMoveRight = false;
bool bMoveFront = false;
bool bMoveBack = false;
bool bFire = false;
bool bJump = false;
bool bUse = false;
bool bFly = false;
bool bCrouch = false;
bool bNextWeapon = false;
bool bPrevWeapon = false;

static bool bGod = false;
static bool bAllWeapons = false;
static bool bGhost = false;

float plr_fMoveSpeed = 10.0f;
static float plr_fPowerJump = 5.f;


CPlayer::CPlayer() 
{
	CPhysicsEntity::CPhysicsEntity();

	pl_strPlayerName = stringCopy("^5Seva^8");
	pl_strTeam = stringCopy("Default team");

	m_iArmor = 0;  
	m_iHealth = 100;

	//pl_ColorName = HEX_toOGL(0x8C00FFFFUL);

	pl_colorIndex = plr_colColor;

	m_color = playerColorTables[pl_colorIndex];

	pl_uiPlacedButtons = 0;

	pl_state = PL_STATE_ACTIVE;

	pl_weapons = nullptr;  
	pl_camera  = nullptr;

	pl_uiWeaponMask = WEAPON_BIT(WEAPON_MELE)| WEAPON_BIT(WEAPON_AR) | WEAPON_BIT(WEAPON_ENERGYGUN) | WEAPON_BIT(WEAPON_ROCKETGUN);

	pl_uiNumDeaths = 0;               
	pl_uiNumKills = 0;                
	pl_uiNumPing = 0;  
	pl_ulCurrentWeapon = WEAPON_AR;

	pl_bRespawn = true;                   

	pl_bSpectate = false;

	pl_bLocal = false;

	pl_sID = 0;

	pl_dDeathTime = 0;
}

CPlayer::~CPlayer()
{
	CPhysicsEntity::End();
}

void CPlayer::getPlayerView(CView* viewPort) 
{
	Vector3D vPos = getCurrentPosition();
	pl_camera->m_position = VECTOR3(vPos.x, (vPos.y + vCollisionStretch.y * 0.5f) + pl_weapons->wp_vWeaponBanking.y * 5.f, vPos.z);

	viewPort->m_viewMatrix = pl_camera->getMatrixView();
	viewPort->m_ViewDir = pl_camera->m_position;
}

void CPlayer::Initialize() 
{
	if (pl_bLocal) 
	{
		pConsoleDataBase->addConsoleVariable(CVT_BOOL, "bMoveLeft", &bMoveLeft);
		pConsoleDataBase->addConsoleVariable(CVT_BOOL, "bMoveRight", &bMoveRight);
		pConsoleDataBase->addConsoleVariable(CVT_BOOL, "bMoveFront", &bMoveFront);
		pConsoleDataBase->addConsoleVariable(CVT_BOOL, "bMoveBack", &bMoveBack);
		pConsoleDataBase->addConsoleVariable(CVT_BOOL, "bFire", &bFire);
		pConsoleDataBase->addConsoleVariable(CVT_BOOL, "bJump", &bJump);
		pConsoleDataBase->addConsoleVariable(CVT_BOOL, "bUse", &bUse);
		pConsoleDataBase->addConsoleVariable(CVT_BOOL, "bCrouch", &bCrouch);

		pConsoleDataBase->addConsoleVariable(CVT_BOOL, "bNextWeapon", &bNextWeapon);
		pConsoleDataBase->addConsoleVariable(CVT_BOOL, "bPrevWeapon", &bPrevWeapon);

		pConsoleDataBase->addConsoleVariable(CVT_FLOAT, "plr_fMoveSpeed", &plr_fMoveSpeed);
	}

	m_ulFlags = 0;
	m_ulFlags |= PHYS_CHARACTER;

	m_uiPhysicFlags = COLLISION_STATIC | COLLISION_ITEM | COLLISION_PROJECTILE;
	m_uiPhysicType = COLLISION_NPC;

	m_strClassName = "Player";

	m_fMassObject = 75.f;

/*	if (!pl_bLocal)
	{*/
		m_strModel =   "Data\\Models\\Players\\Test\\Box.mo";
		m_strTexture = "Data\\Models\\Players\\Test\\Box.te";
/* }
	else 
	{
		m_strModel   = "";// "Data\\Players\\Test\\Box.mo";
		m_strTexture = "";// "Data\\Players\\Test\\Box.te";
	}*/
	//pl_uiWeaponMask = 2;

	pl_weapons = New<CWeapons>();
	pl_camera  = New<CCamera>();	

	vCollisionStretch.x = 0.500000000F;
	vCollisionStretch.y = 1.85f;//0.9250f;
	vCollisionStretch.z = 0.500000000F;

	pl_weapons->wp_player = this;

	pl_weapons->Precache(pl_uiWeaponMask);

	InitCollisionInfo();

//	if (pl_bLocal) {
//		InitAsEditorEntity();
//	}
//	else {
		InitAsModel();
//		m_pmo.m_vScale = vCollisionStretch;
//	}
}

void CPlayer::SetUpPlayerPosition(const Vector3D& vPos)
{
	pl_camera->setCamera(vPos);
}

void CPlayer::End() 
{
	Delete( pl_weapons );
	Delete( pl_camera );

	CPhysicsEntity::End();

	PrintF("Player: %s is killed\n", pl_strPlayerName);

	Free(pl_strPlayerName);
	Free(pl_strTeam);
}

void CPlayer::Respawn() 
{
//	m_vPosition = VECTOR3(0, 1.f, 0);

//	pl_state = PL_STATE_ACTIVE;

//	m_iArmor = 0;
//	m_iHealth = 100;

//	pl_uiWeaponMask = WEAPON_BIT(WEAPON_AR)/* | WEAPON_BIT(WEAPON_SHOTGUN)*/ | WEAPON_BIT(WEAPON_ROCKETGUN) | WEAPON_BIT(WEAPON_ENERGYGUN);

	pl_weapons->selectWeapon(WEAPON_MELE);
	pl_weapons->wp_ammo.m_ARbullets = 0;
	pl_weapons->wp_ammo.m_rockets = 0;
	pl_weapons->wp_ammo.m_shells = 0;
	pl_weapons->wp_ammo.m_cells = 0;

	entityManager.spawnPlayerFromSpawner(this);

	hud_bShowScore = false;


}

void CPlayer::RenderView(void) 
{
	

	//pl_weapons->renderWeapon();
}

void CPlayer::applyTouchEvent(touchHandle_t* handle) 
{

	if (pl_state == PL_STATE_DEATH || pl_state == PL_STATE_FINISHED) return;
	
	float armor, health;

	switch (handle->th_touchType)
	{
	case TT_ITEM:

		if (IsClass(handle->th_entityFrom, "Health"))
		{
			if (m_iHealth == TOP_HEALTH && handle->th_iValue != 100)
			{
				m_iHealth = 100;
			}
			else if (m_iHealth >= TOP_HEALTH && m_iHealth <= MAX_HEALTH && handle->th_iValue == 100)
			{				
				m_iHealth = iClamp(m_iHealth + handle->th_iValue, MAX_HEALTH);
				handle->th_entityFrom->m_bCatch = true;

				((CHealth&)*handle->th_entityFrom).he_dStarted = SDL_GetTicks64() * 0.001 + ITEM_RESPAWN_DEFAULT;
			}
			else if (m_iHealth < 100 && m_iHealth != 0)
			{
				if (handle->th_iValue < 100)
					m_iHealth = iClamp(m_iHealth + handle->th_iValue, TOP_HEALTH);
				else
					m_iHealth += handle->th_iValue;

				handle->th_entityFrom->m_bCatch = true;
				((CHealth&)*handle->th_entityFrom).he_dStarted = SDL_GetTicks64() * 0.001 + ITEM_RESPAWN_DEFAULT;
			}
			else {
				return;
			}
		}
		if (IsClass(handle->th_entityFrom, "Armor"))
		{
			if (m_iArmor == TOP_ARMOR && handle->th_iValue != 100)
			{
				m_iArmor = 100;
			}
			else if (m_iArmor >= TOP_ARMOR && m_iArmor <= MAX_ARMOR && handle->th_iValue == 100)
			{
				m_iArmor = iClamp(m_iArmor + handle->th_iValue, MAX_ARMOR);
				handle->th_entityFrom->m_bCatch = true;

				((CArmor&)*handle->th_entityFrom).ar_dStarted = SDL_GetTicks64() * 0.001 + ITEM_RESPAWN_DEFAULT;
			}
			else if (m_iArmor < 100 && m_iArmor >= 0)
			{
				if (handle->th_iValue < 100)
					m_iArmor = iClamp(m_iArmor + handle->th_iValue, TOP_ARMOR);
				else
					m_iArmor += handle->th_iValue;

				handle->th_entityFrom->m_bCatch = true;
				((CArmor&)*handle->th_entityFrom).ar_dStarted = SDL_GetTicks64() * 0.001 + ITEM_RESPAWN_DEFAULT;
			}
			else {
				return;
			}
		}

		if (IsClass(handle->th_entityFrom, "WeaponItem")) 
		{
			unsigned int iWeapon = ((CWeaponItem&)*handle->th_entityFrom).wi_uiWeapon;
			
			if (pl_uiWeaponMask & WEAPON_BIT(iWeapon)) 	{
				pl_weapons->pickUpWeapon(iWeapon, handle->th_iValue);
				handle->th_entityFrom->m_bCatch = true;
				((CWeaponItem&)*handle->th_entityFrom).wi_dStarted = SDL_GetTicks64() * 0.001 + ITEM_RESPAWN_DEFAULT;
				return;
			}


			pl_uiWeaponMask |= WEAPON_BIT(iWeapon);
			pl_ulCurrentWeapon = iWeapon;
			pl_weapons->pickUpWeapon(iWeapon, handle->th_iValue);
			pl_weapons->selectWeapon(iWeapon);
			handle->th_entityFrom->m_bCatch = true;
		}

		break;
	case TT_PROJECTILE:

		if (IsClass(handle->th_entityFrom, "Projectile"))
		{

			// projectiles caused from owner cannot be harm yourself, its weird, but needed for shot on floor
			if (handle->th_entityLaunched == this)
			{
				//PrintF("You harm yourself!\n");
				return;
			}


			if (m_iArmor == 0)
			{
				m_iHealth -= handle->th_iValue;
			}
			else
			{
				armor = float(handle->th_iValue) * 2.f / 3.f;
				health = float(handle->th_iValue) * 1.f / 3.f;

				m_iArmor -= int(armor);
				m_iHealth -= int(health);

				if (m_iArmor <= 0)
				{
					m_iHealth -= m_iArmor;
					m_iArmor = 0;
				}
			}
		
			handle->th_entityFrom->m_bCatch = true;

			if (m_iHealth <= 0) 
			{
				CPlayer* e = (CPlayer*)handle->th_entityLaunched;
				e->pl_uiNumKills++;
				Death();
				gameSession.updatePlayerScores();
			}			

		}

		if (IsClass(handle->th_entityFrom, "Explosion")) 
		{		

			//PrintF("Dir = %f %f %f\n", ((CExplosion&)*handle->th_entityFrom).ex_vDirection.x, ((CExplosion&)*handle->th_entityFrom).ex_vDirection.y, ((CExplosion&)*handle->th_entityFrom).ex_vDirection.z);
			//PrintF("Force = %f\n", ((CExplosion&)*handle->th_entityFrom).ex_fForce);

			Vector3D vImpulse = ((CExplosion&)*handle->th_entityFrom).ex_vDirection * ((CExplosion&)*handle->th_entityFrom).ex_fForce;

			activateEntityMovement();

			Vector3D v = VECTOR3_NONE;

			//pl_uiPlacedButtons = 0;

			setLinearVel(v);

			//applyImpulse(v);

			setLinearVel(vImpulse);

			if (m_iArmor == 0) 
			{
				m_iHealth -= handle->th_iValue;
			}
			else
			{
				armor = float(handle->th_iValue) * 2.f / 3.f;
				health = float(handle->th_iValue) * 1.f / 3.f;

				m_iArmor -= int(armor);
				m_iHealth -= int(health);

				if (m_iArmor <= 0)
				{
					m_iHealth += m_iArmor;
					m_iArmor = 0;
				}
			}


			if (m_iHealth <= 0) 
			{
				if (handle->th_entityLaunched == this) pl_uiNumKills--;
				else ((CPlayer*)&handle->th_entityLaunched)->pl_uiNumKills++;
				Death();
				gameSession.updatePlayerScores();
			}
		}

		if (IsClass(handle->th_entityFrom, "Bullet")) 
		{

			if (m_iArmor == 0)
			{
				m_iHealth -= handle->th_iValue;
			}
			else
			{
				armor = float(handle->th_iValue) * 2.f / 3.f;
				health = float(handle->th_iValue) * 1.f / 3.f;

				m_iArmor -= int(armor);
				m_iHealth -= int(health);

				if (m_iArmor <= 0)
				{
					m_iHealth -= m_iArmor;
					m_iArmor = 0;
				}
			}

			//handle->th_entityFrom->m_bCatch = true;

			if (m_iHealth <= 0)
			{
				CPlayer* e = (CPlayer*)handle->th_entityLaunched;
				//PrintF("Player \"%s\" shotted \"%s\"\n", e->pl_strPlayerName, pl_strPlayerName);
				e->pl_uiNumKills++;
				Death();				
				gameSession.updatePlayerScores();
			}
		}

		if (IsClass(handle->th_entityFrom, "ZoneHurt")) 
		{
			m_iHealth -= handle->th_iValue;

			if (m_iHealth <= 0)
			{	
				pl_uiNumKills--;
				Death();
				gameSession.updatePlayerScores();
			}
		}

		break;
	default:		
		break;
	}
}

void CPlayer::Death()
{
	
	pl_bLocal ? hud_bShowScore = true : hud_bShowScore = false;
	
	// resset action buttons
	pl_uiPlacedButtons = 0;

	m_iHealth = 0;

	// set camera on floor
	pl_camera->m_front.x = 0.2f;
	// get respawn time
	pl_dDeathTime = SDL_GetTicks64() * 0.001 + 3.0;
	// setup state
	pl_state = PL_STATE_DEATH;
	// setup vector 0 0 0 
	Vector3D v = VECTOR3_NONE;

	setLinearVel(v);
	// increment counter how many player was killed
//	pl_uiNumDeaths++;
}

void CPlayer::Update() 
{
    if (pl_state == PL_STATE_FINISHED) return;

	if (pl_dDeathTime <= (SDL_GetTicks64() * 0.001) && pl_state == PL_STATE_DEATH)
	{
		Respawn();
	}

	if (pl_state == PL_STATE_DEATH) 	return;

	if (!pl_bLocal) 
	{
		doActions();
		getMatrixOrientation();
		return;
	}

	pl_uiPlacedButtons = 0;

	if (bCrouch)    pl_uiPlacedButtons |= BUTT_CROUCH;
	if (bJump)      pl_uiPlacedButtons |= BUTT_JUMP;
	if (bMoveLeft)  pl_uiPlacedButtons |= BUTT_WALK_LEFT;
	if (bMoveRight) pl_uiPlacedButtons |= BUTT_WALK_RIGHT;
	if (bMoveFront) pl_uiPlacedButtons |= BUTT_WALK_FOR;
	if (bMoveBack)  pl_uiPlacedButtons |= BUTT_WALK_BACK;
	if (bFire)      pl_uiPlacedButtons |= BUTT_FIRE;
	if (bNextWeapon)pl_uiPlacedButtons |= BUTT_SELECT_WEAPON;
	if (bPrevWeapon)pl_uiPlacedButtons |= BUTT_PREV_WEAPON;

	doActions();

	getMatrixOrientation();

	//writeToBuffer();
}

void CPlayer::doActions() 
{

	// movement actions

	Vector3D vVelocity = getLinearVel();
	Vector3D vTranslation = VECTOR3_NONE;
	Vector3D vViewDir;
	//m_anglRotaion

	//PrintF("m_front = %f %f %f\n", pl_camera->m_front.x, pl_camera->m_front.y, pl_camera->m_front.z);

	if (pl_bLocal)
		vViewDir = glm::normalize(glm::vec3(pl_camera->m_front.x, 0.0f, pl_camera->m_front.z));
	else
		vViewDir = glm::normalize(glm::vec3(m_anglRotaion.x, 0.0f, m_anglRotaion.z));

	if ( pl_uiPlacedButtons & BUTT_WALK_FOR )
	{
		vTranslation += VECTOR3(vViewDir.x, 0.0f, vViewDir.z);
	}

	if (pl_uiPlacedButtons & BUTT_WALK_BACK)
	{
		vTranslation -= VECTOR3(vViewDir.x, 0.0f, vViewDir.z);
	}

	if ( pl_uiPlacedButtons & BUTT_WALK_RIGHT)
	{
		vTranslation += glm::normalize(glm::cross(vViewDir, pl_camera->m_up));
	}

	if (pl_uiPlacedButtons & BUTT_WALK_LEFT)
	{
		vTranslation -= glm::normalize(glm::cross(vViewDir, pl_camera->m_up));
	}

	if (pl_uiPlacedButtons & BUTT_FIRE && pl_weapons != nullptr)
	{
		pl_weapons->fireWeapon(pl_ulCurrentWeapon);
	}


	if (pl_uiPlacedButtons & BUTT_SELECT_WEAPON) 
	{
		pl_ulCurrentWeapon = pl_weapons->nextWeapon(pl_uiWeaponMask);
		pl_weapons->selectWeapon(pl_ulCurrentWeapon);
	}

	if (pl_uiPlacedButtons & BUTT_PREV_WEAPON) 
	{
		pl_ulCurrentWeapon = pl_weapons->prevWeapon(pl_uiWeaponMask);
		pl_weapons->selectWeapon(pl_ulCurrentWeapon);
	}
	
	
	if (vTranslation.length() > 0.0f) 
	{ 
		activateEntityMovement();
		glm::normalize(vTranslation);
	}

	Vector3D vNew(vTranslation.x * plr_fMoveSpeed, vVelocity.y, vTranslation.z * plr_fMoveSpeed);

	if (pl_uiPlacedButtons & BUTT_JUMP && testIfEntityTouchesGround())
	{
		//Vector3D vImpulse(0, plr_fPowerJump, 0);

		//Vector3D vVelocity2 = getLinearVel();

		//Vector3D setLinear(vVelocity.x, plr_fPowerJump, vVelocity.z);
		vNew.y = plr_fPowerJump;
		//setLinearVel(setLinear);
		//applyImpulse(vImpulse);

		//PrintF("^7Here^8\n");

		bJump = 0;
	}

	setLinearVel(vNew);

	float x = pConsoleDataBase->getInt("mouseX");
	float y = pConsoleDataBase->getInt("mouseY");

	pl_camera->cameraMoveMouse(x, y);

	pConsoleDataBase->executeVar("mouseX = 0", false);
	pConsoleDataBase->executeVar("mouseY = 0", false);
	pConsoleDataBase->executeVar("bNextWeapon = 0", false);
	pConsoleDataBase->executeVar("bPrevWeapon = 0", false);

	
}

void CPlayer::getFromClient(Network::CNetMSG* msg)
{
	/*
	PlayerForServer cl;

	msg->readBlock((byte*)&cl, sizeof(PlayerForServer));

	m_iHealth = cl.health;
	m_iArmor = cl.armor;
	pl_uiPlacedButtons = cl.buttons;
	
//	m_vPosition = glm::vec3(cl.vpos[0], cl.vpos[1], cl.vpos[2]);
//	m_anglRotaion = glm::vec3(cl.arot[0], cl.arot[1], cl.arot[2]);
	
	m_vPosition = cl.vPos;
	m_anglRotaion = cl.aRot;

	pl_camera->m_front = m_anglRotaion;*/
	
	int health, armor;
	Vector3D vPos;

	msg->readBlock((unsigned char*)&health, sizeof(int));
	msg->readBlock((unsigned char*)&armor, sizeof(int));

	if (health != m_iHealth || armor != m_iArmor)
	{
		//PrintF("Player \"%s\" has wrong health and armor values!: %d %d\n", pl_strPlayerName, health, armor);

		health = m_iHealth;
		armor = m_iArmor;
	}

	m_iHealth = health;
	m_iArmor  = armor;

	msg->read32(&pl_uiPlacedButtons);
	msg->readVector(&vPos);	
	msg->readVector(&m_anglRotaion);

	pl_camera->m_front = m_anglRotaion;

	if (vPos == m_vPosition) m_vPosition = vPos;
}



void CPlayer::makeForClient(Network::CNetMSG* msg)
{
	/*
	PlayerForClient cl;

	cl.health = m_iHealth;
	cl.armor = m_iArmor;
	cl.currentWeapon = pl_ulCurrentWeapon;

	m_vPosition = getCurrentPosition();
	m_vPosition.y += vCollisionStretch.y * 0.5f;

//	cl.vPos[0] = m_vPosition.x;
//	cl.vPos[1] = m_vPosition.y;
//	cl.vPos[2] = m_vPosition.z;

	cl.vPos = m_vPosition;

	cl.awAmmo = pl_weapons->wp_ammo;

	msg->writeBlock((byte*)&cl, sizeof(PlayerForClient));
	*/

	
	msg->writeBlock((byte*) &m_iHealth, sizeof(short));
	msg->writeBlock((byte*) &m_iArmor, sizeof(short));

	byte weapon = pl_ulCurrentWeapon & 0xFF;

	msg->write8(weapon);

	msg->write8(pl_uiNumKills & 0xFF);

	msg->write8(pl_state);
	//msg->write32(pl_ulCurrentWeapon);

	if (pl_state == PL_STATE_FINISHED)
	{
		msg->writeVector(m_vPosition);
	}
	else 
	{
		m_vPosition = getCurrentPosition();
		m_vPosition.y += vCollisionStretch.y * 0.5f;
		msg->writeVector(m_vPosition);
	}

	msg->writeBlock((byte*)&pl_weapons->wp_ammo, sizeof(AmmoWeapon_t));
}



void CPlayer::SetPlayerName(const char* strName)
{
	if (pl_strPlayerName != NULL)
	{
		PrintF("Player \"%s\" is now known as ", pl_strPlayerName);
		Free(pl_strPlayerName);
		pl_strPlayerName = stringCopy(strName);
		PrintF("\"%s\"\n", pl_strPlayerName);
	}
}
void CPlayer::SetPlayerTeam(const char* strName)
{
	if (pl_strTeam != NULL)
	{
		PrintF("Player \"%s\" who was in the \"%s\" team, now plays for the ", pl_strPlayerName, pl_strTeam);
		Free(pl_strTeam);
		pl_strTeam = stringCopy(strName);
		PrintF("\"%s\" team\n", pl_strTeam);
	}
}

/*
void CPlayer::writeToBuffer() 
{

}*/