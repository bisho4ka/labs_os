#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>
#include <utility>

class Client {
public:
    Client(int state);
    std::string getLogin();
    std::vector<std::vector<int>> getShips();
    std::string getStatus();
    std::pair<int, int> getCoords();
    void sendMessage(const std::string& message);
    std::string receiveMessage();

private:
    int state;
    int pipefd[2];
};

#endif // CLIENT_H