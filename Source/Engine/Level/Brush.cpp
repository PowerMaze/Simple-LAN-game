#include <stdAfx.h>

#include "Brush.h"

#include <Engine/Common/Console.h>
#include <Engine\Common\FileIO.h>
#include <Engine/Math/Functions.h>

CBrush::CBrush()
{
	m_uFlags = 0;
	m_uIndexRoom = 0;
	m_bPrepared = false;
	m_uPolygonsCount = 0;
}

CBrush::CBrush(CBrush& brushOther)
{
	Clear();

	this->m_uFlags = brushOther.m_uFlags;
	this->m_uIndexRoom = brushOther.m_uIndexRoom;
	this->m_bPrepared = brushOther.m_bPrepared;
	this->m_uPolygonsCount = brushOther.m_uPolygonsCount;
	unsigned int size = brushOther.m_brushPolygons.size();

	FOREACH_ARRAY(size) 
	{
		CPolygonWorld* poly = new CPolygonWorld;
		*poly = *brushOther.m_brushPolygons[i];

		this->m_brushPolygons.push_back(poly);
	}
}

CBrush::~CBrush(void)
{
	Clear();
}

void CBrush::Clear(void) 
{
	if (m_bPrepared) m_bPrepared = false;

	FOREACH_ARRAY(m_uPolygonsCount)
	{
		CPolygonWorld* poly = m_brushPolygons[i];
		delete poly;
	}
	m_brushPolygons.clear();
}

void CBrush::Save(CFileStream& file) 
{
	file.writeID("BRSH");

	file.WriteToFile(&m_uFlags, sizeof(unsigned int));

	file.WriteToFile(&m_uIndexRoom, sizeof(unsigned int));

	file.WriteToFile(&m_uPolygonsCount, sizeof(unsigned int));

	FOREACH_ARRAY(m_uPolygonsCount)
	{
		CPolygonWorld* poly = m_brushPolygons[i];
		if (poly != NULL)
			poly->Save(file);
	}
}

void CBrush::Load(CFileStream& file) 
{
	if (!file.checkID("BRSH")) {
		throw "Cannot get brush id!!!\n";
	}

	file.ReadFromFile(&m_uFlags, sizeof(unsigned int));

	file.ReadFromFile(&m_uIndexRoom, sizeof(unsigned int));

	//unsigned int uiNumPolygons;

	file.ReadFromFile(&m_uPolygonsCount, sizeof(unsigned int));

	FOREACH_ARRAY(m_uPolygonsCount)
	{
		CPolygonWorld* poly = new CPolygonWorld;
		poly->Load(file);
		m_brushPolygons.push_back(poly);
	}
}

void CBrush::Prepare(void)
{
	if ( m_bPrepared ) return;

	FOREACH_ARRAY(m_uPolygonsCount)
	{
		m_brushPolygons[i]->Prepare();
	}

	m_bPrepared = true;

}

void CBrush::Render(CShader& sha)
{
	//CShader sha;
	FOREACH_ARRAY(m_uPolygonsCount)
	{
		m_brushPolygons[i]->Render(sha);
	}
}

