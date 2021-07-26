/*███▄ ▄███▓ ▄▄▄       ██▓███   ██▓    ▓█████   ██████ ▓█████  ██▀███   ██▒   █▓▓█████  ██▀███
  ▓██▒▀█▀ ██▒▒████▄    ▓██░  ██▒▓██▒    ▓█   ▀ ▒██    ▒ ▓█   ▀ ▓██ ▒ ██▒▓██░   █▒▓█   ▀ ▓██ ▒ ██▒
  ▓██    ▓██░▒██  ▀█▄  ▓██░ ██▓▒▒██░    ▒███   ░ ▓██▄   ▒███   ▓██ ░▄█ ▒ ▓██  █▒░▒███   ▓██ ░▄█ ▒
  ▒██    ▒██ ░██▄▄▄▄██ ▒██▄█▓▒ ▒▒██░    ▒▓█  ▄   ▒   ██▒▒▓█  ▄ ▒██▀▀█▄    ▒██ █░░▒▓█  ▄ ▒██▀▀█▄
  ▒██▒   ░██▒ ▓█   ▓██▒▒██▒ ░  ░░██████▒░▒████▒▒██████▒▒░▒████▒░██▓ ▒██▒   ▒▀█░  ░▒████▒░██▓ ▒██▒
  ░ ▒░   ░  ░ ▒▒   ▓▒█░▒▓▒░ ░  ░░ ▒░▓  ░░░ ▒░ ░▒ ▒▓▒ ▒ ░░░ ▒░ ░░ ▒▓ ░▒▓░   ░ ▐░  ░░ ▒░ ░░ ▒▓ ░▒▓░
  ░  ░      ░  ▒   ▒▒ ░░▒ ░     ░ ░ ▒  ░ ░ ░  ░░ ░▒  ░ ░ ░ ░  ░  ░▒ ░ ▒░   ░ ░░   ░ ░  ░  ░▒ ░ ▒░
  ░      ░     ░   ▒   ░░         ░ ░      ░   ░  ░  ░     ░     ░░   ░      ░░     ░     ░░   ░
  ░         ░  ░             ░  ░   ░  ░      ░     ░  ░   ░           ░     ░  ░   ░
  ░ */

#define CURL_STATICLIB
#include <iostream>
#include "TCP/pipe_ret_t.h"
#include "TCP/tcp_server.h"

#include "Packets/Responses/FatalErrorResponse.h"
#include "Packets/Responses/HandshakeResponse.h"
#include "Packets/Responses/LoginResponse.h"

#include "Communication/MatchedClient.h"
#include "Packets/Requests/Request.h"
#include "Utils/StringUtilities.h"
#include <curl/curl.h>

TcpServer server;
server_observer_t listener;

std::vector<MatchedClient*> matchedClients;

void onPacketReceive(const Client& client, const char* msg, size_t size)
{
	Request request = Request(msg, size);
	switch (request.Type)
	{
		case RequestType::Handshake:
		{
			auto mc = new MatchedClient(client);
			matchedClients.push_back(mc);

			std::cout << "mc instance created" << std::endl;

			HandshakeResponse handshakeResponse = HandshakeResponse(mc);
			server.sendToClient(client, &handshakeResponse.Data[0], handshakeResponse.Data.size());
				
			break;
		}
		case RequestType::Login:
		{
			MatchedClient* mc = nullptr;
			for (const auto& cli : matchedClients)
				if (cli->IP == client.getIp())
					mc = cli;

			if (mc == nullptr)
			{
				FatalErrorResponse fatalErrorResponse = FatalErrorResponse("Unknown session");
				server.sendToClient(client, &fatalErrorResponse.Data[0], fatalErrorResponse.Data.size());

				break;
			}

			//todo: i have no clue how to make this look better
			request.RawData[0].erase(request.RawData[0].begin());
			std::string decrypted = StringUtilities::ByteArrayToString(mc->AES.Decrypt(StringUtilities::StringToByteArray(request.RawData[0])));
				
			std::vector<std::string> loginData = StringUtilities::Split(decrypted);
			loginData[1].erase(loginData[1].begin());
			loginData[2].erase(loginData[2].begin());

			auto login = LoginResponse(loginData[0], loginData[1], loginData[2], mc);
			server.sendToClient(client, &login.Data[0], login.Data.size());
				
			break;
		}
		default:
			break;
	}
}

void onClientDisconnected(const Client& client)
{
	for (int i = 0; i < matchedClients.size(); i++)
	{
		if (matchedClients[i]->IP == client.getIp())
		{
			delete matchedClients[i];
			matchedClients.erase(matchedClients.begin() + i);
			
			break;
		}
	}
}

int main()
{
	pipe_ret_t startRet = server.start(9999);
	if (startRet.success)
	{
		std::cout << "Server setup succeeded" << std::endl;
	}
	else
	{
		std::cout << "Server setup failed: " << startRet.msg << std::endl;
	}

	listener.incoming_packet_func = onPacketReceive;
	listener.disconnected_func = onClientDisconnected;
	listener.wantedIp = "";
	
	server.subscribe(listener);

	// init curl
	curl_global_init(CURL_GLOBAL_ALL);
	
	while (true)
	{
		Client client = server.acceptClient(0);
		if (client.isConnected())
		{
			std::cout << "Got client with IP: " << client.getIp() << std::endl;
			server.printClients();
		}
		else
		{
			std::cout << "Accepting client failed: " << client.getInfoMessage() << std::endl;
		}
		Sleep(1); /*important, don't kill our vps cpu, we can rather wait*/
	}
}
