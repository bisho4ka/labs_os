#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <unordered_map>
#include <string>
#include <utility>

using matrix = std::vector<std::vector<int>>;

class Server {
public:
    Server();
    void startGame();
    void clearScreen();
    void arrangeShips(matrix &map, const matrix &ships);
    void printMatrix(const matrix &matrix);
    bool isShipKilled(const matrix &map, const matrix &ships);
    void giveInfo(int x, int y, matrix &mapPlayer, matrix &mapEnemy, matrix &ships, int &countKillShips);
    void movePlayer(matrix &mapCurrent, matrix &mapPlayer, matrix &mapEnemy, const std::string &player, matrix &ships, int &countKillShips);
    void endGame(const std::string& winner);

private:
    int pipeFirst[2];
    int pipeSecond[2];
    std::unordered_map<std::string, std::pair<int, int>> players;
};

#endif // SERVER_H