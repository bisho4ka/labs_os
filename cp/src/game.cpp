#include "game.h"

Game::Game() {
    // Инициализация логики игры
}

void Game::arrangeShips(std::vector<std::vector<int>>& map_player, const std::vector<std::vector<int>>& input_ships) {
    for (size_t i = 0; i < input_ships.size() - 1; ++i) {
        int size_ship = input_ships[i][0];
        switch (size_ship) {
            case 1:
                if (input_ships[i][1] == input_ships[i][3] && input_ships[i][2] == input_ships[i][4]) {
                    int placeX = input_ships[i][1] - 1;
                    int placeY = input_ships[i][2] - 1;
                    map_player[placeX][placeY] = 1;
                }
                break;
            case 2:
            case 3:
            case 4:
                if (input_ships[i][1] == input_ships[i][3]) {
                    int min_place = std::min(input_ships[i][2], input_ships[i][4]) - 1;
                    int max_place = std::max(input_ships[i][2], input_ships[i][4]) - 1;
                    int placeX = input_ships[i][1] - 1;
                    for (size_t j = min_place; j <= max_place; ++j) {
                        map_player[placeX][j] = 1;
                    }
                }
                if (input_ships[i][2] == input_ships[i][4]) {
                    int min_place = std::min(input_ships[i][1], input_ships[i][3]) - 1;
                    int max_place = std::max(input_ships[i][1], input_ships[i][3]) - 1;
                    int placeY = input_ships[i][2] - 1;
                    for (size_t j = min_place; j <= max_place; ++j) {
                        map_player[j][placeY] = 1;
                    }
                }
                break;
            default:
                break;
        }
    }
}

bool Game::isShipKilled(std::vector<std::vector<int>>& map_player, std::vector<std::vector<int>>& input_ships) {
    for (size_t i = 0; i < input_ships.size() - 1; ++i) {
        bool isKill = true;
        int size_ship = input_ships[i][0];
        int placeX, placeY;
        switch (size_ship) {
            case 1:
                placeX = input_ships[i][1] - 1;
                placeY = input_ships[i][2] - 1;
                if (map_player[placeX][placeY] == 2) {
                    input_ships.erase(input_ships.begin() + i);
                    return true;
                }
                break;
            case 2:
            case 3:
            case 4:
                if (input_ships[i][1] == input_ships[i][3]) {
                    int min_place = std::min(input_ships[i][2], input_ships[i][4]) - 1;
                    int max_place = std::max(input_ships[i][2], input_ships[i][4]) - 1;
                    int placeX = input_ships[i][1] - 1;
                    size_t j;
                    for (j = min_place; j <= max_place; ++j) {
                        if (map_player[placeX][j] != 2) {
                            isKill = false;
                            break;
                        }
                    }
                    if (isKill) {
                        input_ships.erase(input_ships.begin() + i);
                        return true;
                    }
                }
                if (input_ships[i][2] == input_ships[i][4]) {
                    int min_place = std::min(input_ships[i][1], input_ships[i][3]) - 1;
                    int max_place = std::max(input_ships[i][1], input_ships[i][3]) - 1;
                    int placeY = input_ships[i][2] - 1;
                    size_t j;
                    for (j = min_place; j <= max_place; ++j) {
                        if (map_player[j][placeY] != 2) {
                            isKill = false;
                            break;
                        }
                    }
                    if (isKill) {
                        input_ships.erase(input_ships.begin() + i);
                        return true;
                    }
                }
                break;
            default:
                return false;
                break;
        }
    }
    return false;
}

void Game::giveInfo(int x, int y, std::vector<std::vector<int>>& map_player, std::vector<std::vector<int>>& map_enemy, std::vector<std::vector<int>>& input_ships, int& count_kill_ships) {
    if (map_player[x][y] == 1) {
        map_player[x][y] = 2;
        map_enemy[x][y] = 2;
        if (isShipKilled(map_player, input_ships)) {
            ++count_kill_ships;
            std::cout << "убил" << std::endl;
        } else {
            std::cout << "ранил" << std::endl;
        }
    } else if (map_player[x][y] == 0) {
        map_player[x][y] = 3;
        map_enemy[x][y] = 3;
        std::cout << "мимо" << std::endl;
    } else {
        std::cout << "ход в молоко" << std::endl;
    }
}