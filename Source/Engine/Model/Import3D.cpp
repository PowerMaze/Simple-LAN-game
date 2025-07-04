#include <stdAfx.h>

#include "Import3D.h"
#include <Engine\Common\FileName.h>
#include <Engine\Common\PrintMessages.h>
#include <Engine\Common\Console.h>


void CFileOBJ::GetFaceFromOBJ(char* str, int* arr)
{
	// first we get the vertex indices
	arr[0] = atoi(str);

	// we set the texture and normal indices to 0
	arr[1] = -1;
	arr[2] = 0;

	// get texture indices
	char* p = str;
	while (*p != '/' && *p != '\0')
		*p++;

	if (*p == '/') {
		*p++;
		// if texture vertices are detected
		if (*p != '/')
			arr[1] = atoi(p);
	}

	if (arr[1] == -1) {
		//char e[] = "Not all faces contains UV coordinates\n";
		//throw e;
		Throw("Not all faces contains UV coordinates");
	}

	// get normal indices
	while (*p != '/' && *p != '\0')
		*p++;
	if (*p == '/')
	{
		*p++;
		arr[2] = atoi(p);
	}
}

void CFileOBJ::Load(const CFileName &StrFileName)
{
	// set absolute path like C:\\Dark-Head\\In-The-Power
	CFileName filename = StrFileName;



	FILE *f = fopen((const char*)filename, "rb");

	if (f == NULL)
	{
		Throw("Error loading file obj:\n%s", (const char*)filename);
    }
	
	glm::vec3 vPoints3d;
	glm::vec2 vPoints2d;

	objFace_t FaceObject;

	char matName[256];
	
	BOOL bFlagForFace = FALSE;
	long FilePosition = 0;
	
	while (1)
	{
		char lineHeader[128];

		if (bFlagForFace)
		{
			fseek(f, FilePosition, SEEK_SET);
			bFlagForFace = FALSE;
		}


		// read the first word of the line
		int res = fscanf(f, "%s", lineHeader);
		if (res == EOF)
			break;
		// read verticles
		if (strcmp(lineHeader, "v") == 0)
		{
			fscanf(f, "%f %f %f", &vPoints3d.x, &vPoints3d.y, &vPoints3d.z);
			//push verticles into array
			vertexList.push_back(vPoints3d);
		}
		// read texture verticles
		if (strcmp(lineHeader, "vt") == 0)
		{
			fscanf(f, "%f %f", &vPoints2d.x, &vPoints2d.y);
			//push verticles into array
			vertexTextureList.push_back(vPoints2d);
		}
		// read material name for face
		if (strcmp(lineHeader, FACE_KEY) == 0)
		{
			fscanf(f, "%s", lineHeader);
			strcpy(matName, lineHeader);
		}
		// read face
		if (strcmp(lineHeader, "f") == 0)
		{
			
			int Inds[3];
			int index = 0;
			while (1)
			{

				FilePosition = ftell(f);

				bFlagForFace = TRUE;

				int result = fscanf(f, "%s", lineHeader);

				if (atoi(lineHeader) == 0 || result == EOF) {
					break;
				}
				if (index > 2) {
					Throw("Error: Not all polygons are triangles!");
				}

				GetFaceFromOBJ(lineHeader, Inds);

				FaceObject.VertexIndex[index]  = Inds[0];
				FaceObject.TextureIndex[index] = Inds[1];
				FaceObject.NormalIndex[index]  = Inds[2];

				index++;

			}
	
			FaceObject.Name = matName;
			faceList.push_back(FaceObject);
		}
	}

	fclose(f);
	// read mtl data materials

	//CTFileName fnMtlName = filename.FileDir() + filename.FileName() + ".MTL";

	filename.ChangeEXT(".mtl");

	objMaterial_t material;
	f = fopen((const char*)filename, "r");
	if (f == NULL)
	{
		Throw("Cannot Loading .mtl file!\n");
	}
	int tag = -1;
	while (TRUE)
	{
		char lineHeader[256];
		// read the first word of the line
		int res = fscanf(f, "%s", lineHeader);
		if (res == EOF)
			break;

		// read material name
		if (strcmp(lineHeader, MATERIAL_KEY) == 0)
		{
			fscanf(f, "%s", lineHeader);
			strcpy(matName, lineHeader);
			tag++;

		}
		// read material color
		if (strcmp(lineHeader, "Kd") == 0)
		{
			fscanf(f, "%lf %lf %lf", &material.diff[0], &material.diff[1], &material.diff[2]);
			material.Name = matName;
			material.Tag = tag;
			materialList.push_back(material);

		}	
	}


	fclose(f);
	
}

objMaterial_t CFileOBJ::GetMaterialFromFace(const std::string &strName)
{
	for (unsigned i = 0; i < materialList.size(); i++)	
	{
		if (materialList[i].Name == strName)
		{
			return materialList[i];
		}
	}
	// if not found
	return { "", {0,0,0} , 0};
}

void CFileOBJ::GetRGB(byte &r, byte &g, byte &b, const double diffColor[3])
{
	r = (byte)(diffColor[0] * 255.0f);
	g = (byte)(diffColor[1] * 255.0f);
	b = (byte)(diffColor[2] * 255.0f);
}

CFileOBJ::~CFileOBJ()
{
	vertexList.clear();			
	//vertexNormalList.clear();		
	vertexTextureList.clear();
	faceList.clear();
	materialList.clear();
}