#pragma once

#define DISTANCE_BULLET 2048.f

class CBullet :  public CPhysicsEntity
{

public:

	int bl_iDamage;
	CEntityMain* entityFrom; // who launhced
	float bl_distance;

public:

	CBullet();
	~CBullet();
	//void End() override;
public:

	void Initialize(void) override;

	void LaunchBullet(void);

	CEntityMain* getTarget(void) override;

	void generateTouchEvent(touchHandle_t* handle) override;

};

