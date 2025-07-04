#include <stdAfx.h>

#include "Model.h"

#include <Engine/Common/FileName.h>
#include <Engine/Common/PrintMessages.h>
#include <Engine/Common/Console.h>

#include <Engine/Graphics/Texture.h>

void proccessFrame(triangleData_t* frame, CFileOBJ& objDecoder, unsigned int *numData)
{

	unsigned int iCount = objDecoder.faceList.size();

	std::vector< triangleData_t> triData;

	for(unsigned int i = 0; i < iCount; i++)
	{
		triangleData_t tri[3];

		/* copy verticies coordinates to triangle data */
		tri[0].vertices = objDecoder.vertexList[objDecoder.faceList[i].VertexIndex[0] - 1];
		tri[1].vertices = objDecoder.vertexList[objDecoder.faceList[i].VertexIndex[1] - 1];
		tri[2].vertices = objDecoder.vertexList[objDecoder.faceList[i].VertexIndex[2] - 1];

		/* copy texture coordinates to triangle data */
		tri[0].texcoords = objDecoder.vertexTextureList[objDecoder.faceList[i].TextureIndex[0] - 1];
		tri[1].texcoords = objDecoder.vertexTextureList[objDecoder.faceList[i].TextureIndex[1] - 1];
		tri[2].texcoords = objDecoder.vertexTextureList[objDecoder.faceList[i].TextureIndex[2] - 1];

		/* copy normals to triangle data */
		// tri[0].normals = obj.normals[obj.faces[i].normalsID[0] - 1];
		// tri[1].normals = obj.normals[obj.faces[i].normalsID[1] - 1];
		// tri[2].normals = obj.normals[obj.faces[i].normalsID[2] - 1];

		/* add triangle data to vertex container */
		//triaData.push_back(tri[0]);
		//triaData.push_back(tri[1]);
		//triaData.push_back(tri[2]);

		triData.push_back(tri[0]);
		triData.push_back(tri[1]);
		triData.push_back(tri[2]);
	}

	if (numData != NULL) *numData = objDecoder.faceList.size() * 3;

	FOREACH_ARRAY(objDecoder.faceList.size() * 3)
	{
		frame[i] = triData[i];
	}

	triData.clear();
}

void createScriptTemplate(CFileName& fileName)
{
	FILE* file;

	fileName.ChangeEXT(".txt");

	file = fopen((const char*)fileName, "w");

	// '#' - this is comment
	fileName.ChangeEXT(".obj");

	fprintf(file, "FRAMES_COUNT 1\n"); // count of animations
	//fprintf(file, "BEGIN                  #always starts of key-word BEGIN \n"); // always starts of key-word BEGIN
	fprintf(file, "ANIMATION ANIM_DEFAULT\n"); // name of animations
	fprintf(file, "SPEED 1.0\n"); // speed of animation
	fprintf(file, "%s\n", (const char*)fileName); // patch of frame
	fprintf(file, "END_FRAMES\n");
	fprintf(file, "END\n"); // key-word end of file
	fclose(file);
}

CModelObject* CreateModel(CFileOBJ& obj, CFileName& strFileName)
{
	CModelObject* mo = nullptr;

	obj.Load(strFileName);		

	// create new object model
	mo = new CModelObject;

	modelLump_t moLump_t;

	moLump_t.fAnimTimeCurrent = 0.0f;
	moLump_t.fTimeAnimationSpeed = 1.0f;
	moLump_t.uiStart = 0;
	moLump_t.uiEnd = 1;

	mo->mo_AnimationInfos["ANIM_DEFAULT"] = moLump_t;

	unsigned length = obj.faceList.size() * 3;

	triangleData_t* triData = new triangleData_t[length];
	memset(triData, 0, length * sizeof(triangleData_t));

	proccessFrame(triData, obj, &mo->numDataVerticies);

	createScriptTemplate(strFileName);

	mo->mo_Frames32.push_back(triData);
	mo->mo_ModelMatrix = glm::mat4(1.0f);

	return mo;
}

