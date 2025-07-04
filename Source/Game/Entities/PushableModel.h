#pragma once

class CPushableModel : public CPhysicsEntity 
{
public:
	float pm_fMass;

	double pm_dLifeTime;
	double pm_dStartTime;

	Vector3D vStretch;
public:

	CPushableModel();

	~CPushableModel();

	void End() override;

	void Initialize() override;

	void Update() override;
};
