#include "stdAfx.h"

#include "Console.h"
#include "FileName.h"
#include "FileIO.h"
#include "PrintMessages.h"


void PrintAllSymbols()
{
	if (pConsoleDataBase != NULL)
		pConsoleDataBase->PrintAllAvaliableFunctionsAndCommands();
}

void createError()
{
	Error("call createError()\n");
}

void createWarning() 
{
	Warning("call createWarning()\n");
}



CConsoleDataBase::CConsoleDataBase() 
{
	addConsoleFunction("PrintAllSymbols()", PrintAllSymbols);
	addConsoleFunction("createError()", createError);
	addConsoleFunction("createWarning()", createWarning);
}

CConsoleDataBase::~CConsoleDataBase() 
{
	Clear();
}
// add some variable to container data
void CConsoleDataBase::addConsoleVariable(byte varType, const char* strConName, void* ptr)
{	

	struct consoleVariable_s* con = (struct consoleVariable_s*)malloc(sizeof(struct consoleVariable_s));

	if (con == NULL) {
		PrintF("CConsoleDataBase: Cannot add variable symbol to console: %s\n", strConName);
		return;
	}

	con->cv_typeOfVar = varType;
	con->cv_pvValue   = ptr;

	cdb_vars[strConName] = con;
}

// add some variable to container data
void CConsoleDataBase::addConsoleFunction(const char* strConName, void (*function)(void))
{
	struct consoleFunction_s* func = (struct consoleFunction_s*)malloc(sizeof(struct consoleFunction_s));

	if (func == NULL) {
		PrintF("CConsoleDataBase: Cannot add function to console: \"%s\"\n", strConName);
		return;
	}

	func->function = function;

	cdb_funcs[strConName] = func;

}
// execute variable - sets a new value
void CConsoleDataBase::executeVar(const char* strConName, bool bPrintRes)
{
	std::string str = strConName;
	size_t pos = str.find('=');

	bool b = true;

	// if we just input symbol name
	if (pos == std::string::npos) 
	{
		auto toFind = cdb_vars.find(strConName);

		if (toFind == cdb_vars.end())
		{
			PrintF("Variable: \"%s\" is not from console data base\n", strConName);
			return;
		}

		struct consoleVariable_s* con = toFind->second;

		if (con == NULL) 
		{
			PrintF("Variable: \"%s\" was not loaded (NULL)\n", strConName);
			return;
		}

		if (!bPrintRes) return;

		switch (con->cv_typeOfVar)
		{
		case CVT_INTEGER:    PrintF("> %s = %d\n", strConName, *(int*)con->cv_pvValue);                 break;
		case CVT_UNSIGNED:   PrintF("> %s = %u\n", strConName, *(unsigned int*)con->cv_pvValue);        break;
		case CVT_BYTE:       PrintF("> %s = %u\n", strConName, *(unsigned char*)con->cv_pvValue);       break;
		case CVT_SHORT:      PrintF("> %s = %d\n", strConName, *(short*)con->cv_pvValue);               break;
		case CVT_FLOAT:      PrintF("> %s = %f\n", strConName, *(float*)con->cv_pvValue);               break;
		case CVT_DOUBLE:     PrintF("> %s = %lf\n", strConName,*(double*)con->cv_pvValue);              break;
		case CVT_BOOL:       PrintF("> %s = %d\n", strConName, *(bool*)con->cv_pvValue);                break;
		default:
			break;
		}

		return;
	}

	// if we put into the command
		
	char* strLine = stringCopy(strConName);

	char* p = strtok(strLine, " =");

	if (p == NULL) {
		PrintF("Cannot execute \"%s\"\n", strConName);
		free(strLine);
		return;
	}


	char* key = stringCopy(p);

	p = strtok(NULL, " =");

	if (p == NULL) {
		PrintF("Cannot execute \"%s\".\nHas no value to execute\n", strConName);
		free(strLine);
		free(key);
		return;
	}

	char* value = stringCopy(p);

	auto toFind = cdb_vars.find(key);

	if (toFind == cdb_vars.end())
	{
		PrintF("Variable \"%s\" is not part of console data base\n", key);
		return;
	}

	struct consoleVariable_s* con = toFind->second;

	if (con == NULL)
	{
		PrintF("Variable: \"%s\" was not loaded (NULL)\n", key);
		return;
	}

	switch (con->cv_typeOfVar)
	{
	case CVT_INTEGER: 

		*(int*)con->cv_pvValue = atoi(value);
		if (bPrintRes) 	PrintF("< %s = %d\n", key, *(int*)con->cv_pvValue);
		break;
	case CVT_UNSIGNED:  

		*(unsigned int*)con->cv_pvValue = atoi(value);
		if (bPrintRes)	PrintF("< %s = %u\n", key, *(unsigned int*)con->cv_pvValue);
		break;
	case CVT_BYTE:  

		*(unsigned char*)con->cv_pvValue = atoi(value);
		if (bPrintRes)	PrintF("< %s = %u\n", key, *(unsigned char*)con->cv_pvValue);
		break;
	case CVT_SHORT:  

		*(short*)con->cv_pvValue = atoi(value);
		if (bPrintRes)	PrintF("< %s = %d\n", key, *(short*)con->cv_pvValue);
		break;
	case CVT_FLOAT:  

		*(float*)con->cv_pvValue = atof(value);
		if (bPrintRes)	PrintF("< %s = %f\n", key, *(float*)con->cv_pvValue);
		break;
	case CVT_DOUBLE:   

		*(double*)con->cv_pvValue = atof(value);
		if (bPrintRes) PrintF("< %s = %lf\n", key, *(double*)con->cv_pvValue);
		break;
	case CVT_BOOL: 		

		if (strcmp(value, "0") == 0 || strcmp(value, "false") == 0) 		
			b = false;
		
		if (strcmp(value, "1") == 0 || strcmp(value, "true") == 0)
			b = true;

		*(bool*)con->cv_pvValue = b;
		if (bPrintRes)	PrintF("< %s = %d\n", key, *(bool*)con->cv_pvValue);
		break;
	default:
		break;
	}
	
	free(strLine);
	free(value);
	free(key);
	p = NULL;
}

