#ifndef VECTOR_H
#define VECTOR_H

#pragma once

#include <EMain.h>

/* vector 2D */
struct vec2
{
	float x, y;
};
/* vector 3D */
struct vec3
{
	float x, y, z;
};
/* vector 4D */
struct vec4
{
	float x, y, z, w;
};


inline vector2 vector2_Set(const float x, const float y)
{
	return (vector2) { x, y }; // i don't think its useful function...
}

/*
/////////////////////////////////////////////
	Vector 3d basic math operation
/////////////////////////////////////////////
*/

// inline functions

/* set a vector of values */
inline vector3 vector3_Set(const float x, const float y, const float z)
{
	return (vector3){ x, y, z }; // i don't think its useful function...
}
/* adding two vectors */
inline vector3 vector3_ADD(const vector3 A, const vector3 B)
{
	return (vector3) { A.x + B.x, A.y + B.y, A.z + B.z };
}
/* subtraction of two vectors */
inline vector3 vector3_SUB(const vector3 A, const vector3 B)
{
	return (vector3) { A.x - B.x, A.y - B.y, A.z - B.z };
}
/* cross product of vectors */
inline vector3 vectorCross(const vector3* A, const vector3* B)
{
	return (vector3)
	{
			A->y* B->z - A->z * B->y,
			A->z* B->x - A->x * B->z,
			A->x* B->y - A->y * B->x
	};
}
/* The length of the vector */
inline float vector3_Len(const vector3* A)
{
	return sqrtf((A->x * A->x) + (A->y * A->y) + (A->z * A->z));
}
/* dot product of vectors */
inline float Vector3_DotProduct(const vector3* A, const vector3* B)
{
	return (A->x * B->x) + (A->y * B->y) + (A->z * B->z);
}
/* convert vector3 to vector4 */
inline vector4 vector3ToVector4(const vector3* vSrc) 
{
	return (vector4) { vSrc->x, vSrc->y, vSrc->z, 1.0f };
}
/* compare two vectors */
inline bool vector3_compare(const vector3* vA, const vector3* vB)
{
	return (vA->x == vB->x) && (vA->y == vB->y) && (vA->z == vB->z);
}



/* make the vector inverse */
ENGINE_API extern void vectorNEG(vector3* A);
/* Make unit vector */
ENGINE_API extern void vectorNormalize(vector3* result);
/* copy vector */
ENGINE_API extern void vectorCopy(vector3* vDst, const vector3* vSrc);
/* adding a vector to a scalar */
ENGINE_API extern void vectorAddScallar(vector3* A, float scal);
/* subtracting a vector from a scalar */
ENGINE_API extern void vectorSubScallar(vector3* A, float scal);
/* multiply vector by scalar */
ENGINE_API extern void vectorMulScallar(vector3* A, float scal);
/* dividing a vector by a scalar */
ENGINE_API extern void vectorDivScallar(vector3* A, float scal);


#endif // VECTOR_H