#include "stdAfx.h"
#include "GCamera.h"

//#include "GMove.h"


static float yaw = -90.0f;
static float pitch = 0.0f;
static float speed = 1.0f;

static float fSpeedFactor = 10.0f;

static float sensinivity = 0.35f;
static float zoom = 90.0f;
static float Acceleration = 10.0f; // Прискорення
static float Deceleration = 5.0f; // Уповільнення

CCamera::CCamera() 
{
	m_position		= glm::vec3(0.0f);
	m_front			= glm::vec3(0.0f, 0.0f, -1.0f);
	m_up			= glm::vec3(0.0f);
	m_right			= glm::vec3(0.0f);
	m_WorldUp		= glm::vec3(0.0f, 1.0f, 0.0f);
	m_angleRotation = glm::vec3(0.0f);

	// angles
	Yaw		= yaw;
	Pitch	= pitch;
	// camera settings
	MovementSpeed		= speed;
	MouseSensitivity	= sensinivity;
	Zoom			    = zoom;

	pConsoleDataBase->addConsoleVariable(CVT_FLOAT, "yaw",          &yaw);
	pConsoleDataBase->addConsoleVariable(CVT_FLOAT, "pitch",        &pitch);
	pConsoleDataBase->addConsoleVariable(CVT_FLOAT, "speed",        &speed);
	pConsoleDataBase->addConsoleVariable(CVT_FLOAT, "sensinivity",  &sensinivity);
	pConsoleDataBase->addConsoleVariable(CVT_FLOAT, "zoom",         &zoom);
	pConsoleDataBase->addConsoleVariable(CVT_FLOAT, "fSpeedFactor", &fSpeedFactor);

	cameraUpDate();
}

CCamera::~CCamera()
{
	m_position		= glm::vec3(0.0f);
	m_front			= glm::vec3(0.0f);
	m_up			= glm::vec3(0.0f);
	m_right			= glm::vec3(0.0f);
	m_WorldUp		= glm::vec3(0.0f);
	m_angleRotation = glm::vec3(0.0f);
	m_velocity      = glm::vec3(0.0f);

	// angles
	Yaw				= 0.0f;
	Pitch			= 0.0f;
	// camera settings
	MovementSpeed		= 0.0f;
	MouseSensitivity	= 0.0f;
	Zoom				= 0.0f;

	cameraUpDate();
}

void CCamera::setCamera(Vector3D vPosition) 
{
	m_position = vPosition;

	cameraUpDate();
}

void CCamera::Write(CFileStream& stream)
{
	stream.WriteToFile("CAMR", sizeof(char) * 4);
	stream.WriteToFile(&m_position, sizeof(glm::vec3));

	stream.WriteToFile(&Yaw,	sizeof(float));
	stream.WriteToFile(&Pitch,	sizeof(float));
}

void CCamera::Read(CFileStream& stream)
{
	if (stream.checkID("CAMR"))
	{
		stream.ReadFromFile(&m_position, sizeof(glm::vec3));

		stream.ReadFromFile(&Yaw,	sizeof(float));
		stream.ReadFromFile(&Pitch, sizeof(float));
	}
	else 
	{
		Throw("Unable to read camera position\n");		
	}
}

glm::mat4 CCamera::getMatrixView()
{
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

void CCamera::cameraMoveKeyboard(ECameraMovementType emt_Camtype, float fTime) 
{
	float fVelocity = MovementSpeed * fTime * fSpeedFactor;
	glm::vec3 vUp(0.0f, 1.0f, 0.0f);

	if(emt_Camtype == CAM_UP)
		m_position += vUp * fVelocity;

	if (emt_Camtype == CAM_DOWN)
		m_position -= vUp * fVelocity;

	if (emt_Camtype == CAM_RIGHT)
		m_position += m_right * fVelocity;	

	if (emt_Camtype == CAM_LEFT)
		m_position -= m_right * fVelocity;
		
	if (emt_Camtype== CAM_FRONT)
		m_position += m_front * fVelocity;
		
	if (emt_Camtype == CAM_BACK)
		m_position -= m_front * fVelocity;


    m_position.y = 1.85f;
}

void CCamera::cameraMoveMouse(float x, float y)
{
	x *= MouseSensitivity;
	y *= MouseSensitivity;

	Yaw		+= x;
	Pitch	-= y;

	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;	

	cameraUpDate();
}

void CCamera::cameraUpDate() 
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	m_front = glm::normalize(front);
	
	m_right = glm::normalize(glm::cross(m_front, m_WorldUp)); 
	m_up = glm::normalize(glm::cross(m_right, m_front));
}
