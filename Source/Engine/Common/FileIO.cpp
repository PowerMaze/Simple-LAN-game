#include <stdAfx.h>

#include "Console.h"
#include "PrintMessages.h"

#include "FileIO.h"
#include "CRC.h"
#include "FileName.h"

char* stringCopy(const char* strSource)
{
	if (strSource == NULL) return NULL;
	unsigned long Length = strlen(strSource);
	char* newString = (char*)malloc((Length + 1) * sizeof(char));
	if (newString == NULL)
		Error("Cannot allocate memory!\n%d bytes needed", Length);
	memcpy(newString, strSource, Length + 1);
	newString[Length] = 0;
	return newString;
}

char* stringCopyF(const char* strFormat, ...) 
{
	va_list args;
	va_start(args, strFormat);

	char format[2048];

	vsprintf(format, strFormat, args);

	va_end(args);

	return stringCopy(format);
}


void stdStringF(std::string& str, const char* strFormat, ...) 
{
	va_list args;
	va_start(args, strFormat);

	char format[2048];

	vsprintf(format, strFormat, args);

	va_end(args);

	str = format;
}

void* FileReadFull(FILE* file, unsigned int *bytes)
{
	// set length of file data
	fseek(file, 0, SEEK_END);
	*bytes = ftell(file);
	rewind(file);

	// allocate buffer to write file
	void* buffer = malloc(*bytes + 1);
	if (buffer == NULL) {
		PrintF("Unable to create buffer for read data!\n");
		fclose(file);
		return NULL;
	}

	// read data
	fread(buffer, 1, *bytes, file);
	//fclose(file);
	return buffer;
}

char* changeFileEXT(const char* path, const char* pref)
{
	if (path == NULL || pref == NULL) return NULL;

	// length of strings
	signed length = strlen(path);
	signed lengthPref = strlen(pref);
	signed lengthDot = 0;
	signed placeRemove = 0;

	// find length of format to delete original format
	for (int i = length - 1; i > 0; i--)
	{
		if (path[i] == '.') {
			placeRemove = i;
			break;
		}
		lengthDot++;
	}

	// set new length
	int newLength = (length - 1) - lengthDot + lengthPref;

	// allocate new string and set 0
	char* newPath = (char*)malloc(newLength + 1);
	if (newPath == NULL) return NULL;


	// copy original path without file extension
	strncpy(newPath, path, placeRemove * sizeof(char));

	memcpy(newPath, path, placeRemove);
	memcpy(newPath + placeRemove, pref, lengthPref);

	// add to new path, new file extension
	//strcat(newPath, pref);

	newPath[newLength] = '\0';

	return newPath;
}

void FileCopy(const CFileName& strFileSrc, const CFileName& strFileDst) 
{
	CFileStream file01, file02;
	unsigned int uBytes;

	if (!file01.open(strFileSrc, 0, 1)) 
	{
		Warning("Cannot open origrnal file\n");
		return;
	}

	void* data = file01.ReadFullFileData(&uBytes);

	file01.closeFile();

	file02.open(strFileDst, 0, 0);

	file02.WriteToFile(data, uBytes);

	file02.closeFile();

}

void FileDeleteAbsolutePatch(CFileName& strFile) 
{
	size_t find = strFile.strFileName.find(strGlobalPatch);

	if (find != std::string::npos) 
	{
		strFile.strFileName.erase(find, strGlobalPatch.length());
	}

	
	
}

void FileSetGlobalPatch(CFileName& strFile)
{
	// needed if we put absolute patch here
	//FileDeleteAbsolutePatch(strFile);

	size_t find = strFile.strFileName.find(strGlobalPatch);

	if (find != std::string::npos)
	{
		return;
	}

	std::string path = strGlobalPatch + strFile.strFileName;
	strFile = path;
}


