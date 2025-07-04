#include <stdAfx.h>
#include <Engine/Common/PrintMessages.h>
#include <Engine/Common/Console.h>

#include "../NetMain.h"

#include <sodium.h>

static byte* enctyptionKey = NULL;

/*---------> client side <------------*/
static byte cl_pk[crypto_kx_PUBLICKEYBYTES];
static byte cl_sk[crypto_kx_SECRETKEYBYTES];
static byte cl_rx[crypto_kx_SESSIONKEYBYTES];
static byte cl_tx[crypto_kx_SESSIONKEYBYTES];

/*---------> server side <----------- */

static byte sv_pk[crypto_kx_PUBLICKEYBYTES];
static byte sv_sk[crypto_kx_SECRETKEYBYTES];
static byte sv_tx[crypto_kx_SESSIONKEYBYTES];
struct serverKeysForClient
{
	byte sv_rx[crypto_kx_SESSIONKEYBYTES];
}sv_keys[NET_MAX_CLIENTS];

void initializeProtection() 
{
	int result = sodium_init();

	if (result < 0)
	{
		Error("Cannot initialize cryptographic library! sodium_init() returned %d", result);
	}


	sodium_memzero(cl_pk, crypto_kx_PUBLICKEYBYTES * sizeof(byte));
	sodium_memzero(cl_sk, crypto_kx_SECRETKEYBYTES * sizeof(byte));
	sodium_memzero(cl_rx, crypto_kx_SESSIONKEYBYTES * sizeof(byte));
	sodium_memzero(cl_tx, crypto_kx_SESSIONKEYBYTES * sizeof(byte));

	sodium_memzero(sv_pk, crypto_kx_PUBLICKEYBYTES * sizeof(byte));
	sodium_memzero(sv_sk, crypto_kx_SECRETKEYBYTES * sizeof(byte));
	sodium_memzero(sv_tx, crypto_kx_SESSIONKEYBYTES * sizeof(byte));

	sodium_memzero(sv_keys, sizeof(struct serverKeysForClient));

}

void destroyKeys() 
{
	sodium_memzero(cl_pk, crypto_kx_PUBLICKEYBYTES * sizeof(byte));
	sodium_memzero(cl_sk, crypto_kx_SECRETKEYBYTES * sizeof(byte));
	sodium_memzero(cl_rx, crypto_kx_SESSIONKEYBYTES * sizeof(byte));
	sodium_memzero(cl_tx, crypto_kx_SESSIONKEYBYTES * sizeof(byte));

	sodium_memzero(sv_pk, crypto_kx_PUBLICKEYBYTES * sizeof(byte));
	sodium_memzero(sv_sk, crypto_kx_SECRETKEYBYTES * sizeof(byte));
	sodium_memzero(sv_tx, crypto_kx_SESSIONKEYBYTES * sizeof(byte));

	sodium_memzero(sv_keys, sizeof(struct serverKeysForClient));
}


void generateServerKeys()
{
	if (crypto_kx_keypair(sv_pk, sv_sk) < 0)
	{
		Throw("Cannot generate crypt-keys\n");
	}
}

void generateClientKeys()
{
	if (crypto_kx_keypair(cl_pk, cl_sk) < 0)
	{
		Throw("Cannot generate crypt-keys\n");
	}
}

bool exchangeOnClientSideKey(Network::CNetMSG* msg) 
{
	msg->readBlock(sv_pk, crypto_kx_PUBLICKEYBYTES);

	if (crypto_kx_client_session_keys(cl_rx, cl_tx,	cl_pk, cl_sk, sv_pk) != 0) 
	{
		PrintF("Cannot get public server key!\n");
		return false;
	}

	//PrintF("cl_rx = ");

	//FOREACH_ARRAY(32)
	//{
	//	PrintF("0x%X ", cl_rx[i]);
	//}

	//PrintF("\n");

	//PrintF("cl_tx = ");

	//FOREACH_ARRAY(32)
	//{
	//	PrintF("0x%X ", cl_tx[i]);
	//}

	//PrintF("\n");

	return true;
}

