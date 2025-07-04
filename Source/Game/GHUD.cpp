#include "stdAfx.h"

#include "GHUD.h"
#include "GameSession.h"
#include "GPlayer.h"
#include "GPlayerClient.h"
#include "GEntities.h"
#include "GWeapon.h"

CTextureObject* p_teHealth = NULL;
CTextureObject* p_teArmor = NULL;
CTextureObject* p_teAmmoRocket = NULL;
CTextureObject* p_teWeaponRocketGun = NULL;
static CTextureObject* pteCrosshair = NULL;



static playerInterFace_t hud_pl;





static inline HEXColor getValueByColor(const int& current, const int& value)
{
	float fValue = float(current) / float(value);

	if (fValue >= 1.0f)                    return HUD_COL_PERFECT;
	if (fValue < 1.0f  && fValue > 0.5f) return HUD_COL_NORMAL;
	if (fValue <= 0.5f && fValue > 0.25f)  return HUD_COL_MIDDLE;
	if (fValue <= 0.25f)                   return HUD_COL_BAD;

	return COLOR_WHITE;
}

namespace GameHUD
{
	void setup() 
	{
		if (Network::isServer()) {
			//hud_player = entityManager.em_Players;			
			hud_pl.weapons = gameSession.getLocalPlayer()->pl_weapons;
			hud_pl.health  = &gameSession.getLocalPlayer()->m_iHealth;
			hud_pl.armor   = &gameSession.getLocalPlayer()->m_iArmor;
			hud_pl.weapon  = &gameSession.getLocalPlayer()->pl_ulCurrentWeapon;
			hud_pl.frags   = (int*)&gameSession.getLocalPlayer()->pl_uiNumKills;
		}
		else
		{
			//hud_pClPlayer = gameSession.getClientPlayer();
	
			hud_pl.weapons = gameSession.getClientPlayer()->cl_Weapons;
			hud_pl.health = &gameSession.getClientPlayer()->cl_Health;
			hud_pl.armor = &gameSession.getClientPlayer()->cl_Armor;
			hud_pl.weapon = &gameSession.getClientPlayer()->cl_CurrentWeapon;
			hud_pl.frags = (int*)&gameSession.getClientPlayer()->cl_kills;
		}


		pteCrosshair = new CTextureObject;
		// pteCrosshair = New<CTextureObject>();

		CFileName strFile = "Data\\Textures\\HUD\\Crosshairs\\Crosshair01.te";
		FileSetGlobalPatch(strFile);
		pteCrosshair->Load(strFile);
		pteCrosshair->renderingFlags |= TEXTURE_CLAMP;
		pteCrosshair->renderingFlags |= TEXTURE_CONST;
		pteCrosshair->Prepare();


	}

	void drawPlayerScore(CView* view) 
	{

		uint uiWidth  = view->GetWidth() / 640;
		uint uiHeight = (view->GetHeight() * 50) / 480;


		if (Game->gaType == GAME_FINISHED_MATCH) 
		{
			char* strWinner = stringCopyF("Player \"%s\" wins the deadmatch", gameSession.gs_playerScore[0].strPlayerName);
			drawText(pFontDisplay, uiWidth, uiHeight - 32, 1.0f, COLOR_YELLOW, strWinner);
			free(strWinner);
		}

		//drawSquare(0, 200, view->GetWidth(), 100, COLOR_ADD_ALPHA(COLOR_GREEN, 87));
		drawText(pFontDisplay, uiWidth, uiHeight, 1.0f, COLOR_WHITE, "Player name");
		drawText(pFontDisplay, uiWidth + 200, uiHeight, 1.0f, COLOR_WHITE, "Kills");

		uiHeight += 32;

		FOREACH_ARRAY(8) 
		{
			if (strlen(gameSession.gs_playerScore[i].strPlayerName) == 0) 
			{
				drawText(pFontDisplay, uiWidth, uiHeight, 1.0f, COLOR_WHITE, "---");
				drawText(pFontDisplay, uiWidth + 200, uiHeight, 1.0f, COLOR_WHITE, "---");
			}
			else 
			{
				drawText(pFontDisplay, uiWidth, uiHeight, 1.0f, COLOR_WHITE, gameSession.gs_playerScore[i].strPlayerName);
				char* str = stringCopyF("%d", gameSession.gs_playerScore[i].iKills);
				drawText(pFontDisplay, uiWidth + 200, uiHeight, 1.0f, COLOR_WHITE, str);
				free(str);
			}
			uiHeight += 32;
		}
	}

