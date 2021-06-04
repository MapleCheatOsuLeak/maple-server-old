#ifndef LOGIN_H
#define LOGIN_H

#include <iostream>
#include <vector>
#include "../Utils/StringUtilities.cpp"
#include "../include/client.h"
#include <curl/curl.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <sys/socket.h>

class LoginPacket
{
public:
	std::string hwid;
	std::string username;
	std::string password;
	std::string ip;

	LoginPacket(std::string _hwid, std::string _username, std::string _password, std::string _ip)
	{
		hwid = _hwid;
		username = _username;
		password = _password;
		ip = _ip;
	}
};

#endif