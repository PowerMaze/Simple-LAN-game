// C headers (with WinApi32)

#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Math.h>


// C++ headers
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

/*
#ifdef _DEBUG
	#pragma comment(lib, "DLinearMath.lib")
	#pragma comment(lib, "DBulletDynamics.lib")
	#pragma comment(lib, "DBulletCollision.lib")
#else
	#pragma comment(lib, "LinearMath.lib")
	#pragma comment(lib, "BulletDynamics.lib")
	#pragma comment(lib, "BulletCollision.lib")
#endif
*/




// SDL2 headers
#include <SDL2/SDL.h>


// OpengGL math headers (GLM)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Bullet Physics Engine headers, only for Engine.DLL
#include "btBulletDynamicsCommon.h"

// In the Engine headers
#include <Engine/Common/BasicFlags.h>
#include <Engine/Common/Common.h>
#include <Engine/Common/DataTypes.h>
#include <Engine/Common/Classes.h>
#include <Engine/Common/Iterators.h>

#include <Engine/Common/Memory.h>

//#include <Engine/Common/Globals.h>
#include <Engine/Graphics/Color.h>
#include <Engine/Graphics/glad.h>
#include <Engine/Graphics/View.h>


