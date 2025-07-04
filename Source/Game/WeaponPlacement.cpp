#include "stdAfx.h"

#include "GWeapon.h"


WeaponPosition_t weaponPositions[MAX_WEAPONS] =
{
	//vPos   Fov Clip FirePos
	{{0,0,0}, 90,0, {0,0,0}}, // WEAPON_ZERO
	{{0.135f,-0.3f,-0.25f}, 90.f,0.1f, {0,0,0}}, // WEAPON_MELE
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_PISTOL
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_DOUBLEPISTOL
	{{0.13f,-0.28f,0}, 75, 0.1f, {0,0,0}}, // WEAPON_SHOTGUN
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_DOUBLESHOTGUN

	{{0.135f,-0.19f,-0.35f}, 41, 0.1f, {0,-0.2f,0}}, // WEAPON_AR

	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_MINIGUN
	{{0.2347f, -0.43133f, 0.47644f}, 90,0.1f, {0,0.78f,0}}, // WEAPON_ROCKETGUN 
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_DOUBLEROCKETGUN
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_GRENADELAUNCHER
	{{0.18f,-0.24f, -0.18f}, 50.f,0.1f, {0,0.85f,0}}, // WEAPON_ENERGYGUN
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_ALIENSNIPER
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_ACIDBOMB
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_NITROGENBOMB
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_PRESSUREGUN
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_NANOSPLITTER
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_HELLSCOUP
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_ARTEFACT
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_LAST
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_ZERO
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_ZERO
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_ZERO
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_ZERO 
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_ZERO
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_ZERO 
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_ZERO
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_ZERO
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_ZERO
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_ZERO
	{{0,0,0}, 0,0, {0,0,0}}, // WEAPON_ZERO
	{{0,0,0}, 0,0, {0,0,0}}  // WEAPON_ZERO
};