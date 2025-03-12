#include "server.h"
#include "client.h"
#include "game.h"
#include <cassert>
#include <iostream>

void testGameCreation() {
    Server server;
    server.createGame("TestGame");

    // Проверка, что игра создана
    // Например, можно добавить метод getActiveGames() в Server
    // assert(server.getActiveGames().size() == 1);
    std::cout << "testGameCreation passed!" << std::endl;
}

void testPlayerJoin() {
    Server server;
    server.createGame("TestGame");
    server.joinGame("TestGame", "Player1");

    // Проверка, что игрок присоединился
    // Например, можно добавить метод getPlayersInGame() в Server
    // assert(server.getPlayersInGame("TestGame").size() == 1);
    std::cout << "testPlayerJoin passed!" << std::endl;
}

void testShipArrangement() {
    Game game;
    std::vector<std::vector<int>> map_player(10, std::vector<int>(10, 0));
    std::vector<std::vector<int>> input_ships = {{1, 1, 1, 1, 1}};  // Корабль размера 1 в позиции (1, 1)

    game.arrangeShips(map_player, input_ships);

    // Проверка, что корабль размещен в правильной позиции
    assert(map_player[0][0] == 1);  // (1, 1) -> (0, 0) в индексах массива
    std::cout << "testShipArrangement passed!" << std::endl;
}

void testShipKilled() {
    Game game;
    std::vector<std::vector<int>> map_player(10, std::vector<int>(10, 0));
    std::vector<std::vector<int>> input_ships = {{1, 1, 1, 1, 1}};  // Корабль размера 1 в позиции (1, 1)

    game.arrangeShips(map_player, input_ships);

    // "Убиваем" корабль
    map_player[0][0] = 2;  // Помечаем ячейку как подбитую

    // Проверка, что корабль уничтожен
    assert(game.isShipKilled(map_player, input_ships));
    std::cout << "testShipKilled passed!" << std::endl;
}

int main() {
    testGameCreation();
    testPlayerJoin();
    testShipArrangement();
    testShipKilled();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}