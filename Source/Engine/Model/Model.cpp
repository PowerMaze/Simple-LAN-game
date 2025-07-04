#include <stdAfx.h>
#include "Model.h"

#include <Engine\Common\FileName.h>
#include <Engine\Graphics\Shader.h>

#include "ImportOBJ.h"


void DumpModelData(CModelObject& mo);
void DumpModelData(COBJDecoder& obj);


CModelObject::CModelObject() 
{
	VAO = 0;
	VBO = 0;
	EBO = 0;
	textureId = teDefaultTextureForModels->texID;
	preparedToRender = false;
	mo_ModelMatrix = glm::mat4(1.0f);
	numDataVerticies = 0;

	mo_fTimeOfAnimationStart = 0;
	mo_fTimeOfAnimation = 0;
	mo_uiStart = 0;
	mo_uiEnd = 0;
	mo_uiIndexFrame = 0;
	mo_uiRenderingFlags = 0;

	mo_modCRC = 0;
}

void CModelObject::Clear()
{
	//mo_indicesID.clear();

	FOREACH_ARRAY(mo_Frames32.size())
	{
		delete[] mo_Frames32[i];
		mo_Frames32[i] = nullptr;
	}

	mo_modCRC = 0;

	mo_Frames32.clear();

	mo_AnimationInfos.clear();

	if (preparedToRender)
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
//		glDeleteBuffers(1, &EBO);
		preparedToRender = false;
	}	
}

CModelObject::~CModelObject() 
{
	Clear();
}

CModelObject::CModelObject(CFileName& strFileName) 
{
	preparedToRender = false;
	Load(strFileName);
}
CModelObject::CModelObject(CModelObject& model)
{
	Clear();
	//this->mo_indicesID = model.mo_indicesID;
	this->mo_ModelMatrix = model.mo_ModelMatrix;	
	this->numDataVerticies = model.numDataVerticies;
	this->textureId = model.textureId;

	unsigned int length = model.mo_Frames32.size();

	FOREACH_ARRAY(length) 
	{
		triangleData_t* tri = new triangleData_t[this->numDataVerticies];
		memcpy(tri, &model.mo_Frames32[i], sizeof(triangleData_t) * this->numDataVerticies);
		this->mo_Frames32.push_back(tri);
	}

	this->mo_AnimationInfos = model.mo_AnimationInfos;

}

CModelObject& CModelObject::operator=(CModelObject& model)
{
	Clear();

	//this->mo_indicesID = model.mo_indicesID;
	this->mo_ModelMatrix = model.mo_ModelMatrix;
	this->numDataVerticies = model.numDataVerticies;
	this->textureId = model.textureId;

	unsigned int length = model.mo_Frames32.size();

	FOREACH_ARRAY(length)
	{
		triangleData_t* tri = new triangleData_t[this->numDataVerticies];
		memcpy(tri, &model.mo_Frames32[i], sizeof(triangleData_t) * this->numDataVerticies);
		this->mo_Frames32.push_back(tri);
	}

	this->mo_AnimationInfos = model.mo_AnimationInfos;

	return *this;
}

unsigned long CModelObject::reportUsedMemory() 
{
	unsigned long lSize = 0;
	lSize += mo_strAnimName.size();
	lSize += mo_Frames32.size() * numDataVerticies * sizeof(triangleData_t);
	lSize += mo_AnimationInfos.size();
	
	FOREACH_STD_MAP(mo_AnimationInfos, std::string, modelLump_t) 
	{
		lSize += iter->first.size();
	}
	return lSize;
}

void CModelObject::PrepareToRender() 
{
	if (preparedToRender) return;
//	DumpModelData(*this);
	//mo_ModelMatrix = glm::mat4(1.0f);
	// Создаем буферные объекты/массивы
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
//	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// Загружаем данные в вершинный буфер
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, numDataVerticies * sizeof(triangleData_t), NULL, GL_DYNAMIC_DRAW);

	// Устанавливаем указатели вершинных атрибутов

	// Координаты вершин
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(triangleData_t), (void*)offsetof(triangleData_t, vertices));
	glEnableVertexAttribArray(0);
	// Нормали вершин
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(triangleData_t), (void*)offsetof(triangleData_t, texcoords));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	preparedToRender = true;

	PlayFirstAnimation();
}

