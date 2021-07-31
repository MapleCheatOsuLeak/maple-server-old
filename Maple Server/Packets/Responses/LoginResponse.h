#pragma once

#include "Response.h"
#include "../../Communication/MatchedClient.h"

enum class LoginResult : unsigned char
{
	Success = 0x0,
	IncorrectCredentials = 0x1,
	HWIDMismatch = 0x2,
	Banned = 0x3,
	InternalError = 0x4
};

class LoginResponse : public Response
{
public:
	LoginResponse(std::string hwid, std::string username, std::string password, MatchedClient* matchedClient);
};
