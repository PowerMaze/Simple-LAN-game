#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>

#include <assert.h>

#include <vector>
#include "FileName.h"

#include <unordered_map>


class CEntity 
{
public:
    std::string strName;
    std::string strAlpha;

    float health;

public:
    CEntity()
    {
        strName = "Marker";
        strAlpha = "Alpha";

        health = 100.f;
    }

    virtual void MainLoop(void) 
    {
        //printf("call some functions\n");
        printf("%s %s %.2f\n", strName.c_str(), strAlpha.c_str(), health);
    }

};

class CMarker : public CEntity
{
public:
    CEntity* ent_Pointer;
    float fTime;
public:
    CMarker() 
    {
        ent_Pointer = NULL;
        fTime = 0;
        CEntity::CEntity();
    }
public:
    virtual void MainLoop () override
    {
        printf("load\n");
        printf("%p, %f\n", ent_Pointer, fTime);

        CEntity::MainLoop();
    }
};

__forceinline unsigned int _swap32u( unsigned int i)
{
    return (i << 24) | ((i & 0x0000FF00) << 8) | ((i & 0x00FF0000) >> 8) | (i >> 24);
}


__forceinline unsigned int _swap32(unsigned int i) 
{
    return (i << 24) | ((i & 0x0000FF00) << 8) | (i & 0x00FF0000) >> 8 | (i >> 24);
}


__forceinline unsigned short _swap16(unsigned short i)
{
    return (i >> 8) | (i << 8);
}

__forceinline float _swapFloat(float f)
{
    unsigned int i = _swap32(*(unsigned int*)&f);
    return *(float*)&i;
}

#define IS_EVEN(x) x & 1

#define MAX_WORLD 4095.0f
#define MIN_WORLD -4095.0f



__forceinline bool __cdecl isBigEndian()
{
    unsigned short x = 1;
    return (*(byte*)&x) == 0;
}

byte Data[32];

byte* getData() 
{
    return Data;
}

int main(int argc, char** argv)
{
    for (int i = 0; i < 32; i++) 
    {
        Data[i] = i * 2;
    }

    byte* d = getData();

    for (int i = 0; i < 32; i++)
    {
        printf("%u ", d[i]);
    }

    return 0;
}


/*
class CTextureObject
{
public:
    unsigned char* raw;
    unsigned int size;
public:
    CTextureObject() : raw(nullptr), size(0) {}
    CTextureObject(const CTextureObject& tex) : raw(nullptr), size(0)
    {
        if (tex.size > 0)
        {
            raw = (unsigned char*)malloc(tex.size);
            memcpy(raw, tex.raw, tex.size);
            size = tex.size;
        }
    }
    ~CTextureObject() { Clear(); }
    CTextureObject(CTextureObject&& other) noexcept : raw(other.raw), size(other.size)
    {
        other.raw = nullptr;
        other.size = 0;
    }
    void Clear()
    {
        if (raw)
            free(raw);
        raw = nullptr;
        size = 0;
    }
    void Set(unsigned int usize)
    {
        Clear();
        size = usize;
        raw = (unsigned char*)malloc(size);
        if (!raw)
        {
            printf("error\n");
            return;
        }
        memset(raw, 0, size);
    }
    void Display()
    {
        if (raw)
            printf("0x%p\n", (void*)raw);
    }

    CTextureObject& operator=(const CTextureObject& other) {
        if (this != &other)
        {
            Clear();
            if (other.raw)
            {
                size = other.size;
                raw = (unsigned char*)malloc(size);
                memcpy(raw, other.raw, size);
            }
        }
        return *this;
    }

    CTextureObject& operator=(CTextureObject&& other) noexcept {
        if (this != &other) {
            Clear();
            raw = other.raw;
            size = other.size;
            other.raw = nullptr;
            other.size = 0;
        }
        return *this;
    }
};

class CRoom
{
public:
	CTextureObject texture;
	int tag;
public:
	CRoom() = default;
	void Set(unsigned int tag, int dim)
	{
		this->tag = tag;
		texture.Set(dim);
	}
};
*/



