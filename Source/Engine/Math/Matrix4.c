#include "Matrix.h"

matrix4 matrix4_Identity(void)
{
	matrix4 A;
	A.mat[0][0] = 1.0f;	A.mat[0][1] = 0.0f;	A.mat[0][2] = 0.0f;	A.mat[0][3] = 0.0f;
	A.mat[1][0] = 0.0f;	A.mat[1][1] = 1.0f;	A.mat[1][2] = 0.0f;	A.mat[1][3] = 0.0f;
	A.mat[2][0] = 0.0f;	A.mat[2][1] = 0.0f;	A.mat[2][2] = 1.0f;	A.mat[2][3] = 0.0f;
	A.mat[3][0] = 0.0f;	A.mat[3][1] = 0.0f;	A.mat[3][2] = 0.0f;	A.mat[3][3] = 1.0f;
	return A;
}

void matrix4_fromFloatArray(matrix4* mMatr, float fMatr[4][4])
{
	for (unsigned i = 0; i < 4; i++)
		for (unsigned j = 0; j < 4; j++)
			mMatr->mat[i][j] = fMatr[i][j];	
}

matrix4 matrix4_Init(void) 
{
	matrix4 A;
	for (unsigned i = 0; i < 4; i++)
	{
		for (unsigned j = 0; j < 4; j++)
		{
			A.mat[i][j] = 0.0f;
		}
	}
	return A;

}

void matrix4_Scale(matrix4* scale, const vector3 vect)
{
	scale->mat[0][0] = vect.x;
	scale->mat[1][1] = vect.y;
	scale->mat[2][2] = vect.z;
}

void matrix4_Translation(matrix4* trans, const vector3 vect)
{
	trans->mat[3][0] = vect.x;
	trans->mat[3][1] = vect.y;
	trans->mat[3][2] = vect.z;
}

void matrix4_RotateByVectors(matrix4* rot, float fRad, const vector3 vect)
{
	rot->mat[0][0] = cosf(fRad) + powf(vect.x, 2) * (1.0f - cosf(fRad));
	rot->mat[0][1] = vect.x * vect.y * (1.0f - cosf(fRad)) - vect.z * sinf(fRad);
	rot->mat[0][2] = vect.x * vect.z * (1.0f - cosf(fRad)) + vect.y * sinf(fRad);

	rot->mat[1][0] = vect.y * vect.x * (1.0f - cosf(fRad)) + vect.z * sinf(fRad);
	rot->mat[1][1] = cosf(fRad) + powf(vect.y, 2) * (1.0f - cosf(fRad));
	rot->mat[1][2] = vect.y * vect.z * (1.0f - cosf(fRad)) - vect.x * sinf(fRad);

	rot->mat[2][0] = vect.z * vect.x * (1.0f - cosf(fRad)) - vect.y * sinf(fRad);
	rot->mat[2][1] = vect.z * vect.y * (1.0f - cosf(fRad)) + vect.x * sinf(fRad);
	rot->mat[2][2] = cosf(fRad) + powf(vect.z, 2) * (1.0f - cosf(fRad));
}
/*
void matrix4_Rotate(matrix4* rot, angle fangles)
{
	float roll = fangles.x;
	float pitch = fangles.y;
	float yaw = fangles.z;
		

}
*/
matrix4 matrix4_Mul(const matrix4* A, const matrix4* B)
{
	matrix4 C;
	for (unsigned i = 0; i < 4; i++)
	{
		for (unsigned j = 0; j < 4; j++)
		{
			C.mat[i][j] = A->mat[i][j] * B->mat[j][i];
		}
	}
	return C;
}

void matrix4_MulScalar(matrix4* A, float scal) 
{
	for (unsigned i = 0; i < 4; i++)
	{
		for (unsigned j = 0; j < 4; j++)
		{
			A->mat[i][j] *= scal;
		}
	}
}

matrix4 matrix4_Add(const matrix4* A, const matrix4* B)
{
	matrix4 C;
	for (unsigned i = 0; i < 4; i++)
	{
		for (unsigned j = 0; j < 4; j++)
		{
			C.mat[i][j] = A->mat[i][j] + B->mat[i][j];
		}
	}
	return C;
}


matrix4 matrix4_Sub(const matrix4* A, const matrix4* B)
{
	matrix4 C;
	for (unsigned i = 0; i < 4; i++)
	{
		for (unsigned j = 0; j < 4; j++)
		{
			C.mat[i][j] = A->mat[i][j] - B->mat[i][j];
		}
	}
	return C;
}

