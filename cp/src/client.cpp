#include "client.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

Client::Client() {
    // Инициализация клиента
}

std::string Client::getLogin() {
    std::cin >> login;
    return login;
}

std::string Client::getShipPositions() {
    std::string request, temp;
    for (size_t i = 0; i < 10; ++i) {
        std::getline(std::cin, temp);
        request += (temp + "\n");
    }
    return request;
}

std::string Client::getStatus() {
    std::cin >> status;
    return status;
}

std::string Client::getCoordinates() {
    int x, y;
    std::cin >> x >> y;
    --x;
    --y;
    return std::to_string(x) + " " + std::to_string(y);
}

std::string Client::getRequest() {
    std::cin >> request;
    return request;
}