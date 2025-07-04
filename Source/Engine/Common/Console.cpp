#include <stdAfx.h>
#include "Console.h"
#include "Engine/Common/PrintMessages.h"
#include "Engine/Common/FileName.h"
#include "Engine/Common/FileIO.h"

CConsole::CConsole() 
{	patch = "";
	hLog = nullptr;
}
void CConsole::End() 
{
/*	hLog = fopen(patch.c_str(), "w");

	if (hLog == nullptr) 
	{
		Warning("Cannot save console log: %s\n", patch.c_str());
	}
	else 
	{

//		char* str = stringCopyF("Tick: %d\n", SDL_GetTicks64());
//		Append(str);
//		free(str);
		fprintf(hLog, buffer.c_str());
		//fwrite(buffer.c_str(), sizeof(char) * patch.length(), 1, hLog);
		fclose(hLog);
	}*/
	if (hLog != nullptr) fclose(hLog);
	hLog = nullptr;
	patch.clear();
	buffer.clear();

}
void CConsole::Init(const char* strAppName) 
{	
	patch = strGlobalPatch + "System\\" + strAppName;

	hLog = fopen(patch.c_str(), "w");

	if (hLog == nullptr) 
	{
		Error("Cannot create console file buffer %s\n", patch.c_str());
	}

	buffer = "---------------------------------------------------------------\n";
	printf("%s", buffer.c_str());
	fprintf(hLog, "%s", buffer.c_str());
	fflush(hLog);
}
void CConsole::Append(const char* strText) 
{
	buffer += strText;
	fprintf(hLog, "%s", strText);
	fflush(hLog);
}

void getConsoleBufferLines(CConsole* console, std::vector<std::string>& aLines)
{

	if (console == NULL) return;

	std::string line = "";
	int length = console->buffer.length();

	for (int i = 0; i < length; i++)
	{
		if (console->buffer[i] == '\n')
		{
			line += '\n';
			aLines.push_back(line);
			line.clear();
			line = "";
			continue;
		}
		line += console->buffer[i];
	}

	line.clear();
}

/*
const char* CConsole::getLastLine(void)
{

}
*/
/*

ConsoleLog_t* ConsoleInit(const char* strPath)
{
	ConsoleLog_t* console = (ConsoleLog_t*)malloc(sizeof(ConsoleLog_t));

	if (console == nullptr)
		return nullptr;

	console->flog = NULL;

	console->path = (char*)malloc(strlen(strPath) + 1);

	console->path[strlen(strPath)] = '\0';
	memcpy(console->path, strPath, strlen(strPath) + 1);

	console->buff.append("\n");

	return console;


}

*/


//static std::string conAppName;
//static std::string conMainBuffer;
//static std::vector<std::string> conLines;

/*
void InitConsole(const char* strAppName) 
{
	conAppName = strAppName;
	conMainBuffer = "";
}
*/

void PrintF(const char* strFormat, ...) 
{
	va_list args;
	va_start(args, strFormat);

	char str[2048];

	vsprintf(str, strFormat, args);
	printf("%s", str);

	_RPT1(_CRT_WARN, "%s", str);

	if (pConsoleMain != NULL) 
	{
		pConsoleMain->Append(str);
	}

	va_end(args);

}
/*
void ConPrintF(const char* strFormat, ...)
{
	static std::string strLine = "";

	va_list args;
	va_start(args, strFormat);

	char str[2048];

	vsprintf(str, strFormat, args);

	printf("%s", str);

	_RPT1(_CRT_WARN, "%s", str);

	va_end(args);

	conMainBuffer += str;

	strLine += str;

	if (strLine.find('\n'))
	{
		conLines.push_back(strLine);
		strLine.clear();
		strLine = "";
	}
}


void CloseConsole(void) 
{
	CFileStream strFileStream;

	CFileName strFileName = "System\\" + conAppName + ".log";
	
	if (!strFileStream.open(strFileName, 0, 0)) 
	{
		Warning("Cannot save console input to log file\n");
	}

	strFileStream.WriteToFile(conMainBuffer.c_str(), conMainBuffer.size());

	strFileStream.closeFile();

	conMainBuffer.clear();
	conLines.clear();
	conAppName.clear();
}

std::vector <std::string>* getLinesFromConsole()
{
	return &conLines;
}
*/


/*
     Close console log
*/
/*
void ConsoleClose(ConsoleLog_t* console)
{
	//printf("%s\n", console->path);
	console->flog = fopen("Power.log", "w");
	if (console->flog == NULL) { 
		Warning("Cannot create file to write console log ((((\n"); 
	}
	else {
		fwrite(console->buff.c_str(), sizeof(char), console->buff.size(), console->flog);

		fclose(console->flog);
	}
	free(console->path);

}*/