float matrix3_det(const matrix3* A) 
{
	return
	(
		A->mat[0][0] * A->mat[1][1] * A->mat[2][2] + 
		A->mat[0][1] * A->mat[1][2] * A->mat[2][1] +
		A->mat[1][2] * A->mat[1][0] * A->mat[2][1] - 
		A->mat[1][2] * A->mat[1][1] * A->mat[2][0] -
		A->mat[0][1] * A->mat[1][0] * A->mat[2][2] -
		A->mat[0][0] * A->mat[1][2] * A->mat[2][1]
	);
}

float matrix4_det(const matrix4* A)
{
	return
	(
		A->mat[0][3] * A->mat[1][2] * A->mat[2][1] * A->mat[3][0] - A->mat[0][2] * A->mat[1][3] * A->mat[2][1] * A->mat[3][0] -
		A->mat[0][3] * A->mat[1][1] * A->mat[2][2] * A->mat[3][0] + A->mat[0][1] * A->mat[1][3] * A->mat[2][2] * A->mat[3][0] +
		A->mat[0][2] * A->mat[1][1] * A->mat[2][3] * A->mat[3][0] - A->mat[0][1] * A->mat[1][2] * A->mat[2][3] * A->mat[3][0] -
		A->mat[0][3] * A->mat[1][2] * A->mat[2][0] * A->mat[3][1] + A->mat[0][2] * A->mat[1][3] * A->mat[2][0] * A->mat[3][1] +
		A->mat[0][3] * A->mat[1][0] * A->mat[2][2] * A->mat[3][1] - A->mat[0][0] * A->mat[1][3] * A->mat[2][2] * A->mat[3][1] -
		A->mat[0][2] * A->mat[1][0] * A->mat[2][3] * A->mat[3][1] + A->mat[0][0] * A->mat[1][2] * A->mat[2][3] * A->mat[3][1] +
		A->mat[0][3] * A->mat[1][1] * A->mat[2][0] * A->mat[3][2] - A->mat[0][1] * A->mat[1][3] * A->mat[2][0] * A->mat[3][2] -
		A->mat[0][3] * A->mat[1][0] * A->mat[2][1] * A->mat[3][2] + A->mat[0][0] * A->mat[1][3] * A->mat[2][1] * A->mat[3][2] +
		A->mat[0][1] * A->mat[1][0] * A->mat[2][3] * A->mat[3][2] - A->mat[0][0] * A->mat[1][1] * A->mat[2][3] * A->mat[3][2] -
		A->mat[0][2] * A->mat[1][1] * A->mat[2][0] * A->mat[3][3] + A->mat[0][1] * A->mat[1][2] * A->mat[2][0] * A->mat[3][3] +
		A->mat[0][2] * A->mat[1][0] * A->mat[2][1] * A->mat[3][3] - A->mat[0][0] * A->mat[1][2] * A->mat[2][1] * A->mat[3][3] -
		A->mat[0][1] * A->mat[1][0] * A->mat[2][2] * A->mat[3][3] + A->mat[0][0] * A->mat[1][1] * A->mat[2][2] * A->mat[3][3]
	);/*
	return
	(
		m->mat[0][0] * (m->mat[1][1] * (m->mat[2][2] * m->mat[3][3] - m->mat[2][3] * m->mat[3][2]) -
		m->mat[1][2] * (m->mat[2][1] * m->mat[3][3] - m->mat[2][3] * m->mat[3][1]) +
		m->mat[1][3] * (m->mat[2][1] * m->mat[3][2] - m->mat[2][2] * m->mat[3][1]))
		- m->mat[0][1] * (m->mat[1][0] * (m->mat[2][2] * m->mat[3][3] - m->mat[2][3] * m->mat[3][2]) -
		m->mat[1][2] * (m->mat[2][0] * m->mat[3][3] - m->mat[2][3] * m->mat[3][0]) +
			m->mat[1][3] * (m->mat[2][0] * m->mat[3][2] - m->mat[2][2] * m->mat[3][0]))
		+ m->mat[0][2] * (m->mat[1][0] * (m->mat[2][1] * m->mat[3][3] - m->mat[2][3] * m->mat[3][1]) -
			m->mat[1][1] * (m->mat[2][0] * m->mat[3][3] - m->mat[2][3] * m->mat[3][0]) +
			m->mat[1][3] * (m->mat[2][0] * m->mat[3][1] - m->mat[2][1] * m->mat[3][0]))
		- m->mat[0][3] * (m->mat[1][0] * (m->mat[2][1] * m->mat[3][2] - m->mat[2][2] * m->mat[3][1]) -
			m->mat[1][1] * (m->mat[2][0] * m->mat[3][2] - m->mat[2][2] * m->mat[3][0]) +
			m->mat[1][2] * (m->mat[2][0] * m->mat[3][1] - m->mat[2][1] * m->mat[3][0]))
		);*/
}