int CConsoleDataBase::getInt(const char* con) 
{
	auto toFind = cdb_vars.find(con);

	if (toFind == cdb_vars.end())
	{
		PrintF("Symbol \"%s\" not found!\n", con);
		return -63;
	}

	if (toFind->second->cv_typeOfVar == CVT_INTEGER && toFind->second->cv_pvValue != NULL)
		return 	*(int*)toFind->second->cv_pvValue;
	return -63;
}

float CConsoleDataBase::getFloat(const char* con)
{
	auto toFind = cdb_vars.find(con);

	if (toFind == cdb_vars.end())
	{
		PrintF("Symbol \"%s\" not found!\n", con);
		return 59.1f;
	}

	if (toFind->second->cv_typeOfVar == CVT_FLOAT && toFind->second->cv_pvValue != NULL)
		return 	*(float*)toFind->second->cv_pvValue;

	return 59.1f;
}

// execute function
void CConsoleDataBase::executeFunc(const char* strConName)
{
	auto toFind = cdb_funcs.find(strConName);

	if (toFind == cdb_funcs.end()) 
	{
		PrintF("Symbol \"%s\" not found!\n", strConName);
		return;
	}

	if (toFind->second->function == NULL) 
	{
		PrintF("Cannot execute function, \"%s\" was NULL\n");
		return;
	}

	struct consoleFunction_s* f = toFind->second;

	PrintF("< %s\n", strConName);

	f->function();	
}
// execute from console output

