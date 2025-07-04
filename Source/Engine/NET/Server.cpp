#include <stdAfx.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include "Netmain.h"

#include <Engine\Common\Console.h>
#include <Engine\Common\PrintMessages.h>

Network::NetData netDataGlobals;


int net_iNumClients = 3;

namespace Network
{
	void startServer()
	{

		if (net_iNumClients > NET_MAX_CLIENTS || net_iNumClients <= 0)
		{
			Throw("Trying to set clients %d, but max clients are %d", net_iNumClients, NET_MAX_CLIENTS);
		}

		net_uiSequenceTick = 0;

		memset(&netDataGlobals, 0, sizeof(struct NetData));

		PrintF("Start initialize server...\n");

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
			Throw("Cannot open winsock");
		}

		PrintF("Winsock opened\n");

		// step 2 get local address from pc
#if NET_ENABLE_LAN		
/*		PrintF("Getting local address(s)...\n");

		char hostname[256];
		gethostname(hostname, sizeof(hostname) - 1);
		PrintF("Host: %s\n", hostname);
		HOSTENT* host = gethostbyname(hostname);				
			

		if (host == NULL) 
		{
			WSACleanup();
			Throw("Cannot get local addreses\n");
		}


		std::string strAddresses = "";

		for (int i = 0; host->h_addr_list[i] != NULL; i++) {
			if (i > 0) 
			{
				strAddresses += ", ";
			}
			strAddresses += inet_ntoa(*(const in_addr*)host->h_addr_list[i]);
		}

		PrintF("Local addresses: %s\n", strAddresses);
		*/
		// step 3 setup UDP socket


		ULONG bufferSize = 0;
		DWORD result = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, nullptr, nullptr, &bufferSize);
		if (result != ERROR_BUFFER_OVERFLOW)
		{
			WSACleanup();
			Throw("GetAdaptersAddresses failed to get buffer size");
		}

		char* buffer = (char*)malloc(bufferSize);



		PIP_ADAPTER_ADDRESSES pAdapterAddresses = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(buffer);

		result = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, nullptr, pAdapterAddresses, &bufferSize);
		if (result != ERROR_SUCCESS) 
		{
			free(buffer);
			WSACleanup();
			Throw("GetAdaptersAddresses failed with error: %d", result);	
		}

		PrintF("Addresses: ");

		for (PIP_ADAPTER_ADDRESSES pAdapter = pAdapterAddresses; pAdapter != nullptr; pAdapter = pAdapter->Next) 
		{
			if (pAdapter->OperStatus == IfOperStatusUp && pAdapter->FirstUnicastAddress != nullptr) 
			{
				for (PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pAdapter->FirstUnicastAddress; pUnicast != nullptr; pUnicast = pUnicast->Next)
				{
					if (pUnicast->Address.lpSockaddr->sa_family == AF_INET) 
					{
						sockaddr_in* pIPv4 = reinterpret_cast<sockaddr_in*>(pUnicast->Address.lpSockaddr);
						char ipAddress[INET_ADDRSTRLEN];
						if (inet_ntop(AF_INET, &(pIPv4->sin_addr), ipAddress, INET_ADDRSTRLEN) != nullptr)
						{							
							PrintF("%s ", ipAddress);
						}
					}
				}
			}
		}

		PrintF("\n");

		free(buffer);



#endif		
		//SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
		netDataGlobals.nd_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (netDataGlobals.nd_socket == INVALID_SOCKET)
		{
			//strAddresses.clear();

			WSACleanup();	
			Throw("Socket creation failed: %s\n", lastErrorWSA());
		}

	//	BOOL bBroadcast = TRUE;
	//	setsockopt(netDataGlobals.nd_socket, SOL_SOCKET, SO_BROADCAST, (char*)&bBroadcast, sizeof(bBroadcast));

		u_long notBlock = 1;

		if (ioctlsocket(netDataGlobals.nd_socket, FIONBIO, &notBlock) == SOCKET_ERROR) 
		{
			closesocket(netDataGlobals.nd_socket);
			WSACleanup();
			Throw("Failed to setup noblocking socket: %s\n", lastErrorWSA());
		}

		// step 4 ssetup server address and client
	
		netDataGlobals.serverAddr.sin_family = AF_INET;
#if 0//NET_ENABLE_LAN				
		inet_pton(AF_INET, "127.0.0.1", &netDataGlobals.serverAddr.sin_addr);
#else		
		netDataGlobals.serverAddr.sin_addr.s_addr = INADDR_ANY;
#endif
		netDataGlobals.serverAddr.sin_port = htons(NET_PORT);

		// step 5 bind socket

		if (bind(netDataGlobals.nd_socket, (sockaddr*)&netDataGlobals.serverAddr, sizeof(netDataGlobals.serverAddr)) == SOCKET_ERROR)
		{			
			closesocket(netDataGlobals.nd_socket);
			WSACleanup();
			Throw("Failed bind UDP socket %s\n", lastErrorWSA());
		}

		PrintF("Server was started succesfull\n");

		PrintF("---------------------------------------------------------------\n");

		netDataGlobals.bOpenAsServer = true;
		netDataGlobals.bOpened = true;
		//netMainServer.

		memset(&netMainServer, 0, sizeof(struct NetServer));


		// in default max clients always 2, where first is server (local client)
		netMainServer.ns_numMaxClients = net_iNumClients;


		FOREACH_ARRAY(NET_MAX_CLIENTS) 
		{
			netDataGlobals.clients[i].clientsSize = sizeof(netDataGlobals.clients[i].clientsAddr);
	//		netDataGlobals.clientsSize[i] = sizeof(netDataGlobals.clientsAddr[i]);
		}

		generateServerKeys();

	}

	void closeServer(void) 
	{
		destroyKeys();

		if (!netDataGlobals.bOpened)
		{
			PrintF("Ouch\n");
			return;
		}

		PrintF("Close server...\n");

		closesocket(netDataGlobals.nd_socket);

		WSACleanup();

		if (pubSendingData != NULL)
			free(pubSendingData);

		pubSendingData = NULL;
	
		memset(&netDataGlobals, 0, sizeof(struct NetData));

		PrintF("Server closed\n");
	}

	bool isServer(void)
	{
		return netDataGlobals.bOpenAsServer;
	}

};