#include "stdAfx.h"

#include "EditorCamera.h"

unsigned long b_MovementFlags = MOVE_NONE;

// mouse speed (2.0 - 50.0)
static float fMouseSpeed = 10.0f;

CCamera::CCamera()
{
	m_position = glm::vec3(0.0f);
	m_front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f);
	m_right = glm::vec3(0.0f);
	m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	m_angleRotation = glm::vec3(0.0f);

	// angles
	Yaw = yaw;
	Pitch = pitch;
	// camera settings
	MovementSpeed = speed;// *fMouseSpeed;
	MouseSensitivity = sensinivity;
	Zoom = zoom;

	cameraUpDate();
}

CCamera::~CCamera()
{
	m_position      = glm::vec3(0.0f);
	m_front         = glm::vec3(0.0f);
	m_up            = glm::vec3(0.0f);
	m_right         = glm::vec3(0.0f);
	m_WorldUp       = glm::vec3(0.0f);
	m_angleRotation = glm::vec3(0.0f);

	// angles
	Yaw   = 0.0f;
	Pitch = 0.0f;
	// camera settings
	MovementSpeed    = 0.0f;
	MouseSensitivity = 0.0f;
	Zoom = 0.0f;

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

	stream.WriteToFile(&Yaw, sizeof(float));
	stream.WriteToFile(&Pitch, sizeof(float));
}

void CCamera::Read(CFileStream& stream)
{
	if (stream.checkID("CAMR"))
	{
		stream.ReadFromFile(&m_position, sizeof(glm::vec3));

		stream.ReadFromFile(&Yaw, sizeof(float));
		stream.ReadFromFile(&Pitch, sizeof(float));
	}
	else
	{
		PrintF("Unable to read camera position\n");
		return;
	}
}

glm::mat4 CCamera::getMatrixView()
{
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

void CCamera::cameraMoveKeyboard(ECameraMovementType emt_Camtype, float fTime)
{

	float fVelocity = (MovementSpeed * fMouseSpeed) * fTime;
	glm::vec3 vUp(0.0f, 1.0f, 0.0f);


	if (emt_Camtype == CAM_UP)
		m_position += vUp * fVelocity;

	if (emt_Camtype == CAM_DOWN)
		m_position -= vUp * fVelocity;

	if (emt_Camtype == CAM_RIGHT)
		m_position += m_right * fVelocity;

	if (emt_Camtype == CAM_LEFT)
		m_position -= m_right * fVelocity;

	if (emt_Camtype == CAM_FRONT)
		m_position += m_front * fVelocity;

	if (emt_Camtype == CAM_BACK)
		m_position -= m_front * fVelocity;

	//	PrintF("camera: %f %f %f\n", m_position.x, m_position.y, m_position.z);
}

void CCamera::cameraMoveMouse(float x, float y)
{
	x *= MouseSensitivity;
	y *= MouseSensitivity;

	Yaw += x;
	Pitch -= y;

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


void cameraInput(const double time) 
{
	SDL_Event e;
	//static int bTest = 0;
	while (SDL_PollEvent(&e))
	{

		if (e.type == SDL_MOUSEMOTION)
		{
			pEditorCamera->cameraMoveMouse((float)e.motion.xrel, (float)e.motion.yrel);
		}

		if (e.type == SDL_MOUSEWHEEL) 
		{		

			if (e.wheel.preciseY > 0.0f)
			{
				fMouseSpeed += 1.0f;
				fMouseSpeed = Clamp(fMouseSpeed, 0.001f, 100.0f);
			}
			else {
				fMouseSpeed -= 1.0f;
				fMouseSpeed = Clamp(fMouseSpeed, 0.001f, 100.0f);
			}

			//fMouseSpeed;

			//PrintF("x = %f y = %f\n", e.wheel.preciseX, e.wheel.preciseY);

		}


		if (e.type == SDL_KEYDOWN)
		{

			if (e.key.keysym.sym == SDLK_w)
				b_MovementFlags |= MOVE_FRONT;
			if (e.key.keysym.sym == SDLK_s)
				b_MovementFlags |= MOVE_BACK;
			if (e.key.keysym.sym == SDLK_a)
				b_MovementFlags |= MOVE_LEFT;
			if (e.key.keysym.sym == SDLK_d)
				b_MovementFlags |= MOVE_RIGHT;
			if (e.key.keysym.sym == SDLK_SPACE)
				b_MovementFlags |= MOVE_UP;
			if (e.key.keysym.sym == SDLK_RCTRL || e.key.keysym.sym == SDLK_LCTRL)
				b_MovementFlags |= MOVE_DOWN;
			if (e.key.keysym.sym == SDLK_e)
			{
				pEditorCamera->bActive = false;
				b_MovementFlags = MOVE_NONE;
				return;
			}
		}

		if (e.type == SDL_KEYUP)
		{
			if (e.key.keysym.sym == SDLK_w)
				b_MovementFlags &= ~MOVE_FRONT;
			if (e.key.keysym.sym == SDLK_s)
				b_MovementFlags &= ~MOVE_BACK;
			if (e.key.keysym.sym == SDLK_a)
				b_MovementFlags &= ~MOVE_LEFT;
			if (e.key.keysym.sym == SDLK_d)
				b_MovementFlags &= ~MOVE_RIGHT;
			if (e.key.keysym.sym == SDLK_SPACE)
				b_MovementFlags &= ~MOVE_UP;
			if (e.key.keysym.sym == SDLK_RCTRL || e.key.keysym.sym == SDLK_LCTRL)
				b_MovementFlags &= ~MOVE_DOWN;
		}

	}

	if (b_MovementFlags & MOVE_FRONT)
		pEditorCamera->cameraMoveKeyboard(CAM_FRONT, time);

	if (b_MovementFlags & MOVE_BACK)
		pEditorCamera->cameraMoveKeyboard(CAM_BACK, time);

	if (b_MovementFlags & MOVE_LEFT)
		pEditorCamera->cameraMoveKeyboard(CAM_LEFT, time);

	if (b_MovementFlags & MOVE_RIGHT)
		pEditorCamera->cameraMoveKeyboard(CAM_RIGHT, time);

	if (b_MovementFlags & MOVE_UP)
		pEditorCamera->cameraMoveKeyboard(CAM_UP, time);

	if (b_MovementFlags & MOVE_DOWN)
		pEditorCamera->cameraMoveKeyboard(CAM_DOWN, time);
}

void cameraEnd(void) 
{
	b_MovementFlags = MOVE_NONE;
}