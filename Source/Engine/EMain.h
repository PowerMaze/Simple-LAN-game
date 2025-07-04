#ifndef EMAIN_H
#define EMAIN_H

/*
----------------> Basic headers <----------------
*/

// C headers

#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <Windows.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <Math.h>


// C++ headers
#include <string>
#include <vector>
#include <map>
#include <unordered_map>


// SDL2 headers
#ifdef USE_ENGINE
	#ifndef NO_SDL
		#include <SDL2/SDL.h>
	#endif
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
    #include <glm/gtc/type_ptr.hpp>
#endif

// OpengGL math headers (GLM)
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>


// Bullet Physics Library headers 
#include <btBulletDynamicsCommon.h>


/*
----------------> In the Engine headers <----------------
*/

// Common engine headers
#include "Common/Common.h"    
#include "Common/BasicFlags.h" // basics flags
#include "Common/Timer.h"
#include "Common/DataTypes.h"  // own Engine data typed (typedef ...)
#include "Common/Classes.h"    // classes predelclarations
#include "Common/Iterators.h"  // iterators for arrays or c++ stl containers
#include "Common/PrintMessages.h" // print formated messages ( Error, Warning, Throw)
#include "Common/FileIO.h"        // for file manipulation
#include "Common/FileName.h"      // classes for file string and file stream (FILE C)
#include "Common/Console.h"       // classes for console data base and console buffer
#include "Common/FontData.h"      // for font data
#include "Common/CRC.h"
#include "Common/Memory.h"
#include "DataBase/DataBase.h"    // data base loading objects


// Graphics headers
#include "Graphics/View.h"       // for rendering to screen canvas
#include "Graphics/Shader.h"     // opengl shader
#include "Graphics/Render.h"     // functions for rendering world
#include "Graphics/Texture.h"    // texture class
#include "Graphics/Color.h"      // contains color manipulation
#include "Graphics/SkyBox.h"     // contains cubemap (skybox)
#include "Graphics/Draw.h"       // functions for drawing 2d objects to screen (UI)
#include "Graphics/glad.h"       // OpenGL functions (GLAD)
#include "Graphics/RenderScene.h"

// Math headers
#include "Math/Functions.h"      // basic functions for math
#include "Math/FrustumCulling.h" // frustum culling for render


// brush headers
#include "Level/Brush.h"

// sound data headers

// model data headers
#include "Model/ImportOBJ.h"    
#include "Model/Import3D.h"  // for import obj file to mo file (In the Engine 3d model)
#include "Model/Model.h"     // In the Engine 3D model


// entities headers
#include "Entities/EntityMain.h"  // basic entity
#include "Entities/EntityLive.h"  // live entity
#include "Entities/CollisionDispatcher.h" // get collision call back

// world data headers
#include "Level/Octree.h"
#include "Level/Room.h"          // for room (sector) data
#include "Level/BaseWorld.h"     // main game world

// network data headers
#include "NET/NetMain.h"  // namespace for all functions, classes, structures for low level networing (WinApi32)

// rendering data headers
#include "RenderWorld/RenderMain.h"

/*
----------------> In the Engine basic functions <----------------
*/

/* Startup In the Engine */
ENGINE_API void InTheEngine_Start(int argc, char** argv, const char* strGame);
/* Run engine frame */
ENGINE_API void InTheEngine_RunFrame(unsigned long miliseconds);
/* Release of resources and termination of the Engine */
ENGINE_API void InTheEngine_Shutdown(void);
/*  */
//ENGINE_API extern WindowCanvas_t *InTheWindow = NULL;

#endif