CBrush* createBrushCube(const float height, const float width, const float length, const glm::vec3& vPos, CTextureObject* defaultTexture)
{
	
	CBrush* brush = new CBrush;

	Matrix4 mat = Matrix4(1.f);

	std::vector<glm::vec2> textureUV =
	{
		glm::vec2(1.0f * length, 1.0f * width),
		glm::vec2(1.0f * length, 0.0f * width),
		glm::vec2(0.0f * length, 0.0f * width),
		glm::vec2(0.0f * length, 1.0f * width)
	};

	float fPyramid = 1.0f;

	fPyramid = Clamp(fPyramid, 0.1f, 1.0f);

	/*
	Vector2D cubeUV[] = 
	{
		VECTOR2(0.0f, 1.0f),
		VECTOR2(1.0f, 1.0f),
		VECTOR2(1.0f, 0.0f),
		VECTOR2(0.0f, 0.0f)
	};*/

	// polygon right
	CPolygonWorld* polygonR = new CPolygonWorld;
	// polygon left
	CPolygonWorld* polygonL = new CPolygonWorld;
	// polygon front
	CPolygonWorld* polygonF = new CPolygonWorld;
	// polygon back
	CPolygonWorld* polygonB = new CPolygonWorld;
	// polygon up
	CPolygonWorld* polygonU = new CPolygonWorld;
	// polygon down
	CPolygonWorld* polygonD = new CPolygonWorld;


	/******************** process UP *******************/

	CVertex vertex[4];

	CFileName strFile = "Data\\Textures\\City\\Floor01.te";

	FileSetGlobalPatch(strFile);
	

	vertex[0].positionInRoomOrBrush.x = -width / 2.f* fPyramid;
	vertex[0].positionInRoomOrBrush.y = height;
	vertex[0].positionInRoomOrBrush.z = length / 2.f* fPyramid;

	vertex[1].positionInRoomOrBrush.x = width / 2.f* fPyramid;
	vertex[1].positionInRoomOrBrush.y = height;
	vertex[1].positionInRoomOrBrush.z = length / 2.f* fPyramid;

	vertex[2].positionInRoomOrBrush.x = width / 2.f* fPyramid;
	vertex[2].positionInRoomOrBrush.y = height;
	vertex[2].positionInRoomOrBrush.z = -length / 2.f* fPyramid;

	vertex[3].positionInRoomOrBrush.x = -width / 2.f* fPyramid;
	vertex[3].positionInRoomOrBrush.y = height;
	vertex[3].positionInRoomOrBrush.z = -length / 2.f* fPyramid;

	for (unsigned i = 0; i < 4; i++) 
	{
		mat = Matrix4(1.f);
		mat = glm::translate(mat, vPos);	
		Vector4D v(vertex[i].positionInRoomOrBrush.x, vertex[i].positionInRoomOrBrush.y, vertex[i].positionInRoomOrBrush.z, 1.f);
		v = mat * v;
		vertex[i].positionInRoomOrBrush = Vector3D(v);
	}

	for (unsigned i = 0; i < 4; i++)
		vertex[i].positionInWorld = vertex[i].positionInRoomOrBrush;

	polygonU->pr_flags |= 0x0UL;
	polygonU->vNormal = VECTOR3(0.0f, 1.0f, 0.0f);
	polygonU->notRender = false;

	polygonU->textureUVs = textureUV;
	polygonU->tex01FileName = strFile;
	polygonU->texture01.Load(strFile);

	for (unsigned i = 0; i < 4; i++)
		polygonU->verticies.push_back(vertex[i]);

	polygonU->tag = 0;
	polygonU->processData();



	/******************** process DOWN *******************/

		// left-up corner
	vertex[3].positionInRoomOrBrush.x = -width/2.f;
	vertex[3].positionInRoomOrBrush.y = 0.0f;
	vertex[3].positionInRoomOrBrush.z = length/2.f;
	// right-up corner
	vertex[2].positionInRoomOrBrush.x = width/2.f;
	vertex[2].positionInRoomOrBrush.y = 0.0f;
	vertex[2].positionInRoomOrBrush.z = length/2.f;
	// right-down corner
	vertex[1].positionInRoomOrBrush.x = width/2.f;
	vertex[1].positionInRoomOrBrush.y = 0.0f;
	vertex[1].positionInRoomOrBrush.z = -length/2.f;
	// left-down corner
	vertex[0].positionInRoomOrBrush.x = -width/2.f;
	vertex[0].positionInRoomOrBrush.y = 0.0f;
	vertex[0].positionInRoomOrBrush.z = -length/2.f;

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
		vertex[i].positionInWorld = vertex[i].positionInRoomOrBrush;


	// set standard flags
	polygonD->pr_flags |= POLYGON_INVISIBLE | POLYGON_IS_PASSABLE;
	polygonD->vNormal = VECTOR3(0.0f, -1.0f, 0.0f);
	//	polygon.notRender = true;

		// set default texture UV
	polygonD->textureUVs = textureUV;
	polygonD->tex01FileName = strFile;
	// load texture data
	polygonD->texture01.Load(strFile);
	// push data vertexies
	for (unsigned i = 0; i < 4; i++)
		polygonD->verticies.push_back(vertex[i]);
	// set tag of polygon
	polygonD->tag = 1;
	// process data for rendering polygon
	polygonD->processData();

	textureUV.clear();

	textureUV =
	{
		glm::vec2(1.0f /* height*/, 1.0f /* width*/),
		glm::vec2(1.0f /* height*/, 0.0f /* width*/),
		glm::vec2(0.0f /* height*/, 0.0f /* width*/),
		glm::vec2(0.0f /* height*/, 1.0f /* width*/)
	};

	float angleRadians = glm::radians(-90.0f);

	glm::mat2 rotationMatrix = glm::mat2(
		glm::cos(angleRadians), -glm::sin(angleRadians),
		glm::sin(angleRadians), glm::cos(angleRadians)
	);

	// Обертання кожного вектора
	for (auto& uv : textureUV) {
		uv = rotationMatrix * uv;
	}

	strFile = "Data\\Textures\\City\\Column01.te";

	FileSetGlobalPatch(strFile);

	
	/******************** process FRONT *******************/

	vertex[3].positionInRoomOrBrush.x = width / 2.f* fPyramid;
	vertex[3].positionInRoomOrBrush.y = height;
	vertex[3].positionInRoomOrBrush.z = -length / 2.f* fPyramid;

	vertex[2].positionInRoomOrBrush.x = -width / 2.f* fPyramid;
	vertex[2].positionInRoomOrBrush.y = height;
	vertex[2].positionInRoomOrBrush.z = -length / 2.f* fPyramid;

	vertex[1].positionInRoomOrBrush.x = -width / 2.f;
	vertex[1].positionInRoomOrBrush.y = 0.0f;
	vertex[1].positionInRoomOrBrush.z = -length / 2.f;

	vertex[0].positionInRoomOrBrush.x = width / 2.f;
	vertex[0].positionInRoomOrBrush.y = 0.0f;
	vertex[0].positionInRoomOrBrush.z = -length / 2.f;

	for (unsigned i = 0; i < 4; i++)
	{
		mat = Matrix4(1.f);
		mat = glm::translate(mat, vPos);
		Vector4D v(vertex[i].positionInRoomOrBrush.x, vertex[i].positionInRoomOrBrush.y, vertex[i].positionInRoomOrBrush.z, 1.f);
		v = mat * v;
		vertex[i].positionInRoomOrBrush = Vector3D(v);
	}

	for (unsigned i = 0; i < 4; i++)
		vertex[i].positionInWorld = vertex[i].positionInRoomOrBrush;

	polygonF->pr_flags |= 0x0UL;
	polygonF->vNormal = VECTOR3(0.0f, 0.0f, -1.0f);
	polygonF->textureUVs = textureUV;
	polygonF->tex01FileName = strFile;
	polygonF->texture01.Load(strFile);

	for (unsigned i = 0; i < 4; i++)
		polygonF->verticies.push_back(vertex[i]);

	polygonF->tag = 4;
	polygonF->processData();

	/******************** process BACK *******************/

		// 0 index for EBO
	vertex[3].positionInRoomOrBrush.x = -width / 2.f* fPyramid;
	vertex[3].positionInRoomOrBrush.y = height;
	vertex[3].positionInRoomOrBrush.z = length / 2.f* fPyramid;
	// 1 index for EBO
	vertex[2].positionInRoomOrBrush.x = width / 2.f* fPyramid;
	vertex[2].positionInRoomOrBrush.y = height;
	vertex[2].positionInRoomOrBrush.z = length / 2.f* fPyramid;
	// 2 index for EBO
	vertex[1].positionInRoomOrBrush.x = width / 2.f;
	vertex[1].positionInRoomOrBrush.y = 0.0f;
	vertex[1].positionInRoomOrBrush.z = length / 2.f;
	// 3 index for EBO
	vertex[0].positionInRoomOrBrush.x = -width / 2.f;
	vertex[0].positionInRoomOrBrush.y = 0.0f;
	vertex[0].positionInRoomOrBrush.z = length / 2.f;

	for (unsigned i = 0; i < 4; i++)
	{
		mat = Matrix4(1.f);
		mat = glm::translate(mat, vPos);
		Vector4D v(vertex[i].positionInRoomOrBrush.x, vertex[i].positionInRoomOrBrush.y, vertex[i].positionInRoomOrBrush.z, 1.f);
		v = mat * v;
		vertex[i].positionInRoomOrBrush = Vector3D(v);
	}

	for (unsigned i = 0; i < 4; i++)
		vertex[i].positionInWorld = vertex[i].positionInRoomOrBrush;

	polygonB->pr_flags |= 0x0UL;

	polygonB->textureUVs = textureUV;
	polygonB->tex01FileName = strFile;
	polygonB->texture01.Load(strFile);

	for (unsigned i = 0; i < 4; i++)
		polygonB->verticies.push_back(vertex[i]);
	polygonB->vNormal = VECTOR3(0.0f, 0.0f, 1.0f);
	polygonB->tag = 5;
	polygonB->processData();

	/******************** process RIGHT *******************/

	textureUV.clear();

	textureUV =
	{
		glm::vec2(1.0f /* height*/, 1.0f /* length*/),
		glm::vec2(1.0f /* height*/, 0.0f /* length*/),
		glm::vec2(0.0f /* height*/, 0.0f /* length*/),
		glm::vec2(0.0f /* height*/, 1.0f /* length*/)
	};

	angleRadians = glm::radians(-90.0f);

	rotationMatrix = glm::mat2(
		glm::cos(angleRadians), -glm::sin(angleRadians),
		glm::sin(angleRadians), glm::cos(angleRadians)
	);

	// Обертання кожного вектора
	for (auto& uv : textureUV) {
		uv = rotationMatrix * uv;
	}

	vertex[3].positionInRoomOrBrush.x = -width / 2.f* fPyramid;
	vertex[3].positionInRoomOrBrush.y = height;
	vertex[3].positionInRoomOrBrush.z = -length / 2.f* fPyramid;

	vertex[2].positionInRoomOrBrush.x = -width / 2.f* fPyramid;
	vertex[2].positionInRoomOrBrush.y = height;
	vertex[2].positionInRoomOrBrush.z = length / 2.f* fPyramid;

	vertex[1].positionInRoomOrBrush.x = -width / 2.f;
	vertex[1].positionInRoomOrBrush.y = 0.0f;
	vertex[1].positionInRoomOrBrush.z = length / 2.f;

	vertex[0].positionInRoomOrBrush.x = -width / 2.f;
	vertex[0].positionInRoomOrBrush.y = 0.0f;
	vertex[0].positionInRoomOrBrush.z = -length / 2.f;

	for (unsigned i = 0; i < 4; i++)
	{
		mat = Matrix4(1.f);
		mat = glm::translate(mat, vPos);
		Vector4D v(vertex[i].positionInRoomOrBrush.x, vertex[i].positionInRoomOrBrush.y, vertex[i].positionInRoomOrBrush.z, 1.f);
		v = mat * v;
		vertex[i].positionInRoomOrBrush = Vector3D(v);
	}

	for (unsigned i = 0; i < 4; i++)
		vertex[i].positionInWorld = vertex[i].positionInRoomOrBrush;

	polygonR->pr_flags |= 0x0UL;
	polygonR->vNormal = VECTOR3(-1.0f, 0.0f, 0.0f);
	polygonR->textureUVs = textureUV;
	polygonR->tex01FileName = strFile;
	polygonR->texture01.Load(strFile);
	//polygonR->pr_flags |= POLYGON_INVISIBLE;

	for (unsigned i = 0; i < 4; i++)
		polygonR->verticies.push_back(vertex[i]);

	polygonR->tag = 2;
	polygonR->processData();

	/******************** process LEFT *******************/

	vertex[3].positionInRoomOrBrush.x = width / 2.f* fPyramid;
	vertex[3].positionInRoomOrBrush.y = height;
	vertex[3].positionInRoomOrBrush.z = length / 2.f* fPyramid;

	vertex[2].positionInRoomOrBrush.x = width / 2.f* fPyramid;
	vertex[2].positionInRoomOrBrush.y = height;
	vertex[2].positionInRoomOrBrush.z = -length / 2.f* fPyramid;

	vertex[1].positionInRoomOrBrush.x = width / 2.f;
	vertex[1].positionInRoomOrBrush.y = 0.0f;
	vertex[1].positionInRoomOrBrush.z = -length / 2.f;

	vertex[0].positionInRoomOrBrush.x = width / 2.f;
	vertex[0].positionInRoomOrBrush.y = 0.0f;
	vertex[0].positionInRoomOrBrush.z = length / 2.f;

	for (unsigned i = 0; i < 4; i++)
	{
		mat = Matrix4(1.f);
		mat = glm::translate(mat, vPos);
		Vector4D v(vertex[i].positionInRoomOrBrush.x, vertex[i].positionInRoomOrBrush.y, vertex[i].positionInRoomOrBrush.z, 1.f);
		v = mat * v;
		vertex[i].positionInRoomOrBrush = Vector3D(v);
	}

	for (unsigned i = 0; i < 4; i++)
		vertex[i].positionInWorld = vertex[i].positionInRoomOrBrush;

	polygonL->pr_flags |= 0;
	polygonL->vNormal = VECTOR3(1.0f, 0.0f, 0.0f);
	polygonL->textureUVs = textureUV;
	polygonL->tex01FileName = strFile;
	polygonL->texture01.Load(strFile);

	for (unsigned i = 0; i < 4; i++)
		polygonL->verticies.push_back(vertex[i]);

	polygonL->tag = 3;
	polygonL->processData();

	brush->m_uFlags = 0x00UL;
	brush->m_uIndexRoom = 0;

	brush->m_brushPolygons.push_back(polygonU);
	brush->m_brushPolygons.push_back(polygonD);

	brush->m_brushPolygons.push_back(polygonR);
	brush->m_brushPolygons.push_back(polygonL);

	brush->m_brushPolygons.push_back(polygonF);
	brush->m_brushPolygons.push_back(polygonB);

	brush->m_uPolygonsCount = brush->m_brushPolygons.size();

	return brush;
}