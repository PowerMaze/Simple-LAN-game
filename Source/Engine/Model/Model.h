#ifndef MODEL_H
#define MODEL_H
#pragma once

//#include <Engine/EMain.h>
//#include <Engine/Common/FileIO.h>
#include <Engine/Common/FileName.h>
//#include "ImportOBJ.h"
#include "Import3D.h"

#define ANIMATION_CONTINUED 0x001
#define ANIMATION_PAUSED    0x002
#define ANIMATION_PLAY      0x004

//extern bool iMO_LerpAnimations = false;
//extern int iMO_QualityType = 1;

/*
/////////////////////////////////////////////////
Version of supported models

version 0 first version, no animation, no LODS, no Tags
version 1 (simple, with frames, no LODS (level of detail), no Tags) << current version
version 2 (with Tags)
version 3 (with LODS)
version 4 (with normals)

/////////////////////////////////////////////////
*/
#define MODEL_VERSION 1
#define MAX_LOD 16
//#define MAX_ANIMATION_FRAMES 512

/*
/////////////////////////////////////////////////

For openGL data triangle. Why texcoords is a
vector 3? Well because for EBO buffer

/////////////////////////////////////////////////
*/
typedef struct triangleData_s 
{
	glm::vec3 vertices;	
	glm::vec2 texcoords;
//	glm::vec3 normals;
}triangleData_t;

typedef struct modelLump_s
{
	unsigned int uiStart; //offset to start frame
	unsigned int uiEnd;   //offset to end frame

	float fTimeAnimationSpeed; // animation speed sec/frames
	float fAnimTimeCurrent;
}modelLump_t;

/*
/////////////////////////////////////////////////

Basic model class for game

/////////////////////////////////////////////////
*/
class CModelObject
{
public:
	/* all frames in model */
	std::vector<triangleData_t*> mo_Frames32;
	/* count of verticies */
	unsigned int numDataVerticies;
	/* texture ID for model draw, UV map */
	unsigned int textureId;	
	/* texture ID for normal mapping */
//	unsigned int normalTextureId;
 	/* texture ID for normal mapping */
//	unsigned int bumpTextureId;
	/* view model matrix */
	glm::mat4 mo_ModelMatrix;

/************* Animation control *************/
	/* map of animation */
	std::map<std::string, modelLump_t> mo_AnimationInfos;
	/* time when animation starts */
	double mo_fTimeOfAnimationStart;
	/* time of animation (sec/per frame) */
	double mo_fTimeOfAnimation;
	/* index frame to start */
	unsigned int mo_uiStart;
	/* index frame to end */
	unsigned int mo_uiEnd;
	/* index of current frame */
	unsigned int mo_uiIndexFrame;
	/* name of current animation */
	std::string mo_strAnimName;

	unsigned int mo_uiRenderingFlags;
public:
	/************* Constructors *************/
	// default constructor
	ENGINE_API CModelObject(void);
	// constructor from filename
	ENGINE_API CModelObject(CFileName& strFileName);
	// constructor from model object
	ENGINE_API CModelObject(CModelObject& model);
	// destructor
	ENGINE_API ~CModelObject(void);

	/************* Model IO *************/
	// load model from file .mo
	ENGINE_API void Load(const CFileName& strFileName);
	// save model to file .mo
	ENGINE_API void Save(const CFileName& strFileName);

	ENGINE_API unsigned long reportUsedMemory(void);

	//void fromObject3D(CFileName& strFileName);

	/************* Rendering control *************/
	// prepare for render model
	void PrepareToRender(void);
	// render model view
	void Render(CShader& sha);
	// cope model
	void CopyModel(CModelObject& model);
	// rotate model
	void RotateByAngle(float* fAngle);
	// delete model object
	ENGINE_API void Clear(void);

	//CModelObject& operator=(CModelObject* model);
	ENGINE_API CModelObject& operator=(CModelObject& model);

	/************* Animation control *************/
	// plays animation
	ENGINE_API void PlayAnimation(const char* animName);
	// reset animation to first frame
	ENGINE_API void ResetCurrentAnimation(void);
	// stop playing animation
	ENGINE_API void StopAnimation(void);
	// play animation but reverse
	ENGINE_API void PlayReverseAnimation(const char* animName);
	// get current animation name
	ENGINE_API inline const char* GetCurrentAnimName(void) {	return mo_strAnimName.c_str(); }
	// get animation length in time
	ENGINE_API double GetAnimationLength(const char* animName);
	// get full animations names
	ENGINE_API char** GetAnimationsNames(void);

	ENGINE_API void PlayFirstAnimation(void);

	ENGINE_API inline unsigned int getCRC() {	return mo_modCRC; }

private:
	/* for render model */
	unsigned int VAO, VBO, EBO;
	/* if model prepared for rendering */
	bool preparedToRender;
public:
	unsigned int mo_modCRC;
};

/*
/////////////////////////////////////////////////

Basic model instance for entites, objects, etc..

/////////////////////////////////////////////////
*/
class CModel 
{
public:
	// pointer to model object
	CModelObject* m_pmo;
	// filename of model path
	CFileName m_strFileToModel;

	Matrix4 modelMatrix;

	Vector3D m_vScale;

	bool m_bUsed;
	// texture ID
	unsigned int m_uiTexID;
	// texture Bump ID
	unsigned int m_uiRenderingFlags;

public:
	// default constructor
	ENGINE_API CModel();
	// destructor
	ENGINE_API ~CModel() { Clear(); };
	// clear object
	ENGINE_API void Clear(void);
	// get model object
	ENGINE_API CModelObject* getModel(void) {	return m_pmo;	}
	// load model from file
	ENGINE_API void Load(const CFileName& strFileName);
	// set model data
	ENGINE_API void SetModel(CModelObject* mo);
	// set texture data (loads from file)
	ENGINE_API void SetTexture(const CFileName& strFileName);
	// set texture
	ENGINE_API void SetTexture(CTextureObject* tex);
	// set texture by id
	ENGINE_API void SetTexture(unsigned int texID);
	// prepare for rendering
	ENGINE_API void SetUpModel(void);
	// render model
	//void RenderModel(void);
	// play animation
	ENGINE_API void PlayAnimation(const char* strAnimName);
	
	// scale all size by ratio
	ENGINE_API void ScaleAll(float fRatio);
	// scale all size by vector 3
	void ScaleAll(glm::vec3& fRatio);
	// scale size by X
	void ScaleX(float fXRatio);
	// scale size by Y
	void ScaleY(float fYRatio);
	// scale size by Z
	void ScaleZ(float fZRatio);

	ENGINE_API unsigned int getCRCTexture() {	return m_texCRC; }
	/*********  Animation control *********/

private:

	unsigned int m_texCRC;

};


ENGINE_API extern CModelObject* CreateModel(CFileOBJ& obj, CFileName& strFileName);

ENGINE_API extern void addAnimations(CModelObject* model, const char* fModelScript);
ENGINE_API extern void reportModelInfo(CModelObject* model);

#endif