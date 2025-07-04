#pragma once

//#include "GPlayer.h"

/*
======================================
		Weapon definitions
======================================
*/
#define WEAPON_ZERO            0          
#define WEAPON_MELE            1         
#define WEAPON_PISTOL          2         
#define WEAPON_DOUBLEPISTOL    3
#define WEAPON_SHOTGUN         4   
#define WEAPON_DOUBLESHOTGUN   5 
#define WEAPON_AR              6      
#define WEAPON_MINIGUN         7    
#define WEAPON_ROCKETGUN       8   
#define WEAPON_DOUBLEROCKETGUN 9
#define WEAPON_GRENADELAUNCHER 10
#define WEAPON_ENERGYGUN       11
#define WEAPON_ALIENSNIPER     12
#define WEAPON_ACIDBOMB        13
#define WEAPON_NITROGENBOMB    14
#define WEAPON_PRESSUREGUN     15
#define WEAPON_NANOSPLITTER    16
#define WEAPON_HELLSCOUP       17 
#define WEAPON_ARTEFACT        18  
#define WEAPON_LAST            19  

#define WEAPON_BIT(weapon)  (1u << weapon)


#define ALL_WEAPONS_MASK 0x3FFFF
#define START_WEAPONS_MASK (WEAPON_BIT(WEAPON_MELE) | WEAPON_BIT(WEAPON_ZERO))

#define MAX_WEAPONS 32

#if DIPLOM
	#define NOT_ALL_WEAPONS (\
		(WEAPON_AR) |\
		(WEAPON_GRENADELAUNCHER)|\
		(WEAPON_ALIENSNIPER)|\
		(WEAPON_ACIDBOMB)|\
		(WEAPON_NITROGENBOMB)|\
		(WEAPON_PRESSUREGUN)|\
		(WEAPON_NANOSPLITTER)|\
		(WEAPON_ARTEFACT)|\
		(WEAPON_HELLSCOUP))
#else
	#define NOT_ALL_WEAPONS 0x0UL
#endif

#define MAX_SMALLBULLETS 50 
#define MAX_SHELLS       100 
#define MAX_BULLETS      600 
#define MAX_ROCKETS      50 
#define MAX_GRENADES     50 
#define MAX_NUKEGRENADES 10 
#define MAX_CELLS        500 
#define MAX_ABOMBS       15 
#define MAX_NBOMBS       15 
#define MAX_PULSE        45 
#define MAX_HELIUM       30 
#define MAX_HELLSAMMO    10
#define MAX_ARTEFACTAMMO 5  

#define WEAPON_STATE_READY   0
#define WEAPON_STATE_SELECT  1
#define WEAPON_STATE_FIRE    2

struct AmmoWeapon_t 
{
//	int m_iPistolBullets;   // ammo for pistols
	short m_shells;           // ammo for shotgun/double shotgun 
	short m_ARbullets;        // ammo for automatic rifle/minigun
	short m_rockets;          // ammo for rocket guns
//	int m_grenades;         // ammo for grenade launcher (1 mode - default grenades)
//	int m_nukeGrenades;     // ammo for grenade launcher(2 mode - nuke grenades)
	short m_cells;            // ammo for energy rifle and nanosplitter
//	int m_aBombs;           // ammo for acid bombs, used by grenade launcher - (3 mode)
//	int m_nBombs;           // ammo for nitrogen bombs, used by grenade laucher - (4 mode)
//	int m_pulseSlugs;       // ammo for pulse accelerator
//	int m_hellsAmmo;        // ammo for hells coupe cannon
//	int m_helium;           // ammo for pressure cannon	 
//	int m_artefactAmmo;     // ammo for ancient artefact
};

struct WeaponPosition_t 
{
	float vPos[3];
	float fFOV;
	float fClip;
	float fFirePosition[3];
};

extern WeaponPosition_t weaponPositions[MAX_WEAPONS];

//extern int aWeaponsAmmo[MAX_WEAPONS];

/*
///////////////////////////////////////////////////

class for player weapon

///////////////////////////////////////////////////
*/
class CWeapons
{
public:
	CModel wp_modelWeapon;         // weapon model

	//CSound wp_soundFire;
	//CSound wp_soundIdle;

	unsigned int wp_currentWeapon; // current weapon for uses
//	bool wp_leftHandedWeaponUses;  // uses for double pistols or double rockets

	AmmoWeapon_t wp_ammo;

//	int wp_aWeaponsAmmo[MAX_WEAPONS];

	Vector3D wp_vPosWeapon;
	Angle3D wp_aRotWeapon;

	byte wp_weaponState;

	double wp_time;
	double wp_timeStarted;

	class CPlayer* wp_player; // pointer to player
	class CPlayerClient* wp_clPlayer;

	float wp_fWeaponBanking;

	Vector2D wp_vWeaponBanking;

public:
	/* default constructor */
	CWeapons();
	/* constructor from weapons mask */
//	CWeapons(unsigned long iweapons);
	/* destructor */
	~CWeapons();
	/* render weapon model */
	void renderWeapon(void);
	/* pickup weapon from CWeaponItem */
	void pickUpWeapon(unsigned int iweapon, unsigned int ivalue);
	/* select weapons */
	void selectWeapon(unsigned int iweapon);
	/* set current weapon */
	void useWeapon(unsigned int iweapon);
	/* fire weapon */
	void fireWeapon(unsigned int iweapon);

	unsigned int getAmmoByWeapon(const unsigned int& iweapon);
	unsigned int getMaxAmmoByWeapon(const unsigned int& iweapon);

	unsigned int nextWeapon(unsigned int& uiWeaponMask);
	unsigned int prevWeapon(unsigned int& uiWeaponMask);

	void Precache(unsigned int iWeaponMask);

private:
	/* clear weapons */
	void Clear(void);
	/* check if has ammo */
	bool hasAmmo();
	/* decrement weapon */
	int  decAmmo(int ammo);
	/* attaches ammo to weapon */
	void attachAmmoToWeapon(void);
	/* select weapon speccifics */
	void proccesSelect(unsigned int iWeapon);

private:
	/* animation when weapon fire */
	void playAnimationAttack(void);
	/* animation when weapon reloads */
	void playAnimationReload(void);
	/* animation when weapon appear */
	void playAnimationShow(void);
	/* animation when weapon hide */
	void playAnimationHide(void);
	/* animation when do nothing 20 sec */
	void playAnimationBoring(void);
	/* animation when no firing, no selected */
	void playAnimationStand(void);

private:
	/*** Weapon attacks **/

	void mele_Attack(void);
//	void pistol_Attack(void);
	void shotgun_Attack(void);
//	void doubleShotGun_Attack(void);
	void AR_Attack(void);
//	void minigunSpinUp(void);
//	void minigunSpinDown(void);
//	void minigun_Attack(void);
	void rocket_Attack(void);
//	void grenade_AttackState01(void);
//	void grenade_AttackState02(void);
//	void grenade_Attack(void);
	void energyRifle_Attack(void);
//	void acidBomb_Attack(void);
//	void nitrogenBomb_Attack(void);
//	void alienSnipper_Attack(void);
//	void pressureCannon_Attack(void);
//	void cupOfHellCannon_Attack(void);
//	void nanoSplitter_Attack(void);
//	void artefact_Attack(void);
};