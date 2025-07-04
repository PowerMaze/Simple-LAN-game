#include <stdAfx.h>
#include "PrintMessages.h"
#include <Engine/EMain.h>

void Error(const char* errorFormat, ...)
{
	va_list args;
	va_start(args, errorFormat);
	char error[1024];
	vsprintf(error, errorFormat, args);
	MessageBoxA(NULL, error, "In the Engine - fatal error", MB_ICONERROR | MB_OK);	
	va_end(args);

	PrintF("---------------------------------------------------------------\n");
	PrintF("In the Engine - fatal error:\n");
	PrintF(error);
	PrintF("---------------------------------------------------------------\n");

	InTheEngine_Shutdown();

	exit(1);
}



void Warning(const char* warnFormat, ...)
{
	va_list args;
	va_start(args, warnFormat);
	char warning[1024];
	vsprintf(warning, warnFormat, args);

#ifdef WIN32
	MessageBoxA(NULL, warning, "In the Engine - warning", MB_ICONWARNING | MB_OK);
#else
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "In the Engine - warning", warning, NULL);
#endif

	va_end(args);
}

void Throw(const char* throwMessage, ...) 
{
	va_list args;
	va_start(args, throwMessage);
	char thr[2048];
	vsprintf(thr, throwMessage, args);
	va_end(args);
	throw thr;
}