#pragma once



ENGINE_API extern void renderPrepareWorld(CGameWorld* wo);

ENGINE_API extern void renderView(CGameWorld* wo, CView* view);

ENGINE_API extern void renderOneModel(CModel* mo, HEXColor color, CView* pView);


ENGINE_API extern void renderClearWorld(CGameWorld* wo);
ENGINE_API extern void renderEntities(std::vector <CEntityMain* > &woEntities, CView* pView, CEntityMain* exlude);
