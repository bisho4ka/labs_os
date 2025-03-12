#ifndef CLIENT_H
#define CLIENT_H

#include <string>

class Client {
public:
    Client();
    std::string getLogin();
    std::string getShipPositions();
    std::string getStatus();
    std::string getCoordinates();
    std::string getRequest();

private:
    std::string login;
    std::string shipPositions;
    std::string status;
    std::string coordinates;
    std::string request;
};

#endif // CLIENT_H