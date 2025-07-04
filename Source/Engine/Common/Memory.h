#pragma once

/*

Simpe memory allocation and manipulation. Basic idea:
Only Engine.dll/DEngibe.dll resposible for memory allocation
*/


// Allocate memory, if we don't have memory
ENGINE_API extern void* Malloc(size_t size);
// Free memory
ENGINE_API extern void Free(void* p);


// allocate memory from special operator new aligned c++ 17
template <typename type>
POWER_INLINE type* New()
{
	return new type;
}
// deletes memory
template <typename type>
POWER_INLINE void Delete(type* classPtr)
{
	delete classPtr;
}