/*
void addFileEXT(char** filename, const char* path)
{
	size_t extSize		= strlen(path);
	size_t filenameSize = strlen(*filename);

	int newSize = extSize + filenameSize + 1;

	*filename = realloc(*filename, newSize);
	strcat(*filename, path);
}*/


/*
/////////////////////////////////////////
CFileStream methods
/////////////////////////////////////////
*/


CFileStream::CFileStream(void)
{
	file = nullptr;
	isReading = false;
	//bBinaryFileMode = false;
}
CFileStream::~CFileStream(void)
{
	if (file)  { 
		fclose(file);
	}
	file = nullptr;
}



bool CFileStream::checkID(const char* strID)
{
	if (file == nullptr)
	{
		PrintF("Cannot check ID file, because file wasn't opened\n");
		return false;
	}

	if (strlen(strID) > 4)
	{
		PrintF("Cannot read ID from file, id: %s\n", strID);
		return false;
	}

	if (!isReading)
	{
		PrintF("Cannot check ID file, because file was in write mode!\n");
		return false;
	}

	char path[5];
	fread(path, sizeof(char) * 4, 1, file);
	path[4] = '\0';

	if (strcmp(path, strID) != 0)
	{
		PrintF("Invalid file ID! Found: %s, expected: %s\n", path, strID);
		return false;
	}

	return true;
}

unsigned int CFileStream::getCurrentPosition() 
{
	if (file) return ftell(file);
	return 0;
}

bool CFileStream::checkID_Safe(const char* strID) 
{
	bool bSafe = false;

	if (file == nullptr)
	{
		PrintF("Cannot check safe ID file, because file wasn't opened\n");
		return bSafe;
	}

	if (strlen(strID) > 4)
	{
		PrintF("Cannot read ID from file, id: %s\n", strID);
		return bSafe;
	}

	if (!isReading)
	{
		PrintF("Cannot check safe ID file, because file was in write mode!\n");
		return bSafe;
	}

	unsigned long upos = ftell(file);

	char path[5];
	fread(path, sizeof(char) * 4, 1, file);
	path[4] = '\0';

	if (strcmp(path, strID) == 0)
	{
		return true;
	}
	
	fseek(file, upos, SEEK_SET);

	return bSafe;
}

bool CFileStream::open(const CFileName& strFileName, unsigned int uMode, unsigned int uType)
{
	std::string mode;
	std::string path;

#ifdef NDEBUG		
	//path = "Data\\";
	path = "C:\\Games\\In-the-Power\\Data\\";
#else
	//path = "C:\\Games\\In-the-Power\\Data\\";
	//path = strGlobalPatch + strFileName.strFileName;
#endif	

	path = strGlobalPatch + strFileName.strFileName;
	//printf("%s\n", path.c_str());

	if (uType == 0) {
		mode = "w";
		isReading = false;
	}
	else {
		mode = "r";
		isReading = true;
	}

	if (uMode == 0)
	{
		mode += "b";
	}

	file = fopen(toCString(path), mode.c_str());

	if (file == NULL)
		return false;
	else
		return true;
}

void CFileStream::WriteToFile(const void* data, size_t size)
{
	if (isReading)
	{
		PrintF("Cannot write data to file, because file was opened in reading mode!\n");
		return;
	}
	fwrite(data, size, 1, file);
}

void CFileStream::WriteString(const char* str)
{
	if (isReading) {
		PrintF("Cannot write string to file, because file was opened in reading mode!\n");
		return;
	}

	unsigned int iLength = strlen(str);

	fwrite(&iLength, sizeof(unsigned int), 1, file);
	fwrite(str, sizeof(char) * iLength, 1, file);
}

void CFileStream::WriteString(const std::string& str) 
{
	//WriteString(str.c_str());

	if (isReading) {
		PrintF("Cannot write string to file, because file was opened in reading mode!\n");
		return;
	}

	unsigned int iLength = str.size();

	fwrite(&iLength, sizeof(unsigned int), 1, file);
	fwrite(str.c_str(), sizeof(char) * iLength, 1, file);

}

