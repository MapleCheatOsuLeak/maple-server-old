#pragma once

#include "Response.h"
#include "../../Communication/MatchedClient.h"

enum class HeartbeatResult : unsigned char
{
	Success = 0x0,
	InvalidSession = 0x2,
	InternalError = 0x3
};

class HeartbeatResponse : public Response
{
public:
	HeartbeatResponse(std::string sessionID, MatchedClient* matchedClient);
};