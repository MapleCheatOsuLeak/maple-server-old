#include "HandshakeResponse.h"

#include "../../Globals.h"
#include "../../Utils/StringUtilities.h"

HandshakeResponse::HandshakeResponse(MatchedClient* matchedClient) : Response(ResponseType::Handshake)
{
	int sigLen = 0xdeadbeef;
	std::vector<unsigned char> encrypted = Globals::RSA->Encrypt(StringUtilities::StringToByteArray(StringUtilities::ByteArrayToString(matchedClient->AES.DumpIV()) + StringUtilities::ByteArrayToString(matchedClient->AES.DumpKey())), &sigLen);

	AddString(std::to_string(sigLen));
	AddString(encrypted);
}
