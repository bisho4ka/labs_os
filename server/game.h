#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <map>
#include <utility>

struct Game {
    std::string player1;
    std::string player2;
    std::vector<std::vector<char>> board1; // Доска игрока 1
    std::vector<std::vector<char>> board2; // Доска игрока 2
    bool player1Turn; // Чей ход (true - player1, false - player2)
    bool gameOver; // Игра завершена
};

class GameManager {
public:
    void createGame(const std::string& gameName, const std::string& player);
    void joinGame(const std::string& gameName, const std::string& player);
    bool makeMove(const std::string& gameName, const std::string& player, int x, int y);
    std::string getGameStatus(const std::string& gameName, const std::string& player);
    void recordWin(const std::string& player);
    void recordLoss(const std::string& player);
    int getStats(const std::string& player) const;

private:
    std::map<std::string, Game> games;
    std::map<std::string, int> stats; // логин -> количество побед

    bool checkWin(const std::vector<std::vector<char>>& board);
    void initializeBoard(std::vector<std::vector<char>>& board);
};

#endif // GAME_H