#include "server.h"
#include <iostream>

int main() {
    Server server;
    server.start();

    // Пример создания игры и присоединения к ней
    server.createGame("SeaBattle");
    server.joinGame("SeaBattle", "Player1");

    server.stop();
    return 0;
}