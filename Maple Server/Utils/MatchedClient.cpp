#pragma once
#include "../TCP/client.h"
#include "../Crypto/Wrapper/AES.h"

class MatchedClient
{
public:
	Client client;
	std::string ip;
	AESWrapper* aes;

	MatchedClient(Client _client)
	{
		client = _client;
		ip = client.getIp();
		aes = new AESWrapper();
	}
};