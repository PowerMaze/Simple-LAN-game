#include <stdAfx.h>
#include "ImportOBJ.h"

bool COBJDecoder::Load(CFileName& strFileName) 
{
	FILE* file;
	OBJFace_t objFace;

	glm::vec3 vector3;
	glm::vec2 vector2;

	file = fopen(strFileName.getCharPointer(), "r");

	if (file == NULL)
	{
		Warning("Cannot open file \"%s\" for create model\n", (const char*)strFileName);
		return false;
	}

	while (true) 
	{
		char lineHeader[128]; 

		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;
		if (strcmp(lineHeader, "v") == 0) 
		{
			fscanf(file, "%f %f %f\n", &vector3.x, &vector3.y, &vector3.z);
			vertices.push_back(vector3);
		}
		else if (strcmp(lineHeader, "vt") == 0) 
		{			
			fscanf(file, "%f %f\n", &vector2.x, &vector2.y);
			textureUV.push_back(vector2);
		}
		else if (strcmp(lineHeader, "vn") == 0) 
		{			
			fscanf(file, "%f %f %f\n", &vector3.x, &vector3.y, &vector3.z);
			normals.push_back(vector3);
		}
		else if (strcmp(lineHeader, "f") == 0) 
		{			
			int matches = fscanf
			(
				file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
				&objFace.verticesID[0],
				&objFace.textureID[0],
				&objFace.normalsID[0],
				
				&objFace.verticesID[1],
				&objFace.textureID[1],
				&objFace.normalsID[1],
				
				&objFace.verticesID[2],
				&objFace.textureID[2],
				&objFace.normalsID[2]
			);

			if (matches != 9) {
				PrintF("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			faces.push_back(objFace);
		}
	}

	fclose(file);

	return true;

}

void COBJDecoder::Clear() 
{
	vertices.clear();
	normals.clear();
	textureUV.clear();
	//faces.clear();
}