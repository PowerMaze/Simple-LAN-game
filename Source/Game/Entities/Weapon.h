#pragma once


enum EWeaponItem
{
	WI_PISTOL,
	WI_SHOTGUN,
	WI_DOUBLESHOTGUN,
	WI_AR,
	WI_MINIGUN,
	WI_ROCKETGUN,
	WI_GRANDEGUN,
	WI_ENERGYGUN,
	WI_ALIENSNIPER,
	WI_ABOMB,
	WI_NBOMB,
	WI_PRESSURECANNOC,
	WI_NANOSPLITTER,
	WI_HELLCOUP,
	WI_ARTEFACT
};

class CWeaponItem : public CPhysicsEntity
{

public:

	short wi_sValue;
	enum EWeaponItem wi_wtWeaponType;

	unsigned int wi_uiWeapon;

	double wi_dStarted;

	bool wi_bRespawn;

public:

	CWeaponItem();
	~CWeaponItem();

	void Update(void) override;

	void End() override;
	// save file
	void Load(CFileStream& stream);
	// load file
	void Save(CFileStream& stream);
	// init as default
	void Initialize(void) override;
	// generates touch event
	void generateTouchEvent(touchHandle_t* handle) override;
};
