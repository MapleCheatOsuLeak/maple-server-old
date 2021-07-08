#pragma once

#include "Response.h"
#include "../../Communication/MatchedClient.h"

class LoginResponse : public Response
{
public:
	LoginResponse(std::string hwid, std::string username, std::string password, MatchedClient* matchedClient);
};
