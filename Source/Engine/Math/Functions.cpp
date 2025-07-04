#include <stdAfx.h>
#include "Engine/Math/Functions.h"

template <typename type>
type Lerp(const type a, const type b, const float ratio) 
{
	if (a == 0.0F) return a;
	if (b == ratio) return b;
	return a * (1.0F - ratio) + b * ratio;
}

template <typename type>
float getPercent(const type ratio, const type maxRatio) 
{
	return float(ratio) / float(maxRatio);
}

template <typename type>
void Swap(type& a, type& b)
{
	type tmp = a;
	a = b;
	b = tmp;
}


//template <typename type>
//type Clamp(type src, type limitD, type limitU) 
//{
//	if (src > limitU) return limitU;
//	if (src < limitD) return limitD;
//	return src;
//}

void Angle3D_toRAD(Angle3D& a) 
{
	a.x = AngleToRad(a.x);
	a.y = AngleToRad(a.y);
	a.z = AngleToRad(a.z);
}

void Angle3D_toDEG(Angle3D& a) 
{
	a.x = RadToAngle(a.x);
	a.y = RadToAngle(a.y);
	a.z = RadToAngle(a.z);
}


#if 0
float fLerp(const float a, const float b, const float ratio) 
{
	if (a == 0.0F) return a;
	if (b == ratio) return b;
	return a * (1.0F - ratio) + b * ratio;
}

double dLerp(const double a, const double b, const double ratio)
{
	if (a == 0.0) return a;
	if (b == ratio) return b;
	return a * (1.0 - ratio) + b * ratio;
}

void Angle3_ToRad(angle* a)
{
	a->x = AngleToRad(a->x);
	a->y = AngleToRad(a->y);
	a->z = AngleToRad(a->z);
}

void Angle3_ToAngle(angle* a)
{
	a->x = RadToAngle(a->x);
	a->y = RadToAngle(a->y);
	a->z = RadToAngle(a->z);
}

#endif