bool exchangeOnServerSideKey(Network::CNetMSG* msg, unsigned int iClient) 
{
	msg->readBlock(cl_pk, crypto_kx_PUBLICKEYBYTES);

	if (crypto_kx_server_session_keys(sv_keys[iClient].sv_rx, sv_tx, sv_pk, sv_sk, cl_pk) != 0) 
	{
		PrintF("Cannot get public client[%d] key!\n", iClient);
		return false;
	}

	//PrintF("sv_rx = ");

	//FOREACH_ARRAY(32) 
	//{
	//	PrintF("0x%X ", sv_keys[iClient].sv_rx[i]);
	//}

	//PrintF("\n");

	//PrintF("sv_tx = ");

	//FOREACH_ARRAY(32)
	//{
	//	PrintF("0x%X ", sv_tx[i]);
	//}

	//PrintF("\n");

	return true;
}

byte* getPublicKey(void)
{
	if (Network::isServer())
		return sv_pk;
	else
		return cl_pk;
}

namespace Network
{
	bool encryptMessage(CNetMSG* msg) 
	{
		unsigned int msgLen = msg->getLength();		

		PrintF("Len = %d\n", msgLen);

		byte nonce[crypto_secretbox_NONCEBYTES];

		randombytes_buf(nonce, sizeof(nonce));

		byte* buff = (byte*)calloc(crypto_secretbox_MACBYTES + msgLen, sizeof(byte));

		assert(buff != NULL);

		if (isServer())
		{

			if (crypto_secretbox_easy(buff, msg->getData(), msgLen, nonce, sv_tx) != 0)
			{
				free(buff);
				PrintF("Error to encrypt msg\n");
				return false;
			}
		}
		else
		{
			if (crypto_secretbox_easy(buff, msg->getData(), msgLen, nonce, cl_tx) != 0)
			{
				free(buff);
				PrintF("Error to encrypt msg\n");
				return false;
			}
		}

		byte lastCMD = msg->msg_netCMD;

		msg->Clear();

		msg->InitMessage(crypto_secretbox_NONCEBYTES + msgLen + crypto_secretbox_MACBYTES);
		msg->writeBlock(nonce, crypto_secretbox_NONCEBYTES);
		msg->writeBlock(buff, crypto_secretbox_MACBYTES + msgLen);
		msg->setCMD(lastCMD);
		free(buff);
//		msg->dumpHexToFile("Enc.txt");
		return true;
	}

	bool decryptMessage(CNetMSG* msg, int iClient) 
	{
//		msg->dumpHexToFile("Dec.txt");
		byte nonce[crypto_secretbox_NONCEBYTES];

		msg->readBlock(nonce, crypto_secretbox_NONCEBYTES);
		

		unsigned int full = msg->getLength(); 
		unsigned int cipherLen = full - crypto_secretbox_NONCEBYTES; 
		unsigned int plainLen = cipherLen - crypto_secretbox_MACBYTES;


		byte* bufftext   = (byte*)calloc(plainLen, sizeof(byte));
		byte* buffcipher = (byte*)calloc(cipherLen, sizeof(byte)); // cipher = mac + ciphertext

		assert(buffcipher != NULL);
		assert(bufftext != NULL);

		msg->readBlock(buffcipher, cipherLen);

		if (isServer())
		{
			if (crypto_secretbox_open_easy(bufftext, buffcipher,
				cipherLen, nonce, sv_keys[iClient].sv_rx) != 0)
			{
				PrintF("Cannot decrypt message from client\n");
				free(bufftext);
				free(buffcipher);
				return false;
			}
		}
		else
		{
			if (crypto_secretbox_open_easy(bufftext, buffcipher,
				cipherLen, nonce, cl_rx) != 0)
			{
				free(buffcipher);
				free(bufftext);
				PrintF("Cannot decrypt message from server");
				return false;
			}
		}

		byte lastCMD = msg->msg_netCMD;

		msg->Clear();

		msg->setCMD(lastCMD);

		msg->InitMessage(plainLen);

		msg->writeBlock(bufftext, plainLen);

		free(bufftext);
		free(buffcipher);

		return true;

	}
};