#pragma once

extern unsigned int uiCRCTable32[256];

namespace CRC32
{
	inline void start(unsigned int* crc) 
	{ 
		*crc = 0xFFFFFFFFU; 
	}

	inline void add8(unsigned int* crc, unsigned char b)
	{
		*crc = (*crc >> 8) ^ uiCRCTable32[unsigned char(*crc) ^ b];
	}

	inline void add16(unsigned int* crc, unsigned short s)
	{
		add8( crc, s >> 8);
		add8( crc, s & 0xFF);
	}

	inline void add32(unsigned int* crc, unsigned int i)
	{
		add8( crc, (i >> 24) & 0xFF );
		add8( crc, (i >> 16) & 0xFF );
		add8( crc, (i >> 8)  & 0xFF );
		add8( crc, (i >> 0)  & 0xFF );
	}

	inline void add64(unsigned int* crc, unsigned long long ll) 
	{
		add8( crc, (ll >> 56) & 0xFF);
		add8( crc, (ll >> 48) & 0xFF);
		add8( crc, (ll >> 40) & 0xFF);
		add8( crc, (ll >> 32) & 0xFF);
		add8( crc, (ll >> 24) & 0xFF);
		add8( crc, (ll >> 16) & 0xFF);
		add8( crc, (ll >> 8)  & 0xFF);
		add8( crc, (ll >> 0)  & 0xFF);
	}

	inline void addFloat(unsigned int* crc, float f) 
	{
		add32( crc, *(unsigned int*)&f);
	}

	inline void addDouble(unsigned int* crc, double d)
	{
		add64( crc, *(unsigned long long*)&d);
	}

	inline void addBlock(unsigned int* crc, unsigned char* pv, unsigned int size) 
	{
		for (unsigned int i = 0; i < size; i++) 
			add8(crc, pv[i]);		
	}

	inline void end(unsigned int* crc)
	{ 
		*crc ^= 0xFFFFFFFFU; 
	}
};