void CFileStream::ReadFromFile(void* data, size_t size)
{
	if (!isReading)
	{
		PrintF("Cannot read data from file, because file was opened in writing mode!\n");
		return;
	}
	fread(data, size, 1, file);
}

void CFileStream::ReadStringFromFile(std::string& string) 
{
	string.clear();
	unsigned int iLength;

	fread(&iLength, sizeof(unsigned int), 1, file);
	char* str = (char*)malloc(iLength + 1);

	if (str == NULL) Throw("Cannot load string from file");

	fread(str, sizeof(char) * iLength, 1, file);	
	str[iLength] = '\0';
	string.append(str);

	delete[] str;
	str = NULL;	
}
char* CFileStream::ReadStringFromFile() 
{
	unsigned int iLength;

	fread(&iLength, sizeof(unsigned int), 1, file);
	char* str = new char[iLength + 1];
	fread(str, sizeof(char) * iLength, 1, file);
	str[iLength] = '\0';

	return str;
}

bool CFileStream::checkVersion(unsigned int version)
{
	if (!isReading)
	{
		PrintF("Cannot check safe ID file, because file was in write mode!\n");
		return false;
	}

	unsigned int _v;
	fread(&_v, sizeof(unsigned int), 1, file);
	if (_v != version)
	{
		PrintF("Invalid version file, version must be %d, but we have %d\n", _v, version);
		return false;
	}

	return true;
}

void CFileStream::writeVersion(unsigned int version)
{
	if (isReading)
	{
		PrintF("Cannot write version to file, because file opened in reading mode!\n");
		return;
	}
	unsigned int ver = version;
	WriteToFile(&ver, sizeof(file));
}

void CFileStream::writeID(const char* strID) 
{
	if (isReading)
	{
		PrintF("Cannot write id to file, because file opened in reading mode!\n");
		return;
	}

	if (strlen(strID) != 4) 
	{
		PrintF("Cannot write id to file, because ID of file must be 4 bytes size!\n");
		return;
	}

	fwrite(strID, sizeof(char) * 4, 1, file);

}

void CFileStream::setToStartFile(void)
{
	rewind(file);

}

void *CFileStream::ReadFullFileData(unsigned int* bytes)
{
	return FileReadFull(file, bytes);
}

void CFileStream::SeekStart()
{
	setToStartFile();
}

unsigned long CFileStream::getLengthFile(void) 
{
	if (file == nullptr) {
		PrintF("Cannot get legth of file, because its was'nt opened\n");
		return 0;
	}

	unsigned long ipos = ftell(file);
	setToStartFile();
	fseek(file, 0, SEEK_END);
	unsigned long length = ftell(file);
	fseek(file, ipos, SEEK_CUR);
	return length;
}

unsigned int CFileStream::calculateFileCRC(void)
{
	if (file == NULL)
	{
		PrintF("Cannot calculate file CRC\n");
		return 0;
	}

	unsigned int crc;
	unsigned int bytes;

	unsigned int ipos = ftell(file);

	byte* data = (byte*)FileReadFull(file, &bytes);

	CRC32::start(&crc);

	CRC32::addBlock(&crc, data, bytes);

	CRC32::end(&crc);

	free(data);

	fseek(file, ipos, SEEK_CUR);

	return crc;
}

void CFileStream::calculateFileCRC(unsigned int *crc)
{
	if (file == NULL)
	{
		PrintF("Cannot calculate file CRC\n");
		return;
	}

	unsigned int bytes;

	unsigned int ipos = ftell(file);

	byte* data = (byte*)FileReadFull(file, &bytes);

	CRC32::addBlock(crc, data, bytes);

	free(data);
	
	fseek(file, ipos, SEEK_CUR);
}

void CFileStream::closeFile()
{
	fclose(file);
	file = nullptr;
}