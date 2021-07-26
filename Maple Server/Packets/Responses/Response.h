#pragma once

#include <string>
#include <vector>

#include "ResponseType.h"
#include "../../Utils/StringUtilities.h"
#include <curl/curl.h>

class Response
{
public:
	
	ResponseType Type;
	std::vector<unsigned char> Data; //type | data size | data

	Response(ResponseType type);

	void AddDelimiter(std::vector<unsigned char>* dest);
	void AddByte(unsigned char byte, std::vector<unsigned char>* dest);
	void AddString(std::string str, std::vector<unsigned char>* dest);
	void AddString(std::vector<unsigned char> str, std::vector<unsigned char>* dest);
	void AddByte(unsigned char byte);
	void AddString(std::string str);
	void AddString(std::vector<unsigned char> str);
};
