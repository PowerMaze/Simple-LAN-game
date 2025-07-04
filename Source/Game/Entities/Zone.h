#pragma once

/* Zone, aka triger in GoldSrc/Source engine, or TouchField in Serious Engine */
class CZone : public CPhysicsEntity
{
public:

	bool zn_bAllowPlayers;

public:

	CZone();
	~CZone(void);

	// save file
	void Load(CFileStream& stream);
	// load file
	void Save(CFileStream& stream);
};

/* hurt zone */
class CZoneHurt : public CZone
{
public:

	int zh_iDamage;
	enum EDamageType zh_dmt;

public:

	CZoneHurt();
	~CZoneHurt() override;

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

class CZonePush : public CZone 
{
public:

	CPhysicsEntity* ePush;

public:
};