	void draw(CView* view) 
	{

		uint uiWidth = pViewPort->GetWidth();
		uint uiHeight = pViewPort->GetHeight();

		uiWidth /= 2;
		uiHeight /= 2;

		uint sizeW = 16 * pViewPort->GetWidth() / 1024;
		uint sizeH = 16 * pViewPort->GetWidth() / 1024;

		drawTextureToScreen(pteCrosshair, pViewPort, uiWidth, uiHeight, sizeW, sizeH, TEX_TRANSPARENT, COLOR_WHITE);

		int w = 0, h = 440;

		w *= view->GetWidth() / 640;
		h = (view->GetHeight() * h) / 480;

		// float fSize = 1.0f;


		// fSize *= (3.f / 4.f) * (float(pViewPort->GetWidth()) / float(pViewPort->GetHeight()));
		// printf("fSize = %f\n", fSize);
		// drawCenteredText(pFontDisplay, pViewPort, 480, 1.0f, COLOR_GREEN, FALSE, "HP = 100");	

		int health = ClampD(*hud_pl.health, 0);
		int armor = ClampD(*hud_pl.armor, 0);

		char* strHealth = stringCopyF("HP = %d%%", health);
		char* strArmor  = stringCopyF("AP = %d%%", armor);
		char* strFrags  = stringCopyF("Frags = %d/%d", *(char*)hud_pl.frags, gameSession.gs_maxFrag);
		
		char* strWeapon = stringCopyF("AMMO = %d%%", hud_pl.weapons->getAmmoByWeapon(*hud_pl.weapon));

		drawText(pFontDisplay, w, h - 64, 1.0f, 0xFC7F03FF, strFrags);
		drawText(pFontDisplay, w, h - 32, 1.0f, getValueByColor(armor, 100),  strArmor);
		drawText(pFontDisplay, w, h, 1.0f, getValueByColor(health, 100), strHealth);

		if (*hud_pl.weapon != WEAPON_MELE) {
			drawText(
				pFontDisplay,
				w + 200,
				h,
				1.0f,
				getValueByColor(hud_pl.weapons->getAmmoByWeapon(*hud_pl.weapon),
					hud_pl.weapons->getMaxAmmoByWeapon(*hud_pl.weapon)),
				strWeapon);
		}
		free(strArmor);
		free(strHealth);
		free(strWeapon);
		free(strFrags);

		if (hud_bShowScore)
			drawPlayerScore(view);


	}
	/*
	void drawClient(CView* view) 
	{
		int w = 0, h = 440;

		w *= view->GetWidth() / 640;
		h = (view->GetHeight() * h) / 480;

		// float fSize = 1.0f;


		// fSize *= (3.f / 4.f) * (float(pViewPort->GetWidth()) / float(pViewPort->GetHeight()));
		// printf("fSize = %f\n", fSize);
		// drawCenteredText(pFontDisplay, pViewPort, 480, 1.0f, COLOR_GREEN, FALSE, "HP = 100");	

		char* strHealth = stringCopyF("HP = %d%%", *hud_pl.health);
		char* strArmor = stringCopyF("AP = %d%%", *hud_pl.armor);
		char* strWeapon = stringCopyF("AMMO = %d%%", hud_pl.weapons->getAmmoByWeapon(*hud_pl.weapon));


		drawText(pFontDisplay, w, h - 32, 1.0f, getValueByColor(*hud_pl.armor, 100), strArmor);
		drawText(pFontDisplay, w, h, 1.0f, getValueByColor(*hud_pl.health, 100), strHealth);


		drawText(
			pFontDisplay,
			w + 200,
			h,
			1.0f,
			getValueByColor(hud_pl.weapons->getAmmoByWeapon(*hud_pl.weapon),
				hud_pl.weapons->getMaxAmmoByWeapon(*hud_pl.weapon)),
			strWeapon);

		free(strArmor);
		free(strHealth);
		free(strWeapon);
	}
	*/
	void clear(void) 
	{

		hud_pl.weapons = nullptr;
		hud_pl.health  = nullptr;
		hud_pl.armor   = nullptr;
		hud_pl.weapon  = nullptr;
		hud_pl.frags   = nullptr;
	    if (pteCrosshair)   
			delete pteCrosshair;
		
		pteCrosshair = NULL;

		//hud_player    = NULL;
		//hud_pClPlayer = NULL;
		//hud_pWeapons  = NULL;
	}
};