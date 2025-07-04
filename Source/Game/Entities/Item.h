#pragma once

/*
////////////////////////////////////////

		Item main class

////////////////////////////////////////
*//*
class CItem : public CPhysicsEntity
{

public:

	short he_Value;
	enum EHealthType he_htHealthType;

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
};
*/