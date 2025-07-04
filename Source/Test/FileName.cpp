#include "FileName.h"
/*
CFileName::CFileName(void) 
{
	this->strFileName = "";
}*/

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

bool CFileName::operator==(const CFileName strToEque)
{
	return this->strFileName == strToEque.strFileName;
}

void CFileName::ChangeEXT(const char* newext)
{
	//std::string New = changeFileEXT(strFileName.c_str(), newext);
	//strFileName = New;

	/*
	size_t posOfDot = strFileName.find_last_of(".");

	std::string newString = "";

	for (int i = 0; i < posOfDot; i++)
	{
		newString += strFileName[i];
	}
	*/		
}

char* CFileName::GetFileNameWitoutDir()
{
	size_t filename = strFileName.find_last_of("\\");
	return (char*)strFileName.substr(filename).c_str();
}