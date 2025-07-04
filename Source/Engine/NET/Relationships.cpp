#include <stdAfx.h>

#include "NetMain.h"

#include <Engine\Common\Console.h>
#include <Engine\Common\PrintMessages.h>
#include <Engine/Common/FileName.h>

/*
----------------> client - server communication in low level <----------------
*/

struct Network::NetServer netMainServer;



#define ENABLE_OLD_COMMUNICATION_NET 0

unsigned int net_uiSequenceTick;
byte         net_ubCryptType   = 1;
unsigned int net_uiCRC         = 0;
bool         net_bCryptText    = true;
bool         net_bCryptAllData = false;
bool         net_bCheckCRCData = false;
bool         net_bDumpPacketToFile = false;

byte* pubSendingData = NULL;


namespace Network
{
	/*
	//////////////////////////////////////////////////////

	                Server communication

	//////////////////////////////////////////////////////
	*/

	void sendToClients(CNetMSG* msg)
	{
		//if (msg == NULL) return;
		//if (msg->getData() == NULL) return;
#if DISABLE_OLD_COMMUNICATION_NET
		
		ITNP_t* packet = NULL;

		//byte* buffer = (byte*)malloc(NET_MAX_DATASEND);
		memset(pubSendingData, 0, NET_MAX_DATASEND);
		//memcpy(pubSendingData, msg->getData(), msg->getLength());

		packet = (ITNP_t*)pubSendingData;
		packet->net_usNetCommand = msg->msg_netCMD;
		packet->net_ullTimeSend = SDL_GetTicks64();
		packet->net_uiNumData = msg->getLength();
		packet->net_uiCheckSum = 0;
		packet->net_bAccept = 0;

		if (packet->net_usNetCommand == NET_CMD_SEND_TEXT) {
			packet->net_bEncrypted = 1;
		}
		else {
			packet->net_bEncrypted = 0;
		}

		memcpy(pubSendingData + sizeof(ITNP_t), msg->getData(), msg->getLength());

		FOREACH_ARRAY(netMainServer.ns_numClients)
		{			
			if (sendto(netDataGlobals.nd_socket, (char*)pubSendingData, NET_MAX_DATASEND, 0, (sockaddr*)&netDataGlobals.clientsAddr[i], netDataGlobals.clientsSize[i]) == SOCKET_ERROR)
			{
				PrintF("^2Warning: cannot send to client[%i]: %s\n", i, lastErrorWSA());
				break;
			}
		}
#endif	
		//free(buffer);
	}

	void sendToClients(CNetMSGHolder* msgHolder) 
	{
#if ENABLE_OLD_COMMUNICATION_NET
		FOREACH_ARRAY(netMainServer.ns_numClients)
		{
			memset(pubSendingData, 0, NET_MAX_DATASEND);

			ITNP_t* packet = NULL;

			CNetMSG* msg = msgHolder->getMSG(i);

			packet = (ITNP_t*)pubSendingData;
			packet->net_usNetCommand = msg->msg_netCMD;
			packet->net_ullTimeSend = SDL_GetTicks64();
			packet->net_uiNumData = msg->getLength();
			packet->net_uiCheckSum = 0;
			packet->net_bAccept = 0;

			memcpy(pubSendingData + sizeof(ITNP_t), msg->getData(), msg->getLength());

			if (sendto(netDataGlobals.nd_socket, (char*)pubSendingData, NET_MAX_DATASEND, 0, (sockaddr*)&netDataGlobals.clientsAddr[i], netDataGlobals.clientsSize[i]) == SOCKET_ERROR)
			{
				PrintF("^2Warning: cannot send to client[%i]: %s\n", i, lastErrorWSA());
				break;
			}
		}
#endif
	}

