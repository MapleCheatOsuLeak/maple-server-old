#pragma once

#include <vector>
#include <string>

class Login
{
private:
	std::string username;
	std::string password;
	std::string hwid;
	
	std::vector<unsigned char> constructedPacket;

	[[nodiscard]] std::vector<unsigned char> ConstructPacket() const;
public:
	Login(std::string username, std::string password, std::string hwid);

	[[nodiscard]] std::vector<unsigned char> GetPacket() const;
};
