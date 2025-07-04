#include <stdAfx.h>

#include "Room.h"

//#include <Engine\Common\FileName.h>
//#include <Engine\Graphics\Texture.h>
//#include <Engine\Graphics\Shader.h>
#include <Engine\Model\ImportOBJ.h>

CRoomBase* createRoomFromObj(COBJDecoder& obj, glm::mat4& mat_transform)
{
	CRoomBase* baseRoom = nullptr;
	CPolygonWorld* polygonRoom = nullptr;

	std::vector<CPolygonWorld> polygons;
	CVertex* vertexData = nullptr;

	return baseRoom;
}

void addPolygon(CRoomBase* room, float height, float width, float length) 
{
	CFileName strFile = "C:\\Games\\In-the-Power\\Data\\Textures\\wood.te";
	CPolygonWorld polygon;

	CVertex vertex[4];

	Matrix4 mat = Matrix4(1.f);

	std::vector<glm::vec2> textureUV =
	{
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 1.0f)
	};

	//vertex[0].positionInRoomOrBrush = glm::vec3(width, height, length);
	//vertex[1].positionInRoomOrBrush = glm::vec3(width, -height, length);
	//vertex[2].positionInRoomOrBrush = glm::vec3(-width, -height, length);
	//vertex[3].positionInRoomOrBrush = glm::vec3(-width, height, length);

	// left-up corner
	vertex[0].positionInRoomOrBrush.x = width;
	vertex[0].positionInRoomOrBrush.y = height;
	vertex[0].positionInRoomOrBrush.z = length;
	// right-up corner
	vertex[1].positionInRoomOrBrush.x = width;
	vertex[1].positionInRoomOrBrush.y = -height;
	vertex[1].positionInRoomOrBrush.z = length;
	// right-down corner
	vertex[2].positionInRoomOrBrush.x = -width;
	vertex[2].positionInRoomOrBrush.y = -height;
	vertex[2].positionInRoomOrBrush.z = length;
	// left-down corner
	vertex[3].positionInRoomOrBrush.x = -width;
	vertex[3].positionInRoomOrBrush.y = height;
	vertex[3].positionInRoomOrBrush.z = length;
/*
	vertex[0].positionInRoomOrBrush.x = -width;
	vertex[0].positionInRoomOrBrush.y = height;
	vertex[0].positionInRoomOrBrush.z = length;
	// right-up corner
	vertex[1].positionInRoomOrBrush.x = width;
	vertex[1].positionInRoomOrBrush.y = height;
	vertex[1].positionInRoomOrBrush.z = length;
	// right-down corner
	vertex[2].positionInRoomOrBrush.x = width;
	vertex[2].positionInRoomOrBrush.y = height;
	vertex[2].positionInRoomOrBrush.z = -length;
	// left-down corner
	vertex[3].positionInRoomOrBrush.x = -width;
	vertex[3].positionInRoomOrBrush.y = height;
	vertex[3].positionInRoomOrBrush.z = -length;
*/
	FOREACH_ARRAY(4)	
		vertex[i].positionInWorld = glm::vec3(0.0f);

	FOREACH_ARRAY(4)
		polygon.verticies.push_back(vertex[i]);


	polygon.pr_flags = 0x0UL;
	// set default texture UV
	polygon.textureUVs = textureUV;
	polygon.tex01FileName = strFile;
	// load texture data
	polygon.texture01.Load(strFile);
	// set tag of polygon
	polygon.tag = 0;
	// process data for rendering polygon
	polygon.processData();

	polygon.vNormal = VECTOR3(0.f, 1.f, 0.f);

	room->ro_polygons.push_back(polygon);
	room->uRoomID = 1;

	polygon.Clear();
	textureUV.clear();

}

