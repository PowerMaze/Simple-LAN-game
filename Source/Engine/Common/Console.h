#pragma once

/* console variable type to support */

enum eConsoleVariableType 
{
	CVT_INTEGER,
	CVT_UNSIGNED,
	CVT_BYTE,
	CVT_SHORT,
	CVT_FLOAT,
	CVT_DOUBLE,
	CVT_BOOL
//	CVT_STD_STRING
};


/*
============================================
		BASIC CLASS FOR CONSOLE
============================================
*/
class CConsole 
{
public:
	std::string buffer; // console buffer
//	std::vector<std::string> lines;
	std::string patch;  // path, which application uses
	FILE* hLog;         // log to save console buffer
public:
	ENGINE_API CConsole(); // default constructor
	ENGINE_API void End(); // end console
	ENGINE_API void Init(const char* strAppName); // init console, and setup who uses In the Engine
	ENGINE_API void Append(const char* strText);  // put string to console buffer
	//ENGINE_API const char* getLastLine(void);
	//ENGINE_API const char* getLineById(uint uid);
};

/*
================================================
BASIC CLASS FOR CONSOLE VARIABLES AND FUNCTIONS
================================================
*/
class CConsoleDataBase
{
private:

	// hash-map of located variables
	std::unordered_map <std::string, struct consoleVariable_s*> cdb_vars;
	// hash-map of located functions
	std::unordered_map <std::string, struct consoleFunction_s*> cdb_funcs;

public:

	ENGINE_API CConsoleDataBase(); // default constructor
	ENGINE_API ~CConsoleDataBase(); // destructor
	// add some variable to container data
	ENGINE_API void addConsoleVariable(byte varType, const char* strConName, void* ptr);
	// add some variable to container data
	ENGINE_API void addConsoleFunction(const char* strConName, void (*function)(void));
	// execute variable - sets a new value
	ENGINE_API void executeVar(const char* strConName, bool bPrintRes = true);

	ENGINE_API void* getVariable(const char* strVarName);

	// execute function
	ENGINE_API void executeFunc(const char* strConName);
	// execute from console output
	ENGINE_API void execute(const char* strConName);
	// delete from container, needed for unloading .dll
	ENGINE_API void deleteConsoleVariable(const char* strConName);
	ENGINE_API void deleteConsoleFunction(const char* strConName);
	// clear object
	ENGINE_API void Clear();
	// save to file
	ENGINE_API void Save(const CFileName& strFileName);
	// load from file
	ENGINE_API void Load(const CFileName& strFileName);
	ENGINE_API void PrintAllAvaliableFunctionsAndCommands(void);


	ENGINE_API float        getFloat(const char* con);
	ENGINE_API int          getInt(const char* con);
	ENGINE_API unsigned int getUnsigned(const char* con);
	ENGINE_API byte         getByte(const char* con);
	ENGINE_API short        getShort(const char* con);
	ENGINE_API double       getDouble(const char* con);
	ENGINE_API bool         getBool(const char* con);


	ENGINE_API void setInt(const char* con, const int& value);
	ENGINE_API void setUInt(const char* con, const unsigned int& value);
	ENGINE_API void setShort(const char* con, const short& value);
	ENGINE_API void setByte(const char* con, const byte& value);
	ENGINE_API void setFloat(const char* con, const float& f);
	ENGINE_API void setDouble(const char* con, const double& f);
	ENGINE_API void setBool(const char* con, const double& d);
private:
	bool isExiting(const char* var);
};

struct consoleVariable_s
{
	void* cv_pvValue; // pointer to variable, located anywhere
	byte cv_typeOfVar; // type of variable
};

struct consoleFunction_s 
{
	void (*function)(void); // function pointer
};


//ENGINE_API extern ConsoleLog_t* ConsoleMain;

// global object of console in the engine
ENGINE_API extern CConsole* pConsoleMain;
// global object of console in the engine
ENGINE_API extern CConsoleDataBase* pConsoleDataBase;

ENGINE_API extern void InitConsole(const char* strAppName);
// print text into console
ENGINE_API extern void PrintF(const char* strFormat, ...);

ENGINE_API extern void CloseConsole(void);


ENGINE_API extern void getConsoleBufferLines(CConsole* console, std::vector<std::string>& aLines);

//ENGINE_API extern ConsoleLog_t* ConsoleInit(const char* strPath);

//ENGINE_API extern void ConsoleClose(ConsoleLog_t* console);
