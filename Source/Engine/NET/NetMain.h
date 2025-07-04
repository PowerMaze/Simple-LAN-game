#pragma once

#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "ws2_32.lib")


/*
----------------> Networks defines <----------------
*/

/*

Enables lan network. This means we can connect from LAN/WLAN to server computer. 

For debugging network game, just disable it

*/
#define NET_ENABLE_LAN 0

// version of protocol
#define NET_PROTOCOL_VERSION 0x96
// port 
#define NET_PORT 5910
// max possible data, which can write and send, (Bytes)
//#define NET_MAX_BUFFER 2048
// max data + size of protocol
//#define NET_MAX_DATASEND ( NET_MAX_BUFFER + sizeof(struct Network::ITNP))
// max possible clients
#define NET_MAX_CLIENTS 8


#define NET_MAX_BUFFER 1441

#define NET_MAX_PACKET_SIZE \
((sizeof (byte)) + (sizeof(unsigned short)) + (sizeof(unsigned short)) + \
(sizeof(unsigned int)) + (sizeof(unsigned int)) + \
(sizeof(unsigned long long)))

#define NET_MAX_DATASEND (NET_MAX_BUFFER) + (NET_MAX_PACKET_SIZE)

/*
----------------> Network command (NetCMD) <----------------
*/

extern bool net_bCryptText;
extern bool net_bCryptAllData;
extern bool net_bCheckCRCData;
extern byte net_ubCryptType;     // 0 - none, 1 - aes256, 2 - own algorithm
extern bool net_bDumpPacketToFile;

extern unsigned int net_uiSequenceTick;
extern int net_iNumClients;


enum eNetCMD 
{
	// layer of network communication

	NET_CMD_BAD, 
	NET_CMD_NONE,
	NET_CMD_CONNECT,
	NET_CMD_DISCONNECT_FROM_SERVER,
	NET_CMD_STOP_SERVER,
	NET_CMD_SEND_TEXT,

	// layer of game/application

	NET_CMD_ACTION,
	NET_CMD_ADD_PLAYER,
	NET_CMD_REMOVE_PLAYER,
	NET_CMD_EVENT_CHANGE_SCORE
};

struct EntityState
{
	unsigned int es_uiCRC_Model;
	unsigned int es_uiCRC_Texture;

	Vector3D vPos;

	byte yaw;
	byte pitch;	
};

typedef EntityState EntityState_t;

typedef struct ClientSnapshot 
{
	unsigned int uiTick;
	unsigned int uiButtons;
	double dTime;
//	Network::CNetMSG* msg;
}ClientSnapshot_t;

/* get last error WSA data */
extern const char* lastErrorWSA(void);

/*
----------------> Network protocol flags <----------------
*/

/*
Note:

flags in packet (uiFlags) generates from network message (ubNetCMD). Other flags, generate from engine
network system, like which compression we use or encryption algorithm

*/

#define NET_UDP_RELIABLE             ( 1u << 0 ) // reliable packets must have ACK
#define NET_UDP_UNRELIABLE           ( 1u << 1 ) // unreliable packets cannot have ACK, so engine dont care if we gets packet
#define NET_UDP_USE_ZLIB_COMPRESSION ( 1u << 2 ) // use ZLIB compression library
#define NET_UDP_USE_ZSTD_COMPRESSION ( 1u << 3 ) // use ZSTD compression library
#define NET_UDP_USE_CRYPT_AES256     ( 1u << 4 ) // crypt messages with aes256
#define NET_UDP_USE_CRYPT_OWN        ( 1u << 5 ) // you can add own algorithm, but if we have on both sides: server and client
#define NET_UDP_USE_CRC              ( 1u << 6 )
#define NET_UDP_ACK                  ( 1u << 7 )
#define NET_UDP_NONE                 (    0    )

#define NET_UDP_FULL  \
((NET_UDP_RELIABLE) | (NET_UDP_USE_ZSTD_COMPRESSION) | \
(NET_UDP_USE_CRYPT_AES256) | (NET_UDP_USE_CRC))
/*
///////////////////////////////////////

@brief namespace for network classes, functions
and structures

///////////////////////////////////////
*/

namespace Network
{

    #pragma pack(push, 1)
	/*
	@brief Main network protocol uses for Engine sending packets.

	ITNP (In The Network Protocol)
	*/
	struct ITNP
	{
		byte net_bEncrypted;   // << check if encrypted message
		byte net_bAccept;      // << check if packet accepted

		byte net_usNetCommand; // << network command type +
		
		unsigned int net_uiFlags;

		unsigned int       net_uiCheckSum;   // << CRC check data  ?

		unsigned long long net_ullTimeSend;  // << time when packet was send to server/client +
		unsigned int  net_uiNumData;         // << num data +
	};
    #pragma pack(pop)


	#pragma pack(push, 1)
	/*
	@brief Main network protocol uses for Engine sending packets.

	ITNP (In The Network Protocol)
	*/
	struct _ITNP
	{
		/* network cmd message */
		byte ubNetCMD;
		/* packet flags */
		unsigned short uwFlags; 
		/* num of data send */
		unsigned short uwNumData;
		/* checksum data */
		unsigned int uiCRC;
		/* number of sequence */
		unsigned int uiSequenceID;
		/* time when we send packet */
		unsigned long long ullTimeSend;
	};
	#pragma pack(pop)

	typedef struct _ITNP ITNP_t;

	struct NetClient
	{
		sockaddr_in clientsAddr;    // address from client
		int clientsSize;            // size of client

		unsigned int lastSeqNumber; // last sequNumber of client

		unsigned long long time;    // time
	};

	struct NetData
	{
		SOCKET nd_socket;       // main socket
		sockaddr_in serverAddr; // server address

