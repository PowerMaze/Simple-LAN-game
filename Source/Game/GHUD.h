#pragma once

#include "GWeapon.h"

#define HUD_COL_PERFECT   COLOR_CYAN
#define HUD_COL_NORMAL    COLOR_GREEN
#define HUD_COL_MIDDLE    COLOR_YELLOW
#define HUD_COL_BAD       COLOR_RED

typedef struct playerInterFace_s 
{
	int* health;
	int* armor;
	unsigned int* weapon;
	int* frags;
	CWeapons* weapons;
}playerInterFace_t;

/*
struct playerScore_s 
{
	char playerName[64];
	int death;
}playerScore[8];
*/

extern bool hud_bShowScore;

namespace GameHUD
{
	void setup(void);

	void drawText(CView* view, int x, int y, const char* string);

	void drawPlayerScore(CView* view);

	void draw(CView* view);

//	void drawClient(CView* view);

	void clear(void);

};