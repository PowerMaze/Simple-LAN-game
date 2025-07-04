#ifndef MATRIX4_H
#define MATRIX4_H

#pragma once

#include "../EMain.h"
//#include "vector.h"

struct mat2
{
	float mat[2][2];
};

struct mat3
{
	float mat[3][3];
};

struct mat4
{
	float mat[4][4];
};



/*
||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

Matrix 4D Functions

||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
*/

extern ENGINE_API void matrix4_fromFloatArray(matrix4* mMatr, float fMatr[4][4]);
extern ENGINE_API matrix4 matrix4_Identity(void);
extern ENGINE_API void matrix4_Scale(matrix4* scale, const  vector3 vect);
extern ENGINE_API void matrix4_Translation(matrix4* trans, const vector3 vect);
extern ENGINE_API void matrix4_RotateByVectors(matrix4* rot, float fRad, const  vector3 vect);
extern ENGINE_API matrix4 matrix4_Mul(const matrix4* A, const matrix4* B);
extern ENGINE_API void matrix4_inverse(matrix4* A);
extern ENGINE_API void matrix4_perspective(matrix4* mat4, float aFOV, float aspectRatio, float zfar, float znear);

//ENGINE_API void MulMat4Vector(const matrix4* A, const vector3 vec);



#endif
