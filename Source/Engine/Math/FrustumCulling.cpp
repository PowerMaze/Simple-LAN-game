#include <stdAfx.h>

#include "FrustumCulling.h"

void createPlane(Plane_t* pl, Vector3D p1, Vector3D vn)
{
    pl->vnNormal = glm::normalize(vn);
    pl->fDistance = glm::dot(pl->vnNormal, p1);
}

CFrustum::CFrustum(void) 
{
    memset(&plnTopFace,    0, sizeof(struct Plane_s));
    memset(&plnBottomFace, 0, sizeof(struct Plane_s));

    memset(&plnRightFace,  0, sizeof(struct Plane_s));
    memset(&plnLeftFace,   0, sizeof(struct Plane_s));

    memset(&plnFarFace,    0, sizeof(struct Plane_s));
    memset(&plnNearFace,   0, sizeof(struct Plane_s));
}

void CFrustum::updateFrustum(CView* pView, Vector3D vPos, Vector3D vFront, Vector3D vUp, Vector3D vRight) 
{ 
    if (pView == NULL) return;

    float halfVerticalSide = pView->m_far * tan(pView->m_FOV * 0.5F);
    float halfHorizontalSide = halfVerticalSide * pView->m_aspectRatio;

    Vector3D vFrontMultFar = pView->m_far * vFront;


    createPlane(&plnNearFace, vPos + pView->m_near * vFront, vFront);
    //createPlane(&plnFarFace, )



}