void addRoom(CRoomBase* room, float height, float width, float length, int dimensions, unsigned long ulRoomTag, const glm::vec3& vPos)
{
//	CRoomBase* room = new CRoomBase;
	/*
	/////////////////////////////////////////////////

		In the Engine have indexes polygons.
		0 - floor
		1 - cel
		2 - right wall
		3 - left wall
		4 - front fall
		5 - back wall
	
	/////////////////////////////////////////////////
	*/



	// by default set room as 4 walls
	dimensions = iClamp(dimensions, 4);
	// polygons array
	std::vector <CPolygonWorld> polygons;
//	std::array<CPolygonWorld, 6> polygons = {};
	// texture data for wall
	CFileName strFile = "Data\\Textures\\Bricks.te";

	std::vector<glm::vec2> textureUV =
	{
		glm::vec2(1.0f * length, 1.0f * width),
		glm::vec2(1.0f * length, 0.0f * width),
		glm::vec2(0.0f * length, 0.0f * width),
		glm::vec2(0.0f * length, 1.0f * width)
	};
	// polygon pointer object
	CPolygonWorld polygon;// = NULL;

	CVertex vertex[4];

	/****************  cell  *****************/

	// left-up corner
	vertex[3].positionInRoomOrBrush.x = -width;
	vertex[3].positionInRoomOrBrush.y = height;
	vertex[3].positionInRoomOrBrush.z = length;
	// right-up corner
	vertex[2].positionInRoomOrBrush.x = width;
	vertex[2].positionInRoomOrBrush.y = height;
	vertex[2].positionInRoomOrBrush.z = length;
	// right-down corner
	vertex[1].positionInRoomOrBrush.x = width;
	vertex[1].positionInRoomOrBrush.y = height;
	vertex[1].positionInRoomOrBrush.z = -length;
	// left-down corner
	vertex[0].positionInRoomOrBrush.x = -width;
	vertex[0].positionInRoomOrBrush.y = height;
	vertex[0].positionInRoomOrBrush.z = -length;

	glm::mat4 mat = glm::mat4(1.0f);

	for (unsigned i = 0; i < 4; i++)
	{
		mat = Matrix4(1.f);
		mat = glm::translate(mat, vPos);
		Vector4D v(vertex[i].positionInRoomOrBrush.x, vertex[i].positionInRoomOrBrush.y, vertex[i].positionInRoomOrBrush.z, 1.f);
		v = mat * v;
		vertex[i].positionInRoomOrBrush = Vector3D(v);
	}

	// set a position a default, we translate if need by translation matrix
	for (unsigned i = 0; i < 4; i++)
		vertex[i].positionInWorld = glm::vec3(0.0f);

	// new polygon
	//polygon = new CPolygonWorld;

	// set standard flags
	polygon.pr_flags |= POLYGON_INVISIBLE;

//	polygon.notRender = true;

	// set default texture UV
	polygon.textureUVs = textureUV;
	polygon.tex01FileName = strFile;
	// load texture data
//	polygon.texture01.Load(strFile);
	// push data vertexies
	for (unsigned i = 0; i < 4; i++)
		polygon.verticies.push_back(vertex[i]);
	// set tag of polygon
	polygon.tag = 0;
	// process data for rendering polygon
	polygon.processData();
	// push polygons

	polygon.vNormal = VECTOR3(0.f, -1.f, 0.f);

	polygons.push_back(polygon);

	polygon.Clear();

	/****************   floor   *****************/
	
	strFile = "Data\\Textures\\Nature\\Grass\\Grass01.te";
	//strFile = "C:\\Games\\In-the-Power\\Data\\Textures\\wood.te";

	vertex[0].positionInRoomOrBrush.x = -width;
	vertex[0].positionInRoomOrBrush.y = 0.0f;
	vertex[0].positionInRoomOrBrush.z = length;

	vertex[1].positionInRoomOrBrush.x = width;
	vertex[1].positionInRoomOrBrush.y = 0.0f;
	vertex[1].positionInRoomOrBrush.z = length;

	vertex[2].positionInRoomOrBrush.x = width;
	vertex[2].positionInRoomOrBrush.y = 0.0f;
	vertex[2].positionInRoomOrBrush.z = -length;

	vertex[3].positionInRoomOrBrush.x = -width;
	vertex[3].positionInRoomOrBrush.y = 0.0f;
	vertex[3].positionInRoomOrBrush.z = -length;
	

	for (unsigned i = 0; i < 4; i++)
	{
		mat = Matrix4(1.f);
		mat = glm::translate(mat, vPos);
		Vector4D v(vertex[i].positionInRoomOrBrush.x, vertex[i].positionInRoomOrBrush.y, vertex[i].positionInRoomOrBrush.z, 1.f);
		v = mat * v;
		vertex[i].positionInRoomOrBrush = Vector3D(v);
	}

	for (unsigned i = 0; i < 4; i++)
		vertex[i].positionInWorld = glm::vec3(0.0f);

	polygon.pr_flags |= 0x0UL;
//	polygon.shader.Create_t("C:\\Games\\In-the-Power\\System\\Shaders\\Model_Test\\vertShader.shr",
//		"C:\\Games\\In-the-Power\\System\\Shaders\\Model_Test\\fragShader.shr");

	polygon.notRender = false;

	polygon.textureUVs = textureUV;
	polygon.tex01FileName = strFile;
//	polygon.texture01.Load(strFile);

	for (unsigned i = 0; i < 4; i++)
		polygon.verticies.push_back(vertex[i]);
	polygon.pr_flags |= POLYGON_INVISIBLE;
	polygon.tag = 1;
	polygon.processData();

	polygon.vNormal = VECTOR3(0.f, 1.f, 0.f);

	polygons.push_back(polygon);

	polygon.Clear();


	textureUV.clear();

	/*************  right wall  ***************/
	
	
	textureUV =
	{
		glm::vec2(1.0f * height * 0.5f, 1.0f * width),
		glm::vec2(1.0f * height * 0.5f, 0.0f * width),
		glm::vec2(0.0f * height * 0.5f, 0.0f * width),
		glm::vec2(0.0f * height * 0.5f, 1.0f * width)
	};
	
	float angleRadians = glm::radians(-90.0f);

	// Створення матриці обертання
	glm::mat2 rotationMatrix = glm::mat2(
		glm::cos(angleRadians), -glm::sin(angleRadians),
		glm::sin(angleRadians), glm::cos(angleRadians)
	);

	// Обертання кожного вектора
	for (auto& uv : textureUV) {
		uv = rotationMatrix * uv;
	}

	strFile = "Data\\Textures\\Bricks.te";

	vertex[0].positionInRoomOrBrush.x = width;
	vertex[0].positionInRoomOrBrush.y = height;
	vertex[0].positionInRoomOrBrush.z = length;

	vertex[1].positionInRoomOrBrush.x = width;
	vertex[1].positionInRoomOrBrush.y = height;
	vertex[1].positionInRoomOrBrush.z = -length;

	vertex[2].positionInRoomOrBrush.x = width;
	vertex[2].positionInRoomOrBrush.y = 0.0f;
	vertex[2].positionInRoomOrBrush.z = -length;

	vertex[3].positionInRoomOrBrush.x = width;
	vertex[3].positionInRoomOrBrush.y = 0.0f;
	vertex[3].positionInRoomOrBrush.z = length;

	for (unsigned i = 0; i < 4; i++)
	{
		mat = Matrix4(1.f);
		mat = glm::translate(mat, vPos);
		Vector4D v(vertex[i].positionInRoomOrBrush.x, vertex[i].positionInRoomOrBrush.y, vertex[i].positionInRoomOrBrush.z, 1.f);
		v = mat * v;
		vertex[i].positionInRoomOrBrush = Vector3D(v);
	}

	for (unsigned i = 0; i < 4; i++)
		vertex[i].positionInWorld = glm::vec3(0.0f);

	polygon.vNormal = VECTOR3(-1.f, 0.f, 0.f);

	polygon.pr_flags |= 0x0UL;
	polygon.pr_flags |= POLYGON_INVISIBLE;
	polygon.textureUVs = textureUV;
	polygon.tex01FileName = strFile;
//	polygon.texture01.Load(strFile);

	for (unsigned i = 0; i < 4; i++)
		polygon.verticies.push_back(vertex[i]);

	polygon.tag = 2;
	polygon.processData();

	polygons.push_back(polygon);

	polygon.Clear();

	/*************  left wall  ****************/
	
	vertex[0].positionInRoomOrBrush.x = -width;
	vertex[0].positionInRoomOrBrush.y = height;
	vertex[0].positionInRoomOrBrush.z = -length;

	vertex[1].positionInRoomOrBrush.x = -width;
	vertex[1].positionInRoomOrBrush.y = height;
	vertex[1].positionInRoomOrBrush.z = length;

	vertex[2].positionInRoomOrBrush.x = -width;
	vertex[2].positionInRoomOrBrush.y = 0.0f;
	vertex[2].positionInRoomOrBrush.z = length;

	vertex[3].positionInRoomOrBrush.x = -width;
	vertex[3].positionInRoomOrBrush.y = 0.0f;
	vertex[3].positionInRoomOrBrush.z = -length;

	
	for (unsigned i = 0; i < 4; i++)
	{
		mat = Matrix4(1.f);
		mat = glm::translate(mat, vPos);
		Vector4D v(vertex[i].positionInRoomOrBrush.x, vertex[i].positionInRoomOrBrush.y, vertex[i].positionInRoomOrBrush.z, 1.f);
		v = mat * v;
		vertex[i].positionInRoomOrBrush = Vector3D(v);
	}

	for (unsigned i = 0; i < 4; i++)
		vertex[i].positionInWorld = glm::vec3(0.0f);

	polygon.pr_flags |= 0x0UL;
//	polygon.shader.Create_t("C:\\Games\\In-the-Power\\System\\Shaders\\Model_Test\\vertShader.shr",
//		"C:\\Games\\In-the-Power\\System\\Shaders\\Model_Test\\fragShader.shr");
	polygon.pr_flags |= POLYGON_INVISIBLE;
	polygon.textureUVs = textureUV;
	polygon.tex01FileName = strFile;
//	polygon.texture01.Load(strFile);

	for (unsigned i = 0; i < 4; i++)
		polygon.verticies.push_back(vertex[i]);

	polygon.tag = 3;
	polygon.processData();

	polygon.vNormal = VECTOR3(1.f, 0.f, 0.f);

	polygons.push_back(polygon);

	polygon.Clear();

	/*************  front wall  ****************/
	
	// 0 index for EBO
	vertex[0].positionInRoomOrBrush.x = -width; 
	vertex[0].positionInRoomOrBrush.y = height; 
	vertex[0].positionInRoomOrBrush.z = length;
	// 1 index for EBO
	vertex[1].positionInRoomOrBrush.x = width;
	vertex[1].positionInRoomOrBrush.y = height;
	vertex[1].positionInRoomOrBrush.z = length;
	// 2 index for EBO
	vertex[2].positionInRoomOrBrush.x = width;
	vertex[2].positionInRoomOrBrush.y = 0.0f;
	vertex[2].positionInRoomOrBrush.z = length;
	// 3 index for EBO
	vertex[3].positionInRoomOrBrush.x = -width;
	vertex[3].positionInRoomOrBrush.y = 0.0f;
	vertex[3].positionInRoomOrBrush.z = length;

	for (unsigned i = 0; i < 4; i++)
	{
		mat = Matrix4(1.f);
		mat = glm::translate(mat, vPos);
		Vector4D v(vertex[i].positionInRoomOrBrush.x, vertex[i].positionInRoomOrBrush.y, vertex[i].positionInRoomOrBrush.z, 1.f);
		v = mat * v;
		vertex[i].positionInRoomOrBrush = Vector3D(v);
	}

	for (unsigned i = 0; i < 4; i++)
		vertex[i].positionInWorld = glm::vec3(0.0f);

	polygon.pr_flags |= 0x0UL;
//	polygon.shader.Create_t("C:\\Games\\In-the-Power\\System\\Shaders\\Model_Test\\vertShader.shr",
//		"C:\\Games\\In-the-Power\\System\\Shaders\\Model_Test\\fragShader.shr");
	polygon.pr_flags |= POLYGON_INVISIBLE;
	polygon.textureUVs = textureUV;
	polygon.tex01FileName = strFile;
//	polygon.texture01.Load(strFile);

	for (unsigned i = 0; i < 4; i++)
		polygon.verticies.push_back(vertex[i]);

	polygon.tag = 4;
	polygon.processData();

	polygon.vNormal = VECTOR3(0.f, 0.f, -1.f);

	polygons.push_back(polygon);

	polygon.Clear();

	/*************  back wall  ****************/

	vertex[0].positionInRoomOrBrush.x = width;
	vertex[0].positionInRoomOrBrush.y = height;
	vertex[0].positionInRoomOrBrush.z = -length;

	vertex[1].positionInRoomOrBrush.x = -width;
	vertex[1].positionInRoomOrBrush.y = height;
	vertex[1].positionInRoomOrBrush.z = -length;

	vertex[2].positionInRoomOrBrush.x = -width;
	vertex[2].positionInRoomOrBrush.y = 0.0f;
	vertex[2].positionInRoomOrBrush.z = -length;

	vertex[3].positionInRoomOrBrush.x = width;
	vertex[3].positionInRoomOrBrush.y = 0.0f;
	vertex[3].positionInRoomOrBrush.z = -length;

	
	for (unsigned i = 0; i < 4; i++)
	{
		mat = Matrix4(1.f);
		mat = glm::translate(mat, vPos);
		Vector4D v(vertex[i].positionInRoomOrBrush.x, vertex[i].positionInRoomOrBrush.y, vertex[i].positionInRoomOrBrush.z, 1.f);
		v = mat * v;
		vertex[i].positionInRoomOrBrush = Vector3D(v);
	}

	for (unsigned i = 0; i < 4; i++)
		vertex[i].positionInWorld = glm::vec3(0.0f);

	polygon.pr_flags |= 0x0UL;
//	polygon.shader.Create_t("C:\\Games\\In-the-Power\\System\\Shaders\\Model_Test\\vertShader.shr",
//		"C:\\Games\\In-the-Power\\System\\Shaders\\Model_Test\\fragShader.shr");

	polygon.pr_flags |= POLYGON_INVISIBLE;
	polygon.textureUVs = textureUV;
	polygon.tex01FileName = strFile;
//	polygon.texture01.Load(strFile);

	for (unsigned i = 0; i < 4; i++)
		polygon.verticies.push_back(vertex[i]);

	polygon.tag = 5;
	polygon.processData();

	polygon.vNormal = VECTOR3(0.f, 0.f, 1.f);

	polygons.push_back(polygon);

	// done

	room->ro_polygons = polygons;
	room->uRoomID = ulRoomTag;

	polygons.clear();
	polygon.Clear();
//	delete room;
}

