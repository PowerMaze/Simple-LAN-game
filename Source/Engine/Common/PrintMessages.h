#pragma once

// error report
ENGINE_API extern void Error(const char* errorFormat, ...);
// warning report
ENGINE_API extern void Warning(const char* warnFormat, ...);
// exceptions report
ENGINE_API extern void Throw(const char* errorFormat, ...);

/*ENGINE_API extern void ErrorOpenGL();*/
