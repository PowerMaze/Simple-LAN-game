#pragma once

/*
///////////////////////////////////////////////////////////

Yes i know, this exactly same class of camera in Game.dll,
see GCamera.h/GCamera.cpp

///////////////////////////////////////////////////////////
*/


#define MOVE_NONE   0x000000UL
#define MOVE_BACK   (1UL << 0)
#define MOVE_FRONT  (1UL << 1)
#define MOVE_LEFT   (1UL << 2)
#define MOVE_RIGHT  (1UL << 3)
#define MOVE_UP     (1UL << 4)
#define MOVE_DOWN   (1UL << 5)

enum ECameraMovementType
{
	CAM_UP,
	CAM_DOWN,
	CAM_RIGHT,
	CAM_LEFT,
	CAM_BACK,
	CAM_FRONT
};

const float yaw = -90.0f;
const float pitch = 0.0f;
const float speed = 1.0f;
const float sensinivity = 0.35f;
const float zoom = 90.0f;
//unsigned long b_MovementFlags = MOVE_NONE;

class CCamera
{
public:
	Vector3D m_position;
	Vector3D m_front;
	Vector3D m_up;
	Vector3D m_right;
	Vector3D m_WorldUp;
	Angle3D m_angleRotation;

	// angles
	float Yaw;
	float Pitch;
	// camera settings
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	bool bActive;

public:
	CCamera(void);
	~CCamera(void);

	void setCamera(Vector3D vPosition);

	glm::mat4 getMatrixView(void);

	void cameraMoveKeyboard(ECameraMovementType emt_Camtype, float fTime);
	void cameraMoveMouse(float x, float y);

	void Write(CFileStream& stream);
	void Read(CFileStream& stream);

public:
	void cameraUpDate(void);
};


void cameraInput(const double time);
void cameraEnd(void);

extern CCamera *pEditorCamera;