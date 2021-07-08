#pragma once

enum class RequestType : unsigned char
{
	Handshake = 0xA0,
	Login = 0xF3
};