void CModelObject::Render(CShader& sha) 
{
	
//	if (mo_uiRenderingFlags & MODEL_WEAPON)
//	{
//		glDepthRange(0.0, 0.1);
//	}

	// activate current texture
	glActiveTexture(GL_TEXTURE0);
	// binds this models VAO
	glBindVertexArray(VAO);
	// binds texture used by model
	glBindTexture(GL_TEXTURE_2D, textureId);
	// binds this model array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// get frame from frames array
	triangleData_t* triData = mo_Frames32[mo_uiIndexFrame];
	// push data verticies to accelerator
	glBufferSubData(GL_ARRAY_BUFFER, 0, numDataVerticies * sizeof(triangleData_t), (float*)&triData[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawArrays(GL_TRIANGLES, 0, numDataVerticies);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	/* animation process */
	
	double fTime = double(SDL_GetTicks64() * 0.001);

	if (mo_fTimeOfAnimationStart <= fTime)
	{
		mo_uiIndexFrame++;
		mo_fTimeOfAnimationStart += mo_fTimeOfAnimation;		
	}

	if (mo_uiIndexFrame == mo_uiEnd) 
	{
		mo_uiIndexFrame = mo_uiStart;
	}

	glDepthRange(0.0, 1.0);
}

void CModelObject::PlayFirstAnimation(void)
{
	//PlayAnimation("NONE");

	auto animToFind = mo_AnimationInfos.begin();

	if (animToFind == mo_AnimationInfos.end())
	{
		Error("Cannot find first animation in model!\n");
	}

	mo_strAnimName = animToFind->first;

	modelLump_t lump_t = animToFind->second;

	mo_fTimeOfAnimationStart = double(lump_t.fTimeAnimationSpeed) + double(SDL_GetTicks64() * 0.001);
	mo_fTimeOfAnimation = double(lump_t.fTimeAnimationSpeed);
	mo_uiStart = lump_t.uiStart;
	mo_uiEnd = lump_t.uiEnd;

	mo_uiIndexFrame = mo_uiStart;

}

void CModelObject::PlayAnimation(const char* animName) 
{

	char* strUpper = stringCopy(animName);

	if (strcmp(mo_strAnimName.c_str(), _strupr(strUpper)) == 0) {
		free(strUpper);
		return;
	}
		
	auto animToFind = mo_AnimationInfos.find(strUpper);

	if (animToFind == mo_AnimationInfos.end())
	{
		PrintF("Model doesn't contains this animation: %s, so we use first animation\n", strUpper);

		animToFind = mo_AnimationInfos.begin();

		if (animToFind == mo_AnimationInfos.end())
		{
			free(strUpper);
			Error("Cannot find first animation in model!\n");
		}
	}

	mo_strAnimName = animName;

	modelLump_t lump_t = animToFind->second;

	mo_fTimeOfAnimationStart = double(lump_t.fTimeAnimationSpeed) + double(SDL_GetTicks64() * 0.001);
	mo_fTimeOfAnimation = double(lump_t.fTimeAnimationSpeed);
	mo_uiStart = lump_t.uiStart;
	mo_uiEnd = lump_t.uiEnd;

	mo_uiIndexFrame = mo_uiStart;

	free(strUpper);
}

double CModelObject::GetAnimationLength(const char* animName)
{
	char* strUpper = stringCopy(animName);

	auto animToFind = mo_AnimationInfos.find(_strupr(strUpper));

	if (animToFind == mo_AnimationInfos.end())
	{
		PrintF("Model doesn't contains this animation: %s\n", strUpper);
		free(strUpper);
		return 0.00000;
	}

	free(strUpper);

	return animToFind->second.fTimeAnimationSpeed * double(animToFind->second.uiEnd - animToFind->second.uiStart);
}

char** CModelObject::GetAnimationsNames(void)
{
	char** strAnimNames;

	unsigned int iSize = mo_AnimationInfos.size();
	unsigned int i = 0;

	strAnimNames = (char**)malloc((iSize + 1) * sizeof(char));
	if (strAnimNames == NULL)
	{
		Warning("Cannot get list of animations!\n");
		return NULL;
	}

	FOREACH_STD_MAP(mo_AnimationInfos, std::string, modelLump_t) 
	{
		strAnimNames[i] = stringCopy(iter->first.c_str());
		i++;
	}
	strAnimNames[iSize] = NULL;

	return strAnimNames;
}


static void saveAnimationNamesOfModel(CModelObject* pmo, const CFileName& filename)
{
	char** strListAnimations = pmo->GetAnimationsNames();
	unsigned index = 0;

	if (strListAnimations == NULL)
	{
		Warning("Cannot get list of animations!\n");
		return;
	}

	FILE* file = fopen((const char*)filename, "w");

	if (file == NULL) {
		Warning("Cannot create file for save!\n");
		for (int i = 0; i < index + 1; i++)
		{
			free(strListAnimations[i]);
		}
		free(strListAnimations);
		return;
	}

	fprintf(file, "Lists of animations:\n");

	for (index = 0; strListAnimations[index] != NULL; index++) {
		fprintf(file, "	Animation name: %s\n", strListAnimations[index]);
	}

	fclose(file);

	for (int i = 0; i < index + 1; i++)
	{
		free(strListAnimations[i]);
	}
	free(strListAnimations);

}


void DumpModelData(CModelObject& mo)
{
	/*
	printf("################################################\n");
	printf("Face count %d\n", mo.mo_face.size());
	FOREACH_ARRAY(mo.mo_face.size())
	{
		printf("face %d ====================\n", i);
		printf("verticies: %f %f %f\n", mo.mo_face[i].vertices.x, mo.mo_face[i].vertices.y, mo.mo_face[i].vertices.z);
		printf("textureID: %f %f\n", mo.mo_face[i].texcoords.x, mo.mo_face[i].texcoords.y);
		//printf("indices: %d\n", mo.mo_indicesID[i]);

	}
	*/
	//PrintF("count indices %d\n", mo.mo_indicesID.size());
	//FOREACH_ARRAY(mo.mo_indicesID.size())
	//{
	//	printf("indices: %d\n", mo.mo_indicesID[i]);
	//}
	//printf("################################################\n");
}

void DumpModelData(COBJDecoder& obj)
{
	std::vector<unsigned int> indices;
	FOREACH_ARRAY(obj.faces.size())
	{
		indices.push_back(obj.faces[i].normalsID[0]);
		indices.push_back(obj.faces[i].normalsID[1]);
		indices.push_back(obj.faces[i].normalsID[2]);
	}
	printf("Size = %d\n", indices.size());

	FOREACH_ARRAY(indices.size())
	{
		printf("%d\n", indices[i]);
	}
	/*
	FOREACH_ARRAY(obj.vertices.size())
	{
		printf("v %f %f %f\n", obj.vertices[i].x, obj.vertices[i].y, obj.vertices[i].z);
	}
	printf("################################################\n");*//*
	FOREACH_ARRAY(obj.faces.size())
	{
		printf("f %d/%d/%d %d/%d/%d %d/%d/%d\n",
			obj.faces[i].verticesID[0], obj.faces[i].textureID[0], obj.faces[i].normalsID[0],
			obj.faces[i].verticesID[1], obj.faces[i].textureID[1], obj.faces[i].normalsID[1],
			obj.faces[i].verticesID[2], obj.faces[i].textureID[2], obj.faces[i].normalsID[2]
			);

	}
	printf("################################################\n");*/
}

void reportModelInfo(CModelObject* model) 
{
	PrintF("---------------------------------------------------------------\n");
	PrintF("Verticies count: %d\n", model->numDataVerticies);
	PrintF("Animations count: %d\n", model->mo_AnimationInfos.size());
	PrintF("Total frames: %d\n", model->mo_Frames32.size());
	PrintF("Animation info:\n");

	FOREACH_STD_MAP(model->mo_AnimationInfos, std::string, modelLump_t) 
	{
		modelLump_t& lump = iter->second;

		PrintF("	animation name: %s\n", iter->first.c_str());
		PrintF("	animation frames: %d\n", lump.uiEnd - lump.uiStart);
		PrintF("	animation speed: %f\n",lump.fTimeAnimationSpeed);
		PrintF("------------------------\n");
	}
	PrintF("End report of model info\n");
}


