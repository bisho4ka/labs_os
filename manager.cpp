#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <zmq.hpp>
#include <csignal>
#include <cstdlib>
#include <unistd.h>

std::map<int, std::string> nodes; // id -> адрес узла (tcp://localhost:port)
std::map<int, bool> available;   // id -> доступность

void create_node(int id, int parent = -1) {
    if (nodes.count(id)) {
        std::cout << "Error: Already exists" << std::endl;
        return;
    }
    if (parent != -1 && (!nodes.count(parent) || !available[parent])) {
        std::cout << "Error: Parent not found or unavailable" << std::endl;
        return;
    }

    pid_t pid = fork();
    if (pid == -1) {
        std::cout << "Error: Unable to fork process" << std::endl;
        return;
    }
    if (pid == 0) {
        std::string port = std::to_string(5550 + id);
        execlp("./node", "./node", std::to_string(id).c_str(), port.c_str(), NULL);
        exit(0);
    }

    std::string address = "tcp://localhost:" + std::to_string(5550 + id);
    nodes[id] = address;
    available[id] = true;
    std::cout << "Ok: " << pid << std::endl;
}

void exec_command(int id, const std::vector<int>& params) {
    if (!nodes.count(id) || !available[id]) {
        std::cout << "Error:" << id << ": Node is unavailable" << std::endl;
        return;
    }

    try {
        zmq::context_t context(1);
        zmq::socket_t socket(context, zmq::socket_type::req);
        socket.connect(nodes[id]);

        std::ostringstream oss;
        oss << "exec " << params.size();
        for (const auto& param : params) {
            oss << " " << param;
        }

        std::string command = oss.str();
        zmq::message_t request(command.size());
        memcpy(request.data(), command.c_str(), command.size());
        socket.send(request, zmq::send_flags::none);

        zmq::message_t reply;
        socket.recv(reply, zmq::recv_flags::none);
        std::string result(static_cast<char*>(reply.data()), reply.size());

        std::cout << "Ok:" << id << ": " << result << std::endl;

    } catch (const zmq::error_t& e) {
        std::cout << "Error:" << id << ": Node communication failed" << std::endl;
    }
}

void pingall() {
    std::vector<int> unreachable;
    for (auto& [id, address] : nodes) {
        try {
            zmq::context_t context(1);
            zmq::socket_t socket(context, zmq::socket_type::req);
            socket.connect(address);
            socket.set(zmq::sockopt::rcvtimeo, 2000);
            socket.set(zmq::sockopt::linger, 0);

            zmq::message_t request(4);
            memcpy(request.data(), "ping", 4);
            socket.send(request, zmq::send_flags::none);

            zmq::message_t reply;
            zmq::recv_result_t bytes = socket.recv(reply, zmq::recv_flags::none);
            if (!bytes) {
                unreachable.push_back(id);
            } else {
                std::string result(static_cast<char*>(reply.data()), reply.size());
                if (result != "pong") {
                    unreachable.push_back(id);
                }
            }

        } catch (...) {
            available[id] = false;
            unreachable.push_back(id);
        }
    }

    if (unreachable.empty()) {
        std::cout << "Ok: -1" << std::endl;
    } else {
        for (size_t i = 0; i < unreachable.size(); ++i) {
            std::cout << unreachable[i];
            if (i + 1 != unreachable.size()) std::cout << ";";
        }
        std::cout << std::endl;
    }
}

int main() {
    std::string command;
    while (std::getline(std::cin, command)) {
        std::istringstream iss(command);
        std::string cmd;
        iss >> cmd;

        if (cmd == "create") {
            int id, parent = -1;
            iss >> id >> parent;
            create_node(id, parent);
        } else if (cmd == "exec") {
            int id, n;
            iss >> id >> n;
            std::vector<int> params(n);
            for (int i = 0; i < n; ++i) {
                iss >> params[i];
            }
            exec_command(id, params);
        } else if (cmd == "pingall") {
            pingall();
        } else {
            std::cout << "Unknown command" << std::endl;
        }
    }
    return 0;
}