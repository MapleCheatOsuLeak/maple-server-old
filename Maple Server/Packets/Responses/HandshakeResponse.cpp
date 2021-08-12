#include "HandshakeResponse.h"

#include "../../Globals.h"
#include "../../Utils/StringUtilities.h"
#include <chrono>

HandshakeResponse::HandshakeResponse(MatchedClient* matchedClient) : Response(ResponseType::Handshake)
{
	std::chrono::milliseconds msEpoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

	long long xord = (msEpoch.count() / 2) ^ 0xDA;

	int sigLen = 0xdeadbeef;
	std::vector<unsigned char> encrypted = Globals::RSA->Encrypt(StringUtilities::StringToByteArray(StringUtilities::ByteArrayToString(matchedClient->AES.DumpIV()) + StringUtilities::ByteArrayToString(matchedClient->AES.DumpKey()) + std::to_string(xord)), &sigLen);

	AddString(std::to_string(sigLen));
	AddString(encrypted);
}
