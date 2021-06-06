// Maple Server.cpp : Defines the entry point for the application.
//

#include "Maple Server.h"
#include "src/Crypto/rsa.h"

TcpServer server;

server_observer_t observer1, observer2;
std::vector<MatchedClient> mclients = std::vector<MatchedClient>();

char* unconstchar(const char* s) {
	if (!s)
		return NULL;
	int i;
	char* res = NULL;
	res = (char*)malloc(strlen(s) + 1);
	if (!res) {
		fprintf(stderr, "Memory Allocation Failed! Exiting...\n");
		exit(EXIT_FAILURE);
	}
	else {
		for (i = 0; s[i] != '\0'; i++) {
			res[i] = s[i];
		}
		res[i] = '\0';
		return res;
	}
}

void onIncomingMsg2(Client& client, const char* msg, size_t size)
{
	std::string msgStr = msg;
	std::cout << "Observer2 got client msg: " << msgStr << std::endl;

	char* msgn = strdup(msg);
	char type = msgn[0];
	std::cout << type << std::endl;
	memmove(msgn, msgn + 1, strlen(msgn)); // remove first char
	switch (type) {
		//case 0x30: // Heartbeat
		//	break;
		case 0x31: // Login
		{
			LoginPacket lp = ConstructLoginPacket(msgn);
			HandleLogin(client, lp);
			break;
		}
		//case 0x32: // Request Maple
		//	break;
		case 0x33: // handshake
		{
			MatchedClient mc = MatchedClient(client);
			mclients.push_back(mc);
			
			std::string hs = CreateHandshake(mc);
			server.sendToClient(client, hs.c_str(), (size_t)hs.size());
			break;
		}
		default:
			break;
	}

	//cbmsg(client, msg);

	std::string replyMsg = "server got this msg: " + msgStr;
	server.sendToClient(client, msg, size);
}

void onClientDisconnected(const Client& client)
{
	std::cout << "Client: " << client.getIp() << " disconnected: " << client.getInfoMessage() << std::endl;
}

int main()
{
	/*pipe_ret_t startRet = server.start(9999);
	if (startRet.success) {
		std::cout << "Server setup succeeded" << std::endl;
	} else {
		std::cout << "Server setup failed: " << startRet.msg << std::endl;
		return EXIT_FAILURE;
	}

	observer2.incoming_packet_func = onIncomingMsg2;
	observer2.disconnected_func = onClientDisconnected;
	observer2.wantedIp = "";
	server.subscribe(observer2);

	while (1) {
		Client client = server.acceptClient(0);
		if (client.isConnected()) {
			std::cout << "Got client with IP: " << client.getIp() << std::endl;
			server.printClients();
		}
		else {
			std::cout << "Accepting client failed: " << client.getInfoMessage() << std::endl;
		}
		sleep(1);
	}*/
}
