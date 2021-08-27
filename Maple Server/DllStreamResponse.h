#pragma once

#include "Response.h"

class FatalErrorResponse : public Response
{
public:
	FatalErrorResponse(std::string errorMessage);
};
