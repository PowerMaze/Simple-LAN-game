#include <stdAfx.h>
#include "BaseWorld.h"
#include <Engine/Graphics/Color.h>
#include <Engine/Graphics/SkyBox.h>

#include <Engine/Common/Console.h>
#include <Engine/Common/PrintMessages.h>


CGameWorld::CGameWorld()
{
	/* In the Engine world data */
	gw_strName = "Untitled";
	gw_description = "Base world";

	gw_flags = 0;

	gw_numRooms = 0;
	gw_numBrushes = 0;
	gw_numEntities = 0;

	gw_fGravity = 9.81f;

	gw_nextEntityID = 0;
	gw_nextRoomID = 0;

	gw_bckgColour = COLOR_CYAN;

	gw_openInEditor = false;

	skybox = nullptr;
	gw_Sun = nullptr;

	/* Bullet engine data */
	gw_CollisionConfiguration = nullptr;
	gw_ColDispatcher          = nullptr;
	gw_DynamicsWorld          = nullptr;

	gw_Broadphase             = nullptr;
	gw_Csolver                = nullptr;

	gw_WorldTriangleMesh      = nullptr;
	gw_LevelShape             = nullptr;

	//gw_player                 = nullptr;

	/* this used only for world collision, static rooms and brushes */
	gw_LevelBody = nullptr;

//	gw_entityOffsetPos = 0;
}

CGameWorld::CGameWorld(CFileName& strFileName)
{
	Load(strFileName);
}

CGameWorld::~CGameWorld()
{
	Clear();
}


void CGameWorld::Load(CFileName& strFileName)
{
	Clear();

//	PrintF("Here\n");


	CFileStream file;
	char* name = NULL;
	char* desc = NULL;
	unsigned int bytesToRead;

	if (!file.open(strFileName, 0, 1))
	{
		Error("Unable to load level: %s\n", strFileName.getCharPointer());
	}

	if (!file.checkID("WORL"))
	{
		Error("Invalid level format\n");
	}

	file.ReadFromFile(&bytesToRead, sizeof(unsigned int));
	name = new char[bytesToRead + 1];
	file.ReadFromFile(name, bytesToRead);
	name[bytesToRead] = '\0';
	gw_strName = (const char*)name;

	file.ReadFromFile(&bytesToRead, sizeof(unsigned int));
	desc = new char[bytesToRead + 1];
	file.ReadFromFile(desc, bytesToRead);
	desc[bytesToRead] = '\0';
	gw_description = (const char*)desc;

	file.ReadFromFile(&gw_flags, sizeof(unsigned int));

	file.ReadFromFile(&gw_numRooms, sizeof(unsigned int));

	file.ReadFromFile(&gw_numBrushes, sizeof(unsigned int));

	file.ReadFromFile(&gw_numEntities, sizeof(unsigned int));

	file.ReadFromFile(&gw_nextEntityID, sizeof(unsigned int));

	file.ReadFromFile(&gw_nextRoomID, sizeof(unsigned int));

	file.ReadFromFile(&gw_bckgColour, sizeof(unsigned long));

	file.ReadFromFile(&gw_fGravity, sizeof(float));

//	file.ReadFromFile(&bytesToRead, sizeof(unsigned int));

	FOREACH_ARRAY(gw_numRooms)
	{
		CRoomBase room;
		room.Load(file);
		gw_rooms.push_back(room);
		room.Clear();
	}
	delete[] name;
	delete[] desc;
	
	FOREACH_ARRAY(gw_numBrushes) 
	{
		CBrush* brush = new CBrush;

		brush->Load(file);

		gw_brushes.push_back(brush);
	}

	skybox = new CSkyBox;
	skybox->Load(file);

	if (!skybox->loadSuccess) 
	{
		delete skybox;
		skybox = NULL;
		PrintF("Sky box NULL\n");
	}

	if (!file.checkID_Safe("SUNN"))
	{
		file.closeFile();
		return;
	}

	gw_Sun = new Sun_t;

	file.ReadFromFile(&gw_Sun->su_type, sizeof(byte));
	file.ReadFromFile(&gw_Sun->su_fSize, sizeof(float));

	file.ReadFromFile(&gw_Sun->su_aDirection.x, sizeof(float));
	file.ReadFromFile(&gw_Sun->su_aDirection.y, sizeof(float));
	file.ReadFromFile(&gw_Sun->su_aDirection.z, sizeof(float));

	HEXColor color;

	file.ReadFromFile(&color, sizeof(HEXColor));
	gw_Sun->su_ColorAmbient = HEX_toOGL(color);

	file.ReadFromFile(&color, sizeof(HEXColor));
	gw_Sun->su_ColorDiffuse = HEX_toOGL(color);

	file.ReadFromFile(&color, sizeof(HEXColor));
	gw_Sun->su_ColorSpecular = HEX_toOGL(color);

	file.closeFile();
}

