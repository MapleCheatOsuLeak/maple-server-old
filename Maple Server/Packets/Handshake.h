#pragma once
#include "../Utils/MatchedClient.cpp"
#include "../Globals.h"

class Handshake
{
private:
	MatchedClient* matchedClient;
	std::string constructedPacket;

	std::string ConstructPacket();
public:
	Handshake(MatchedClient* mc);

	std::string GetPacket() const;
};
