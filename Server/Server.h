#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 8080
#define BUF_SIZE 512
#define MULTIPLICITY 32

class Server {
private:
    int sock;
    struct sockaddr_in address;
    char buf[BUF_SIZE];
public:
    Server();
    ~Server();
    void start();
    void getMessage();
    void processingMessage(std::string& s);
};

#endif