#ifndef MATCHEDCLIENT_CPP
#define MATCHEDCLIENT_CPP

#include "../include/client.h"
#include "../Crypto/Wrapper/AES.h"

class MatchedClient
{
public:
	Client client;
	AES aes;

	MatchedClient(Client& _client) : client(Client())
	{
		client = _client;
		aes = AES();
	}
};

#endif