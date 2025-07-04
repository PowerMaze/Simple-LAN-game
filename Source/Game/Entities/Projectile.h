#pragma once

enum EProjectileType
{
	PRT_ROCKET,
	PRT_GRENADE,
	PRT_ENERGY_BALL,
	PRT_POWER_BALL,
	PRT_HELL_BALL
};

//#define SCALE_PROJECTILE_ENEREGY glm::vec3(0.25, 0.25, 0.25)
#define SCALE_PROJECTILE_ENEREGY 0.15f

/*
/////////////////////////////////////////////////// 
              projectile entity
///////////////////////////////////////////////////
*/
class CProjectile : public CPhysicsEntity
{
public:

	double m_fWaitAfterDeath;
	double m_fDuringTime;

	double m_fWaitTime;
	enum EProjectileType m_eptType;

	CEntityMain* m_whoLaunched;

public:
	CProjectile(void);
	~CProjectile(void);

	void Initialize(void) override;
	void Update(void) override;
	void End(void) override;

	void generateTouchEvent(touchHandle_t* handle) override;

	void Explosion();

private:
	void InitRocket(void);
	void InitGrenade(void);
	void InitEnergyBall(void);
	void InitHellBall(void);

};
