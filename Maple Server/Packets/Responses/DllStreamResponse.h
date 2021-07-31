#pragma once

#include "Response.h"
#include "../../Communication/MatchedClient.h"

enum class DllStreamResult : unsigned char
{
	Success = 0x0,
	NotSubscribed = 0x1
};

class DllStreamResponse : public Response
{
public:
	DllStreamResponse(int cheatID, MatchedClient* matchedClient);
};
