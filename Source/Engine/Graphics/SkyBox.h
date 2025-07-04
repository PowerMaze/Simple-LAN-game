#pragma once

class CSkyBox 
{
public:

	// verticies of skybox
	std::vector<float> verticies;
	// names of skybox textures
	std::vector<std::string> strTexturesNameSkyBox;
	// cube map texture
	unsigned int cubeMap;
	// prepare for render
	bool prepareForRender;

	bool loadSuccess;

public:
	// default constrctor
	ENGINE_API CSkyBox();
	// default destructor
	ENGINE_API ~CSkyBox();

	ENGINE_API void makeSkyBox(std::vector<std::string>& textures);

	// load skybox from file
	ENGINE_API void Load(CFileStream& file);
	ENGINE_API void Save(CFileStream& file);

	ENGINE_API void Prepare(void);
	ENGINE_API void Render(CShader& sha);
	ENGINE_API void Clear();

private:
	unsigned int VAO, VBO, EBO;
};
