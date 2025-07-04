#include "stdAfx.h"

#include "../GEntities.h"

CPushableModel::CPushableModel()
{
	CPhysicsEntity::CPhysicsEntity();
	pm_fMass = 90.f;
	vStretch = VECTOR3(0.5f, 0.5f, 0.5f);
}
CPushableModel::~CPushableModel()
{
	PrintF("Here > ~CPushableModel()\n");
	CPhysicsEntity::End();
}

void CPushableModel::End()
{
	PrintF("Here > End()\n");
	CPhysicsEntity::End();

	entityManager.addEntityToDelete(this);
}

void CPushableModel::Initialize()
{
	vCollisionStretch = vStretch;
	m_fMassObject = pm_fMass;


	m_strClassName = "PushableModel";
	m_strName = "Unnamed";
	m_ulID = 3;

	//m_ulFlags |= PHYS_CHARACTER;

	m_strModel   = "Data\\Models\\Misc\\TestAnimations\\MorphCube.mo";
	//m_strModel = "Data\\Models\\Players\\Test\\Box.mo";
	//m_strTexture = "Data\\Models\\Players\\Test\\Box.te";
	m_strTexture = "Data\\Models\\Misc\\wood.te";

	pm_dLifeTime = 5.0;
	pm_dStartTime = (double(SDL_GetTicks64()) * 0.001) + pm_dLifeTime;

	InitAsModel();

	m_pmo.m_vScale = vStretch;

	InitCollisionInfo();
}

void CPushableModel::Update()
{
	if (m_bDead) return;

	if (pm_dStartTime < double(SDL_GetTicks64()) * 0.001)
	{
		End();
	}

	getMatrixOrientation();
}