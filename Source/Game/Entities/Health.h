#pragma once

enum EHealthType
{
	HP_10,
	HP_25,
	HP_50,
	HP_100
};

#define ITEM_RESPAWN_DEFAULT 10.0

class CHealth : public CPhysicsEntity
{

public:

	short he_Value;
	enum EHealthType he_htHealthType;
	bool he_bRespawn;

	double he_dStarted;

public:

	CHealth();
	~CHealth();

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