CRoomBase::CRoomBase() 
{
	ro_world = NULL; 
	uRoomID=0;
}


void CRoomBase::RenderRoom(CShader &sha) 
{
	for (uint i = 0; i < ro_polygons.size(); i ++)
	{
		ro_polygons[i].Render(sha);
	}
}

void CRoomBase::Save(CFileStream& file) 
{

	file.WriteToFile("ROOM", sizeof(char) * 4);

	file.WriteToFile(&uRoomID, sizeof(unsigned long));

	uint numPolygons = ro_polygons.size();

	file.WriteToFile(&numPolygons, sizeof(uint));

	FOREACH_ARRAY(numPolygons) 
	{
		ro_polygons[i].Save(file);
	}
}

void CRoomBase::Clear()
{
	uRoomID = 0;
	for (unsigned int i = 0; i < ro_polygons.size(); i++) 
	{
		ro_polygons[i].Clear();
	}
	ro_polygons.clear();

	ro_world = NULL;
}

void CRoomBase::Load(CFileStream& file)
{
	uint numPolygons;

	CPolygonWorld polygons;

	if (!file.checkID("ROOM"))
	{
		PrintF("Error loading room\n");
		return;
	}

	file.ReadFromFile(&uRoomID, sizeof(unsigned long));	

	file.ReadFromFile(&numPolygons, sizeof(uint));

	FOREACH_ARRAY(numPolygons)
	{
		polygons.Load(file);
		ro_polygons.push_back(polygons);
		polygons.Clear();
	}
}