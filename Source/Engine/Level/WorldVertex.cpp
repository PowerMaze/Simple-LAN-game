#include <stdAfx.h>
#include "Room.h"
//#include <Engine\Common\FileName.h>

CVertex::CVertex()
{
	positionInWorld = glm::vec3(0.0f, 0.0f, 0.0f);
	positionInRoomOrBrush = glm::vec3(0.0f, 0.0f, 0.0f);
}

void CVertex::ReadData(CFileStream& file)
{

	file.ReadFromFile(&positionInWorld.x, sizeof(float));
	file.ReadFromFile(&positionInWorld.y, sizeof(float));
	file.ReadFromFile(&positionInWorld.z, sizeof(float));

	file.ReadFromFile(&positionInRoomOrBrush.x, sizeof(float));
	file.ReadFromFile(&positionInRoomOrBrush.y, sizeof(float));
	file.ReadFromFile(&positionInRoomOrBrush.z, sizeof(float));
}

void CVertex::WriteData(CFileStream& file)
{

	file.WriteToFile(&positionInWorld.x, sizeof(float));
	file.WriteToFile(&positionInWorld.y, sizeof(float));
	file.WriteToFile(&positionInWorld.z, sizeof(float));

	file.WriteToFile(&positionInRoomOrBrush.x, sizeof(float));
	file.WriteToFile(&positionInRoomOrBrush.y, sizeof(float));
	file.WriteToFile(&positionInRoomOrBrush.z, sizeof(float));
}