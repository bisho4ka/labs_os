#include "client.h"
#include <iostream>
#include <sstream>
#include <unistd.h>

Client::Client(int state) : state(state) {
    if (pipe(pipefd) < 0) {
        perror("pipe");
        exit(1);
    }
}

std::string Client::getLogin() {
    std::string login;
    std::cin >> login;
    return login;
}

std::vector<std::vector<int>> Client::getShips() {
    std::vector<std::vector<int>> ships;
    std::string line;
    for (int i = 0; i < 10; ++i) {
        std::getline(std::cin, line);
        std::istringstream iss(line);
        std::vector<int> ship;
        int number;
        while (iss >> number) {
            ship.push_back(number);
        }
        ships.push_back(ship);
    }
    return ships;
}

std::string Client::getStatus() {
    std::string status;
    std::cin >> status;
    return status;
}

std::pair<int, int> Client::getCoords() {
    int x, y;
    std::cin >> x >> y;
    return {x - 1, y - 1};
}

void Client::sendMessage(const std::string& message) {
    write(pipefd[1], message.c_str(), message.size() + 1);
}

std::string Client::receiveMessage() {
    char buffer[256];
    int n = read(pipefd[0], buffer, sizeof(buffer));
    if (n > 0) {
        return std::string(buffer, n);
    }
    return "";
}