	void sendToClient(CNetMSG* msg, int iClient) 
	{
		assert(msg != NULL);
		assert(msg->getData() != NULL);
//		if (msg == NULL) return;
//		if (msg->getData() == NULL) return;

		ITNP_t* packet = NULL;

		memset(pubSendingData, 0, NET_MAX_DATASEND);
		packet = (ITNP_t*)pubSendingData;
		packet->uiSequenceID = net_uiSequenceTick;
		packet->ubNetCMD = msg->msg_netCMD;
		packet->ullTimeSend = SDL_GetTicks64();
		packet->uiCRC = 0;
		

		if (packet->ubNetCMD == NET_CMD_SEND_TEXT && net_bCryptText && !net_bCryptAllData)
		{
			if (net_ubCryptType == 1) 
			{
				packet->uwFlags |= NET_UDP_USE_CRYPT_OWN;
				encryptMessage(msg);
			}
			else if (net_ubCryptType == 2)
				packet->uwFlags |= NET_UDP_USE_CRYPT_AES256;
			else
				packet->uwFlags |= 0;
		}

		if (net_bCryptAllData && !net_bCryptText)
		{
			if (net_ubCryptType == 1) 
			{
				packet->uwFlags |= NET_UDP_USE_CRYPT_OWN;
				encryptMessage(msg);
			}
			else if (net_ubCryptType == 2)
				packet->uwFlags |= NET_UDP_USE_CRYPT_AES256;
			else
				packet->uwFlags |= 0;
		}

		packet->uwNumData = msg->getLength();

		memcpy(pubSendingData + sizeof(ITNP_t), msg->getData(), msg->getLength());

		//PrintF("Sending to client[%d]...\n", iClient);

		if (sendto(netDataGlobals.nd_socket, (char*)pubSendingData, NET_MAX_DATASEND, 0, (sockaddr*)&netDataGlobals.clients[iClient].clientsAddr, netDataGlobals.clients[iClient].clientsSize) == SOCKET_ERROR)
		{
			PrintF("^2Warning: cannot send to client[%i]: %s\n", iClient, lastErrorWSA());
		}
		
	}

	void getPackets(CNetMSGHolder* holder)
	{
#if ENABLE_OLD_COMMUNICATION_NET		
		//if (netMainServer.ns_numClients == 0) return;

		//memset(&netDataGlobals, 0, sizeof(struct NetServer));

		CNetMSG msgTmp;
		ITNP_t* protocol = NULL;
		char buffer[1024];
		unsigned short generateID;
		unsigned short dummy;
	//	msg->InitMessage(NET_MAX_BUFFER);

		//byte* data = (byte*)malloc(NET_MAX_DATASEND);

		//memset(data, 0, NET_MAX_DATASEND);

		int bytes = 0;

		// we start because 
		for (int i = 0; i < netMainServer.ns_numMaxClients; i++) 
		{

			CNetMSG* msg = holder->getMSG(i);

			msg->InitMessage(NET_MAX_BUFFER);

			bytes = recvfrom(netDataGlobals.nd_socket,( char* )pubSendingData, NET_MAX_DATASEND, 0, (sockaddr*)&netDataGlobals.clientsAddr[i], &netDataGlobals.clientsSize[i]);

			//if (data == NULL) continue;

			if (bytes > 0)
			{
				//printf("Here\n");

				protocol = (ITNP_t*)pubSendingData;

				switch (protocol->net_usNetCommand)
				{
				case NET_CMD_CONNECT:	

					netMainServer.ns_numClients++;
					holder->msgh_uiNum++;

					PrintF("^5Here^8\n");

					msgTmp.InitMessage(512);
					memset(msgTmp.msg_data, 0, 512 * sizeof(unsigned char));

					strcpy(buffer, "Request from server: client connected");

					generateID = netMainServer.ns_numClients - 1;

					msgTmp.write16(generateID);
					msgTmp.write32(strlen(buffer));

					msgTmp.writeBlock((byte*)buffer, strlen(buffer));

					msgTmp.setCMD(NET_CMD_SEND_TEXT);
					//memcpy(&netMainServer.ns_Clients[client.index], &client, sizeof(struct NetClient));

					sendToClient(&msgTmp, generateID);

					msgTmp.Clear();

					//std::string str = "Request from server: client connected";

					//msg.InitMessage(str.size());
					//msg.setCMD()

					break;
				case NET_CMD_ADD_PLAYER:

					holder->msgh_uiNum++;
					msg->copyData(pubSendingData + sizeof(ITNP_t), protocol->net_uiNumData);
					msg->setCMD(NET_CMD_ADD_PLAYER);

					break;

				case NET_CMD_SEND_TEXT:

					holder->msgh_uiNum++;
					msg->copyData(pubSendingData + sizeof(ITNP_t), protocol->net_uiNumData);
					msg->setCMD(NET_CMD_SEND_TEXT);

					break;
				case NET_CMD_ACTION:
					// just write block from player
					holder->msgh_uiNum++;
					msg->copyData(pubSendingData + sizeof(ITNP_t), protocol->net_uiNumData);		
					msg->setCMD(NET_CMD_ACTION);

					msg->read16(&dummy);

					PrintF("From i = %d; id = %d\n", i, dummy);
					msg->rewindMSG();

					break;
				case NET_CMD_NONE:
				default:

					PrintF("NONE\n");

					break;

				}
				

			}
		}
		

		//free(data);
#endif
	}



