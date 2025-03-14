#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PIPE_NAME "/tmp/battleship_pipe"

void sendMessage(const std::string& message) {
    int fd = open(PIPE_NAME, O_WRONLY);
    write(fd, message.c_str(), message.size() + 1);
    close(fd);
}

std::string receiveMessage() {
    int fd = open(PIPE_NAME, O_RDONLY);
    char buffer[256];
    read(fd, buffer, 256);
    close(fd);
    return std::string(buffer);
}

int main() {
    std::string login;
    std::cout << "Enter your login: ";
    std::cin >> login;

    while (true) {
        std::string action;
        std::cout << "Enter action (CREATE <game_name>, JOIN <game_name>, MOVE <game_name> <x> <y>, STATUS <game_name>, STATS): ";
        std::cin >> action;

        if (action == "CREATE" || action == "JOIN" || action == "STATUS") {
            std::string gameName;
            std::cin >> gameName;
            sendMessage(login + " " + action + " " + gameName);
            std::cout << receiveMessage() << std::endl;
        } else if (action == "MOVE") {
            std::string gameName;
            int x, y;
            std::cin >> gameName >> x >> y;
            sendMessage(login + " MOVE " + gameName + " " + std::to_string(x) + " " + std::to_string(y));
            std::cout << receiveMessage() << std::endl;
        } else if (action == "STATS") {
            sendMessage(login + " STATS");
            std::cout << receiveMessage() << std::endl;
        } else {
            std::cout << "Unknown action" << std::endl;
        }
    }

    return 0;
}