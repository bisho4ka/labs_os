#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <string>
#include <unordered_map>

class Server {
public:
    Server();
    void start();
    void stop();
    void createGame(const std::string& gameName);
    void joinGame(const std::string& gameName, const std::string& playerName);
    void movePlayer(int pipe[2], std::vector<std::vector<int>>& map_current, std::vector<std::vector<int>>& map_player, std::vector<std::vector<int>>& map_enemy, const std::string& player, std::vector<std::vector<int>>& input_ships, int& count_kill_ships);
    void startGame(int pipe[2], std::vector<std::vector<int>>& map_player, std::vector<std::vector<int>>& input_ships, std::unordered_map<std::string, std::pair<int, int>>::iterator iterator_players);
    void clear(int pipe[2]);
    void print_matrix(const std::vector<std::vector<int>>& matrix);
    void arrangeShips(std::vector<std::vector<int>>& map_player, const std::vector<std::vector<int>>& input_ships);
    bool isShipKilled(std::vector<std::vector<int>>& map_player, std::vector<std::vector<int>>& input_ships);
    void giveInfo(int x, int y, std::vector<std::vector<int>>& map_player, std::vector<std::vector<int>>& map_enemy, std::vector<std::vector<int>>& input_ships, int& count_kill_ships);

    // Объявление функции startMenu
    std::string startMenu(int pipe[2]);

private:
    std::vector<std::string> activeGames;
    std::unordered_map<std::string, std::pair<int, int>> players;
};

#endif // SERVER_H