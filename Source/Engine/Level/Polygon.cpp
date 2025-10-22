#include <stdAfx.h>
#include "Room.h"

#include "BaseWorld.h"

#include <Engine/Common/Console.h>
#include <Engine/Common/FileName.h>
//#include <Engine/Graphics/View.h>

CPolygonWorld::CPolygonWorld()
{
	pr_flags = 0;
	tag = 0;
	renderPrepared = false;
	notRender = false;
	vNormal = VECTOR3_NONE;
	pr_brush = NULL;
	pr_room = NULL;
	pr_color = COLOR_WHITE;
//	indices = nullptr;

	VAO = 0; 
	VBO = 0; 
	EBO = 0;
}

void CPolygonWorld::Clear() 
{
	verticies.clear(); // verticies of polygon	
	textureUVs.clear(); // texture uv for polygon
//	texture01.Clear();
	//shader.end();
	RenderData.clear();
	indices.clear();
	notRender = false;
	pr_flags = POLYGON_NONE;
	pr_room = NULL;
	pr_brush = NULL;
	pr_color = COLOR_WHITE;
	if (renderPrepared) 
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		renderPrepared = false;
	}
//	if (indices)
//		delete[] indices;
}

CGameWorld* CPolygonWorld::getWorld() 
{
	if (pr_room) return pr_room->ro_world;
	if (pr_brush) return pr_brush->m_world;
	else return NULL;
}

void CPolygonWorld::processData() 
{
	unsigned int iSize = verticies.size();// +textureUVs.size();


	for (unsigned int i = 0; i < iSize; i++)
	{
		RenderData.push_back(verticies[i].positionInRoomOrBrush.x);
		RenderData.push_back(verticies[i].positionInRoomOrBrush.y);
		RenderData.push_back(verticies[i].positionInRoomOrBrush.z);

		RenderData.push_back(textureUVs[i].x);
		RenderData.push_back(textureUVs[i].y);

	//	if (pr_flags & POLYGON_FULL_BRIGHT) continue;

		RenderData.push_back(vNormal.x);
		RenderData.push_back(vNormal.y);
		RenderData.push_back(vNormal.z);
	}

	// dummy indices
	unsigned int inds[6] =
	{
		0, 1, 3,
		1, 2, 3 
	};
	
	FOREACH_ARRAY(6) 
	{
		indices.push_back(inds[i]);
	}

}

void CPolygonWorld::Save(CFileStream& file) 
{
	file.WriteToFile(&tag, sizeof(unsigned long));
	file.WriteToFile(&pr_flags, sizeof(unsigned long));
	//file.WriteToFile(&notRender, sizeof(unsigned int));

	FileDeleteAbsolutePatch(tex01FileName);

	unsigned int lengthString = tex01FileName.strFileName.length();

	file.WriteToFile(&lengthString, sizeof(unsigned int));
	file.WriteToFile(tex01FileName.getCharPointer(), lengthString * sizeof(const char));

	unsigned int numData = verticies.size();

	file.WriteToFile(&numData, sizeof(unsigned int));

	FOREACH_ARRAY(numData)
	{
		verticies[i].WriteData(file);
	}

	numData = textureUVs.size();

	file.WriteToFile(&numData, sizeof(unsigned int));

	FOREACH_ARRAY(numData)
	{
		file.WriteToFile(&textureUVs[i].x, sizeof(float));
		file.WriteToFile(&textureUVs[i].y, sizeof(float));
	}

	file.WriteToFile(&vNormal.x, sizeof(float));
	file.WriteToFile(&vNormal.y, sizeof(float));
	file.WriteToFile(&vNormal.z, sizeof(float));
}

void CPolygonWorld::Load(CFileStream& file)
{
	Clear();

	unsigned int numData;
	char* textureFilePath = NULL;

	CVertex vtx;

	file.ReadFromFile(&tag, sizeof(unsigned long));
	file.ReadFromFile(&pr_flags, sizeof(unsigned long));

	file.ReadFromFile(&numData, sizeof(unsigned int));

	textureFilePath = (char*)malloc(numData + 1 * sizeof(char));


	file.ReadFromFile(textureFilePath, numData * sizeof(char));
	textureFilePath[numData] = '\0';

	tex01FileName = textureFilePath;

	FileSetGlobalPatch(tex01FileName);

	texture01.Load(tex01FileName);

	file.ReadFromFile(&numData, sizeof(unsigned int));

	FOREACH_ARRAY(numData)
	{
		vtx.ReadData(file);
		verticies.push_back(vtx);
	}

	file.ReadFromFile(&numData, sizeof(unsigned int));

	FOREACH_ARRAY(numData)
	{
		glm::vec2 UV;
		file.ReadFromFile(&UV.x, sizeof(float));
		file.ReadFromFile(&UV.y, sizeof(float));
		textureUVs.push_back(UV);
	}

	file.ReadFromFile(&vNormal.x, sizeof(float));
	file.ReadFromFile(&vNormal.y, sizeof(float));
	file.ReadFromFile(&vNormal.z, sizeof(float));

	processData();

	free(textureFilePath); // <------ don't forget release pointer
}