void CGameWorld::LoadWithoutEntities(CFileStream& file)
{
	Clear();

	char* name = NULL;
	char* desc = NULL;
	unsigned int bytesToRead;

	if (!file.checkID("WORL"))
	{
		Throw("Invalid level format\n");
	}

	file.ReadFromFile(&bytesToRead, sizeof(unsigned int));
	name = new char[bytesToRead + 1];
	file.ReadFromFile(name, bytesToRead);
	name[bytesToRead] = '\0';
	gw_strName = (const char*)name;

	file.ReadFromFile(&bytesToRead, sizeof(unsigned int));
	desc = new char[bytesToRead + 1];
	file.ReadFromFile(desc, bytesToRead);
	desc[bytesToRead] = '\0';
	gw_description = (const char*)desc;

	file.ReadFromFile(&gw_flags, sizeof(unsigned int));

	file.ReadFromFile(&gw_numRooms, sizeof(unsigned int));

	file.ReadFromFile(&gw_numBrushes, sizeof(unsigned int));

	file.ReadFromFile(&gw_numEntities, sizeof(unsigned int));

	file.ReadFromFile(&gw_nextEntityID, sizeof(unsigned int));

	file.ReadFromFile(&gw_nextRoomID, sizeof(unsigned int));

	file.ReadFromFile(&gw_bckgColour, sizeof(unsigned long));

	file.ReadFromFile(&gw_fGravity, sizeof(float));

	//	file.ReadFromFile(&bytesToRead, sizeof(unsigned int));

	FOREACH_ARRAY(gw_numRooms)
	{
		CRoomBase room;
		room.Load(file);
		gw_rooms.push_back(room);
		room.Clear();
	}
	delete[] name;
	delete[] desc;

	FOREACH_ARRAY(gw_numBrushes)
	{
		CBrush* brush = new CBrush;

		brush->Load(file);

		gw_brushes.push_back(brush);
	}

	skybox = new CSkyBox;
	skybox->Load(file);

	if (!skybox->loadSuccess)
	{
		delete skybox;
		skybox = NULL;
		PrintF("Sky box NULL\n");
	}
	
	
	if (!file.checkID_Safe("SUNN"))
	{
		return;
	}

	gw_Sun = new Sun_t;

	file.ReadFromFile(&gw_Sun->su_type, sizeof(byte));
	file.ReadFromFile(&gw_Sun->su_fSize, sizeof(float));


	file.ReadFromFile(&gw_Sun->su_aDirection.x, sizeof(float));
	file.ReadFromFile(&gw_Sun->su_aDirection.y, sizeof(float));
	file.ReadFromFile(&gw_Sun->su_aDirection.z, sizeof(float));

	HEXColor color;

	file.ReadFromFile(&color, sizeof(HEXColor));
	gw_Sun->su_ColorAmbient = HEX_toOGL(color);

	
	file.ReadFromFile(&color, sizeof(HEXColor));
	gw_Sun->su_ColorDiffuse = HEX_toOGL(color);

	
	file.ReadFromFile(&color, sizeof(HEXColor));	
	gw_Sun->su_ColorSpecular = HEX_toOGL(color);



	FOREACH_ARRAY(gw_numRooms) 
	{
		gw_rooms[i].ro_world = this;

		for (int j = 0; j < gw_rooms[i].ro_polygons.size(); j++) 
		{
			gw_rooms[i].ro_polygons[j].pr_room = &gw_rooms[i];
		}
	}

	FOREACH_ARRAY(gw_numBrushes)
	{
		gw_brushes[i]->m_world = this;

		for (int j = 0; j < gw_brushes[i]->m_brushPolygons.size(); j++)
		{
			gw_brushes[i]->m_brushPolygons[j]->pr_brush = gw_brushes[i];
		}
	}


}

void CGameWorld::deleteOneEntity(CEntityMain* entity) 
{
	if (entity == NULL) return;

	auto toDelete = std::find(gw_allEntities.begin(), gw_allEntities.end(), entity);

	if (toDelete != gw_allEntities.end())
	{
		unsigned int index = toDelete - gw_allEntities.begin();

		// delete entity
		delete  gw_allEntities[index];
		gw_allEntities[index] = NULL;

		gw_allEntities.erase(gw_allEntities.begin() + index);
	}
}

