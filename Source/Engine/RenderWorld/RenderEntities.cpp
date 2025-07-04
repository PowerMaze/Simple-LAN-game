#include <stdAfx.h>

#include "RenderMain.h"

#include <Engine/Entities/EntityMain.h>
#include <Engine/Graphics/Shader.h>

void renderOneModel(CModel* mo, HEXColor color, CView* pView)
{
	//if (mo == NULL) return;

	shaModel.use();
	shaModel.shaderSet("diffuse_texture", 0);

	shaModel.shaderSetMatrix("projection", pView->getProjection());
	shaModel.shaderSetMatrix("view", pView->m_viewMatrix);

	shaModel.shaderSetVector("modelColor", HEX_toOGL(color));

	//mo->modelMatrix = glm::mat4(1.f);

	///mo->modelMatrix = glm::translate(mo->modelMatrix, vPos);
	//mo->modelMatrix = glm::scale(mo->modelMatrix, mo->m_vScale);

	shaModel.shaderSetMatrix("model", mo->modelMatrix);

	CModelObject* _pmoData = mo->getModel();

	assert(_pmoData != NULL);

	_pmoData->textureId = mo->m_uiTexID;

	if (mo->m_uiRenderingFlags & MODEL_WEAPON)
	{
		glDepthRange(0.0, 0.1);
	}

	_pmoData->Render(shaModel);
}

void renderEntities(std::vector <CEntityMain* >& woEntities, CView* pView, CEntityMain* exlude)
{
	FOREACH_ARRAY(woEntities.size())
	{
		CEntityMain* e = woEntities[i];

		if (e == NULL) continue;

		if (e->IsValid() && e->m_ulFlags & ENTITY_MODEL && e != exlude)
		{
			CModel* mo = &e->m_pmo;
			mo->modelMatrix = e->m_matModel;
			renderOneModel(mo, e->m_color, pView);
		}
	}
}