void matrix4_perspective(matrix4* mat4, float aFOV, float aspectRatio, float zfar, float znear) 
{
	mat4->mat[0][0] = 1.0f / (aspectRatio * tanf(aFOV / 2));
	mat4->mat[1][1] = aspectRatio * tanf(aFOV / 2);
	mat4->mat[2][2] = -1.0f * ((zfar + znear) / (zfar - znear));
	mat4->mat[2][3] = -1.0f * ((2 * zfar * znear) / (zfar - znear));
	mat4->mat[3][2] = -1.0f;
}

void matrix4_adjoint(matrix4* mat4) 
{
	matrix3 matrs[16];

/* ------------ M(1,1) ----------- */
	matrs[0].mat[0][0] = mat4->mat[1][1];
	matrs[0].mat[0][1] = mat4->mat[1][2];
	matrs[0].mat[0][2] = mat4->mat[1][3];

	matrs[0].mat[1][0] = mat4->mat[2][1];
	matrs[0].mat[1][1] = mat4->mat[2][2];
	matrs[0].mat[1][2] = mat4->mat[2][3];

	matrs[0].mat[2][0] = mat4->mat[3][1];
	matrs[0].mat[2][1] = mat4->mat[3][2];
	matrs[0].mat[2][2] = mat4->mat[3][3];

/* ------------ M(1,2) ----------- */
	matrs[1].mat[0][0] = mat4->mat[1][0];
	matrs[1].mat[0][1] = mat4->mat[1][2];
	matrs[1].mat[0][2] = mat4->mat[1][3];

	matrs[1].mat[1][0] = mat4->mat[2][0];
	matrs[1].mat[1][1] = mat4->mat[2][2];
	matrs[1].mat[1][2] = mat4->mat[2][3];

	matrs[1].mat[2][0] = mat4->mat[3][0];
	matrs[1].mat[2][1] = mat4->mat[3][2];
	matrs[1].mat[2][2] = mat4->mat[3][3];

/* ------------ M(1,3) ----------- */
	matrs[2].mat[0][0] = mat4->mat[1][0];
	matrs[2].mat[0][1] = mat4->mat[1][1];
	matrs[2].mat[0][2] = mat4->mat[1][3];

	matrs[2].mat[1][0] = mat4->mat[2][0];
	matrs[2].mat[1][1] = mat4->mat[2][1];
	matrs[2].mat[1][2] = mat4->mat[2][3];

	matrs[2].mat[2][0] = mat4->mat[3][0];
	matrs[2].mat[2][1] = mat4->mat[3][1];
	matrs[2].mat[2][2] = mat4->mat[3][3];
	
/* ------------ M(1,4) ----------- */
	matrs[3].mat[0][0] = mat4->mat[1][0];
	matrs[3].mat[0][1] = mat4->mat[1][1];
	matrs[3].mat[0][2] = mat4->mat[1][2];

	matrs[3].mat[1][0] = mat4->mat[2][0];
	matrs[3].mat[1][1] = mat4->mat[2][1];
	matrs[3].mat[1][2] = mat4->mat[2][2];

	matrs[3].mat[2][0] = mat4->mat[3][0];
	matrs[3].mat[2][1] = mat4->mat[3][1];
	matrs[3].mat[2][2] = mat4->mat[3][2];



	/* ------------ M(2,1) ----------- */
	matrs[4].mat[0][0] = mat4->mat[0][1];
	matrs[4].mat[0][1] = mat4->mat[0][2];
	matrs[4].mat[0][2] = mat4->mat[0][3];

	matrs[4].mat[1][0] = mat4->mat[2][1];
	matrs[4].mat[1][1] = mat4->mat[2][2];
	matrs[4].mat[1][2] = mat4->mat[2][3];

	matrs[4].mat[2][0] = mat4->mat[3][1];
	matrs[4].mat[2][1] = mat4->mat[3][2];
	matrs[4].mat[2][2] = mat4->mat[3][3];

	/* ------------ M(2,2) ----------- */
	matrs[5].mat[0][0] = mat4->mat[0][0];
	matrs[5].mat[0][1] = mat4->mat[0][2];
	matrs[5].mat[0][2] = mat4->mat[0][3];

	matrs[5].mat[1][0] = mat4->mat[2][0];
	matrs[5].mat[1][1] = mat4->mat[2][2];
	matrs[5].mat[1][2] = mat4->mat[2][3];

	matrs[5].mat[2][0] = mat4->mat[3][0];
	matrs[5].mat[2][1] = mat4->mat[3][2];
	matrs[5].mat[2][2] = mat4->mat[3][3];

	/* ------------ M(2,3) ----------- */
	matrs[6].mat[0][0] = mat4->mat[0][0];
	matrs[6].mat[0][1] = mat4->mat[0][1];
	matrs[6].mat[0][2] = mat4->mat[0][3];

	matrs[6].mat[1][0] = mat4->mat[2][0];
	matrs[6].mat[1][1] = mat4->mat[2][1];
	matrs[6].mat[1][2] = mat4->mat[2][3];

	matrs[6].mat[2][0] = mat4->mat[3][0];
	matrs[6].mat[2][1] = mat4->mat[3][1];
	matrs[6].mat[2][2] = mat4->mat[3][3];

	/* ------------ M(2,4) ----------- */
	matrs[7].mat[0][0] = mat4->mat[0][0];
	matrs[7].mat[0][1] = mat4->mat[0][1];
	matrs[7].mat[0][2] = mat4->mat[0][2];

	matrs[7].mat[1][0] = mat4->mat[2][0];
	matrs[7].mat[1][1] = mat4->mat[2][1];
	matrs[7].mat[1][2] = mat4->mat[2][2];

	matrs[7].mat[2][0] = mat4->mat[3][0];
	matrs[7].mat[2][1] = mat4->mat[3][1];
	matrs[7].mat[2][2] = mat4->mat[3][2];




	/* ------------ M(3,1) ----------- */
	matrs[8].mat[0][0] = mat4->mat[0][1];
	matrs[8].mat[0][1] = mat4->mat[0][2];
	matrs[8].mat[0][2] = mat4->mat[0][3];

	matrs[8].mat[1][0] = mat4->mat[1][1];
	matrs[8].mat[1][1] = mat4->mat[1][2];
	matrs[8].mat[1][2] = mat4->mat[1][3];

	matrs[8].mat[2][0] = mat4->mat[3][1];
	matrs[8].mat[2][1] = mat4->mat[3][2];
	matrs[8].mat[2][2] = mat4->mat[3][3];

	/* ------------ M(3,2) ----------- */
	matrs[9].mat[0][0] = mat4->mat[0][0];
	matrs[9].mat[0][1] = mat4->mat[0][2];
	matrs[9].mat[0][2] = mat4->mat[0][3];

	matrs[9].mat[1][0] = mat4->mat[1][0];
	matrs[9].mat[1][1] = mat4->mat[1][2];
	matrs[9].mat[1][2] = mat4->mat[1][3];

	matrs[9].mat[2][0] = mat4->mat[3][0];
	matrs[9].mat[2][1] = mat4->mat[3][2];
	matrs[9].mat[2][2] = mat4->mat[3][3];

	/* ------------ M(3,3) ----------- */
	matrs[10].mat[0][0] = mat4->mat[0][0];
	matrs[10].mat[0][1] = mat4->mat[0][1];
	matrs[10].mat[0][2] = mat4->mat[0][3];

	matrs[10].mat[1][0] = mat4->mat[1][0];
	matrs[10].mat[1][1] = mat4->mat[1][1];
	matrs[10].mat[1][2] = mat4->mat[1][3];

	matrs[10].mat[2][0] = mat4->mat[3][0];
	matrs[10].mat[2][1] = mat4->mat[3][1];
	matrs[10].mat[2][2] = mat4->mat[3][3];

	/* ------------ M(3,4) ----------- */
	matrs[11].mat[0][0] = mat4->mat[0][0];
	matrs[11].mat[0][1] = mat4->mat[0][1];
	matrs[11].mat[0][2] = mat4->mat[0][2];

	matrs[11].mat[1][0] = mat4->mat[1][0];
	matrs[11].mat[1][1] = mat4->mat[1][1];
	matrs[11].mat[1][2] = mat4->mat[1][2];

	matrs[11].mat[2][0] = mat4->mat[3][0];
	matrs[11].mat[2][1] = mat4->mat[3][1];
	matrs[11].mat[2][2] = mat4->mat[3][2];
	



	/* ------------ M(4,1) ----------- */
	matrs[12].mat[0][0] = mat4->mat[0][1];
	matrs[12].mat[0][1] = mat4->mat[0][2];
	matrs[12].mat[0][2] = mat4->mat[0][3];

	matrs[12].mat[1][0] = mat4->mat[1][1];
	matrs[12].mat[1][1] = mat4->mat[1][2];
	matrs[12].mat[1][2] = mat4->mat[1][3];

	matrs[12].mat[2][0] = mat4->mat[2][1];
	matrs[12].mat[2][1] = mat4->mat[2][2];
	matrs[12].mat[2][2] = mat4->mat[2][3];

	/* ------------ M(4,2) ----------- */
	matrs[13].mat[0][0] = mat4->mat[0][0];
	matrs[13].mat[0][1] = mat4->mat[0][2];
	matrs[13].mat[0][2] = mat4->mat[0][3];

	matrs[13].mat[1][0] = mat4->mat[1][0];
	matrs[13].mat[1][1] = mat4->mat[1][2];
	matrs[13].mat[1][2] = mat4->mat[1][3];

	matrs[13].mat[2][0] = mat4->mat[2][0];
	matrs[13].mat[2][1] = mat4->mat[2][2];
	matrs[13].mat[2][2] = mat4->mat[2][3];

	/* ------------ M(4,3) ----------- */
	matrs[14].mat[0][0] = mat4->mat[0][0];
	matrs[14].mat[0][1] = mat4->mat[0][1];
	matrs[14].mat[0][2] = mat4->mat[0][3];

	matrs[14].mat[1][0] = mat4->mat[1][0];
	matrs[14].mat[1][1] = mat4->mat[1][1];
	matrs[14].mat[1][2] = mat4->mat[1][3];

	matrs[14].mat[2][0] = mat4->mat[2][0];
	matrs[14].mat[2][1] = mat4->mat[2][1];
	matrs[14].mat[2][2] = mat4->mat[2][3];

	/* ------------ M(4,4) ----------- */
	matrs[15].mat[0][0] = mat4->mat[0][0];
	matrs[15].mat[0][1] = mat4->mat[0][1];
	matrs[15].mat[0][2] = mat4->mat[0][2];

	matrs[15].mat[1][0] = mat4->mat[1][0];
	matrs[15].mat[1][1] = mat4->mat[1][1];
	matrs[15].mat[1][2] = mat4->mat[1][2];

	matrs[15].mat[2][0] = mat4->mat[2][0];
	matrs[15].mat[2][1] = mat4->mat[2][1];
	matrs[15].mat[2][2] = mat4->mat[2][2];


	for (unsigned i = 0; i < 4; i++)
	{
		for (unsigned j = 0; j < 4; j++) 
		{
			float fsgn;
			if ((i + j) % 2 == 0) fsgn = 1.0f;
			else				  fsgn = -1.0f;
			mat4->mat[i][j] = fsgn * matrix3_det(&matrs[i * 4 + j]);
		}
	}
}

