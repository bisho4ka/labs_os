#include <sys/wait.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include "include/utils.h"
#include "include/GameManager.h"

struct Game {
    pid_t clients[2];
    int pipe1_send[2];
    int pipe1_recv[2];
    int pipe2_send[2];
    int pipe2_recv[2];
    role_t roles[2];
    bool is_running;
};

std::unordered_map<std::string, Game> active_games;

void create_game(const std::string& game_name) {
    Game game;
    game.is_running = true;
    game.roles[0] = ROLE_DEFEND;
    game.roles[1] = ROLE_ATTACK;

    create_pipe(game.pipe1_send);
    create_pipe(game.pipe1_recv);
    create_pipe(game.pipe2_send);
    create_pipe(game.pipe2_recv);

    pid_t client1 = create_child();
    if (client1 == 0) {
        close(game.pipe1_recv[READ]);
        close(game.pipe1_send[WRITE]);
        close(game.pipe2_recv[READ]);
        close(game.pipe2_recv[WRITE]);
        close(game.pipe2_send[READ]);
        close(game.pipe2_send[WRITE]);

        // Запуск первого клиента в новом терминале (konsole)
        execl("/usr/bin/konsole", "konsole", "-e", "./client", std::to_string(game.pipe1_send[READ]).c_str(),
              std::to_string(game.pipe1_recv[WRITE]).c_str(), "0", game_name.c_str(), NULL);
    } else {
        pid_t client2 = create_child();
        if (client2 == 0) {
            close(game.pipe2_recv[READ]);
            close(game.pipe2_send[WRITE]);
            close(game.pipe1_recv[READ]);
            close(game.pipe1_recv[WRITE]);
            close(game.pipe1_send[READ]);
            close(game.pipe1_send[WRITE]);

            // Запуск второго клиента в новом терминале (konsole)
            execl("/usr/bin/konsole", "konsole", "-e", "./client", std::to_string(game.pipe2_send[READ]).c_str(),
                  std::to_string(game.pipe2_recv[WRITE]).c_str(), "1", game_name.c_str(), NULL);
        } else {
            game.clients[0] = client1;
            game.clients[1] = client2;
            active_games[game_name] = game;
            std::cout << "[INFO]: Game '" << game_name << "' created with PIDs " << client1 << " and " << client2 << std::endl;
        }
    }
}

void join_game(const std::string& game_name) {
    if (active_games.find(game_name) == active_games.end()) {
        std::cerr << "[ERROR]: Game '" << game_name << "' not found!" << std::endl;
        return;
    }

    Game& game = active_games[game_name];
    char buf[2];
    read(game.pipe1_recv[READ], buf, 2);
    if (buf[1] == 'r') std::cout << "[INFO]: Client 1 is ready -- OK" << std::endl;
    read(game.pipe2_recv[READ], buf, 2);
    if (buf[1] == 'r') std::cout << "[INFO]: Client 2 is ready -- OK" << std::endl;

    while (game.is_running) {
        if (game.roles[0] == ROLE_ATTACK) {
            read(game.pipe1_recv[READ], buf, 2);
            write(game.pipe2_send[WRITE], buf, 2);
            read(game.pipe2_recv[READ], buf, 2);

            if (buf[1] == 'h') std::cout << "Client 1 hit" << std::endl;
            else if (buf[1] == 'm') {
                std::cout << "Client 1 missed" << std::endl;
                game.roles[0] = ROLE_DEFEND;
                game.roles[1] = ROLE_ATTACK;
            } else if (buf[1] == 'c') {
                std::cout << "Client 1 won" << std::endl;
                game.is_running = false;
            }

            write(game.pipe1_send[WRITE], buf, 2);
        } else {
            read(game.pipe2_recv[READ], buf, 2);
            write(game.pipe1_send[WRITE], buf, 2);
            read(game.pipe1_recv[READ], buf, 2);

            if (buf[1] == 'h') std::cout << "Client 2 hit" << std::endl;
            else if (buf[1] == 'm') {
                std::cout << "Client 2 missed" << std::endl;
                game.roles[1] = ROLE_DEFEND;
                game.roles[0] = ROLE_ATTACK;
            } else if (buf[1] == 'c') {
                std::cout << "Client 2 won" << std::endl;
                game.is_running = false;
            }

            write(game.pipe2_send[WRITE], buf, 2);
        }
    }

    waitpid(game.clients[0], NULL, 0);
    waitpid(game.clients[1], NULL, 0);

    close(game.pipe1_recv[READ]);
    close(game.pipe1_send[WRITE]);
    close(game.pipe2_recv[READ]);
    close(game.pipe2_send[WRITE]);

    active_games.erase(game_name);
}

int main() {
    std::cout << "=== BATTLESHIPS GAME SERVER ===" << std::endl;
    std::cout << "PID [" << getpid() << "]" << std::endl;

    std::string command;
    while (true) {
        std::cout << "Enter command (create/join/exit): ";
        std::cin >> command;

        if (command == "create") {
            std::string game_name;
            std::cout << "Enter game name: ";
            std::cin >> game_name;
            create_game(game_name);
        } else if (command == "join") {
            std::string game_name;
            std::cout << "Enter game name: ";
            std::cin >> game_name;
            join_game(game_name);
        } else if (command == "exit") {
            break;
        } else {
            std::cerr << "Unknown command!" << std::endl;
        }
    }

    return EXIT_SUCCESS;
}