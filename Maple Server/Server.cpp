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

class MatchedClient;
TcpServer server;
server_observer_t listener;

std::vector<MatchedClient*> matchedClients = std::vector<MatchedClient*>();

void onIncomingMsg2(const Client& client, const char* msg, size_t size)
{
	char* message = _strdup(msg);
	char type = message[0];
	memmove(message, message + 1, strlen(message)); /*Remove first character from string*/

	switch(type)
	{
	case 0x33:
		// Create new MatchedClient
		MatchedClient* mc = new MatchedClient(client);
		matchedClients.push_back(mc);
		break;
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
	std::vector<MatchedClient*> toRemove = std::vector<MatchedClient*>();
	//std::cout << "Client: " << client.getIp() << " disconnected: " << client.getInfoMessage() << std::endl;
	for(int i = 0; i < matchedClients.size(); i++)
		if (matchedClients[i]->ip == client.getIp())
			toRemove.push_back(matchedClients[i]);

	for (int i = 0; i < toRemove.size(); i++)
		matchedClients.erase(std::remove(matchedClients.begin(), matchedClients.end(), toRemove[i]), matchedClients.end());

	// after all, gc
	toRemove.clear();
}

void startListening()
{
	pipe_ret_t startRet = server.start(9999);
	if (startRet.success) {
		std::cout << "Server setup succeeded" << std::endl;
	} else {
		std::cout << "Server setup failed: " << startRet.msg << std::endl;
	}

	listener.incoming_packet_func = onIncomingMsg2;
	listener.disconnected_func = onClientDisconnected;
	listener.wantedIp = "";
	server.subscribe(listener);

	while (true) {
		Client client = server.acceptClient(0);
		if (client.isConnected()) {
			std::cout << "Got client with IP: " << client.getIp() << std::endl;
			server.printClients();
		}
		else {
			std::cout << "Accepting client failed: " << client.getInfoMessage() << std::endl;
		}
		Sleep(100); /*important, don't kill our vps cpu, we can rather wait*/
	}
}

int main()
{
    std::cout << "Hello World!\n"; 
}