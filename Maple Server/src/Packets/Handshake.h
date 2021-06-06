#ifndef HANDSHAKE_H
#define HANDSHAKE_H

#include <string>
#include "../Utils/StringUtilities.cpp"
#include "../include/client.h"
#include <curl/curl.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include "../Utils/MatchedClient.cpp"
#include <sys/socket.h>
#include "../Crypto/Wrapper/RSAEncrypt.h"

class HandshakePacket {
public:
	std::string hwid;
	std::string encryptedString;

	//CreateHandshake(MatchedClient mc);
};

#endif