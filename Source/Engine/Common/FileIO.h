#pragma once

/*
///////////////////////////////////////////////

Helper functions for file I/O
Author: Dark-Head
Date: 12.02.2025

///////////////////////////////////////////////


/*
///////////////////////////////////////////////
change file extension

@param path: original path
@param pref: new extension to change

@return new path string

///////////////////////////////////////////////
*/
extern ENGINE_API char* changeFileEXT(const char* path, const char* pref);
/*
///////////////////////////////////////////////
dublicate string

@param strSource: string to dublicate

@return coppied string

///////////////////////////////////////////////
*/
extern ENGINE_API char* stringCopy(const char* strSource);
/*
///////////////////////////////////////////////
dublicate string, but formatted

@param strSource: string to dublicate, and params like in printf (%s, %c, %d etc..)

@return coppied string

///////////////////////////////////////////////
*/
extern ENGINE_API char* stringCopyF(const char* strFormat, ...);
/*
///////////////////////////////////////////////
Make std::string formatted like sprintf

@param str: std::string (out)
@param strFormat: formated buffer

///////////////////////////////////////////////
*/
extern ENGINE_API void stdStringF(std::string& str, const char* strFormat, ...);
/*
///////////////////////////////////////////////
Read a whole file into a raw buffer

@param file: file handler
@param bytes: reading bytes

@return raw file data

///////////////////////////////////////////////
*/
ENGINE_API extern void* FileReadFull(FILE* file, unsigned int* bytes);
/*
///////////////////////////////////////////////
Removes absolute patch from file

@param &strFile: path of file

///////////////////////////////////////////////
*/
ENGINE_API extern void FileDeleteAbsolutePatch(CFileName& strFile);
/*
///////////////////////////////////////////////
Sets absolute patch for file name

@param &strFile: path of file

///////////////////////////////////////////////
*/
ENGINE_API extern void FileSetGlobalPatch(CFileName& strFile);