void addAnimations(CModelObject* model, const char* fModelScript)
{
	if (model == nullptr)			return;
	if (fModelScript == nullptr)	return;

	std::string strAnimScriptPath = strGlobalPatch + std::string(fModelScript);

	FILE* file = fopen(strAnimScriptPath.c_str(), "r");

	if (file == NULL)
	{
		PrintF("Error loading model script!!!\n");
		return;
	}

	CModelObject safeTempModel(*model);
	unsigned int iStartFrame = 0;
	unsigned int iEndFrame = 0;
	unsigned int iCount = 0;
	unsigned int iCountFrames = 0;
	modelLump_t  lump_t = {0,0,0,0};

	model->Clear();



	char lineHeader[128];
	fscanf(file, "%s", lineHeader);

	if (strcmp(lineHeader, "FRAMES_COUNT") != 0) 
	{
		PrintF("Error loading model script: key word FRAMES_COUNT not found\n");
		fclose(file);
		*model = safeTempModel;
		return;
	}

	fscanf(file, "%d", &iCount);


	while (strcmp(lineHeader, "END") != 0) 
	{

		fscanf(file, "%s", lineHeader);

		if (strcmp(lineHeader, "ANIMATION") == 0) 
		{
			fscanf(file, "%s", lineHeader);

		    std::string strAnimName = _strupr(lineHeader);		

			fscanf(file, "%s", lineHeader);

			if (strcmp(lineHeader, "SPEED") != 0)
			{
				PrintF("Error loading model script: key word SPEED not found\n");
				fclose(file);
				*model = safeTempModel;
				return;
			}

			fscanf(file, "%f", &lump_t.fTimeAnimationSpeed);

			fscanf(file, "%s", lineHeader);

			while (strcmp(lineHeader, "END_FRAMES") != 0) 
			{
				CFileName strFileName;
				CFileOBJ obj;

				strFileName = lineHeader;

				//strFileName.strFileName = /*strGlobalPatch + std::string(lineHeader);*/


				try {
					obj.Load(strFileName);
				}
				catch (char* err) {
					Warning("addAnimations(): Cannot load \"%s\"\n%s", (const char*)strFileName, err);
					*model = safeTempModel;
					safeTempModel.Clear();
					fclose(file);
					return;
				}

				/*
				if (!obj.Load(strFileName))
				{
					Warning("addAnimations(): Cannot load \"%s\"\n", (const char*)strFileName);
					*model = safeTempModel;
					safeTempModel.Clear();
					fclose(file);
					return;
				}*/

				triangleData_t* triData = new triangleData_t[obj.faceList.size() * 3];

				proccessFrame(triData, obj, &model->numDataVerticies);

				model->mo_Frames32.push_back(triData);

				iCountFrames++;

				fscanf(file, "%s", lineHeader);
				
			}

			lump_t.uiStart = lump_t.uiEnd;
			lump_t.uiEnd += iCountFrames;

			iCountFrames = 0;

			model->mo_AnimationInfos.insert(std::pair<std::string, modelLump_t>(strAnimName, lump_t));
		}		
	}
	fclose(file);

}

