#pragma once

#include "../TCP/client.h"
#include "../Crypto/Wrapper/AES.h"

class MatchedClient
{
public:
	Client Client;
	std::string IP;
	AESWrapper AES;

	MatchedClient(::Client client)
	{
		Client = client;
		IP = client.getIp();
		AES = AESWrapper();
	}
};