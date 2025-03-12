#ifndef GAME_H
#define GAME_H

#include <vector>
#include <iostream> 
#include <cstddef>

class Game {
public:
    Game();
    void arrangeShips(std::vector<std::vector<int>>& map_player, const std::vector<std::vector<int>>& input_ships);
    bool isShipKilled(std::vector<std::vector<int>>& map_player, std::vector<std::vector<int>>& input_ships);
    void giveInfo(int x, int y, std::vector<std::vector<int>>& map_player, std::vector<std::vector<int>>& map_enemy, std::vector<std::vector<int>>& input_ships, int& count_kill_ships);

private:
    const int MAX_COUNT_SHIPS = 10;
};

#endif // GAME_H