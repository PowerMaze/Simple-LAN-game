#include <stdAfx.h>
#include "FileIO.h"
#include "FileName.h"


CFileName::CFileName(const std::string newFileName)
{
	this->strFileName = newFileName;
}

CFileName::CFileName(const char* newFileName)
{
	this->strFileName = newFileName;
}

CFileName &CFileName::operator=(const CFileName newFilename)
{
	this->strFileName = newFilename.strFileName;
	return *this;
}

CFileName& CFileName::operator=(const std::string& newFilename)
{
	this->strFileName = newFilename;
	return *this;
}

CFileName& CFileName::operator=(const char* newFilename)
{
	this->strFileName = newFilename;
	return *this;
}

void CFileName::ChangeEXT(const char* newext)
{

	char* strNew = changeFileEXT(strFileName.c_str(), newext);
	strFileName = strNew;

	free(strNew);

	/*
	size_t posOfDot = strFileName.find_last_of(".");

	std::string newString = "";

	for (int i = 0; i < posOfDot; i++)
	{
		newString += strFileName[i];
	}
	*/

	
}

bool CFileName::CompareEXT(const char* ext) 
{
	int pos = strFileName.find_last_of('.');
	std::string EXT = strFileName.substr(pos);

	return strcmp(EXT.c_str(), ext) == 0;
	//if (strcmp(EXT.c_str(), ext) == 0)
	//	return true;
	//else
	//	return false;
}

std::string CFileName::GetFileNameWitoutDir()
{
	size_t filename = strFileName.find_last_of("\\");
	return strFileName.substr(filename + 1);
}