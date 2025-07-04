#pragma once

enum EArmorType
{
	AR_10,
	AR_50,
	AR_100
};

#define ITEM_RESPAWN_DEFAULT 10.0

class CArmor : public CPhysicsEntity
{

public:

	short ar_Value;
	enum EArmorType ar_atArmorType;
	bool ar_bRespawn;

	double ar_dStarted;

public:

	CArmor();
	~CArmor();

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

	//	void applyTouchEvent(touchHandle_t* handle) override;
};