void CGameWorld::SaveWithoutEntities(CFileStream& file)
{
	file.WriteToFile("WORL", sizeof(char) * 4);

	//	file.checkVersion(1);

	unsigned int bytestoWrite = gw_strName.length();
	file.WriteToFile(&bytestoWrite, sizeof(unsigned int));
	file.WriteToFile(gw_strName.c_str(), gw_strName.length() * sizeof(char));

	bytestoWrite = gw_description.length();
	file.WriteToFile(&bytestoWrite, sizeof(unsigned int));
	file.WriteToFile(gw_description.c_str(), gw_description.length() * sizeof(char));

	file.WriteToFile(&gw_flags, sizeof(unsigned int));

	file.WriteToFile(&gw_numRooms, sizeof(unsigned int));

	file.WriteToFile(&gw_numBrushes, sizeof(unsigned int));

	file.WriteToFile(&gw_numEntities, sizeof(unsigned int));

	file.WriteToFile(&gw_nextEntityID, sizeof(unsigned int));

	file.WriteToFile(&gw_nextRoomID, sizeof(unsigned int));

	file.WriteToFile(&gw_bckgColour, sizeof(unsigned long));

	file.WriteToFile(&gw_fGravity, sizeof(float));


	FOREACH_ARRAY(gw_numRooms)
	{
		gw_rooms[i].Save(file);
	}

	FOREACH_ARRAY(gw_numBrushes)
	{
		gw_brushes[i]->Save(file);
	}

	if (skybox != NULL)
	{
		skybox->Save(file);
	}

	if (gw_Sun != NULL)
	{
		file.WriteToFile("SUNN", sizeof(char) * 4);

		file.WriteToFile(&gw_Sun->su_type,  sizeof(byte));
		file.WriteToFile(&gw_Sun->su_fSize, sizeof(float));

		file.WriteToFile(&gw_Sun->su_aDirection.x, sizeof(float));
		file.WriteToFile(&gw_Sun->su_aDirection.y, sizeof(float));
		file.WriteToFile(&gw_Sun->su_aDirection.z, sizeof(float));

		HEXColor color = OGL_toHEX(gw_Sun->su_ColorAmbient);
		file.WriteToFile(&color, sizeof(HEXColor));

		color = OGL_toHEX(gw_Sun->su_ColorDiffuse);
		file.WriteToFile(&color, sizeof(HEXColor));

		color = OGL_toHEX(gw_Sun->su_ColorSpecular);
		file.WriteToFile(&color, sizeof(HEXColor));
	}


}

