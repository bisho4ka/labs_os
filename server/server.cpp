#include "game.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sstream>

#define PIPE_NAME "/tmp/battleship_pipe"

GameManager gameManager;

void handleClient(int fd) {
    char buffer[256];
    read(fd, buffer, 256);
    std::string message(buffer);

    std::istringstream iss(message);
    std::string action, player, gameName;
    iss >> player >> action;

    if (action == "CREATE") {
        iss >> gameName;
        gameManager.createGame(gameName, player);
        std::cout << "Game " << gameName << " created by " << player << std::endl;
        std::string response = "Game " + gameName + " created by " + player;
        write(fd, response.c_str(), response.size() + 1);
    } else if (action == "JOIN") {
        iss >> gameName;
        gameManager.joinGame(gameName, player);
        std::cout << "Player " << player << " joined game " << gameName << std::endl;
        std::string response = "Player " + player + " joined game " + gameName;
        write(fd, response.c_str(), response.size() + 1);
    } else if (action == "MOVE") {
        int x, y;
        iss >> gameName >> x >> y;
        bool success = gameManager.makeMove(gameName, player, x, y);
        std::string response = success ? "Move successful" : "Invalid move";
        write(fd, response.c_str(), response.size() + 1);
    } else if (action == "STATUS") {
        iss >> gameName;
        std::string status = gameManager.getGameStatus(gameName, player);
        write(fd, status.c_str(), status.size() + 1);
    } else if (action == "STATS") {
        int stats = gameManager.getStats(player);
        std::string response = "STATS " + std::to_string(stats);
        write(fd, response.c_str(), response.size() + 1);
    }

    close(fd);
}

int main() {
    mkfifo(PIPE_NAME, 0666);

    while (true) {
        int fd = open(PIPE_NAME, O_RDONLY);
        handleClient(fd);
    }

    return 0;
}