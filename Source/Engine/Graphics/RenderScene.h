#pragma once

/**/


//ENGINE_API extern void RenderPrepareWorld(CGameWorld* wo) {};
//ENGINE_API extern void RenderViewScene(CGameWorld* wo, CView* view) {};

ENGINE_API extern void RenderView(CView* pvView, CGameWorld* wo);

ENGINE_API extern void RenderEntitiesInRoom(CView* view, std::vector < CEntityMain* > & _aEntities);

ENGINE_API extern void RenderOneModel(CView* view, CModel* model);




