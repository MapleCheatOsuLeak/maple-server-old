#pragma once

#include "../TCP/client.h"
#include "../Crypto/Wrapper/AES.h"

class MatchedClient
{
public:
	// PC Info
	Client Client;
	std::string IP;
	AESWrapper AES;
	// Maple Info
	std::string HWID;
	std::string SessionToken;
	std::string ExpiresAt;

	MatchedClient(::Client client)
	{
		Client = client;
		IP = client.getIp();
		AES = AESWrapper();
	}
};