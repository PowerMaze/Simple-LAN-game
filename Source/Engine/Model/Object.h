#pragma once

//#include <Engine/EMain.h>

class CObject
{
public:

	unsigned int numData;
	std::vector<float> data;

	//CShader shader;
	unsigned textureID;

	ENGINE_API CObject(void);
	ENGINE_API CObject(unsigned int numdata, float* Points);

	ENGINE_API void  CreateObject(unsigned int numData, float* points, unsigned skin);

	ENGINE_API void LoadObject(std::string& strName);
	ENGINE_API void SaveObject(std::string& strName);

	ENGINE_API void Draw(CShader& shader);
	ENGINE_API void Clear(void);

private:
	unsigned int VBO, VAO;


};