#pragma once

#include <Engine/Model/Model.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Graphics/Shader.h>


struct objectDraw 
{
	//unsigned int VBO, VAO/*, EBO*/;
	//float* vertices;
	CShader* shader;
	CModelObject* model;
	CTextureObject* texture;
};


ENGINE_API extern void RenderSetupScene(struct objectDraw* oD);
ENGINE_API extern void RenderToScreen(objectDraw_t* oD);
ENGINE_API extern void RenderEnd(objectDraw_t* oD);

//ENGINE_API extern void RenderModel(CModelObject* model);

// function to prepare entire world for rendering
ENGINE_API extern void RenderSetupWorld(CGameWorld* world);
// function for render world, includes: skybox, rooms, models, entites, etc.
ENGINE_API extern void RenderWorld(CGameWorld* world);
// cleanup rendering world
ENGINE_API extern void RenderEND(CGameWorld* world);
// setup model object
ENGINE_API extern void RenderSetupOneModel(CModelObject* mo);
// render model object
ENGINE_API extern void RenderOneModel(CModelObject* mo, HEXColor color);

///////////////////////////////////////////////////////////////////////////

ENGINE_API extern void RenderModel(CModel* mo, Matrix4 &mat, Vector3D vPos, HEXColor color);

//ENGINE_API extern void RenderModels(CModelObject** mos);
// make screenshot of view
ENGINE_API extern int MakeScreenShoot(const CFileName& strFileName);
