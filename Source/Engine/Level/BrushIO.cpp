#include <stdAfx.h>

#include <Engine/Model/Import3D.h>

#include "Room.h"
#include "Brush.h"


/*
///////////////////////////////////////////////////////
Create brush from obj wavefront

Note: obj model must be grouping on materials for each plane
for example, if we have model cube - we have a 6 materials,
because cube has 6 faces, and 12 triangles

///////////////////////////////////////////////////////
*/
void CBrush::createFromOBJ(CFileOBJ& obj) 
{
	Clear();


	std::vector<CVertex> vtxBrush;
	std::vector<Vector2D> uvMapping;
	std::vector<Vector3D> vNormals;


	FOREACH_ARRAY(obj.vertexList.size()) 
	{
		CVertex vertx;
		vertx.positionInRoomOrBrush = obj.vertexList[i];
		vtxBrush.push_back(vertx);
	}

	FOREACH_ARRAY(obj.vertexTextureList.size()) 
	{
		Vector2D v2 = obj.vertexTextureList[i];
		uvMapping.push_back(v2);
	}

	objFace_t* faces = &obj.faceList[0];
	objMaterial_t* materials = &obj.materialList[0];

	int index = 0;

	for (unsigned int i = 0; i < obj.faceList.size(); i++)
	{		

		std::string matName = faces[index].Name;

		CPolygonWorld* polygon = new CPolygonWorld;

		while (matName == faces[index].Name)
		{
			CVertex vtx[3];
			vtx[0] = vtxBrush[faces[index].VertexIndex[0] - 1];
			vtx[1] = vtxBrush[faces[index].VertexIndex[1] - 1];
			vtx[2] = vtxBrush[faces[index].VertexIndex[2] - 1];

			Vector2D v2[3];

			v2[0] = uvMapping[faces[index].TextureIndex[0] - 1];
			v2[1] = uvMapping[faces[index].TextureIndex[1] - 1];
			v2[2] = uvMapping[faces[index].TextureIndex[2] - 1];


			polygon->verticies.push_back(vtx[0]);
			polygon->verticies.push_back(vtx[1]);
			polygon->verticies.push_back(vtx[2]);

		
		}
	}
}