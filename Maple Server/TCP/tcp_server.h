#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <thread>
#include <functional>
#include <cstring>
#include <errno.h>
#include <iostream>
#include "client.h"
#include "server_observer.h"
#include "pipe_ret_t.h"
#pragma comment(lib, "Ws2_32.lib")
#include <io.h>
#include <process.h>
#include <WS2tcpip.h>
#include <winsock.h>
#include <WinSock2.h>
typedef unsigned int uint;

#define MAX_PACKET_SIZE 4096

class TcpServer
{
private:
    unsigned int m_sockfd;
    struct sockaddr_in m_serverAddress;
    struct sockaddr_in m_clientAddress;
    fd_set m_fds;
    std::vector<Client> m_clients;
    std::vector<server_observer_t> m_subscibers;
    std::thread * threadHandle;

    void publishClientMsg(const Client & client, const char * msg, size_t msgSize);
    void publishClientDisconnected(const Client & client);
    void receiveTask(/*void * context*/);
public:
    pipe_ret_t start(int port);
    Client acceptClient(uint timeout);
    bool deleteClient(Client & client);
    void subscribe(const server_observer_t & observer);
    void unsubscribeAll();
    pipe_ret_t sendToAllClients(unsigned  char * msg, size_t size);
    pipe_ret_t sendToClient(const Client & client, unsigned char * msg, size_t size);
    pipe_ret_t finish();
    void printClients();
};