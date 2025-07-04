#pragma once

class CExplosion : public CPhysicsEntity 
{
public:

	float ex_fRadius;   // radius of damage
	int ex_iMaxDamage;  // max damage amout

	int ex_iOnce;       // explosion appears once

	Vector3D ex_vDirection; 

	float ex_fForce;


	CEntityMain* ex_entityReason; // who caused explode, player or explosion barrels?

public:

	CExplosion();
	~CExplosion();

	void End() override;

public:

	void Update(void) override;
	// init as default
	void Initialize(void) override;
	// generates touch event
	void generateTouchEvent(touchHandle_t* handle) override;

};
