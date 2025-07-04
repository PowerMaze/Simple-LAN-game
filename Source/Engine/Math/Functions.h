#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (b) : (a))
#define SGN(x) (((x) > (0)) ? (1) : (((x) < (0)) ? (-1) : (0)))

#define CLAMP(clamp,down,up) ((clamp) >= (down)) ? ((clamp) <= (up) ? (clamp) : (up)): (down))

template <typename type>
inline type Clamp(type src, type limitD, type limitU) 
{
	if (src > limitU) return limitU;
	if (src < limitD) return limitD;
	return src;
}

template <typename type>
inline type ClampD(type src, type limitD)
{
	if (src < limitD) return limitD;
	return src;
}

template <typename type>
inline type ClampU(type src, type limitU)
{
	if (src > limitU) return limitU;
	return src;
}

template <typename type>
inline void Swap(type* a, type* b) 
{
	type tmp = *a;
	*a = *b;
	*b = tmp;
}

inline int iClamp(int clamp, int limit)
{
	return clamp <= limit ? clamp : limit;
}

inline float fClamp(float clamp, float limit)
{
	return clamp <= limit ? clamp : limit;
}

inline double dClamp(double clamp, double limit)
{
	return clamp <= limit ? clamp : limit;
}

inline float AngleToRad(float fAngle)
{
	return fAngle * (PI / 180.0f);
}

inline float RadToAngle(float fRad)
{
	return fRad * (180.0f / PI);
}

// Qfabs from id tech
inline float _fabs(float f) 
{
	int i = *(int*)&f;
	i &= 0x7FFFFFFF;
	return *(float*)&i;
}

/*
ENGINE_API extern void Angle3_ToRad(angle* a);

ENGINE_API extern void Angle3_ToAngle(angle* a);
*/
/*
inline void SwapI(int* a, int* b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

inline void SwapF(float* a, float* b)
{
	float tmp = *a;
	*a = *b;
	*b = tmp;
}

inline void SwapD(double* a, double* b)
{
	double tmp = *a;
	*a = *b;
	*b = tmp;
}


*/
#endif