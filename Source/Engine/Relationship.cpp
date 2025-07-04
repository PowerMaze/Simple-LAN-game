#include <stdAfx.h>

#include <Engine\NET\NetMain.h>
#include <Engine\Common\Console.h>

void sendToServer(byte iClient, Network::CNetMSG* msg) 
{
	/*
	if (msg == NULL)
	{
		PrintF("Error: network message is null!\n");
		return;
	}

	if (msg->getData() == NULL) {
		PrintF("Error: network message is null!\n");
		return;
	}

	byte* buffer = (byte*)malloc(sizeof(Network::ITNP_t) + msg->getLength());

	Network::ITNP_t* itnpPacket = (Network::ITNP_t*)buffer;

	itnpPacket->net_bAccept = 0;
	itnpPacket->net_ullTimeSend = SDL_GetTicks64();
	itnpPacket->net_uiNumData = msg->getLength();
	itnpPacket->net_usNetCommand = 0x00;
	itnpPacket->net_bEncrypted = 0;
	itnpPacket->net_uiCheckSum = 0;

	memcpy(buffer + sizeof(Network::ITNP_t), msg->getData(), msg->getLength());

	sendto(clientSocket, buffer, msg->getLength() + sizeof(Network::ITNP_t), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
	*/
}
