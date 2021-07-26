#pragma once

#include "Response.h"
#include "../../Globals.h"
#include "../../Communication/MatchedClient.h"

class DllStreamResponse : public Response
{
public:
	DllStreamResponse(std::string session, MatchedClient* matchedClient);
};
