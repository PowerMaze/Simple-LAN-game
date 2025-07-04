#pragma once

typedef struct Plane_s
{
    Vector3D vnNormal;
    float    fDistance;
}Plane_t;

ENGINE_API extern void createPlane(Plane_t* pl, Vector3D p1, Vector3D vn);

class ENGINE_API CFrustum
{
    Plane_t plnTopFace;
    Plane_t plnBottomFace;

    Plane_t plnRightFace;
    Plane_t plnLeftFace;

    Plane_t plnFarFace;
    Plane_t plnNearFace;

    CFrustum(void);

    void updateFrustum(CView* pView, Vector3D vPos, Vector3D vFront, Vector3D vUp, Vector3D vRight);

    bool isSphereInFrustum(void);
    bool isPoint3DInFrustum(const Vector3D& vPoint);  
    bool isPolugonInFrustum(const CPolygonWorld* polygon);
};
/*
class ENGINE_API CPlace
{
public:
    Vector3D vPosition;
    Angle3D  aRotaion;
public:
    CPlace(void);
    CPlace& operator=(const CPlace& pl);
};

CPlace ENGINE_API LerpPositions(const CPlace& vP, const CPlace& aA);


ENGINE_API extern Frustum_t createFrustumFromCameraView(Vector3D vPos, Vector3D vFront, Vector3D vUp, Vector3D vRight);
*/