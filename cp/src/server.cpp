#include "server.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>

Server::Server() {
    if (pipe(pipeFirst) < 0 || pipe(pipeSecond) < 0) {
        perror("create pipe");
        exit(1);
    }
}

void Server::startGame() {
    pid_t pidFirst = fork();
    if (pidFirst == 0) {
        close(pipeFirst[0]);
        dup2(pipeFirst[1], STDOUT_FILENO);
        close(pipeFirst[1]);
        execl("./client", "./client", "1", NULL);
        perror("exec");
        exit(1);
    } else {
        pid_t pidSecond = fork();
        if (pidSecond == 0) {
            close(pipeSecond[0]);
            dup2(pipeSecond[1], STDOUT_FILENO);
            close(pipeSecond[1]);
            execl("./client", "./client", "1", NULL);
            perror("exec");
            exit(1);
        } else {
            wait(NULL);
            wait(NULL);
            char resultFirst[200];
            char resultSecond[200];
            int bytesReadFirst = read(pipeFirst[0], resultFirst, sizeof(resultFirst));
            int bytesReadSecond = read(pipeSecond[0], resultSecond, sizeof(resultSecond));
            if (bytesReadFirst > 0) {
                std::string player(resultFirst, bytesReadFirst);
                players[player] = std::make_pair(0, 0);
                std::cout << player + " is logged in" << std::endl;
            }
            if (bytesReadSecond > 0) {
                std::string player2(resultSecond, bytesReadSecond);
                players[player2] = std::make_pair(0, 0);
                std::cout << player2 + " is logged in" << std::endl;
            }
        }
    }
}

void Server::clearScreen() {
    pid_t pid = fork();
    if (pid == 0) {
        close(pipeFirst[0]);
        dup2(pipeFirst[1], STDOUT_FILENO);
        close(pipeFirst[1]);
        execl("./client", "./client", "3", NULL);
        perror("exec");
        exit(1);
    } else {
        wait(NULL);
        char result[200];
        int bytesRead = read(pipeFirst[0], result, sizeof(result));
        if (bytesRead > 0) {
            std::string status(result, bytesRead);
            if (status == "ok") {
                std::cout << "\x1B[2J\x1B[H";
            }
        }
    }
}

void Server::arrangeShips(matrix &map, const matrix &ships) {
    for (size_t i = 0; i < ships.size() - 1; ++i) {
        int sizeShip = ships[i][0];
        if (ships[i][1] == ships[i][3] && ships[i][2] == ships[i][4]) {
            int placeX = ships[i][1] - 1;
            int placeY = ships[i][2] - 1;
            map[placeX][placeY] = 1;
        } else if (ships[i][1] == ships[i][3]) {
            int minPlace = std::min(ships[i][2], ships[i][4]) - 1;
            int maxPlace = std::max(ships[i][2], ships[i][4]) - 1;
            int placeX = ships[i][1] - 1;
            for (int j = minPlace; j <= maxPlace; ++j) {
                map[placeX][j] = 1;
            }
        } else if (ships[i][2] == ships[i][4]) {
            int minPlace = std::min(ships[i][1], ships[i][3]) - 1;
            int maxPlace = std::max(ships[i][1], ships[i][3]) - 1;
            int placeY = ships[i][2] - 1;
            for (int j = minPlace; j <= maxPlace; ++j) {
                map[j][placeY] = 1;
            }
        }
    }
}

void Server::printMatrix(const matrix &matrix) {
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (matrix[i][j] == 3) {
                std::cout << "\u2715 ";
            } else if (matrix[i][j] == 2) {
                std::cout << "\u25A1\u0338 ";
            } else if (matrix[i][j] == 1) {
                std::cout << "\u25A1 ";
            } else {
                std::cout << "\u00B7 ";
            }
        }
        std::cout << std::endl;
    }
}

bool Server::isShipKilled(const matrix &map, const matrix &ships) {
    for (size_t i = 0; i < ships.size() - 1; ++i) {
        bool isKill = true;
        int sizeShip = ships[i][0];
        if (sizeShip == 1) {
            int placeX = ships[i][1] - 1;
            int placeY = ships[i][2] - 1;
            if (map[placeX][placeY] != 2) {
                isKill = false;
            }
        } else if (sizeShip == 2 || sizeShip == 3 || sizeShip == 4) {
            if (ships[i][1] == ships[i][3]) {
                int minPlace = std::min(ships[i][2], ships[i][4]) - 1;
                int maxPlace = std::max(ships[i][2], ships[i][4]) - 1;
                int placeX = ships[i][1] - 1;
                for (int j = minPlace; j <= maxPlace; ++j) {
                    if (map[placeX][j] != 2) {
                        isKill = false;
                        break;
                    }
                }
            } else if (ships[i][2] == ships[i][4]) {
                int minPlace = std::min(ships[i][1], ships[i][3]) - 1;
                int maxPlace = std::max(ships[i][1], ships[i][3]) - 1;
                int placeY = ships[i][2] - 1;
                for (int j = minPlace; j <= maxPlace; ++j) {
                    if (map[j][placeY] != 2) {
                        isKill = false;
                        break;
                    }
                }
            }
        }
        if (isKill) {
            return true;
        }
    }
    return false;
}

void Server::giveInfo(int x, int y, matrix &mapPlayer, matrix &mapEnemy, matrix &ships, int &countKillShips) {
    if (mapPlayer[x][y] == 1) {
        mapPlayer[x][y] = 2;
        mapEnemy[x][y] = 2;
        if (isShipKilled(mapPlayer, ships)) {
            ++countKillShips;
            std::cout << "Убил" << std::endl;
        } else {
            std::cout << "Ранил" << std::endl;
        }
    } else if (mapPlayer[x][y] == 0) {
        mapPlayer[x][y] = 3;
        mapEnemy[x][y] = 3;
        std::cout << "Мимо" << std::endl;
    } else {
        std::cout << "Ход в молоко" << std::endl;
    }
}

void Server::movePlayer(matrix &mapCurrent, matrix &mapPlayer, matrix &mapEnemy, const std::string &player, matrix &ships, int &countKillShips) {
    std::cout << player + " ходит" << std::endl;
    pid_t pid = fork();
    if (pid == 0) {
        close(pipeFirst[0]);
        dup2(pipeFirst[1], STDOUT_FILENO);
        close(pipeFirst[1]);
        execl("./client", "./client", "4", NULL);
        perror("exec");
        exit(1);
    } else {
        wait(NULL);
        char result[200];
        int bytesRead = read(pipeFirst[0], result, sizeof(result));
        if (bytesRead > 0) {
            std::stringstream ss(result);
            std::string line;
            std::getline(ss, line, '\n');
            std::istringstream iss(line);
            int x, y;
            iss >> x >> y;
            giveInfo(x, y, mapPlayer, mapEnemy, ships, countKillShips);
            std::cout << "Ваше поле:" << std::endl;
            printMatrix(mapCurrent);
            std::cout << std::endl;
            std::cout << "Поле противника:" << std::endl;
            printMatrix(mapEnemy);
        }
    }
}

void Server::endGame(const std::string& winner) {
    std::cout << "Игра окончена! Победитель: " << winner << std::endl;
}