#include <stdAfx.h>

#include "RenderScene.h"
#include <Level\BaseWorld.h>
#include <Level\Brush.h>
#include <Level\Room.h>
#include "SkyBox.h"
/*
void RenderViewScene(CGameWorld* wo, CView* view, Matrix4& matLookUp)
{
	if (wo   == nullptr)   return;
	if (view == nullptr)   return;


	glm::vec4 color = HEX_toOGL(wo->gw_bckgColour);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (wo->skybox != NULL)  wo->skybox->Render(shaModel);

	///////////////////////////////////////////////////////////////
	//               Render world ROOM
	///////////////////////////////////////////////////////////////





}

*/