void CConsoleDataBase::execute(const char* strConName) 
{
	/* first detect function */

	char *strLine = stringCopy(strConName);
	char *strOriginal = stringCopy(strConName);
	
	char* p = strtok(strLine, " ()");
	int best01 = 0, best02 = 0;
	bool bLeft = false, bRight = false;

	if (strcmp(strConName, strLine) == 0) 
	{
		free(strOriginal);
		free(strLine);

		goto VariableTest;		
	}

	for (int i = 0; *(strOriginal + i) != 0; i++)
	{
		best01++;
		if (strOriginal[i] == '(') { bLeft = true; break; }
	}

	for (int i = 0; *(strOriginal + i) != 0; i++)
	{
		best02++;
		if (strOriginal[i] == ')') { bRight = true; break; }
	}

	if ((best01 > best02) || !bLeft || !bRight)
	{
		free(strOriginal);
		free(strLine);

		goto VariableTest;
	}
		
	strOriginal[strlen(strLine)]     = '(';
	strOriginal[strlen(strLine) + 1] = ')';
	strOriginal[strlen(strLine) + 2] = '\0';

	executeFunc(strOriginal);
	free(strOriginal);
	free(strLine);

	return;

	/* if not found function, find variable symbol */
VariableTest:

	executeVar(strConName);

}
// delete from container, needed for unloading .dll
void CConsoleDataBase::deleteConsoleVariable(const char* strConName) 
{
	auto toFind = cdb_vars.find(strConName);

	if (toFind == cdb_vars.end())
	{
#ifdef _DEBUG
		PrintF("Symbol \"%s\" not found!\n", strConName);
#endif
		return;
	}

	if (toFind->second == NULL) {
		PrintF("Cannot delete symbol \"%s\", because its NULL\n", strConName);
		return;
	}

	toFind->second->cv_pvValue = NULL;

	free(toFind->second);

	toFind->second = NULL;

	cdb_vars.erase(toFind);
}

void* CConsoleDataBase::getVariable(const char* strVarName)
{
	auto toFind = cdb_vars.find(strVarName);

	if (toFind == cdb_vars.end())
	{
		PrintF("Symbol \"%s\" not found!\n", strVarName);
		return 0;
	}

	if (toFind->second == NULL) {
		PrintF("Cannot get symbol \"%s\", because its NULL\n", strVarName);
		return 0;
	}

	struct consoleVariable_s* con = toFind->second;

	switch (con->cv_typeOfVar)
	{
	case CVT_INTEGER:    return ( int* )con->cv_pvValue;
	case CVT_UNSIGNED:   return ( unsigned int* )con->cv_pvValue;
	case CVT_BYTE:       return ( unsigned char* )con->cv_pvValue;
	case CVT_SHORT:      return ( short* )con->cv_pvValue;
	case CVT_FLOAT:      return ( float* )con->cv_pvValue;
	case CVT_DOUBLE:     return ( double* )con->cv_pvValue;
	case CVT_BOOL:       return ( bool* )con->cv_pvValue;
	default:
		return NULL;
	}

}

void CConsoleDataBase::deleteConsoleFunction(const char* strConName) 
{

}
// clear object
void CConsoleDataBase::Clear() 
{

	FOREACH_STD_UNORDERED_MAP(cdb_vars, std::string, struct consoleVariable_s*) 
	{
		if (iter->second != NULL) 
		{
			iter->second->cv_pvValue = NULL;
			free(iter->second);
		}
	}

	FOREACH_STD_UNORDERED_MAP(cdb_funcs, std::string, struct consoleFunction_s*)
	{
		if (iter->second != NULL)
		{
			iter->second->function = NULL;
			free(iter->second);
		}
	}

	cdb_vars.clear();
	cdb_funcs.clear();
}
// save to file
void CConsoleDataBase::Save(const CFileName& strFileName)
{
	return;

	FILE* file;

	CFileName strFile = strFileName;

	FileSetGlobalPatch(strFile);

	file = fopen((const char*)strFile, "w");

	if (file == NULL) 
	{
		PrintF("Cannot save cosole variables: %s\n", (const char*)strFileName);
		return;
	}	

	FOREACH_STD_UNORDERED_MAP(cdb_vars, std::string, struct consoleVariable_s*)
	{
		struct consoleVariable_s* con = iter->second;

		if (con == NULL) continue;

		switch (con->cv_typeOfVar)
		{
		case CVT_INTEGER:

			fprintf(file, "%s = %d;\n", iter->first.c_str(), *(int*)con->cv_pvValue);

			break;
		case CVT_UNSIGNED:   

			fprintf(file, "%s = %u;\n", iter->first.c_str(), *(unsigned int*)con->cv_pvValue);


			break;
		case CVT_BYTE:  

			fprintf(file, "%s = %u;\n", iter->first.c_str(), *(byte*)con->cv_pvValue);

			break;
		case CVT_SHORT:   

			fprintf(file, "%s = %d;\n", iter->first.c_str(), *(short*)con->cv_pvValue);

			break;
		case CVT_FLOAT:  

			fprintf(file, "%s = %f;\n", iter->first.c_str(), *(float*)con->cv_pvValue);

			break;
		case CVT_DOUBLE: 

			fprintf(file, "%s = %lf;\n", iter->first.c_str(), *(double*)con->cv_pvValue);

			break;
		case CVT_BOOL:   

			fprintf(file, "%s = %d;\n", iter->first.c_str(), *(bool*)con->cv_pvValue);

			break;
		default:
			continue;
		}
	}

	fclose(file);
}

