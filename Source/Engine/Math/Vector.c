#include "Vector.h"


void vectorNEG(vector3* A)
{
	A->x = -A->x;
	A->y = -A->y;
	A->z = -A->z;
}

void vectorNormalize(vector3* result) 
{
	vector3 tmp = *result;
	result->x = tmp.x / vector3_Len(&tmp);
	result->y = tmp.y / vector3_Len(&tmp);
	result->z = tmp.z / vector3_Len(&tmp);
}

void vectorCopy(vector3* vDst, const vector3* vSrc)
{
	vDst->x = vSrc->x;
	vDst->y = vSrc->y;
	vDst->z = vSrc->z;
}

void vectorAddScallar(vector3* A, float scal) 
{
	A->x += scal;
	A->y += scal;
	A->z += scal;
}


void vectorSubScallar(vector3* A, float scal)
{
	A->x -= scal;
	A->y -= scal;
	A->z -= scal;
}

void vectorMulScallar(vector3* A, float scal)
{
	A->x *= scal;
	A->y *= scal;
	A->z *= scal;
}

void vectorDivScallar(vector3* A, float scal)
{
	A->x /= scal;
	A->y /= scal;
	A->z /= scal;
}
