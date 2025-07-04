#include <stdAfx.h>

#include "NetMain.h"

#include <Engine/Common/Console.h>
#include <Engine/Common/PrintMessages.h>
#include <Engine/Common/FileName.h>
#include <Engine/Common/FileIO.h>

namespace Network
{
	CNetMSG::CNetMSG(void)
	{
		msg_data = NULL;
		msg_uiCurrentPosition = 0;
		msg_uiLastPosition = msg_uiCurrentPosition;
		msg_uiMaxData = 0;
		msg_uiNumData = 0;
		msg_netCMD    = NET_CMD_NONE;
	}

	CNetMSG::CNetMSG(size_t numData)
	{
		msg_data = NULL;
		msg_uiCurrentPosition = 0;
		msg_uiLastPosition = msg_uiCurrentPosition;
		msg_uiMaxData = 0;
		msg_uiNumData = 0;

		msg_netCMD = NET_CMD_NONE;

		InitMessage(numData);
	}

	CNetMSG::~CNetMSG(void)
	{
		Clear();
	}

	void CNetMSG::Clear()
	{
		if (msg_data != NULL)
			free(msg_data);
		msg_data = NULL;
		msg_uiCurrentPosition = 0;
		msg_uiLastPosition    = 0;
		msg_uiNumData         = 0;
		msg_uiMaxData         = 0;
		msg_netCMD            = 0;
	}

	CNetMSG& CNetMSG::operator=(CNetMSG& other)
	{
		Clear();

		this->msg_uiCurrentPosition = other.msg_uiCurrentPosition;
		this->msg_uiLastPosition = other.msg_uiLastPosition;
		this->msg_uiNumData = other.msg_uiNumData;
		this->msg_uiMaxData = other.msg_uiMaxData;
		this->msg_netCMD = other.msg_netCMD;

		if (other.getData() == NULL)
		{
			this->msg_data = NULL;
			return *this;
		}

		memcpy(this->msg_data, other.getData(), other.msg_uiNumData);

		return *this;
	}

	void CNetMSG::copyData(byte* data, size_t numData)
	{

		if (numData > NET_MAX_BUFFER)
			Throw("Network message is to large!\n");

		Clear();

		//msg_data = (byte*)malloc(numData);

		//if (msg_data == NULL)
		//	Error("Cannot allocate message!\n");

		msg_uiMaxData = numData;
		msg_uiNumData = numData;

		msg_data = (byte*)copyMemory(data, numData);
	}

	void CNetMSG::setData(byte* data, size_t numData)
	{
		Clear();

		msg_data = data;
		msg_uiNumData = numData;
		msg_uiMaxData = numData;
	}

	void CNetMSG::InitMessage(size_t numData)
	{
		if (numData > NET_MAX_BUFFER)		
			Throw("Network message is to large!\n");

		Clear();
		
		msg_data = (byte*)malloc(numData);
		

		if (msg_data == NULL)
			Error("Cannot allocate message!\n");

		memset(msg_data, 0, sizeof(numData));

		msg_uiMaxData = numData;
	}

	void CNetMSG::dumpHexToFile(const std::string& strFile)
	{
		CFileName filename = "System\\" + strFile;

		FileSetGlobalPatch(filename);

		FILE* F = fopen((const char*)filename, "w");

		assert(F != NULL);

		FOREACH_ARRAY(getLength()) 
		{
			fprintf(F, "0x%X ", msg_data[i]);
		}

		fclose(F);
	}

	/******** Writing methods ********/

	void CNetMSG::write8(const byte& b)
	{
		if (msg_data == NULL) 
		{ 
			Warning("Cannot write data, because data = NULL\n");
			return;
		}

		if (msg_uiNumData + sizeof(byte) >= NET_MAX_BUFFER) {
			Throw("Network message is to large!\n");
		}

		msg_data[msg_uiNumData] = b;
		msg_uiNumData += sizeof(unsigned char);
	}

	void CNetMSG::write16(const unsigned short& s)
	{
		write8(s >> 8);
		write8(s & 0xFF);
	}

	void CNetMSG::write32(const unsigned int& i)
	{
		write8((i & 0xFF000000) >> 24);
		write8((i & 0x00FF0000) >> 16);
		write8((i & 0x0000FF00) >> 8);
		write8((i & 0x000000FF) >> 0);
	}

	void CNetMSG::write64(unsigned long long& ll)
	{
		write8((ll >> 56) & 255ui8);
		write8((ll >> 48) & 255ui8);
		write8((ll >> 40) & 255ui8);
		write8((ll >> 32) & 255ui8);
		write8((ll >> 24) & 255ui8);
		write8((ll >> 16) & 255ui8);
		write8((ll >> 8)  & 255ui8);
		write8((ll >> 0)  & 255ui8);
	}

	void CNetMSG::writeFloat(const float& f)
	{
		//writeBlock((byte*)&f, sizeof(float));
		write32(*(unsigned int*)&f);
	}

	void CNetMSG::writeDouble(const double& d) 
	{
		write64(*(unsigned long long*)&d);
	}

