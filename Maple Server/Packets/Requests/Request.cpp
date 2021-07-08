#include "Request.h"

#include "../../Utils/StringUtilities.h"

Request::Request(const char* msg, size_t size)
{
	RawRequest = msg;
	RequestSize = size;
	Type = static_cast<RequestType>(RawRequest[0]);

	RawData = StringUtilities::Split(std::string(RawRequest, RequestSize));
	RawData.erase(RawData.begin());
}
