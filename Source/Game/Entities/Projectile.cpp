#include "stdAfx.h"

#include "../GEntities.h"
#include "../GMain.h"

CProjectile::CProjectile(void) 
{
	CPhysicsEntity::CPhysicsEntity();
}

CProjectile::~CProjectile() 
{
	CPhysicsEntity::End();
	m_whoLaunched = nullptr;
}

void CProjectile::Initialize(void) 
{
	

	m_ulID = 3;

	m_strName = "Projectile";
	m_strClassName = m_strName;
	m_bPhysics = true;

	m_ulFlags |= PHYS_STATIC;

	//m_whoLaunched = nullptr;
	
	m_uiPhysicFlags = COLLISION_STATIC | COLLISION_NPC;
	m_uiPhysicType = COLLISION_PROJECTILE;

	switch (m_eptType) 
	{
	case PRT_ROCKET:        InitRocket();       break;
	case PRT_GRENADE:       //InitGrenade();      break;
	case PRT_HELL_BALL:     //InitHellBall();     break;
	case PRT_POWER_BALL:    //InitPowerBall();    break;
	case PRT_ENERGY_BALL:	InitEnergyBall();	break;
	default: Throw("None to initialize projectile!");
	}

	InitCollisionInfo();


	//float fVelocitySpeed = m_fSpeed * Game->lastTime;

	//m_vPosition = getLinearVel();

	//m_vPosition = glm::normalize(m_anglRotaion) * fVelocitySpeed;

	//setLinearVel(m_vPosition);

}

void CProjectile::Explosion()
{
	CExplosion* explode = new CExplosion;

	explode->ex_iMaxDamage = 100;
	explode->m_pwo = m_pwo;
	explode->ex_entityReason = m_whoLaunched;

	m_vPosition = getCurrentPosition();

	explode->m_vPosition = m_vPosition - glm::normalize(m_anglRotaion) * 0.25f;

	explode->Initialize();
	

	entityManager.em_allServerEntities.push_back(explode);
}

void CProjectile::Update(void)
{
	// do not follow the logic
	if (m_iHealth == 0 || m_bDead) return;

	// if life time has passed -> kill entity
	if (m_fDuringTime <= (double(SDL_GetTicks64()) * 0.001)) 
	{
		if (m_eptType == PRT_GRENADE || m_eptType == PRT_ROCKET)	Explosion();
		End();
		return;
	}

	// if entity hit -> kill entity
	if (m_bCatch)
	{
		if (m_eptType == PRT_GRENADE || m_eptType == PRT_ROCKET)	Explosion();		
		End();
		return;
	}

	// do logic

	//float fVelocitySpeed = m_fSpeed * Game->lastTime;

	//m_vPosition = getLinearVel();
	m_vPosition = glm::normalize(m_anglRotaion) * m_fSpeed;

	setLinearVel(m_vPosition);

	getMatrixOrientation();
}

void CProjectile::generateTouchEvent(touchHandle_t* handle) 
{
	handle->th_touchType = TT_PROJECTILE;
	handle->th_entityFrom = this;
	handle->th_entityLaunched = m_whoLaunched;

	switch (m_eptType)
	{
	case PRT_ROCKET:  

		handle->th_damageType = DMT_EXPLOSION;
		handle->th_iValue = 75;

		break;
	case PRT_GRENADE:          
		break;
	case PRT_HELL_BALL:        
		break;
	case PRT_POWER_BALL:     
		break;
	case PRT_ENERGY_BALL:	
		handle->th_damageType = DMT_BURNING;
		handle->th_iValue = 20;
		break;
	default: PrintF("None to generate projectile damage!");
	}
}

void CProjectile::End(void) 
{
	CPhysicsEntity::End();
	m_whoLaunched = nullptr;
	entityManager.addEntityToDelete(this);
}

void CProjectile::InitRocket(void) 
{
	m_strModel = "Data\\Models\\Weapons\\EnergyRifle\\Projectile\\Cube.mo";
	m_strTexture = "Data\\Models\\Weapons\\EnergyRifle\\Projectile\\Cube.te";

	InitAsModel();

	m_pmo.ScaleAll(SCALE_PROJECTILE_ENEREGY);

	vCollisionStretch = Vector3D(SCALE_PROJECTILE_ENEREGY);

	m_color = COLOR_WHITE;
	m_fMassObject = 1.0f;
	m_fWaitTime = 5.0f;

	m_fSpeed = 30.0f;

	m_iHealth = 10;

	m_fDuringTime = (double(SDL_GetTicks64()) * 0.001) + m_fWaitTime;
}

void CProjectile::InitGrenade(void) 
{
	//PrintF("call CProjectile::InitGrenade()\n");
	
}

void CProjectile::InitEnergyBall(void)
{
	m_strModel   = "Data\\Models\\Weapons\\EnergyRifle\\Projectile\\Cube.mo";
	m_strTexture = "Data\\Models\\Weapons\\EnergyRifle\\Projectile\\Cube.te";

	InitAsModel();

	m_pmo.ScaleAll(SCALE_PROJECTILE_ENEREGY);


	vCollisionStretch = Vector3D(SCALE_PROJECTILE_ENEREGY);

	m_color = COLOR_WHITE;
	m_fMassObject = 1.0f;
	m_fWaitTime = 5.0f;

	m_fSpeed = 45.0f;
	
	m_iHealth = 20;

	m_fDuringTime = ( double( SDL_GetTicks64() ) * 0.001 ) + m_fWaitTime;

}

void CProjectile::InitHellBall(void) 
{

	
}