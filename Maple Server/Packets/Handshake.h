#pragma once
#include "../Globals.h"
#include "../Utils/MatchedClient.cpp"

class Handshake
{
private:
	MatchedClient* matchedClient;
	std::vector<unsigned char> constructedPacket;

	[[nodiscard]] std::vector<unsigned char> ConstructPacket() const;
public:
	Handshake(MatchedClient* mc);

	[[nodiscard]] std::vector<unsigned char> GetPacket() const;
};
