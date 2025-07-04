#include <stdAfx.h>

#include "NetMain.h"

#include <Engine\Common\Console.h>
#include <Engine\Common\PrintMessages.h>


char *strAddressString = NULL;

namespace Network
{
	void startClient(void) 
	{
		net_uiSequenceTick = 0;

		memset(&netDataGlobals, 0, sizeof(struct NetData));

		PrintF("Start initialize client...\n");

		pubSendingData = (byte*)malloc(NET_MAX_DATASEND);

		if (pubSendingData == NULL)
		{
			Throw("Cannot create buffer data for sending msg");
		}

		memset(pubSendingData, 0, NET_MAX_DATASEND);

		// step 1 set up winsock

		WSADATA wsaData;

		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			Throw("Cannot open winsock\n");
		}

		PrintF("Winsock opened\n");

		netDataGlobals.nd_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (netDataGlobals.nd_socket == INVALID_SOCKET)
		{
			WSACleanup();
			Throw("Socket creation failed: %d\n", lastErrorWSA());			
			return;
		}

		u_long notBlock = 1;

		if (ioctlsocket(netDataGlobals.nd_socket, FIONBIO, &notBlock) == SOCKET_ERROR)
		{
			WSACleanup();
			Throw("Failed to setup noblocking socket: %s\n", lastErrorWSA());
		}

		// Налаштування адреси сервера
		netDataGlobals.serverAddr.sin_family = AF_INET;		

#if NET_ENABLE_LAN
		inet_pton(AF_INET, "127.0.0.1", &netDataGlobals.serverAddr.sin_addr);
#else
		if (inet_pton(AF_INET, strAddressString/*"127.0.0.1"*/, &netDataGlobals.serverAddr.sin_addr) == SOCKET_ERROR)
		{
			closesocket(netDataGlobals.nd_socket);
			WSACleanup();
			Throw("Failed to setup noblocking socket: %s\n", lastErrorWSA());
		}
#endif
		netDataGlobals.serverAddr.sin_port = htons(NET_PORT);

		//перевірка
//		u_long isNonBlocking = 0;
//		ioctlsocket(netDataGlobals.nd_socket, FIONBIO, &isNonBlocking);
//		PrintF("Socket blocking = %d\n", isNonBlocking);

		netDataGlobals.bOpened = true;
		netDataGlobals.bOpenAsServer = false;	

		generateClientKeys();

	}
	void closeClient(void) 
	{
		destroyKeys();

		if (!netDataGlobals.bOpened) return;

		PrintF("Close client connection...\n");

		closesocket(netDataGlobals.nd_socket);

		WSACleanup();

		if (pubSendingData != NULL)
			free(pubSendingData);

		pubSendingData = NULL;

		memset(&netDataGlobals, 0, sizeof(struct NetData));

		PrintF("Client stopped succesfull\n");
	}

};