void matrix4_inverse(matrix4* A) 
{
	float fDet  = matrix4_det(A);

	if (fDet == 0.0f) {
		PrintF("det of matrix is zero!!!\n");
		return;
	}

	matrix4_adjoint(A);

	matrix4_MulScalar(A, 1.0f / fDet);
}

void matrix4_transpose(matrix4* m) 
{
	m->mat[0][0] = m->mat[0][0];
	m->mat[0][1] = m->mat[1][0];
	m->mat[0][2] = m->mat[2][0];
	m->mat[0][3] = m->mat[3][0];

	m->mat[1][0] = m->mat[0][1];
	m->mat[1][1] = m->mat[1][1];
	m->mat[1][2] = m->mat[2][1];
	m->mat[1][3] = m->mat[3][1];

	m->mat[2][0] = m->mat[0][2];
	m->mat[2][1] = m->mat[1][2];
	m->mat[2][2] = m->mat[2][2];
	m->mat[2][3] = m->mat[3][2];

	m->mat[3][0] = m->mat[0][3];
	m->mat[3][1] = m->mat[1][3];
	m->mat[3][2] = m->mat[2][3];
	m->mat[3][3] = m->mat[3][3];
}

void matrix4_toArray(const matrix4* m, float* arr)
{
	for (unsigned i = 0; i < 4; i++)	
		for (unsigned j = 0; j < 4; j++)		
			arr[i * 4 + j] = m->mat[i][j];	
}

void ArrayToMatrix4(const float* arr, matrix4* m)
{
	for (unsigned i = 0; i < 4; i++)
		for (unsigned j = 0; j < 4; j++)
			m->mat[i][j] = arr[i * 4 + j];
}


void matrix3_to_matrix4(matrix4* mat4, const matrix3* mat3)
{
	for (unsigned int i = 0; i < 3; i++)
		for (unsigned int j = 0; j < 3; j++)
			mat4->mat[i][j] = mat3->mat[i][j];
}