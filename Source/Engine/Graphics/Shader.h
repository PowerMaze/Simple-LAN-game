#pragma once

/*
//////////////////////////////////////////////////////////////

Main shader class used in the engine. In future can be adapted
for Vulkan Api, but now.. we use OpenGL 4.0

//////////////////////////////////////////////////////////////
*/
class ENGINE_API CShader
{
public:
	// default constructor
	CShader(void) { shID = 0; }
	// constructor
	CShader(const char* strPathVert, const char* strPathFrag);
//	CShader(const CShaderRequest& s);
	// destructor
	~CShader(void);

	// create shader
	void Create_t(const char* strPathVert, const char* strPathFrag);

	void CreateFromString(const char* VertShader, const char* FragShader);
	// use shader
	inline void use(void);
	// destroy shader
	inline void end(void);

	// set shaders value
	inline void shaderSet(const char* strName, const int iValue);
	inline void shaderSet(const char* strName, const float fValue);
	inline void shaderSet(const char* strName, const double dValue);
	// set shaders vectors
	inline void shaderSetVector(const char* strName, const float fx, const float fy);
	inline void shaderSetVector(const char* strName, const float fx, const float fy, const float fz);
	inline void shaderSetVector(const char* strName, const float fx, const float fy, const float fz, const float fw);
	// set shaders vectors
	inline void shaderSetVector(const char* strName, const glm::vec2& vector2);
	inline void shaderSetVector(const char* strName, const glm::vec3& vector3);
	inline void shaderSetVector(const char* strName, const glm::vec4& vector4);
	// set shaders matrix
	inline void shaderSetMatrix(const char* strName, const glm::mat3& matrix3);
	inline void shaderSetMatrix(const char* strName, const glm::mat4& matrix4);

	inline void shaderSet(uint& ID) { this->shID = ID; }

	//CShader &operator=(const CShader sha);

public:
	// program id
	unsigned int shID;
	// check compile errors
	void checkErrors(const unsigned int prog, const char* strError);
};


/* shader for put texture to screen (HUD, GUI, etc..) */
extern CShader shaTexture;
/* shader for blending screen and fill color */
extern CShader shaFillColor;
/* shader for rendering model */
extern CShader shaModel;
/* shader for rendering text font */
extern CShader shaText;
/* shader for rendering brush/room */
extern CShader shaPolygon;
/* shader for rendering skybox (this can be sets when world prepare to rendering) */
extern CShader shaSkyBox;