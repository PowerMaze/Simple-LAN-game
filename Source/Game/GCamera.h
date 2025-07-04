#pragma once

#include "Game/GMain.h"

enum ECameraMovementType 
{
	CAM_UP,
	CAM_DOWN,
	CAM_RIGHT,
	CAM_LEFT,
	CAM_BACK,
	CAM_FRONT
};

//static float yaw = -90.0f;
//static float pitch = 0.0f;
//static float speed = 1.0f;
//static float sensinivity = 0.35f;
//static float zoom = 90.0f;

class CCamera
{
public:
	Vector3D m_position;
	Vector3D m_front;
	Vector3D m_up;
	Vector3D m_right;
	Vector3D m_WorldUp;
	Angle3D m_angleRotation;
	Vector3D m_velocity; // Поточна швидкість

	// angles
	float Yaw;
	float Pitch;
	// camera settings
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

public:
	CCamera(void);
	~CCamera(void);

	void setCamera(Vector3D vPosition);

	glm::mat4 getMatrixView(void);

	void cameraMoveKeyboard(ECameraMovementType emt_Camtype, float fTime);
	void cameraMoveMouse(float x, float y);

	void Write(CFileStream& stream);
	void Read(CFileStream& stream);

private:
	void cameraUpDate(void);
//	glm::mat4 viewMatrix;
};

extern CCamera* camera;