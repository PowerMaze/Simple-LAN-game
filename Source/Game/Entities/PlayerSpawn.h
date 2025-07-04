#pragma once

class CPlayerSpawn : public CEntityMain
{
public:

	unsigned int psp_uiStartedWeaponPack;	// setup player with this mask
	
	int psp_iHealth;   // setup health 
	int psp_iArmor;	   // setup armor 

	bool psp_bAllowed; // if we remach deadmach, do not allowed telergaph

//	double psp_dWaitTime;

public:

	CPlayerSpawn();
	~CPlayerSpawn() override;

	void SpawnPlayer(class CPlayer* ePlayer);

	void Save(CFileStream& file);
	void Load(CFileStream& file);

	void Initialize() override;
};