	void CNetMSG::writeString(std::string& str) 
	{
		write32(str.size());
		writeBlock((byte*)str.c_str(), str.size());
	}

	void CNetMSG::writeBlock(byte* data, size_t num) 
	{
		
		FOREACH_ARRAY(num) 
		{
			write8(data[i]);
		}

		/*
		if (msg_uiNumData + num >= NET_MAX_BUFFER) {
			PrintF("Cannot write block data! Buffer overflow!\n");
			return;
		}

		memcpy(msg_data + msg_uiNumData + num, data, num);

		msg_uiNumData += num;*/
	}

	void CNetMSG::writeVector(Vector3D& v)
	{
		writeFloat(v.x);
		writeFloat(v.y);
		writeFloat(v.z);
	}

	/******** Reading methods ********/

	void CNetMSG::read8(byte* b)
	{
		if (msg_uiCurrentPosition + sizeof(unsigned char) > NET_MAX_BUFFER || msg_uiCurrentPosition > msg_uiNumData) 
		{
			PrintF("Cannot read message max = %d, your = %d, read = %d, msg = %d\n", NET_MAX_BUFFER, msg_uiNumData, msg_uiCurrentPosition, msg_netCMD);
			return;
		}

		if (msg_data == NULL) 
		{
			PrintF("Cannot read NULL data!\n");
			return;
		}

		*b = msg_data[msg_uiCurrentPosition];

		msg_uiCurrentPosition += sizeof(unsigned char);
	}
	void CNetMSG::read16(unsigned short* s) 
	{
		unsigned char a, b;

		read8(&a);
		read8(&b);

		*s = (a << 8) | (b << 0);
	}
	void CNetMSG::read32(unsigned int* i) 
	{
		unsigned char a, b, c, d;
		read8(&a);
		read8(&b);
		read8(&c);
		read8(&d);

		// yes its weird, buuut its just same method which uses below
		*i = RGBA_toHEX(a, b, c, d);

		//*i = (a << 24) | (b << 16) | (c << 8) | (d << 0);
	}
	void CNetMSG::read64(unsigned long long* ll)
	{
		unsigned char a, b, c, d, e, f, g, h;

		read8(&a);
		read8(&b);
		read8(&c);
		read8(&d);
		read8(&e);
		read8(&f);
		read8(&g);
		read8(&h);

		*ll = (unsigned long long(a) << 56ui64) | 
			  (unsigned long long(b) << 48ui64) |
			  (unsigned long long(c) << 40ui64) |
			  (unsigned long long(d) << 32ui64) |
			  (unsigned long long(e) << 24ui64) |
			  (unsigned long long(f) << 16ui64) |
			  (unsigned long long(g) << 8ui64)  |
			  (unsigned long long(h) << 0ui64);
	}
	void CNetMSG::readFloat(float* f) 
	{
		unsigned int i;
		read32(&i);
		*f = *(float*)&i;
	}
	void CNetMSG::readDouble(double* d) 
	{
		unsigned long long ll;
		read64(&ll);
		*d = *(unsigned long long*)&ll;
	}
	void CNetMSG::readString(std::string& str) 
	{
		unsigned int length;
		read32(&length);

		char* data = (char*)malloc(length);
		readBlock((byte*)data, length);
		data[length] = 0;

		str = data;

		free(data);
	}
	void CNetMSG::readBlock(byte* data, size_t num)
	{
		FOREACH_ARRAY(num)
		{
			read8(&data[i]);
		}
	}

	void CNetMSG::readVector(Vector3D* v)
	{
		readFloat(&v->x);
		readFloat(&v->y);
		readFloat(&v->z);
	}

	/******** I/O methods ********/

	void CNetMSG::rewindMSG(void) 
	{
		msg_uiLastPosition = msg_uiCurrentPosition;
		msg_uiCurrentPosition = 0;
	}
	void CNetMSG::setPos(const unsigned int& newPos) 
	{
		msg_uiCurrentPosition = newPos;
	}

	byte* CNetMSG::getData(void)
	{
		return msg_data;
	}



	CNetMSGHolder::CNetMSGHolder() 
	{
		bCleaned = 0;
		msgh_uiNum = 0;
		/*
		FOREACH_ARRAY(8) 
		{
			msg[i].msg_data = nullptr;
		}*/

	}
	CNetMSGHolder::~CNetMSGHolder() 
	{
		//if (!bCleaned) Clear();
		Clear();
	}

	void CNetMSGHolder::Clear() 
	{
		FOREACH_ARRAY(msgh_uiNum)
		{
			delete msgh_aIncommingMessages[i];
		}

		msgh_aIncommingMessages.clear();
		msgh_uiNum = 0;
		//bCleaned = true;
	}

	void CNetMSGHolder::setNew(int i)
	{

	}

	CNetMSG* CNetMSGHolder::getMSG(int index)
	{
		return msgh_aIncommingMessages[index];
	}

	void CNetMSGHolder::setMSG(int index) 
	{

	}

};