	void listenClients(CNetMSGHolder* holder)
	{

		if (net_uiSequenceTick == 0xFFFFFFFF) 	net_uiSequenceTick = 0;

		net_uiSequenceTick++;

		sockaddr_in addrWhere; // address where we getting data
		int iLenAddr = sizeof(sockaddr_in); // length of address
		int iBytes; // num of bytes, without size UDP
		CNetMSG msg; // msg for stuff things
		CNetMSG* pMsg = NULL;

		char buffer[1024]; // buffer, usually for text
		int j = -1;

		bool bResult = false;
		unsigned int c = 0;
		CFileStream fileDummy;

		while ( j <= 7 ) 
		{

			j++;

			memset(pubSendingData, 0, NET_MAX_DATASEND);

			iBytes = recvfrom(netDataGlobals.nd_socket, (char*)pubSendingData, NET_MAX_DATASEND, 0, (sockaddr*)&addrWhere, &iLenAddr);

			if (iBytes <= 0) break;

			

			// because max clients 8
			unsigned short clId = 666;

			// find incoming address
	/*		FOREACH_ARRAY(netMainServer.ns_numMaxClients)
			{
				if (memcmp(&netDataGlobals.clientsAddr[i], &addrWhere, sizeof(sockaddr_in)) == 0)
				{
					clId = i;
					break;
				}
			}*/

			FOREACH_ARRAY(netMainServer.ns_numMaxClients)
			{
				if (memcmp(&netDataGlobals.clients[i].clientsAddr, &addrWhere, sizeof(sockaddr_in)) == 0)
				{
					clId = i;
					break;
				}
			}

			// get incoming address
/*			if (clId == 666)
			{
				FOREACH_ARRAY(netMainServer.ns_numMaxClients)
				{
					if (netDataGlobals.clientsAddr[i].sin_port == 0)
					{
						netDataGlobals.clientsAddr[i] = addrWhere;
						clId = i;
						break;
					}
				}
			}
*/
			if (clId == 666)
			{
				FOREACH_ARRAY(netMainServer.ns_numMaxClients)
				{
					if (netDataGlobals.clients[i].clientsAddr.sin_port == 0)
					{
						netDataGlobals.clients[i].clientsAddr = addrWhere;
						clId = i;
						break;
					}
				}
			}
			
			// get info from packet
			if (clId != 666)
			{		
				

				ITNP_t* packet = (ITNP_t*)pubSendingData;
				
				// if we have double packet, or packet has incorrect order, just skip this
				if (packet->uiSequenceID <= netDataGlobals.clients[clId].lastSeqNumber)
				{
					continue;
				}
				else 
				{
					netDataGlobals.clients[clId].lastSeqNumber = packet->uiSequenceID;
				}


				switch (packet->ubNetCMD)
				{
				case NET_CMD_CONNECT:

					holder->msgh_uiNum++;	

					pMsg = new CNetMSG;
					pMsg->copyData(pubSendingData + sizeof(ITNP_t), packet->uwNumData);

					//holder->msg[j].copyData(pubSendingData + sizeof(ITNP_t), packet->uwNumData);

					bResult = exchangeOnServerSideKey(pMsg, clId);

					pMsg->read32(&c);

					holder->msgh_aIncommingMessages.push_back(pMsg);

					//holder->msg[j].read32(&c);
					
					if (c != net_uiCRC) 
					{
						msg.InitMessage(64);
						memset(msg.msg_data, 0, 64 * sizeof(unsigned char));
						strcpy(buffer, "Request from server: bad CRC");
						msg.write32(strlen(buffer));
						msg.writeBlock((byte*)buffer, strlen(buffer));
						msg.setCMD(NET_CMD_BAD);

						sendToClient(&msg, clId);

						memset(&netDataGlobals.clients[clId].clientsAddr, 0, sizeof(sockaddr_in));
						netDataGlobals.clients[clId].lastSeqNumber = 0;
					}					
					if (!bResult) 
					{
						msg.InitMessage(512);
						memset(msg.msg_data, 0, 512 * sizeof(unsigned char));
						strcpy(buffer, "Request from server: Bad key exchange");
						msg.write32(strlen(buffer));
						msg.writeBlock((byte*)buffer, strlen(buffer));
						msg.setCMD(NET_CMD_BAD);

						sendToClient(&msg, clId);

						memset(&netDataGlobals.clients[clId].clientsAddr, 0, sizeof(sockaddr_in));
						netDataGlobals.clients[clId].lastSeqNumber = 0;
					}
					else 
					{
						strcpy(buffer, "Request from server: client connected");

						msg.InitMessage(128);

						msg.writeBlock(getPublicKey(), 32 * sizeof(byte));
						msg.write16(clId);
						msg.write32(strlen(buffer));
						msg.writeBlock((byte*)buffer, strlen(buffer));
						msg.setCMD(NET_CMD_CONNECT);

						sendToClient(&msg, clId);

					}
					/*
					msg.InitMessage(512);
					memset(msg.msg_data, 0, 512 * sizeof(unsigned char));

					strcpy(buffer, "Request from server: client connected");

					msg.write16(clId);
					msg.write32(strlen(buffer));
					msg.writeBlock((byte*)buffer, strlen(buffer));
					msg.setCMD(NET_CMD_SEND_TEXT);

					sendToClient(&msg, clId);*/

					break;
				case NET_CMD_DISCONNECT_FROM_SERVER:

					memset(&netDataGlobals.clients[clId].clientsAddr, 0, sizeof(sockaddr_in));
					netDataGlobals.clients[clId].lastSeqNumber = 0;

					pMsg = new CNetMSG;

					holder->msgh_uiNum++;

					pMsg->copyData(pubSendingData + sizeof(ITNP_t), packet->uwNumData);
					pMsg->setCMD(NET_CMD_DISCONNECT_FROM_SERVER);
					holder->msgh_aIncommingMessages.push_back(pMsg);

					//holder->msg[j].copyData(pubSendingData + sizeof(ITNP_t), packet->uwNumData);
					//holder->msg[j].setCMD(NET_CMD_DISCONNECT_FROM_SERVER);
										
					break;
				case NET_CMD_SEND_TEXT:

					if (net_bDumpPacketToFile)
					{
						fileDummy.open("System\\Data.pa", 0, 0);
						fileDummy.WriteToFile(pubSendingData, NET_MAX_DATASEND);
						fileDummy.closeFile();
						net_bDumpPacketToFile = false;
					}

					holder->msgh_uiNum++;

					pMsg = new CNetMSG;

					pMsg->copyData(pubSendingData + sizeof(ITNP_t), packet->uwNumData);
					pMsg->setCMD(NET_CMD_SEND_TEXT);

					if (packet->uwFlags & NET_UDP_USE_CRYPT_OWN)
					{
						bResult = decryptMessage(pMsg, clId);

						if (!bResult) 
						{
							PrintF("Invalid text message\n");
						}
						else 
						{
							memset(buffer, 0, sizeof(buffer));
							pMsg->readBlock((byte*)buffer, pMsg->getLength());
							PrintF("%s\n", buffer);
						}
					}
					else 
					{
						memset(buffer, 0, sizeof(buffer));
						pMsg->readBlock((byte*)buffer, packet->uwNumData);
						PrintF("%s\n", buffer);
					}

					holder->msgh_aIncommingMessages.push_back(pMsg);

					break;

				case NET_CMD_BAD:

					PrintF("NET_CMD_BAD\n");

					break;
/*
				case NET_CMD_ADD_PLAYER:
					
					holder->msgh_uiNum++;
					holder->msg[j].copyData(pubSendingData + sizeof(ITNP_t), packet->uwNumData);
					holder->msg[j].setCMD(NET_CMD_ADD_PLAYER);

					break;
				case NET_CMD_ACTION:


					holder->msgh_uiNum++;
					holder->msg[j].copyData(pubSendingData + sizeof(ITNP_t), packet->uwNumData);
					holder->msg[j].setCMD(NET_CMD_ACTION);

					break;
*/


				default:


					pMsg = new CNetMSG;

					holder->msgh_uiNum++;
					pMsg->copyData(pubSendingData + sizeof(ITNP_t), packet->uwNumData);
					pMsg->setCMD(packet->ubNetCMD);

					holder->msgh_aIncommingMessages.push_back(pMsg);

					break;


					PrintF("None");
				}
			}
		}
	}