void DumpPulygonData(CPolygonWorld* polygon) 
{
	int index = 0;
	PrintF("---------------------------------------------------------------\n");
	PrintF("DUMP POLYGON DATA\n");
	for (unsigned int i = 0; i < polygon->verticies.size(); i++)
	{
		PrintF("Polygon: %d verticies: %f %f %f, is rendered: %d\n", index, polygon->verticies[i].positionInRoomOrBrush.x, polygon->verticies[i].positionInRoomOrBrush.y, polygon->verticies[i].positionInRoomOrBrush.z, polygon->notRender);
		index++;
	}
	PrintF("---------------------------------------------------------------\n");
}


void CPolygonWorld::Prepare() 
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// Çàãðóæàåì äàííûå â âåðøèííûé áóôåð
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, RenderData.size() * sizeof(float), &RenderData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


	// Êîîðäèíàòû âåðøèí
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(0*sizeof(float)));

	// Òåêñòóðíûå êîîðäèíàòû âåðøèí
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));

	// Íîðìàëè âåðøèí
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(5 * sizeof(float)));

	/*
		// Êàñàòåëüíûé âåêòîð âåðøèíû
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

		// Âåêòîð áèíîðìàëè âåðøèíû
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));*/

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	if (pr_flags & POLYGON_INVISIBLE) 
	{
		notRender = true;
	}

#ifndef NDEBUG
	//DumpPulygonData(this);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		PrintF("OpenGL Error: %d\n", error);
	}
#endif


	texture01.Prepare();

//	PrintF("Inds: %d\n", indices.size());

	renderPrepared = true;
}

CPolygonWorld& CPolygonWorld::operator=(CPolygonWorld& polygon)
{
	this->pr_flags = polygon.pr_flags;
	this->tex01FileName = polygon.tex01FileName;
//	this->shader = polygon.shader;
	this->textureUVs = polygon.textureUVs;
	this->texture01 = polygon.texture01;
	this->verticies = polygon.verticies;
	this->notRender = polygon.notRender;
	this->vNormal = polygon.vNormal;
	this->pr_room = polygon.pr_room;
	this->pr_brush = polygon.pr_brush;
	return *this;
}

void CPolygonWorld::Render(CShader &sha) 
{
	if (notRender)
		return;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture01.texID);

	Sun_t* sun = getWorld()->gw_Sun;

	Vector3D vDir, vAmb, vDiff, vSpec;

	if (sun == NULL) 
	{
//		printf("NULL\n");
		vDir = vAmb = vDiff = vSpec = VECTOR3_NONE;
	}
	else {
		vDir = sun->su_aDirection;
		vAmb  = VECTOR3(sun->su_ColorAmbient.x,  sun->su_ColorAmbient.y,  sun->su_ColorAmbient.z);
		vDiff = VECTOR3(sun->su_ColorDiffuse.x,  sun->su_ColorDiffuse.y,  sun->su_ColorDiffuse.z);
		vSpec = VECTOR3(sun->su_ColorSpecular.x, sun->su_ColorSpecular.y, sun->su_ColorSpecular.z);
	}

	sha.use();

	sha.shaderSetVector("viewPos", pViewPort->m_ViewDir);
	sha.shaderSet("material.shininess", 128.0f);

	sha.shaderSetVector("material.ambient", 1.00f, 1.00f, 1.00f);
	sha.shaderSetVector("material.diffuse", 1.0f, 1.0f, 1.0f);
	sha.shaderSetVector("material.specular", 0.2f, 0.2f, 0.2f);

	sha.shaderSetVector("dirLight.direction", vDir);
	sha.shaderSetVector("dirLight.ambient",   vAmb);
	sha.shaderSetVector("dirLight.diffuse",   vDiff);
	sha.shaderSetVector("dirLight.specular",  vSpec);


	sha.shaderSetMatrix("projection", pViewPort->getProjection());
	sha.shaderSetMatrix("view", pViewPort->m_viewMatrix);

	glm::mat4 model = glm::mat4(1.0f);

	sha.shaderSetMatrix("model", model);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//	glDrawArrays(GL_TRIANGLES, 0, 6/*indices.size() */ );
	glBindVertexArray(0);

	// Ñ÷èòàåòñÿ õîðîøåé ïðàêòèêîé âîçâðàùàòü çíà÷åíèÿ ïåðåìåííûõ ê èõ ïåðâîíà÷àëüíûì çíà÷åíèÿì
	//glActiveTexture(GL_TEXTURE0);
}
