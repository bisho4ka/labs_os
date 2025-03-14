#include "game.h"
#include <iostream>

void GameManager::createGame(const std::string& gameName, const std::string& player) {
    Game newGame;
    newGame.player1 = player;
    newGame.player2 = "";
    newGame.board1 = std::vector<std::vector<char>>(10, std::vector<char>(10, ' '));
    newGame.board2 = std::vector<std::vector<char>>(10, std::vector<char>(10, ' '));
    newGame.player1Turn = true;
    newGame.gameOver = false;
    initializeBoard(newGame.board1); // Расставляем корабли для игрока 1
    games[gameName] = newGame;
}

void GameManager::joinGame(const std::string& gameName, const std::string& player) {
    if (games.find(gameName) != games.end()) {
        games[gameName].player2 = player;
        initializeBoard(games[gameName].board2); // Расставляем корабли для игрока 2
    }
}

bool GameManager::makeMove(const std::string& gameName, const std::string& player, int x, int y) {
    if (games.find(gameName) == games.end()) return false; // Игра не найдена

    Game& game = games[gameName];
    if (game.gameOver) return false; // Игра уже завершена

    // Проверяем, чей ход
    if ((game.player1Turn && game.player1 != player) || (!game.player1Turn && game.player2 != player)) {
        return false; // Не ход игрока
    }

    // Проверяем корректность координат
    if (x < 0 || x >= 10 || y < 0 || y >= 10) {
        return false; // Некорректные координаты
    }

    // Получаем доску противника
    std::vector<std::vector<char>>& targetBoard = game.player1Turn ? game.board2 : game.board1;

    // Проверяем, что клетка еще не была атакована
    if (targetBoard[x][y] != ' ' && targetBoard[x][y] != 'S') {
        return false; // Клетка уже атакована
    }

    // Выполняем ход
    if (targetBoard[x][y] == 'S') {
        targetBoard[x][y] = 'X'; // Попадание
    } else {
        targetBoard[x][y] = 'O'; // Промах
    }

    // Проверяем, завершена ли игра
    if (checkWin(targetBoard)) {
        game.gameOver = true;
        if (game.player1Turn) {
            recordWin(game.player1);
            recordLoss(game.player2);
        } else {
            recordWin(game.player2);
            recordLoss(game.player1);
        }
    } else {
        game.player1Turn = !game.player1Turn; // Переход хода
    }

    return true;
}

std::string GameManager::getGameStatus(const std::string& gameName, const std::string& player) {
    if (games.find(gameName) == games.end()) return "Game not found";

    Game& game = games[gameName];
    if (game.gameOver) {
        return "Game over. Winner: " + (game.player1Turn ? game.player1 : game.player2);
    } else {
        return "Current turn: " + (game.player1Turn ? game.player1 : game.player2);
    }
}

void GameManager::recordWin(const std::string& player) {
    stats[player]++;
}

void GameManager::recordLoss(const std::string& player) {
    stats[player]--;
}

int GameManager::getStats(const std::string& player) const {
    if (stats.find(player) != stats.end()) {
        return stats.at(player);
    }
    return 0;
}

bool GameManager::checkWin(const std::vector<std::vector<char>>& board) {
    for (const auto& row : board) {
        for (char cell : row) {
            if (cell == 'S') return false; // Есть непотопленные корабли
        }
    }
    return true; // Все корабли уничтожены
}

void GameManager::initializeBoard(std::vector<std::vector<char>>& board) {
    // Простая расстановка кораблей (для примера)
    board[0][0] = 'S'; // Корабль на позиции (0, 0)
    board[1][1] = 'S'; // Корабль на позиции (1, 1)
    board[2][2] = 'S'; // Корабль на позиции (2, 2)
}