bool CConsoleDataBase::isExiting(const char* var)
{
	auto toFind = cdb_vars.find(var);

	if (toFind == cdb_vars.end())
	{
		PrintF("Symbol \"%s\" not found!\n", var);
		return false;
	}

	if (toFind->second == NULL) {
		PrintF("Cannot get symbol \"%s\", because its NULL\n", var);
		return false;
	}

	return true;
}

void CConsoleDataBase::setInt(const char* con, const int& value)
{

	if (!isExiting(con)) return;

	//struct consoleVariable_s* conv = toFind->second;;

	//*(int*)conv->cv_pvValue = value;

}
void CConsoleDataBase::setUInt(const char* con, const unsigned int& value)
{

}
void CConsoleDataBase::setShort(const char* con, const short& value)
{

}
void CConsoleDataBase::setByte(const char* con, const byte& value)
{

}
void CConsoleDataBase::setFloat(const char* con, const float& f)
{

}
void CConsoleDataBase::setDouble(const char* con, const double& f)
{

}
void CConsoleDataBase::setBool(const char* con, const double& d)
{

}

// load from file
void CConsoleDataBase::Load(const CFileName& strFileName) 
{

}

void CConsoleDataBase::PrintAllAvaliableFunctionsAndCommands(void) 
{
	PrintF("All loaded console symbols\n");

	PrintF("Variables:\n");

	FOREACH_STD_UNORDERED_MAP(cdb_vars, std::string, struct consoleVariable_s*)
	{
		struct consoleVariable_s* con = iter->second;

		if (con == NULL) continue;

		switch (con->cv_typeOfVar)
		{
		case CVT_INTEGER:    PrintF(" %s = %d\n", iter->first.c_str(),  *( int* ) con->cv_pvValue);                 break;
		case CVT_UNSIGNED:   PrintF(" %s = %u\n", iter->first.c_str(),  *( unsigned int* ) con->cv_pvValue);        break;
		case CVT_BYTE:       PrintF(" %s = %u\n", iter->first.c_str(),  *( unsigned char* ) con->cv_pvValue);       break;
		case CVT_SHORT:      PrintF(" %s = %d\n", iter->first.c_str(),  *( short* ) con->cv_pvValue);               break;
		case CVT_FLOAT:      PrintF(" %s = %f\n", iter->first.c_str(),  *( float* ) con->cv_pvValue);               break;
		case CVT_DOUBLE:     PrintF(" %s = %lf\n", iter->first.c_str(), *( double* ) con->cv_pvValue);              break;
		case CVT_BOOL:       PrintF(" %s = %d\n", iter->first.c_str(),  *( bool* ) con->cv_pvValue);                break;
		default:
			continue;
		}	
	}

	PrintF("Functions:\n");

	FOREACH_STD_UNORDERED_MAP(cdb_funcs, std::string, struct consoleFunction_s*) 
	{
		PrintF(" %s\n", iter->first.c_str());
	}
	PrintF("---------------------------------------------------------------\n");
}

bool isVariable(char* str) 
{
	//for (unsigned int i = 0; i < strlen(str); i++) 
	//{
	//	if (str[i] == ' ' || str[i] == '=')
	//}
	return false;
}