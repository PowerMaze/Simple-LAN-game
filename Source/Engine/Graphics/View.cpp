#include <stdAfx.h>
#include "View.h"

CView::CView() 
{
	m_width = 0;
	m_height = 0;

	m_aspectRatio = 0.0f;
	m_far = 0.0f;
	m_near = 0.0f;
	m_FOV = 90.0f;

	m_startX = 0;
	m_startY = 0;

	m_fAdjusterScale = 1.0F;

	m_ProjectionMatrix = glm::mat4(1.0f);
	m_OrtoMatrix       = glm::mat4(1.0f);
	m_viewMatrix	   = glm::mat4(1.0f);
	m_ViewDir          = VECTOR3_NONE;
}

CView::CView(uint w, uint h, float _far, float _near, float fov) 
{
	m_width = w;
	m_height = h;

	m_aspectRatio = (float)m_width / (float)m_height;

	m_far = _far;
	m_near = _near;
	m_FOV = fov;

	m_ProjectionMatrix = glm::mat4(1.0f);
	m_viewMatrix = glm::mat4(1.0f);

	calcPerspectiveMatrix();

	m_OrtoMatrix = glm::ortho(0.0f, float(m_width), float(m_height), 0.0f, -1.0f, 1.0f);


	m_fAdjusterScale = (4.f / 3.f) * ((float)m_height / (float)m_width);
}



void CView::setUpView(uint w, uint h, float _far, float _near, float fov) 
{
	m_width = w;
	m_height = h;

	m_aspectRatio = (float)m_width / (float)m_height;

	m_far = _far;
	m_near = _near;
	m_FOV = fov;

	m_ProjectionMatrix = glm::mat4(1.0f);
	m_viewMatrix = glm::mat4(1.0f);

	//glViewport(0, 0, m_width, m_height);

	calcPerspectiveMatrix();

	m_OrtoMatrix = glm::ortho(0.0f, float(m_width), float(m_height), 0.0f, -1.0f, 1.0f);

	m_fAdjusterScale = (4.f / 3.f) * ((float)m_height / (float)m_width);
}

void CView::calcPerspectiveMatrix() 
{
	m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_aspectRatio, m_far, m_near);
}
/*
inline glm::mat4 CView::getOrto(void) 
{
	return glm::ortho(0.0f, float(m_width), float(m_height), 0.0f, -1.0f, 1.0f);
}*/

void setCurrentViewPort(CView* pView) 
{
	if (pView == NULL) return;

	glViewport(pView->m_startX, pView->m_startY, pView->m_width, pView->m_height);
}