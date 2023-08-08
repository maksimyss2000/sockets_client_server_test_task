#include "Client.h"

Client::Client() {
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    int res = inet_pton(AF_INET, IP, &address.sin_addr);
    if (res == 0) {
        std::cout << "inet_pton failed: not a valid network address\n";
        exit(EXIT_FAILURE);
    } else if (res == -1) {
        std:perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }
    connectToServer();
}

Client::~Client() {
    close(sock);
}

void Client::connectToServer() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sock < 0) {
        std::perror("Error opening socket");
        exit(EXIT_FAILURE);
    }

    while (connect(sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }

    std::cout << "Connected to server\n";
}

void Client::readData() {
    while (1) { 
        std::string data;
        getline(std::cin, data);
        if (!checkString(data)) {
            std::cout << "String is incorrect\n";
            continue;
        }

        std::sort(data.begin(), data.end(), [](const auto& a, const auto& b) {
            return a > b;
        });

        std::unique_lock<std::mutex> locker(m);
        buffer.push_back(std::regex_replace(data, std::regex(PATTERN), REPLACE));
        condVar.notify_one();
    }
}

bool Client::checkString(std::string& s) {
    if (s.length() > MAX_STR_LEN || !s.length()) {
        return false;
    }

    for (auto& sym: s) {
        if (!std::isdigit(sym)) {
            return false;
        }
    }

    return true;
}

void Client::processingData() {
    while (1) {
        std::unique_lock<std::mutex> locker(m);
        condVar.wait(locker, [&](){
            return buffer.size();
        });
        auto message = buffer.back();
        std::cout << "Transformed message: " << message << '\n';
        int sum = 0;
        for (auto& sym: message) {
            if (!std::isdigit(sym)) {
                continue;
            }
            sum += sym - '0';
        }
        buffer.pop_back();
        locker.unlock();
        sendValue(sum);
    }
}

void Client::start() {
    std::thread t1(&Client::readData, this);
    std::thread t2(&Client::processingData, this);

    t1.join();
    t2.join();
}

void Client::sendValue(int& v) {
    std::string s = std::to_string(v);
    int total = 0;
    int len = s.length() + 1;
    while (total < len) {
        int sendBytes = send(sock, s.c_str() + total, len - total, MSG_NOSIGNAL);
        if (errno == 32) {
            close(sock);
            connectToServer();
            continue;
        }
        if (sendBytes == -1) {
            continue;
        }
        total += sendBytes;
    }
}