	/*
	///////////////////////////////////////////////////////

	              Client communication

	///////////////////////////////////////////////////////
	*/

	void sendToServer(CNetMSG* msg)
	{
		//if (msg == NULL) return;
		assert(msg != NULL);

		if (net_uiSequenceTick == 0xFFFFFFFF) net_uiSequenceTick = 0;

		net_uiSequenceTick++;

		unsigned int length = msg->getLength() + sizeof(ITNP_t);

		if (length > NET_MAX_DATASEND)
		{
			PrintF("Cannot send data, because size bigger than maximum possible data\n");
			return;
		}

		memset(pubSendingData, 0, NET_MAX_DATASEND);

		ITNP_t* packet = (ITNP_t*)pubSendingData;

		packet->uiSequenceID = net_uiSequenceTick;
		packet->ubNetCMD     = msg->msg_netCMD;
		packet->ullTimeSend  = SDL_GetTicks64();	
		packet->uiCRC        = 0;
		packet->uwFlags     |= NET_UDP_UNRELIABLE;

	    // text always encrypted, but you can change this option
		if (packet->ubNetCMD == NET_CMD_SEND_TEXT && net_bCryptText && !net_bCryptAllData)
		{
			if (net_ubCryptType == 1) 
			{
				packet->uwFlags |= NET_UDP_USE_CRYPT_OWN;

				encryptMessage(msg);

			}
			else if (net_ubCryptType == 2)
				packet->uwFlags |= NET_UDP_USE_CRYPT_AES256;
			else
				packet->uwFlags |= 0;
		}
		// you can encrypt all data
		if (net_bCryptAllData && !net_bCryptText)
		{
			if (net_ubCryptType == 1) 
			{
				packet->uwFlags |= NET_UDP_USE_CRYPT_OWN;
				encryptMessage(msg);
			}
			else if (net_ubCryptType == 2)
				packet->uwFlags |= NET_UDP_USE_CRYPT_AES256;
			else
				packet->uwFlags |= 0;
		}
		// this sents if we have packet unreliable
		if (packet->ubNetCMD == NET_CMD_CONNECT)
		{
			packet->uwFlags &= ~NET_UDP_UNRELIABLE;
			packet->uwFlags |= NET_UDP_RELIABLE;
		}

		packet->uwNumData = msg->getLength();

		memcpy(pubSendingData + sizeof(ITNP_t), msg->getData(), packet->uwNumData);

		if (sendto(netDataGlobals.nd_socket, (const char*)pubSendingData, NET_MAX_DATASEND, 0, (sockaddr*)&netDataGlobals.serverAddr, sizeof(netDataGlobals.serverAddr)) == SOCKET_ERROR) 
		{
			PrintF("Cannot send data to server: %s\n", lastErrorWSA());
		}

		packet = NULL;
	}

