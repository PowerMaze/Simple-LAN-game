#pragma once

#include <Engine\EMain.h>
#include <Engine/Common/FileName.h>
#include <Engine/Model/Model.h>

typedef struct OBJFace_s 
{
	unsigned int verticesID[3];
	unsigned int textureID[3];
	unsigned int normalsID[3];
}OBJFace_t;


class COBJDecoder 
{
public:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureUV;

	std::vector<OBJFace_t> faces;

	COBJDecoder() = default;
	~COBJDecoder() { Clear(); }

	bool Load(CFileName& strFileName);

	bool EncodeMTL(CFileName& strFileName) {};

	ENGINE_API void Clear();

};

extern void proccessFrame(CModelAnimFrames* frame, COBJDecoder& objDecoder, unsigned int* numData);
//ENGINE_API extern CModelObject* CreateModel(COBJDecoder& obj, CFileName& strFileName);