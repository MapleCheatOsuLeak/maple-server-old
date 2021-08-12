#pragma once

#include "Response.h"
#include "../../Communication/MatchedClient.h"

enum class LoginResult : unsigned char
{
	Success = 0x0,
	IncorrectCredentials = 0x1,
	HashMismatch = 0x2,
	HWIDMismatch = 0x3,
	Banned = 0x4,
	InternalError = 0x5
};

class LoginResponse : public Response
{
public:
	LoginResponse(std::string hwid, std::string hash, std::string username, std::string password, MatchedClient* matchedClient);
};
