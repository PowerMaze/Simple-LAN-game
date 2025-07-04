#include <stdAfx.h>
#include "Shader.h"
#include <Engine/Common/PrintMessages.h>
#include <Engine/Common/Console.h>
#include <Engine/Common/FileIO.h>
#include <Engine/Common/FileName.h>

CShader::CShader(const char* strPathVert, const char* strPathFrag)
{
	Create_t(strPathVert, strPathFrag);
}
/*
CShader::CShader(const CShaderRequest& s)
{
//	Create_t(s.strFileVertShader, s.strFileFragShader);
}
*/
CShader::~CShader()
{
	if ( shID )
		end();
}

void CShader::CreateFromString(const char* VertShader, const char* FragShader)
{

	unsigned int vertex, fragment;

	/* setup vertex shader */
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &VertShader, NULL);
	glCompileShader(vertex);
	checkErrors(vertex, (string_t)"VERTEX");

	/* setup fragment shader */
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &FragShader, NULL);
	glCompileShader(fragment);
	/* check errors for succesful compile */
	checkErrors(fragment, (string_t)"FRAGMENT");

	/* create shader program and attach previous shaders (vertex and fragment) */
	shID = glCreateProgram();
	glAttachShader(shID, vertex);
	glAttachShader(shID, fragment);
	glLinkProgram(shID);
	/* check errors for succesful compile */
	checkErrors(shID, (string_t)"PROGRAM");

	/* delete attached shaders */
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void CShader::Create_t(const char* strPathVert, const char* strPathFrag)
{
	//FILE* shaOpenF = NULL;
	//FILE* shaOpenV = NULL;

	CFileName fnmPV = strPathVert;
	CFileName fnmPF = strPathFrag;

	unsigned int bytesF, bytesV; /* needed for place nul terminal string '\0' */

	char* bufferFileV = NULL;
	char* bufferFileF = NULL;

	//FileSetGlobalPatch(fnmPV);
	//FileSetGlobalPatch(fnmPF);

//	PrintF("%s\n", (const char*)fnmPV);
//	PrintF("%s\n", (const char*)fnmPF);

	CFileStream fileF;
	CFileStream fileV;


	if (!fileV.open(fnmPV, FILE_BIN, FILE_READ)) 
	{
		PrintF("Cannot open vertex shader %s\nNo such file or directory!\n", fnmPV.GetFileNameWitoutDir().c_str());
		Warning("Cannot open vertex shader %s\nNo such file or directory!\n", fnmPV.GetFileNameWitoutDir().c_str());
		return;
	}

	if (!fileF.open(fnmPF, FILE_BIN, FILE_READ))
	{
		PrintF("Cannot open fragment shader %s\nNo such file or directory!\n", fnmPF.GetFileNameWitoutDir().c_str());
		Warning("Cannot open fragment shader %s\nNo such file or directory!\n", fnmPF.GetFileNameWitoutDir().c_str());
		fileV.closeFile();
		return;
	}

	bufferFileV = (char*)fileV.ReadFullFileData(&bytesV);

	if (bufferFileV == NULL)
	{
		PrintF("Unable to read vertex shader\n");
		Warning("Unable to read vertex shader\n");
		fileV.closeFile();
		fileF.closeFile();
		return;
	}

	bufferFileF = (char*)fileF.ReadFullFileData(&bytesF);

	if (bufferFileF == NULL)
	{
		PrintF("Unable to read vertex shader\n");
		Warning("Unable to read vertex shader\n");
		free(bufferFileV);
		fileV.closeFile();
		fileF.closeFile();
		return;
	}

	/* set nul terminal */
	bufferFileF[bytesF] = '\0';
	bufferFileV[bytesV] = '\0';

	CreateFromString(bufferFileV, bufferFileF);

	/* free memory from buffers */
	free(bufferFileV);
	free(bufferFileF);

	fileV.closeFile();
	fileF.closeFile();

	////unsigned int vertex, fragment;
	//unsigned int bytesF, bytesV; /* needed for place nul terminal string '\0' */

	///* open vertex shader file */
	//shaOpenV = fopen(strPathVert, "rb");
	///* check errors */
	//if (shaOpenV == NULL)
	//{
	//	PrintF("Cannot open vertex shader.\nNo such file or directory!\n");
	//	Warning("Cannot open vertex shader.\nNo such file or directory!\n");
	//	return;
	//}
	///* open fragment shader file */
	//shaOpenF = fopen(strPathFrag, "rb");
	///* check errors */
	//if (shaOpenF == NULL)
	//{
	//	PrintF("Cannot open fragment shader.\nNo such file or directory!\n");
	//	Warning("Cannot open fragment shader.\nNo such file or directory!\n");
	//	fclose(shaOpenV);
	//	return;
	//}

	///* read text data to buffer from file vertex shader */
	//string_t bufferFileV = (char*)FileReadFull(shaOpenV, &bytesV);
	//if (bufferFileV == NULL)
	//{
	//	PrintF("Unable to read vertex shader\n");
	//	Warning("Unable to read vertex shader\n");
	//	fclose(shaOpenV);
	//	fclose(shaOpenF);
	//	return;
	//}
	///* read text data to buffer from file fragment shader */
	//string_t bufferFileF = (char*)FileReadFull(shaOpenF, &bytesF);
	//if (bufferFileF == NULL)
	//{
	//	PrintF("Unable to read fragment shader\n");
	//	Warning("Unable to read fragment shader\n");
	//	free(bufferFileV);
	//	fclose(shaOpenV);
	//	fclose(shaOpenF);
	//	return;
	//}

	///* set nul terminal */
	//bufferFileF[bytesF] = '\0';
	//bufferFileV[bytesV] = '\0';

	//CreateFromString(bufferFileV, bufferFileF);	

	///* free memory from buffers */
	//free(bufferFileV);
	//free(bufferFileF);

	//fclose(shaOpenV);
	//fclose(shaOpenF);
}


