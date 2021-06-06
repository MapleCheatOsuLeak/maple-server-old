#include "Handshake.h"

static std::string CreateHandshake(MatchedClient mc)
{
	RSAEncrypt rsa = RSAEncrypt();
	std::string ivkey = std::string(mc.aes.DumpIV().begin(), mc.aes.DumpIV().end()) + std::string(mc.aes.DumpKey().begin(), mc.aes.DumpKey().end());
	std::string rsaS = rsa.Encrypt(ivkey);

	return (std::to_string(rsaS.size())) + "|" + rsaS;
}