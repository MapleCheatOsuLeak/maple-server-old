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


#include <iostream>
#include "TCP/pipe_ret_t.h"
#include "TCP/tcp_server.h"
#include "Utils/MatchedClient.cpp"
#include "Packets/Handshake.h"

class MatchedClient;
TcpServer server;
server_observer_t listener;

std::vector<MatchedClient*> matchedClients = std::vector<MatchedClient*>();

void onPacketReceive(const Client& client, const char* msg, size_t size)
{
	char* message = _strdup(msg);
	unsigned char type = message[0];
	memmove(message, message + 1, strlen(message)); /*Remove first character from string*/
	std::cout << type << std::endl;

	switch (type)
	{
	case 0xA0:
		{
			/*Handshake*/
			auto mc = new MatchedClient(client);
			matchedClients.push_back(mc);
			std::cout << "mc instance created" << std::endl;

			auto hs = Handshake(mc);
			std::vector<unsigned char> packet = hs.GetPacket();
			//std::cout << packet << std::endl;

			server.sendToClient(client, &packet[0], packet.size());
			break;
		}
	case 0xF3:
		{
			MatchedClient* mc = nullptr;
			for (const auto& cli : matchedClients)
				if (cli->ip == client.getIp())
					mc = cli;

			if (mc == nullptr)
			{
				std::vector<unsigned char> packet = std::vector<unsigned char>();
				for (const auto& c : "Unknown session")
					packet.push_back(c);
				server.sendToClient(client, &packet[0], packet.size());
			}

			std::vector<unsigned char> packet = std::vector<unsigned char>();
			for (int i = 0; i < strlen(message); i++)
				packet.push_back(message[i]);
			
			std::vector<unsigned char> decrypted = mc->aes->Decrypt(packet);
		}
	default:
		break;
	}


	//std::string msgStr = msg;
	//std::cout << "Observer2 got client msg: " << msgStr << std::endl;

	//char* msgn = strdup(msg);
	//char type = msgn[0];
	//std::cout << type << std::endl;
	//memmove(msgn, msgn + 1, strlen(msgn)); // remove first char
	//switch (type) {
	//	//case 0x30: // Heartbeat
	//	//	break;
	//case 0x31: // Login
	//{
	//	LoginPacket lp = ConstructLoginPacket(msgn);
	//	HandleLogin(client, lp);
	//	break;
	//}
	////case 0x32: // Request Maple
	////	break;
	//case 0x33: // handshake
	//{
	//	MatchedClient mc = MatchedClient(client);
	//	mclients.push_back(mc);

	//	std::string hs = CreateHandshake(mc);
	//	server.sendToClient(client, hs.c_str(), (size_t)hs.size());
	//	break;
	//}
	//default:
	//	break;
	//}

	////cbmsg(client, msg);

	//std::string replyMsg = "server got this msg: " + msgStr;
	//server.sendToClient(client, msg, size);
}

void onClientDisconnected(const Client& client)
{
	//std::vector<MatchedClient*> toRemove = std::vector<MatchedClient*>();
	//for(int i = 0; i < matchedClients.size(); i++)
	//	if (matchedClients[i]->ip == client.getIp())
	//		toRemove.push_back(matchedClients[i]);

	//for (int i = 0; i < toRemove.size(); i++)
	//	matchedClients.erase(std::remove(matchedClients.begin(), matchedClients.end(), toRemove[i]), matchedClients.end());

	//// after all, gc
	//toRemove.clear();
}

void startListening()
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
	// Start the server
	//CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(startListening), nullptr, 0, nullptr);
	//system("pause");
}