	void getFromServer(CNetMSG* msg) 
	{
		//if (msg == NULL) return;

		ITNP_t* packet = NULL;

		sockaddr_in serverAddrNew;

		int size = sizeof(serverAddrNew);

		char buffer[1024];

		

		int bytes;

		double start = (double(SDL_GetTicks64()) * 0.001) + 10.0;

		while (1) 
		{
			memset(pubSendingData, 0, NET_MAX_DATASEND);

			bytes = recvfrom(netDataGlobals.nd_socket, (char*)pubSendingData, NET_MAX_DATASEND, 0, (sockaddr*)&serverAddrNew, &size);

			if (bytes > 0) break;

			if (start < (double(SDL_GetTicks64()) * 0.001)) 
			{
				Warning("Server: request timeout\n");
				pConsoleDataBase->executeFunc("Quit()");
			}
	/*		if (recvfrom(netDataGlobals.nd_socket, (char*)pubSendingData, NET_MAX_DATASEND, 0, (sockaddr*)&serverAddrNew, &size) == SOCKET_ERROR)
			{
				PrintF("Cannot get data from server: %s\n", lastErrorWSA());
				return;
			}*/
		}
		packet = (ITNP_t*)pubSendingData;

		// if we have doubled packets, just ignore it, command of stop server must be ignored
		if (packet->uiSequenceID <= netDataGlobals.uiLastTick && packet->ubNetCMD != NET_CMD_STOP_SERVER)
		{
			msg->InitMessage(15);
			memset(msg->msg_data, 0, sizeof(byte) * 15);
			msg->setCMD(NET_CMD_BAD);
			return;
		}
		else 
		{
			netDataGlobals.uiLastTick = packet->uiSequenceID;
		}

		msg->copyData(pubSendingData + sizeof(ITNP_t), packet->uwNumData);
		msg->msg_netCMD = packet->ubNetCMD;

		if (msg->msg_netCMD == NET_CMD_CONNECT) 
		{
			bool b = exchangeOnClientSideKey(msg);
			if (!b) msg->msg_netCMD = NET_CMD_BAD;
		}

		if (msg->msg_netCMD == NET_CMD_SEND_TEXT) 
		{
			if (packet->uwFlags & NET_UDP_USE_CRYPT_OWN)
			{
				bool bResult = decryptMessage(msg);

				if (!bResult)
				{
					PrintF("Invalid text message\n");
				}
				else
				{
					memset(buffer, 0, sizeof(buffer));
					msg->readBlock((byte*)buffer, packet->uwNumData);
					PrintF("%s\n", buffer);
				}
			}
			else 
			{
				memset(buffer, 0, sizeof(buffer));
				msg->readBlock((byte*)buffer, packet->uwNumData);
				PrintF("%s\n", buffer);
			}
		}
	}
};