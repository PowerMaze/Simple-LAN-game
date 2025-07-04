#include <stdAfx.h>

#include "RenderMain.h"

#include <Engine/Common/PrintMessages.h>
#include <Engine/Common/Console.h>
#include <Engine/Level/BaseWorld.h>
#include <Engine/Level/Brush.h>
#include <Engine/Level/Room.h>
#include <Graphics/Shader.h>
#include <Graphics/SkyBox.h>

//static bool bPrepared = false;

void renderPrepareWorld(CGameWorld* wo) 
{
	//if (bPrepared) return;

	FOREACH_ARRAY(wo->gw_numRooms)
	{
		for (int poly = 0; poly < wo->gw_rooms[i].ro_polygons.size(); poly++)
		{
			wo->gw_rooms[i].ro_polygons[poly].Prepare();
		}
	}

	FOREACH_ARRAY(wo->gw_numBrushes)
	{
		for (int poly = 0; poly < wo->gw_brushes[i]->m_brushPolygons.size(); poly++)
		{
			wo->gw_brushes[i]->m_brushPolygons[poly]->Prepare();
		}
	}

	if (wo->skybox != NULL) 
	{
		wo->skybox->Prepare();
	}

	shaModel.shaderSet("diffuse_texture", 0);
	shaSkyBox.shaderSet("diffuse_texture", 0);
	shaPolygon.shaderSet("diffuse_texture", 0);


	PrintF("World: %s ready for rendering\n", wo->getName());
	//bPrepared = true;
}

void renderView(CGameWorld* wo, CView* view) 
{
	glm::vec4 color = HEX_toOGL(wo->gw_bckgColour);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// 1 - render color
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 2 render skybox, if this extists
	if (wo->skybox != NULL)
	{
		wo->skybox->Render(shaSkyBox);
	}
	// 3 render world rooms (sectors)
	for (unsigned int i = 0; i < wo->gw_numRooms; i++)
	{
		wo->gw_rooms[i].RenderRoom(shaPolygon);
	}

	for (unsigned int i = 0; i < wo->gw_brushes.size(); i++) {
		wo->gw_brushes[i]->Render(shaPolygon);
	}
}

void renderClearWorld(CGameWorld* wo) 
{
	wo->Clear();
}