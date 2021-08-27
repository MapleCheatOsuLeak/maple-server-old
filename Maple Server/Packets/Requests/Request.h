#pragma once

#include <string>
#include <vector>

#include "RequestType.h"
#include "../../Communication/MatchedClient.h"

class Request
{
public:
	const char* RawRequest;
	size_t RequestSize;
	RequestType Type;
	std::vector<std::string> RawData;

	Request(const char* msg, size_t size);
};