void CShader::use() 
{
	glUseProgram(shID);
}

void CShader::end()
{
	glDeleteProgram(shID);
	shID = 0;
}

void CShader::checkErrors(const unsigned int prog, const char * strError)
{
	int success;
	char infoLog[1024];
	if (strcmp(strError, "PROGRAM") != 0)
	{
		glGetShaderiv(prog, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(prog, 1024, NULL, infoLog);
			PrintF("IN THE ENGINE ERROR::SHADER_COMPILATION_ERROR of type : %s\n%s\n----------------------------\n", strError, infoLog);
		}
	}
	else
	{
		glGetProgramiv(prog, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(prog, 1024, NULL, infoLog);
			PrintF("IN THE ENGINE ERROR::SHADER_COMPILATION_ERROR of type : %s\n%s\n----------------------------\n", strError, infoLog);
		}
	}
}


/*
//////////////////////////////////////////////
		set data types to shader program
//////////////////////////////////////////////
*/

// set int value to shader program
void CShader::shaderSet(const char* strName, const int iValue) 
{
	glUniform1i(glGetUniformLocation(shID, strName), iValue);
}
// set float value to shader program
void CShader::shaderSet(const char* strName, const float fValue)
{
	glUniform1f(glGetUniformLocation(shID, strName), fValue);
}
// set double value to shader program
void CShader::shaderSet(const char* strName, const double dValue)
{
	glUniform1d(glGetUniformLocation(shID, strName), dValue);
}

/*
//////////////////////////////////////////////
		set vectors to shader program
//////////////////////////////////////////////
*/

void CShader::shaderSetVector(const char* strName, const float fx, const float fy)
{
	glUniform2f(glGetUniformLocation(shID, strName), fx, fy);
}
void CShader::shaderSetVector(const char* strName, const float fx, const float fy, const float fz)
{
	glUniform3f(glGetUniformLocation(shID, strName), fx, fy, fz);
}
void CShader::shaderSetVector(const char* strName, const float fx, const float fy, const float fz, const float fw)
{
	glUniform4f(glGetUniformLocation(shID, strName), fx, fy, fz, fw);
}


void CShader::shaderSetVector(const char* strName, const glm::vec2& vector2)
{
	glUniform2fv(glGetUniformLocation(shID, strName), 1, &vector2[0]);
}
void CShader::shaderSetVector(const char* strName, const glm::vec3& vector3)
{
	glUniform3fv(glGetUniformLocation(shID, strName), 1, &vector3[0]);
}
void CShader::shaderSetVector(const char* strName, const glm::vec4& vector4)
{
	glUniform4fv(glGetUniformLocation(shID, strName), 1, &vector4[0]);
}

/*
//////////////////////////////////////////////
		set matrix to shader program
//////////////////////////////////////////////
*/

void CShader::shaderSetMatrix(const char* strName, const glm::mat3& matrix3)
{
	glUniformMatrix3fv(glGetUniformLocation(shID, strName), 1, GL_FALSE, glm::value_ptr(matrix3));
}
void CShader::shaderSetMatrix(const char* strName, const glm::mat4& matrix4) 
{
	glUniformMatrix4fv(glGetUniformLocation(shID, strName), 1, GL_FALSE, glm::value_ptr(matrix4));
}