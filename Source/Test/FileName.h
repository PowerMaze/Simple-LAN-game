#pragma once

#include <string>

/*
//////////////////////////////////////////////////
   Basic class for 
//////////////////////////////////////////////////
*/
class CFileName 
{
public:
	std::string strFileName;

	CFileName(void) = default;
	CFileName(const std::string newFileName);
	CFileName(const char* newFileName);


	void ChangeEXT(const char* newExt);
	char* GetFileNameWitoutDir(void);

	CFileName &operator=(const CFileName newFilename);
	CFileName &operator=(const std::string& newFilename);
	CFileName &operator=(const char* newFilename);

	bool operator==(const CFileName);
};