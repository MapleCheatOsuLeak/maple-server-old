#pragma once

#include "Response.h"
#include "../../Communication/MatchedClient.h"

class HandshakeResponse : public Response
{
public:
	HandshakeResponse(MatchedClient* matchedClient);
};
