#include "Login.h"

std::vector<unsigned char> Login::ConstructPacket() const
{
	int loginResult = 0x0; // 0x0 - success, 0x1 - incorrect credentials, 0x2 - hwid mismatch
	std::string sessionToken("qwertyuiop");
	std::string expiresAt("Never");
	
	std::vector<unsigned char> loginInfo = std::vector<unsigned char>();
	
	loginInfo.push_back(loginResult);

	for (const auto& c : "0xdeadbeef")
		loginInfo.push_back(c);

	for (const auto& c : sessionToken)
		loginInfo.push_back(c);

	for (const auto& c : "0xdeadbeef")
		loginInfo.push_back(c);

	for (const auto& c : expiresAt)
		loginInfo.push_back(c);

	std::string aesEncryptedLoginInfo = matchedClient->aes->Encrypt(loginInfo);

	std::vector<unsigned char> returnValue = std::vector<unsigned char>();

	returnValue.push_back(0xF3); // login identifier

	for (const auto& c : "0xdeadbeef")
		returnValue.push_back(c);
	
	for (const auto& byte : aesEncryptedLoginInfo)
		returnValue.push_back(byte);

	return returnValue;
}

Login::Login(MatchedClient* matchedClient, std::string hwid, std::string username, std::string password)
{
	this->matchedClient = matchedClient;
	this->hwid = hwid;
	this->username = username;
	this->password = password;
	
	constructedPacket = ConstructPacket();
}

std::vector<unsigned char>Login::GetPacket() const
{
	return constructedPacket;
}
