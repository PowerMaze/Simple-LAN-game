#include <stdAfx.h>

#include <Engine/Common/PrintMessages.h>
#include <Engine/Common/FontData.h>
#include "Draw.h"
#include "View.h"
#include "Color.h"
#include "Shader.h"
#include "Texture.h"
//#include "View.h"

void simpleBoxInit(simpleBox2D_t* box2d)
{
	box2d->textureID = 0;

	float vertices[] = { 
		-1.0f,  1.0, 0.0f,
		 1.0f,  1.0, 0.0f,
		 1.0f, -1.0, 0.0f,
		-1.0f, -1.0, 0.0f 
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3  
	};

	glGenVertexArrays(1, &box2d->VAO);
	glGenBuffers(1, &box2d->VBO);
	glGenBuffers(1, &box2d->EBO);

	glBindVertexArray(box2d->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, box2d->VBO);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, box2d->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindVertexArray(0);
}

simpleBox2D_t* simpleBoxInitTextured(void) 
{
	simpleBox2D_t* b = nullptr;

	b = (simpleBox2D_t*)malloc(sizeof(simpleBox2D_t));

	if (b == nullptr)	return nullptr;

	b->textureID = teDefaultTextureForModels->texID;


	float vertices[] = {
	    -1.0f,  1.0, 0.0f, 1.0f,
	     1.0f,  1.0, 1.0f, 1.0f,
	     1.0f, -1.0, 1.0f, 0.0f,
	    -1.0f, -1.0, 0.0f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &b->VAO);
	glGenBuffers(1, &b->VBO);
	glGenBuffers(1, &b->EBO);

	glBindVertexArray(b->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, b->VBO);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindVertexArray(0);

	return b;
}

void simpleBoxEnd(simpleBox2D_t* box2d)
{
	if (box2d == NULL) return;

	glDeleteVertexArrays(1, &box2d->VAO);
	glDeleteBuffers(1, &box2d->VBO);
	glDeleteBuffers(1, &box2d->EBO);
}

void drawCenteredText(CFont* font, CView* pview, uint uiY, float fScale, const HEXColor color, bool bAllowResolutionScreen, const char* strText)
{
	if (pview == NULL || font == NULL || strText == NULL) return;

	float fResolutionScale = 1.0f;

	if (bAllowResolutionScreen)
	{

	}
	
	int len = strlen(strText) * font->GetSpace();

	unsigned int uiScreenX = pview->GetWidth() / 2;

	uiScreenX -= len / 2;

	drawText(font, uiScreenX, uiY, fScale, color, strText);
}

void drawTextureToScreen(CTextureObject* te, CView* view, uint x, uint y, uint width, uint height, byte renderType, HEXColor color/* = COLOR_WHITE*/) 
{
	if (te == nullptr) return;
	if (view == nullptr) return;

	glm::vec4 fcolor = HEX_toOGL(color);

	glm::mat4 modelMatrix = glm::mat4(1.0f);

	modelMatrix = glm::translate(modelMatrix, glm::vec3(float(x), float(y), 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(float(width), float(height), 1.0f));

	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	if (renderType == TEX_TRANSPARENT)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	drawingTexturedShapeBox->textureID = te->texID;

	shaTexture.use();

	shaTexture.shaderSet("texture1", 0);

	shaTexture.shaderSetMatrix("projection", view->m_OrtoMatrix);
	shaTexture.shaderSetMatrix("model", modelMatrix);
	shaTexture.shaderSetVector("textureColor", fcolor);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(drawingTexturedShapeBox->VAO);
	glBindTexture(GL_TEXTURE_2D, drawingTexturedShapeBox->textureID);


	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDisable(GL_BLEND);	

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glBindTexture(GL_TEXTURE_2D, 0);

}


void drawSquare(uint x, uint y, uint width, uint height, HEXColor color) 
{
	glm::vec4 fcolor = HEX_toOGL(color);

	glm::mat4 modelMatrix = glm::mat4(1.0f);

	modelMatrix = glm::translate(modelMatrix, glm::vec3(float(x), float(y), 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(float(width), float(height), 1.0f));

	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shaFillColor.use();

	shaFillColor.shaderSetMatrix("projection", pViewPort->m_OrtoMatrix);
	shaFillColor.shaderSetMatrix("model", modelMatrix);
	shaFillColor.shaderSetVector("Color", fcolor);

	glBindVertexArray(drawingShapeBox->VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);	

	glDisable(GL_BLEND);

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
}

void drawColorFull(HEXColor color) 
{
	glm::vec4 col = HEX_toOGL(color);

	glClearColor(col.x, col.y, col.z, col.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}