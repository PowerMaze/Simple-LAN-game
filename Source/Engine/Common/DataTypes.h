#pragma once


#ifndef __cplusplus
	typedef unsigned int bool;
#endif

/*

Specification of naming variables used by In the Engine

b_   - bool
ub_  - byte
us_  - unsigned short
ui_  - unsigned int
ull  - unsigned long long (unsigned __int64? idk)

f_   - float
d_   - double

i_   - int
ll_  - long long
col_ - HEXColor
str_ - string (char* / std::string)

a_   - array (simple array[elements] or std::vector)

mp_  - std::map
ump_ - std::unordered_map
bt_  - binary tree
ot_  - octree

v_   - void

p_   - pointer (*) example void* pvData, byte* pubData;

If this class or structure, naming this:

CClassName = cn, for example CModelObject = mo, CTextureObject = to, etc..
StructName = sn, for example AnimationLump_t = al

*/


typedef unsigned char	byte;
typedef char*			string_t;
	
//typedef unsigned int	Shader;

typedef char			int8;
typedef short			int16;
typedef int				int32;
typedef long long		int64;

typedef unsigned long	HEXColor;

typedef unsigned int	uint;
typedef signed int		sint;
typedef unsigned long	ulong;
typedef signed long		slong;

// angles

// x - Pith, Y - yaw, z - Roll
typedef glm::vec3		Angle3D; 
// x - Pith, Y - yaw
typedef glm::vec2		Angle2D;

// vectors
typedef glm::vec2		Vector2D;
typedef glm::vec3		Vector3D;
typedef glm::vec4		Vector4D;
// matrixs
typedef glm::mat4       Matrix4;
typedef glm::mat3       Matrix3;


#define VECTOR3_NONE     glm::vec3(0.0f)

#define VECTOR3(x,y,z)   glm::vec3(x, y, z)
#define VECTOR2(x,y)     glm::vec2(x, y)
#define VECTOR4(x,y,z,w) glm::vec4(x, y, z, w)

#define VEC3_TO_BTVEC3 (glm::vec3(vec)) btVector3(vec.x, vec.y, vec.z)
#define BTVEC3_TO_VEC3 (vec) VECTOR3(vec.x, vec.y, vec.z)

/* windef.h has this definition of true/false */

#ifndef __cplusplus
	#ifndef TRUE
		#define TRUE 1
	#endif
	#ifndef FALSE
		#define FALSE 0
	#endif
#endif

#define YAW		0
#define PITCH	1
#define ROLL	2


#define PI 3.14159265358979323846f
//#define PI_float 


/*
typedef struct mat4 matrix4;
typedef struct mat3 matrix3;

typedef struct vec2 vector2;
typedef struct vec3 vector3;
typedef struct vec4 vector4;

typedef struct vec3 angle;

*/

/* test object */
typedef struct objectDraw objectDraw_t;

