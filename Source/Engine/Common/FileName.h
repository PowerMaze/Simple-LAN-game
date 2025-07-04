#pragma once

/*
//////////////////////////////////////////////////
   Basic class for file name and file io operations
//////////////////////////////////////////////////
*/

#define FILE_BIN	0
#define FILE_TXT	1
#define FILE_WRITE	0
#define FILE_READ	1 


//#define FILENAME(str) CFileName file(str)

class CFileName 
{
public:
	/* ����� ����� */
	std::string strFileName;
public:
	/* ����������� ����������� */
	ENGINE_API CFileName() = default;
	/* ����������� ����� std::string */
	ENGINE_API CFileName(const std::string newFileName);
	/* ���������� ����� const char */
	ENGINE_API CFileName(const char* newFileName);
	/* 
	���� ���������� �����. 
	
	�������: ���������� ����� ������� �� ����� ���������
	strFileName.ChangeEXT(".wo");
	*/
	ENGINE_API void ChangeEXT(const char* newExt);
	/* ��������� ����� � �-����� char @return const char*  */
	ENGINE_API const char* getCharPointer(void) { return strFileName.c_str();	}
	/* �������� ����� ����� */
	ENGINE_API std::string GetFileNameWitoutDir(void);
	/* ��������� ���������� ����� */
	ENGINE_API bool CompareEXT(const char* ext);
	/* ����������� ��������� � ������ ����� CFileName */
	ENGINE_API CFileName &operator=(const CFileName newFilename);
	/* ����������� ��������� � std::string ����� */
	ENGINE_API CFileName &operator=(const std::string& newFilename);
	/* ����������� ��������� � ������ char* ����� */
	ENGINE_API CFileName &operator=(const char* newFilename);
	ENGINE_API inline operator const char*(void) { return strFileName.c_str(); }
	ENGINE_API inline operator const char* (void) const { return strFileName.c_str(); }
	ENGINE_API inline operator std::string(void) { return strFileName; }
};

class ENGINE_API CFileStream
{
public:
	/* default constructor */
	CFileStream(void);
	/* destructor */
	~CFileStream(void);
	/* check file ID */
	bool checkID(const char* strID);
	/* check version of file */
	bool checkVersion(unsigned int version);
	bool checkID_Safe(const char* strID);
	/* write version to file */
	void writeVersion(unsigned int version);
	/* write file ID */
	void writeID(const char* strID);
	/*
	  open file method
	  @param uMode - mode to open, 0 = binary, 1 = text 
	  @param uType - type of open file, 0 = for writing, 1 = for reading
	  @return succesfful of opening file, false = file not opening, true - opening is well
	*/
	bool open(const CFileName& strFileName, unsigned int uMode, unsigned int uType);
	/*
	  write data to file
	  @param data - block to write
	  @param size - size of block data in bytes
	*/
	void WriteToFile(const void* data, size_t size);
	/*
	  write std::string to file
	*/
	void WriteString(const std::string& str);
	/*
	  write string to file
	*/
	void WriteString(const char* str);
	/*
	  read data from file
	  @param data - block for read
	  @param size - size of block data in bytes
	*/
	void ReadFromFile(void* data, size_t size);
	/*
	  set pointer position to start file
	*/
	void setToStartFile(void);
	/*
	  read std::string from file
	*/
	void ReadStringFromFile(std::string& string);
	/*
	  read char string and return value
	*/
	char* ReadStringFromFile(void);
	/*
	  read a whole file
	  @param bytes - output bytes of reading all data file
	  @return full raw data from file
	*/
	void* ReadFullFileData(unsigned int* bytes);
	/*
	  set pointer position to start file
	  see setToStartFile()
	*/
	void SeekStart();
	/*
	  gets length of file
	*/
	unsigned long getLengthFile(void);
	/*
	  close handle of file
	*/
	void closeFile();

	unsigned int calculateFileCRC(void);

	void calculateFileCRC(unsigned int* crc);

	unsigned int getCurrentPosition(void);
private:
	bool isReading; // writing to file or reading from file mode
	FILE* file;	    // main file handler
};

// global path for load/save file
ENGINE_API extern std::string strGlobalPatch;
//ENGINE_API extern char* strMod;