void CGameWorld::SetUpPhysics() 
{
	if (gw_openInEditor) 
	{
		PrintF("Cannot init physics engine, because world opened by In the Editor!\n");
		return;
	}

	gw_collisionDispatcher.setWorld(this);


	gw_CollisionConfiguration = new btDefaultCollisionConfiguration;
	gw_ColDispatcher = new btCollisionDispatcher(gw_CollisionConfiguration);
	gw_Broadphase = new btDbvtBroadphase;
	gw_Csolver = new btSequentialImpulseConstraintSolver;
	gw_DynamicsWorld = new btDiscreteDynamicsWorld
	(
		gw_ColDispatcher, gw_Broadphase, gw_Csolver, gw_CollisionConfiguration
	);
	gw_DynamicsWorld->setGravity(btVector3(0, -gw_fGravity, 0));

	gw_WorldTriangleMesh = new btTriangleMesh;

	FOREACH_ARRAY(gw_numRooms)
	{

		unsigned int numPolygons = gw_rooms[i].ro_polygons.size();

		for (unsigned int poly = 0; poly < numPolygons; poly++)
		{

			// skip if we have pasables polygons
			if (gw_rooms[i].ro_polygons[poly].pr_flags & POLYGON_IS_PASSABLE/*/ || gw_rooms[i].ro_polygons[poly].pr_flags & POLYGON_INVISIBLE*/) continue;

			Vector3D v1 = gw_rooms[i].ro_polygons[poly].verticies[0].positionInRoomOrBrush;
			Vector3D v2 = gw_rooms[i].ro_polygons[poly].verticies[1].positionInRoomOrBrush;
			Vector3D v3 = gw_rooms[i].ro_polygons[poly].verticies[2].positionInRoomOrBrush;
			Vector3D v4 = gw_rooms[i].ro_polygons[poly].verticies[3].positionInRoomOrBrush;

			// because our polygons is reactangle, his contain 2 triangles, so we call 2 time addTriangle
			gw_WorldTriangleMesh->addTriangle
			(
				btVector3(v1.x, v1.y, v1.z), btVector3(v2.x, v2.y, v2.z), btVector3(v3.x, v3.y, v3.z)
			);

			gw_WorldTriangleMesh->addTriangle
			(
				btVector3(v1.x, v1.y, v1.z), btVector3(v3.x, v3.y, v3.z), btVector3(v4.x, v4.y, v4.z)
			);
			
		}	
	}

	FOREACH_ARRAY(gw_numBrushes)
	{

		unsigned int numPolygons = gw_brushes[i]->m_brushPolygons.size();

		for (unsigned int poly = 0; poly < numPolygons; poly++)
		{

			// skip if we have pasables polygons
			if (gw_brushes[i]->m_brushPolygons[poly]->pr_flags & POLYGON_IS_PASSABLE/* || gw_brushes[i]->m_brushPolygons[poly]->pr_flags & POLYGON_INVISIBLE*/) continue;

			Vector3D v1 = gw_brushes[i]->m_brushPolygons[poly]->verticies[0].positionInWorld;
			Vector3D v2 = gw_brushes[i]->m_brushPolygons[poly]->verticies[1].positionInWorld;
			Vector3D v3 = gw_brushes[i]->m_brushPolygons[poly]->verticies[2].positionInWorld;
			Vector3D v4 = gw_brushes[i]->m_brushPolygons[poly]->verticies[3].positionInWorld;

			// because our polygons is reactangle, his contain 2 triangles, so we call 2 time addTriangle
			gw_WorldTriangleMesh->addTriangle
			(
				btVector3(v1.x, v1.y, v1.z), btVector3(v2.x, v2.y, v2.z), btVector3(v3.x, v3.y, v3.z)
			);

			gw_WorldTriangleMesh->addTriangle
			(
				btVector3(v1.x, v1.y, v1.z), btVector3(v3.x, v3.y, v3.z), btVector3(v4.x, v4.y, v4.z)
			);

		}
	}

	btDefaultMotionState* gw_groundMotionState = new btDefaultMotionState(btTransform::getIdentity());
	
	gw_LevelShape = new btBvhTriangleMeshShape(gw_WorldTriangleMesh, true);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, gw_groundMotionState, gw_LevelShape);

	gw_LevelBody = new btRigidBody(rbInfo);

	// for ghost objects
	gw_DynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	// add main world collision from Rooms
	gw_DynamicsWorld->addRigidBody(gw_LevelBody, COLLISION_STATIC, COLLISION_ALL);
	//FOREACH_ARRAY(gw_allEntities.size())
	//{
	//	break;
	//}

	gw_entityWorld = new CEntityMain;
	gw_entityWorld->m_pwo = this;
	gw_entityWorld->m_ulFlags &= ~ENTITY_MODEL;
	gw_entityWorld->m_ulFlags |= ENTITY_EDITOR;

	gw_entityWorld->m_strClassName = "World";

	gw_LevelBody->setUserPointer((void*)gw_entityWorld);

	gContactAddedCallback = collisionRigidContact;

//	gw_entityWorld->InitCollisionInfo();	

}

void CGameWorld::Save(CFileName& strFileName)
{
	CFileStream file;

	unsigned int bytestoWrite;

	if (!file.open(strFileName, 0, 0))
		Error("Unable to save level!\n");

	file.WriteToFile("WORL", sizeof(char) * 4);

//	file.checkVersion(1);

	bytestoWrite = gw_strName.length();
	file.WriteToFile(&bytestoWrite, sizeof(unsigned int));
	file.WriteToFile(gw_strName.c_str(), gw_strName.length() * sizeof(char));

	bytestoWrite = gw_description.length();
	file.WriteToFile(&bytestoWrite, sizeof(unsigned int));
	file.WriteToFile(gw_description.c_str(), gw_description.length() * sizeof(char));

	file.WriteToFile(&gw_flags, sizeof(unsigned int));

	file.WriteToFile(&gw_numRooms, sizeof(unsigned int));

	file.WriteToFile(&gw_numBrushes, sizeof(unsigned int));

	file.WriteToFile(&gw_numEntities, sizeof(unsigned int));

	file.WriteToFile(&gw_nextEntityID, sizeof(unsigned int));

	file.WriteToFile(&gw_nextRoomID, sizeof(unsigned int));

	file.WriteToFile(&gw_bckgColour, sizeof(unsigned long));

	file.WriteToFile(&gw_fGravity, sizeof(float));
//	bytestoWrite = gw_rooms.size();
//	file.WriteToFile(&bytestoWrite, sizeof(unsigned int));

	FOREACH_ARRAY(gw_numRooms)
	{
		gw_rooms[i].Save(file);
	}

	FOREACH_ARRAY(gw_numBrushes) 
	{
		gw_brushes[i]->Save(file);
	}

	if (skybox != NULL)
	{
		skybox->Save(file);
	}

	if (gw_Sun != NULL)
	{
		file.WriteToFile("SUNN", sizeof(char) * 4);

		file.WriteToFile(&gw_Sun->su_type, sizeof(byte));
		file.WriteToFile(&gw_Sun->su_fSize, sizeof(float));

		file.WriteToFile(&gw_Sun->su_aDirection.x, sizeof(float));
		file.WriteToFile(&gw_Sun->su_aDirection.y, sizeof(float));
		file.WriteToFile(&gw_Sun->su_aDirection.z, sizeof(float));

		HEXColor color = OGL_toHEX(gw_Sun->su_ColorAmbient);
		file.WriteToFile(&color, sizeof(HEXColor));

		color = OGL_toHEX(gw_Sun->su_ColorDiffuse);
		file.WriteToFile(&color, sizeof(HEXColor));

		color = OGL_toHEX(gw_Sun->su_ColorSpecular);
		file.WriteToFile(&color, sizeof(HEXColor));
	}

	file.closeFile();
}

