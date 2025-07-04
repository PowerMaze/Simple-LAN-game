#include <stdAfx.h>

#include "Memory.h"
#include "PrintMessages.h"

void* Malloc(size_t size)
{
	void* p = malloc(size);

	if (p == nullptr)
		Error("Cannot allocate memory, %d bytes we need\n", size);

	return p;
}

void Free(void* p)
{
	free(p);
}