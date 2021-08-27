#include "FatalErrorResponse.h"

#include "../../Globals.h"
#include "../../Utils/StringUtilities.h"

FatalErrorResponse::FatalErrorResponse(std::string errorMessage) : Response(ResponseType::FatalError)
{
	int sigLen = 0xdeadbeef;
	std::vector<unsigned char> encrypted = Globals::RSA->Encrypt(StringUtilities::StringToByteArray(errorMessage), &sigLen);

	AddString(std::to_string(sigLen));
	AddString(encrypted);
}
