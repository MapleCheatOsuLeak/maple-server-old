#include "Handshake.h"

std::vector<unsigned char> Handshake::ConstructPacket() const
{
	std::vector<unsigned char> iv = matchedClient->aes->DumpIV();
	std::vector<unsigned char> key = matchedClient->aes->DumpKey();

	std::vector<unsigned char> ivKey = std::vector<unsigned char>();
	
	for (const auto& ivVal : iv)
		ivKey.push_back(ivVal);
	
	for (const auto& keyVal : key)
		ivKey.push_back(keyVal);

	int sigLen = 0xdeadbeef;
	
	std::vector<unsigned char> rsaEncryptedIVKey = Globals::RSA->Encrypt(ivKey, &sigLen);

	std::cout << std::to_string(sigLen) << std::endl;

	std::vector<unsigned char> returnValue = std::vector<unsigned char>();

	// TODO: make a wrapper for this, this is ugly!!
	// macro, template, anything would do
	returnValue.push_back(0xA0); // heart beat identifier
	
	for (const auto& c : "0xdeadbeef")
		returnValue.push_back(c);
	
	for (const auto& c : std::to_string(sigLen))
		returnValue.push_back(c);
	
	for (const auto& c : "0xdeadbeef")
		returnValue.push_back(c);
	
	for (const auto& byte : rsaEncryptedIVKey)
		returnValue.push_back(byte);

	return returnValue;
	
	//return (std::to_string(rsaEncryptedIVKey.size())) + "0xdeadbeef" + rsaEncryptedIVKey;
}

Handshake::Handshake(MatchedClient* mc)
{
	matchedClient = mc;
	constructedPacket = ConstructPacket();
}

std::vector<unsigned char>Handshake::GetPacket() const
{
	return constructedPacket;
}