void CModelObject::Save(const CFileName& strFileName)
{
	CFileStream file;
	unsigned int uiCountData;

	if (!file.open(strFileName, 0, 0))
	{
		PrintF("Cannot save model file: \"%s\"\n", (const char*)strFileName);
		return;
	}

	// write main ID file
	file.WriteToFile("MODL", sizeof(char) * 4);
	// write version of model ".mo"
	file.writeVersion(MODEL_VERSION);
	// write count of verticies data
	file.WriteToFile(&numDataVerticies, sizeof(unsigned int));
	// write count of frames
	file.WriteToFile("FRMS", sizeof(char) * 4);
	uiCountData = mo_Frames32.size();
	file.WriteToFile(&uiCountData, sizeof(unsigned int));
	// write all frames

	//unsigned lengthOfData = numDataVerticies * uiCountData;

	FOREACH_ARRAY(uiCountData)
	{
		triangleData_t* data = mo_Frames32[i];

		for (unsigned j = 0; j < numDataVerticies; j++) 
		{
			file.WriteToFile(&data[j].vertices.x, sizeof(float));
			file.WriteToFile(&data[j].vertices.y, sizeof(float));
			file.WriteToFile(&data[j].vertices.z, sizeof(float));

			file.WriteToFile(&data[j].texcoords.x, sizeof(float));
			file.WriteToFile(&data[j].texcoords.y, sizeof(float));
		}
	}
	// write animation ID data
	file.WriteToFile("ANIM", sizeof(char) * 4);
	// write count of animations
	uiCountData = mo_AnimationInfos.size();
	file.WriteToFile(&uiCountData, sizeof(unsigned int));
	// write animation
	FOREACH_STD_MAP(mo_AnimationInfos, std::string, modelLump_t) 
	{
		file.WriteString(iter->first);

		file.WriteToFile(&iter->second.uiStart,             sizeof(unsigned int));
		file.WriteToFile(&iter->second.uiEnd,               sizeof(unsigned int));
		file.WriteToFile(&iter->second.fTimeAnimationSpeed, sizeof(float));
		file.WriteToFile(&iter->second.fAnimTimeCurrent,    sizeof(float));

	}

	file.closeFile();
}


void CModelObject::Load(const CFileName& strFileName)
{

	Clear();

	CFileStream file;
	unsigned int uiNumData;
	//unsigned int indicesData;

	if (!file.open(strFileName, FILE_BIN, FILE_READ))
	{
		PrintF("Cannot open model file: \"%s\"\n", (const char*)strFileName);
		return;
	}

	if (!file.checkID("MODL"))
	{
		PrintF("Cannot read model file: %s\nInvalid file format\n", (const char*)strFileName);
		file.closeFile();
		return;
	}

	if (!file.checkVersion(MODEL_VERSION))
	{
		PrintF("Cannot read model file, invalid version .mo!\n");
		file.closeFile();
		return;
	}
	// load count of verticies
	file.ReadFromFile(&numDataVerticies, sizeof(unsigned int));

	// check id ANIM
	if (!file.checkID("FRMS"))
	{
		PrintF("Cannot read model file, no frames!!!\n");
		file.closeFile();
		return;
	}

	file.ReadFromFile(&uiNumData, sizeof(unsigned int));

	FOREACH_ARRAY(uiNumData)
	{
		triangleData_t* data = new triangleData_t[numDataVerticies];

		for (unsigned int j = 0; j < numDataVerticies; j++) 
		{
			file.ReadFromFile(&data[j].vertices.x, sizeof(float));
			file.ReadFromFile(&data[j].vertices.y, sizeof(float));
			file.ReadFromFile(&data[j].vertices.z, sizeof(float));

			file.ReadFromFile(&data[j].texcoords.x, sizeof(float));
			file.ReadFromFile(&data[j].texcoords.y, sizeof(float));
		}
		mo_Frames32.push_back(data);
	}

	// check id ANIM
	if (!file.checkID("ANIM"))
	{
		PrintF("Cannot read model file, no animations!!!\n");
		file.closeFile();
		return;
	}
	// load count of frames
	file.ReadFromFile(&uiNumData, sizeof(unsigned int));
	// for each frame
	FOREACH_ARRAY(uiNumData)
	{

		std::string animName = file.ReadStringFromFile();
		modelLump_t lump_t;

		file.ReadFromFile(&lump_t.uiStart, sizeof(unsigned int));
		file.ReadFromFile(&lump_t.uiEnd, sizeof(unsigned int));
		file.ReadFromFile(&lump_t.fTimeAnimationSpeed, sizeof(float));
		file.ReadFromFile(&lump_t.fAnimTimeCurrent, sizeof(float));

		mo_AnimationInfos.insert(std::pair<std::string, modelLump_t>(animName, lump_t));
	}


	file.closeFile();

	textureId = teDefaultTextureForModels->texID;

#ifndef NDEBUG
	PrintF("Model loaded succesfull\n");
#endif
}