		struct NetClient clients[NET_MAX_CLIENTS]; // client on server, active only on server side

		unsigned int uiLastTick; // last sequence id

		bool bOpenAsServer; // check if we server
		bool bOpened;       // check if we open socket
	};


	struct NetServer 
	{
		struct NetClient ns_Clients[NET_MAX_CLIENTS];
		int ns_numClients;    // currents players count
		int ns_numMaxClients; // max player count in server (ns_numMaxClients <= NET_MAX_CLIENTS)
		long long ns_time;
	};

	/*
	Network message. This can be like file i/o, but only for memory. This Sending mesage data to server/client
	communication from high level (<GameName>.dll) to low level (Engine.dll)

	TODO: this supports only unsigneds integers, make support with negative values
	*/
	class ENGINE_API CNetMSG
	{
	public:

		unsigned int msg_uiNumData;        // num data of network message
		unsigned int msg_uiMaxData;        // max data of network message
		unsigned int msg_uiCurrentPosition;// current position of network message
		unsigned int msg_uiLastPosition;   // last positions (uses for rewind)

		byte* msg_data;             // main data in bytes
		byte  msg_netCMD;           // network command

	public:
		CNetMSG(void);
		CNetMSG(size_t numData);
		~CNetMSG(void);

		inline void Clear();

		inline void InitMessage(size_t numData);

		inline void setData(byte* data, size_t numData);

		inline void copyData(byte* data, size_t numData);

		inline void setCMD(byte cmd) { msg_netCMD = cmd; }

		/******** Writing methods ********/

		
		inline void write8(const byte &b);
		inline void write16(const unsigned short &s);
		inline void write32(const unsigned int &i);
		inline void write64(unsigned long long &ll);

		inline void writeFloat(const float &f);
		inline void writeDouble(const double &d);
		inline void writeString(std::string& str);
		inline void writeBlock(byte* data, size_t num);
		inline void writeVector(Vector3D& v);

		/******** Reading methods ********/

		inline void read8(byte* b);
		inline void read16(unsigned short* s);
		inline void read32(unsigned int* i);
		inline void read64(unsigned long long* ll);
		inline void readFloat(float* f);
		inline void readDouble(double* d);
		inline void readString(std::string& str);
		inline void readBlock(byte* data, size_t num);
		inline void readVector(Vector3D* v);

		/******** I/O methods ********/

		inline void rewindMSG(void);
		inline void setPos(const unsigned int& newPos);
		inline byte* getData(void);

		void dumpHexToFile(const std::string &strFile);

		inline unsigned int getLength() { return msg_uiNumData; }

		CNetMSG& operator=(CNetMSG& other);
	};
	/*
	
	Array holder of network message
	
	*/
	class CNetMSGHolder
	{
	public:

		int msgh_uiNum;
//		CNetMSG msg[NET_MAX_CLIENTS]; // FIXME: in GServer.cpp we can read from NULL!!!

		std::vector<CNetMSG*> msgh_aIncommingMessages;

		bool bCleaned;
	public:		
		ENGINE_API CNetMSGHolder();
		ENGINE_API ~CNetMSGHolder();

		ENGINE_API void Clear();

		ENGINE_API void setNew(int i);

		ENGINE_API CNetMSG* getMSG(int index);
		ENGINE_API void setMSG(int index);
	};

	// encrypt message
//	void Encrypt(byte* data, size_t size, byte* key, size_t keySize);
	// decrypt message
//	void Decrypt(byte* data, size_t size, byte* key, size_t keySize);

	bool encryptMessage(CNetMSG* msg);
	bool decryptMessage(CNetMSG* msg, int iClient = 0);

	// send Text to all clients
	ENGINE_API void sendText(const char* strText);

	//ENGINE_API void setupSocket(void);
	//ENGINE_API void freeSocket(void);

	/*
    ----------------> Server interface <----------------
    */

	// initialize server
	ENGINE_API void startServer(void); // see Server.cpp
	// close server
	ENGINE_API void closeServer(void); // see Server.cpp
	// send data to clients
	ENGINE_API void sendToClients(CNetMSG* msg);
	ENGINE_API void sendToClients(CNetMSGHolder* msgHolder);
	// send to client
	ENGINE_API void sendToClient(CNetMSG* msg, int iClient);

	// get data from clients
	ENGINE_API void getPackets(CNetMSGHolder* holder);


	ENGINE_API void listenClients(CNetMSGHolder* holder);

	// check if we server
	ENGINE_API inline bool isServer(void);

	/*
    ----------------> Client interface <----------------
    */
	
	ENGINE_API void startClient(void); // see Client.cpp
	ENGINE_API void closeClient(void); // see Client.cpp

	ENGINE_API void sendToServer(CNetMSG* msg);

	ENGINE_API void getFromServer(CNetMSG* msg);

	/*
    ----------------> Connect/dissconect <----------------
    */

};

// initialize libsodium library
extern void initializeProtection();
// generate server keys
extern void generateServerKeys();
// generate client keys
extern void generateClientKeys();
// destroy server and clientkeys
extern void destroyKeys();
// exchange key on client side -> gets public server key 
extern bool exchangeOnClientSideKey(Network::CNetMSG* msg);
// exchange key on server side -> gets public client key 
extern bool exchangeOnServerSideKey(Network::CNetMSG* msg, unsigned int iClient);
// gets public key
extern ENGINE_API byte* getPublicKey(void);




/* check sum  */
extern ENGINE_API unsigned int net_uiCRC;

extern Network::NetData netDataGlobals;

/* server structure, active only on server computer */
extern Network::NetServer netMainServer;
/* client structure, active only on client computer */
extern Network::NetClient netMainClient;
/* main buffer data sended to client/server */
extern byte* pubSendingData;


extern ENGINE_API char *strAddressString;