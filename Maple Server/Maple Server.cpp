// Maple Server.cpp : Defines the entry point for the application.
//

#include "Maple Server.h"

TcpServer server;

server_observer_t observer1, observer2;

void onIncomingMsg2(const Client& client, const char* msg, size_t size)
{
	std::string msgStr = msg;
	std::cout << "Observer2 got client msg: " << msgStr << std::endl;

	std::string replyMsg = "server got this msg: " + msgStr;
	server.sendToClient(client, msg, size);
}

void onClientDisconnected(const Client& client)
{
	std::cout << "Client: " << client.getIp() << " disconnected: " << client.getInfoMessage() << std::endl;
}

int main()
{
	pipe_ret_t startRet = server.start(9999);
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
	}
}
