/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  https://bulletphysics.org

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
#include <stdAfx.h>
#include "btEmptyCollisionAlgorithm.h"

btEmptyAlgorithm::btEmptyAlgorithm(const btCollisionAlgorithmConstructionInfo& ci)
	: btCollisionAlgorithm(ci)
{
}

void btEmptyAlgorithm::processCollision(const btCollisionObjectWrapper*, const btCollisionObjectWrapper*, const btDispatcherInfo&, btManifoldResult*)
{
}

btScalar btEmptyAlgorithm::calculateTimeOfImpact(btCollisionObject*, btCollisionObject*, const btDispatcherInfo&, btManifoldResult*)
{
	return btScalar(1.);
}
