#include "Server.h"

Server::Server() {
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        std::perror("Opening socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *) &address, sizeof(address)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(sock, 1) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
}

Server::~Server() {
    close(sock);
}

void Server::start() {
    while(1) {
        getMessage();
    }
}

void Server::getMessage() {
    int fd = accept(sock, NULL, NULL);
    if (fd == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Client connected\n";
    
    while (1) {
        int bytesRead = recv(fd, buf, BUF_SIZE, 0);
        if (bytesRead <= 0) {
            break;
        }
        std::string message(buf);
        processingMessage(message);
    }

    close(fd);
}

void Server::processingMessage(std::string& s) {
    if (s.length() > 2 and std::atoi(s.c_str()) % MULTIPLICITY == 0) {
        std::cout << "Data received successfully. Get value: " << s << '\n';
        return;
    }
    std::cout << "Invalid data. Get value: " << s << '\n';
}