void CGameWorld::EndPysics(void)
{

	//gw_player = nullptr;

	gw_collisionDispatcher.Clear();

	FOREACH_ARRAY(gw_allPhysicsObjects.size()) 
	{
		if (gw_allPhysicsObjects[i] == nullptr) continue;

		gw_DynamicsWorld->removeRigidBody(gw_allPhysicsObjects[i]);
		delete gw_allPhysicsObjects[i]->getMotionState();
		delete gw_allPhysicsObjects[i];
		gw_allPhysicsObjects[i] = NULL;
	}

	gw_allPhysicsObjects.clear();

	if (gw_LevelBody != nullptr) 
	{
		gw_LevelBody->setUserPointer(NULL);

		gw_DynamicsWorld->removeRigidBody(gw_LevelBody);
		delete gw_LevelBody->getMotionState();
		delete gw_LevelBody;
	}

	gw_LevelBody = nullptr;

	if (gw_CollisionConfiguration != nullptr)
	{
		delete gw_CollisionConfiguration;
		gw_CollisionConfiguration = nullptr;
	}
	if (gw_ColDispatcher != nullptr) 
	{ 
		delete gw_ColDispatcher; 
		gw_ColDispatcher = nullptr; 
	}
	if (gw_DynamicsWorld != nullptr) 
	{ 
		delete gw_DynamicsWorld; 
		gw_DynamicsWorld = nullptr;
	}

	if (gw_Broadphase != nullptr) 
	{ 
		delete gw_Broadphase; 
		gw_Broadphase = nullptr; 
	}

	if (gw_Csolver != nullptr) 
	{ 
		delete gw_Csolver; 
		gw_Csolver = nullptr;
	}

	if (gw_WorldTriangleMesh != nullptr)
	{
		delete gw_WorldTriangleMesh;
		gw_WorldTriangleMesh = nullptr;
	}

	if (gw_LevelShape != nullptr)
	{
		delete gw_LevelShape;
		gw_LevelShape = nullptr;
	}
}

void CGameWorld::UpdatePhysics(double fTime) 
{
	int maxSubSteps = 10;

	gw_DynamicsWorld->stepSimulation(fTime, maxSubSteps, 1.0f / 120.f);

	gw_collisionDispatcher.handleCollision();
}

void CGameWorld::Clear(void) 
{

	FOREACH_ARRAY(gw_allEntities.size())
	{
		if (gw_allEntities[i]) {
			gw_allEntities[i]->End();
			delete gw_allEntities[i];
		}
	}

	gw_allEntities.clear();

	EndPysics();

	FOREACH_ARRAY(gw_allImportedEntities.size()) 
	{
		if (gw_allImportedEntities[i])
			delete gw_allImportedEntities[i];
	}

	gw_allImportedEntities.clear();

	for (unsigned int i = 0; i < gw_rooms.size(); i++)
	{
		gw_rooms[i].Clear();
	}
	gw_rooms.clear();


	FOREACH_ARRAY(gw_brushes.size()) 
	{
		delete gw_brushes[i];
	}

	gw_brushes.clear();

	if (skybox != NULL)
	{
		skybox->Clear();
		delete skybox;
	}

	if (gw_Sun != NULL)
		delete gw_Sun;

	gw_Sun = NULL;
	skybox = NULL;
}