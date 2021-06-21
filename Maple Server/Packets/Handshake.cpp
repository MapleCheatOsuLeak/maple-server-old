#include "Handshake.h"

std::string Handshake::ConstructPacket() const
{
	const std::string ivKey = std::string(matchedClient->aes->DumpIV().begin(), matchedClient->aes->DumpIV().end()) +
		std::string(matchedClient->aes->DumpKey().begin(), matchedClient->aes->DumpKey().end());

	const std::string rsaEncryptedIVKey = Globals::RSA->Encrypt(ivKey);

	return (std::to_string(rsaEncryptedIVKey.size())) + "|" + rsaEncryptedIVKey;
}

Handshake::Handshake(MatchedClient* mc)
{
	matchedClient = mc;
	constructedPacket = ConstructPacket();
}

std::string Handshake::GetPacket() const
{
	return constructedPacket;
}
