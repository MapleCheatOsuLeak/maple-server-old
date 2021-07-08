#include "LoginResponse.h"

LoginResponse::LoginResponse(std::string hwid, std::string username, std::string password, MatchedClient* matchedClient) : Response(ResponseType::Login)
{
	std::cout << hwid << " | " << username << " | " << password << std::endl;
	unsigned char loginResult = 0x0; // 0x0 - success, 0x1 - incorrect credentials, 0x2 - hwid mismatch
	std::string sessionToken("qwertyuiop");
	std::string expiresAt("Never");

	std::vector<unsigned char> loginInfo;
	AddByte(loginResult, &loginInfo);
	AddString(sessionToken, &loginInfo);
	AddString(expiresAt, &loginInfo);

	AddString(matchedClient->AES.Encrypt(loginInfo));
}
