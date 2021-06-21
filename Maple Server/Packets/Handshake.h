#pragma once
#include "../Globals.h"
#include "../Utils/MatchedClient.cpp"

class Handshake
{
private:
	MatchedClient* matchedClient;
	std::string constructedPacket;

	[[nodiscard]] std::string ConstructPacket() const;
public:
	Handshake(MatchedClient* mc);

	[[nodiscard]] std::string GetPacket() const;
};
