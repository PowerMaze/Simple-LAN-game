#include <stdAfx.h>

#include "NetMain.h"

#include <Engine\Common\Console.h>

/*/
static SOCKET serverSocket;
static sockaddr_in serverAddr;
static sockaddr_in clientAddr[NET_MAX_CLIENTS];


namespace Network 
{
	void setupSocket(void)
	{

		PrintF("Initialaze socket...\n");

		WSADATA wsaData;

		memset(&mainSockets, 0, sizeof(struct SocketsInfo));

		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			PrintF("Failed to init winsock!\n");
			return;
		}

		mainSockets.si_serverAddr.sin_addr.s_addr = INADDR_ANY;

	}

	void freeSocket(void) 
	{

		closesocket(mainSockets.si_serverSocket);
		WSACleanup();
	}
}
*/