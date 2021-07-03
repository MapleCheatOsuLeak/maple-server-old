#pragma once

#include "../Utils/MatchedClient.cpp"

#include <vector>
#include <string>

class Login
{
	MatchedClient* matchedClient;
	
	std::string hwid;
	std::string username;
	std::string password;
	
	std::vector<unsigned char> constructedPacket;

	[[nodiscard]] std::vector<unsigned char> ConstructPacket() const;
public:
	Login(MatchedClient* matchedClient, std::string hwid, std::string username, std::string password);

	[[nodiscard]] std::vector<unsigned char> GetPacket() const;
};
