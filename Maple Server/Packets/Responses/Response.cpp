#include "Response.h"

Response::Response(ResponseType type)
{
	Type = type;

	AddByte(static_cast<unsigned char>(Type));
}

void Response::AddDelimiter(std::vector<unsigned char>* dest)
{
	for (const auto& c : "0xdeadbeef")
		dest->push_back(c);
}

void Response::AddByte(unsigned char byte, std::vector<unsigned char>* dest)
{
	if (!dest->empty())
		AddDelimiter(dest);

	dest->push_back(byte);
}

void Response::AddString(std::string str, std::vector<unsigned char>* dest)
{
	if (!dest->empty())
		AddDelimiter(dest);

	for (const auto& c : str)
		dest->push_back(c);
}

void Response::AddString(std::vector<unsigned char> str, std::vector<unsigned char>* dest)
{
	if (!dest->empty())
		AddDelimiter(dest);

	for (const auto& c : str)
		dest->push_back(c);
}

void Response::AddByte(unsigned char byte)
{
	AddByte(byte, &Data);
}

void Response::AddString(std::string str)
{
	AddString(str, &Data);
}

void Response::AddString(std::vector<unsigned char> str)
{
	AddString(str, &Data);
}
