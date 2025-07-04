#pragma once


#pragma warning(disable: 4018) // signed/unsigned dismatch
#pragma warning(disable: 4244) // unsigned long to byte


/*
#ifdef NULL
    #undef NULL
    #define NULL nullptr
#endif
*/

#ifdef ENGINE_EXPORTS
	#define ENGINE_API __declspec(dllexport)
#else
	#define ENGINE_API __declspec(dllimport)
#endif

#ifdef _MSC_VER
	#define POWER_INLINE __forceinline
#else
	#define POWER_INLINE inline
#endif

/*
Copy memory, if error, returns NULL.
*/
POWER_INLINE void* copyMemory(void* pvsrc, unsigned int size) 
{
	void* p = malloc(size);
	if (p == NULL) return NULL;

	memcpy(p, pvsrc, size);

	return p;
}
/*
Grows or shrink memory
*/
POWER_INLINE void resizeMemory(void** ppsrc, unsigned int newSize)
{
	void* p = realloc(ppsrc, newSize);

	if (p == NULL) return;

	*ppsrc = p;
}

POWER_INLINE const char* toCString(std::string& str)
{
	return str.c_str();
}

POWER_INLINE unsigned int _swap32(unsigned int i)
{
	return (i << 24) | ((i & 0x0000FF00) << 8) | ((i & 0x00FF0000) >> 8) | (i >> 24);
}

POWER_INLINE  short _swap16( short i )
{
	return (i >> 8) | (i << 8);
}

POWER_INLINE float _swapFloat( float f ) 
{
	int i = _swap32(*(int*)&f);
	return *(float*)&i;
}

POWER_INLINE long long _swap64(long long i) 
{

}


__forceinline short LittleShort(short i)
{
	return _swap16(i);
}


