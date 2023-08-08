#ifndef CLIENT_H
#define CLIENT_H

#include <cstdio>
#include <cctype>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>

#define IP "127.0.0.1"
#define PORT 8080
#define MAX_STR_LEN 64
#define PATTERN "0|2|4|6|8"
#define REPLACE "KB"

class Client {
private:
    int sock;
    struct sockaddr_in address;
    std::vector<std::string> buffer;
    std::mutex m;
    std::condition_variable condVar;
public:
    Client();
    ~Client();
    void connectToServer();
    void readData();
    void processingData();
    bool checkString(std::string& s);
    void start();
    void sendValue(int& v);
};

#endif