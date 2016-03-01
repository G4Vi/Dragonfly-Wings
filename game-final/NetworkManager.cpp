// NetworkManager.cpp
//
//
#include "NetworkManager.h"
#include "GameManager.h"
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>

using namespace df;

#include <iostream>

NetworkManager::NetworkManager(){};
NetworkManager::~NetworkManager(){};

bool NetworkManager::isValid(std::string event_type)const
{
    return true;
}
NetworkManager& NetworkManager::getInstance()
{
    static NetworkManager instance;
    return instance;
}

int NetworkManager::getServSock()
{
    int tcp_socket;
    struct addrinfo socket_info, *results;
    memset(&socket_info, 0, sizeof socket_info); //clear struct to be safe
    socket_info.ai_family = AF_UNSPEC; //compatible with IPV4 and 6
    socket_info.ai_socktype = SOCK_STREAM; //TCP
    socket_info.ai_flags  = AI_PASSIVE;

    //setup the socket
    int status = getaddrinfo(NULL,theSet->port.c_str(),&socket_info, &results);

    if(status != 0) //Check getaddrinfo worked before proceeding with bad information
    {
        printf("getaddrinfo failed!\n error: %s\n", gai_strerror(status));
        exit(1);
    }

    if ((tcp_socket = socket(results->ai_family, results->ai_socktype, results->ai_protocol)) == -1)
    {
        printf("Socket opening error\n");
        perror("socket");
        exit(1);
    }

    // set SO_REUSEADDR on a socket to true (1):
    int optval = 1;
    setsockopt(tcp_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

    if(bind(tcp_socket, results->ai_addr, results->ai_addrlen) < 0)
    {
        printf("Socket binding error\n");
        perror("bind");
        exit(1);
    }

    if (listen(tcp_socket, 20) != 0)
    {
        printf("Socket listen error\n%s\n", strerror(errno));
        exit(1);
    }

    freeaddrinfo(results); //Free linked list as its no longer used;
    return tcp_socket; 
}

int NetworkManager::accept2()
{
    struct sockaddr_in client_addr;
    socklen_t addrlen =sizeof(client_addr);
    // memset(m_message, 0, MESSAGE_LEN);
    // cout << "Listening ..." << endl;
    betterSock  = accept(sock,(struct sockaddr*)&client_addr, &addrlen); //accept connection
    if(betterSock == -1)
    {
        perror("accept");
        return -1;
    }
    return 0;
}
int NetworkManager::send2(void* buffer, int bytes)
{
    if (send(betterSock, buffer, bytes, MSG_NOSIGNAL)!= -1) //Send frame
    {
        return 0;
    }
    return -1;
}

int NetworkManager::receive(void* buffer, int nbytes, bool peak)
{
    int flags; 
    if(peak)
        flags = MSG_DONTWAIT | MSG_PEEK;
    else
        flags = MSG_DONTWAIT;

    return recv(betterSock, buffer,nbytes, flags);
}

int NetworkManager::isData() 
{
    char tempbuf[5];
    return  receive(tempbuf, 5, true);
}

int NetworkManager::getMessageCount()
{
    char tinybuf[3];
    recv(betterSock, tinybuf,2, MSG_DONTWAIT);
    tinybuf[2]='\0';
    return atoi(tinybuf);
}

int NetworkManager::getMessageLength()
{
    char tinybuf[3];
    recv(betterSock, tinybuf,2, MSG_DONTWAIT);
    tinybuf[2]='\0';
    return atoi(tinybuf);
}

int NetworkManager::getClientSock(void** results)
{
    int tcp_socket;
    struct addrinfo socket_info; 
    memset(&socket_info, 0, sizeof socket_info); //clear struct to be safe
    socket_info.ai_family = AF_UNSPEC; //compatible with IPV4 and 6
    socket_info.ai_socktype = SOCK_STREAM; //TCP
    socket_info.ai_flags  = AI_PASSIVE;

    //setup the socket
    int status = getaddrinfo(theSet->ipAddress.c_str(),theSet->port.c_str(),&socket_info, (struct addrinfo **) results);

    if(status != 0) //Check getaddrinfo worked before proceeding with bad information
    {
        printf("getaddrinfo failed!\n error: %s\n", gai_strerror(status));
        exit(1);
    }

    if ((tcp_socket = socket((* (struct addrinfo **)results)->ai_family,(* (struct addrinfo **)results)->ai_socktype,(*(struct addrinfo **)results)->ai_protocol)) == -1)
    {
        printf("Socket opening error\n");
        exit(1);
    }

    return tcp_socket;
}

int NetworkManager::connect2(void* connStuff)
{
    if (connect(betterSock , ((struct addrinfo*)connStuff)->ai_addr,((struct addrinfo*)connStuff)->ai_addrlen) != -1) //Check if connect succeeds
    {
        return 0;
    }
    else
    {
        perror("connect");
        return -1;
    }
}
int NetworkManager::startUp(Settings* info)
{
    theSet = info; 
    if(theSet->type == 0)
    {
        sock = getServSock();
        accept2();
    }
    else
    {
        struct addrinfo* results;
        //the casting dance begins here in order to pass addrinfo without the compiler trying to put addrinfo into the df namespace
        betterSock = getClientSock((void **)&results);
        if(connect2(results) == -1)
        {
            df::GameManager &game_manager = df::GameManager::getInstance();
            game_manager.setGameOver();
        }
    }
    return 0;
}

bool NetworkManager::isConnected() const
{
    if(betterSock > 0)
        return true;
    else
        return false;
}
void NetworkManager::shutDown()
{
    close(sock);
    close(betterSock);
}

