#pragma once

class CView 
{
public:
	// width of window
	uint m_width;
	// height of window
	uint m_height;

	// x coordinate were openGL must be start render
	short m_startX;
	// y coordinate were openGL must be start render
	short m_startY;

	// aspect ratio screen
	float m_aspectRatio;
	float m_far;
	float m_near;
	float m_FOV;

	float m_fAdjusterScale; // used for HOR+

	// projection matrix
	glm::mat4 m_ProjectionMatrix;
	// view matrix
	glm::mat4 m_viewMatrix;
	// ortographic matrix
	glm::mat4 m_OrtoMatrix;
	
	Vector3D m_ViewDir;

public:

	ENGINE_API CView();
	ENGINE_API CView(uint w, uint h, float _far = 1.0f, float _near = 100.0f, float fov = 90.0f);

	ENGINE_API void setUpView(uint w, uint h, float _far, float _near, float fov);

	ENGINE_API inline uint GetWidth(void)  { return m_width;  }
	ENGINE_API inline uint GetHeight(void) { return m_height; }

	ENGINE_API inline float GetAdjuster(void) {	return m_fAdjusterScale; }

	ENGINE_API void calcPerspectiveMatrix(void);
//	void calcOrthoMatrix(void);

	ENGINE_API glm::mat4 getProjection(void) {	return m_ProjectionMatrix; }
//	ENGINE_API inline glm::mat4 getOrto(void);
};


ENGINE_API extern CView* pViewPort;

ENGINE_API extern void setCurrentViewPort(CView* pView);