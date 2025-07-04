#include <stdAfx.h>

#include "NetMain.h"

#include <Engine/Common/Console.h>
#include <Engine/Common/PrintMessages.h>

namespace Network
{
	void sendText(const char* strText) 
	{
		assert(strText != nullptr);

		char buff[1024];

		memset(buff, 0, sizeof(buff));

		strcpy(buff, strText);

		CNetMSG msg;

		msg.InitMessage(strlen(buff) + 1);

		msg.writeBlock((byte*)buff, strlen(buff));

		msg.setCMD(NET_CMD_SEND_TEXT);

		if (isServer()) 
		{
			FOREACH_ARRAY(NET_MAX_CLIENTS) 
			{
				if (netDataGlobals.clients[i].lastSeqNumber != 0) 
				{
					sendToClient(&msg, i);
				}
			}
		}
		else 
		{
			sendToServer(&msg);
		}

	}
};