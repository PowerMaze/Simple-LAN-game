#include <stdAfx.h>
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "Render.h"

#include <Engine/Common/FileName.h>
#include <Engine/Model/Model.h>
#include <Engine/Level/BaseWorld.h>
#include <Engine/Graphics/SkyBox.h>


struct draw 
{
	unsigned int VBO, VAO, EBO;
};

//struct draw Draw_t;


int MakeScreenShoot(const CFileName& strFileName)
{	
	int success = 0;

	unsigned int height = pViewPort->m_height;
	unsigned int width  = pViewPort->m_width;

	byte* pixelsToWrite = (byte*)malloc(3 * height * width);

	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixelsToWrite);	

	for (int y = 0; y < height / 2; ++y) 
	{
		for (int x = 0; x < width * 3; ++x) 
		{
			std::swap(pixelsToWrite[y * width * 3 + x], pixelsToWrite[(height - 1 - y) * width * 3 + x]);
		}
	}

	success = stbi_write_png((const char*)strFileName, width, height, 3, pixelsToWrite, width * 3);

	free(pixelsToWrite);

	return success;
}

//bool recursion = false;

CShader shader;
CShader skyShader;
CShader modelShader;
CModelObject* _pmo = NULL;
CTextureObject* texture = NULL;

void RenderSetupWorld(CGameWorld* world) 
{
	// prepare rendering polygons
	for (unsigned int iRoom = 0; iRoom < world->gw_rooms.size(); iRoom++)
	{
		for (unsigned int iPoly = 0; iPoly < world->gw_rooms[iRoom].ro_polygons.size(); iPoly++)
		{
			world->gw_rooms[iRoom].ro_polygons[iPoly].Prepare();
		}
	}

	for (unsigned int iBrush = 0; iBrush < world->gw_brushes.size(); iBrush++) 
	{
		world->gw_brushes[iBrush]->Prepare();
	}

	_pmo = new CModelObject;
	texture = new CTextureObject;
	
	CFileName strFileName = "C:\\Games\\In-the-Power\\Data\\Textures\\Bricks.te";
	//CFileName strModel = "Models\\Misc\\TestAnimations\\Cat.mo";

    //CFileName strFileName = "C:\\Games\\In-the-Power\\Data\\Models\\Plants\\Verba\\Tree02.te";
	//CFileName strModel = "Models\\Plants\\Verba\\Verba02.mo";

	_pmo->Load(CFileName("Data\\Models\\Misc\\TestAnimations\\MorphCube.mo"));
	texture->Load(strFileName);
	texture->Prepare();

	//modelShader.Create_t("C:\\Games\\In-the-Power\\System\\Shaders\\Model_Test\\vertShader.shr",
	//	"C:\\Games\\In-the-Power\\System\\Shaders\\Model_Test\\fragShader.shr");

	shaModel.shaderSet("diffuse_texture", 0);

	_pmo->textureId = texture->texID;
	_pmo->PrepareToRender();
	// create shaders for polygons
	shader.Create_t
	(
		"System\\Shaders\\RoomShaders_\\vertShader.shr",
		"System\\Shaders\\RoomShaders_\\fragShader.shr"
	);

	// set shaders texture
	shader.shaderSet("diffuse_texture", 0);

	// prepare rendering skybox, if this extists
	if (world->skybox != NULL) 
	{
		// create shader for skybox rendering
		skyShader.Create_t
		(
			"System\\Shaders\\SkyBoxShaders\\vertShader.shr",
			"System\\Shaders\\SkyBoxShaders\\fragShader.shr"
		);

		world->skybox->Prepare();
	}

}

//float change = 1.0f;

void RenderWorld(CGameWorld* world)
{
	// get color from world
	glm::vec4 color = HEX_toOGL(world->gw_bckgColour);

	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// 1 - render color
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 2 render skybox, if this extists
	if (world->skybox != NULL) 
	{
		world->skybox->Render(skyShader);
	}
	// 3 render world rooms (sectors)
	for (unsigned int i = 0; i < world->gw_numRooms; i++)
	{
		world->gw_rooms[i].RenderRoom(shader);
	}

	for (unsigned int i = 0; i < world->gw_brushes.size(); i++) {
		world->gw_brushes[i]->Render(shader);
	}
	// 4 render particles
	//glDisable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	// 5 render entities


	FOREACH_ARRAY(world->gw_allEntities.size()) 
	{
		CEntityMain* e = world->gw_allEntities[i];

		if (e == NULL) continue;

		if (e->IsValid() && e->m_ulFlags & ENTITY_MODEL)
		{
			RenderModel(&e->m_pmo, e->m_matModel, e->m_vPosition, COLOR_WHITE);
		}
	}

	//_pmo->PlayAnimation("ANIM_MORPH");
	RenderOneModel(NULL, COLOR_WHITE);

}

void RenderEND(CGameWorld* world) 
{
	world->Clear();
	shader.end();
	skyShader.end();
	//modelShader.end();

	if (_pmo)
		delete _pmo; 
	_pmo = NULL;
	if(texture)
		delete texture; 
	texture = NULL;
}

void RenderSetupOneModel(CModelObject* mo)
{
	if (mo == NULL) return;

	mo->PrepareToRender();
	//shader.shaderSet("diffuse_texture", 0);
}

void RenderModel(CModel* mo, Matrix4 &mat, Vector3D vPos, HEXColor color)
{
	if (mo == NULL) return;

	shaModel.use();
	shaModel.shaderSet("diffuse_texture", 0);

	shaModel.shaderSetMatrix("projection", pViewPort->getProjection());
	shaModel.shaderSetMatrix("view", pViewPort->m_viewMatrix);

	shaModel.shaderSetVector("modelColor", HEX_toOGL(color));

	mo->modelMatrix = glm::mat4(1.f);

	///mo->modelMatrix = glm::translate(mo->modelMatrix, vPos);
	mo->modelMatrix = glm::scale(mo->modelMatrix, mo->m_vScale);

	shaModel.shaderSetMatrix("model", mat);

	CModelObject* _pmoData = mo->getModel();

	_pmoData->Render(shaModel);

}

void RenderOneModel(CModelObject* mo, HEXColor color)
{

	if (mo == NULL) return;

	shaModel.use();
	shaModel.shaderSet("diffuse_texture", 0);

	shaModel.shaderSetMatrix("projection", pViewPort->getProjection());
	shaModel.shaderSetMatrix("view", pViewPort->m_viewMatrix);

	shaModel.shaderSetVector("modelColor", HEX_toOGL(color));

	mo->mo_ModelMatrix = glm::mat4(1.0f);

	shaModel.shaderSetMatrix("model", mo->mo_ModelMatrix);



	mo->Render(shaModel);
}