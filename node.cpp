#include <iostream>
#include <thread>
#include <string>
#include <zmq.hpp>
#include <csignal>

int node_id;

void process_requests(const std::string& address) {
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::rep);
    socket.bind(address);

    while (true) {
        zmq::message_t request;
        socket.recv(request, zmq::recv_flags::none);
        std::string command(static_cast<char*>(request.data()), request.size());

        if (command == "ping") {
            std::string reply = "pong";
            socket.send(zmq::buffer(reply), zmq::send_flags::none);
        } else if (command.rfind("exec", 0) == 0) {
            std::istringstream iss(command.substr(5));
            int n;
            iss >> n;
            int sum = 0;
            for (int i = 0; i < n; ++i) {
                int num;
                iss >> num;
                sum += num;
            }
            socket.send(zmq::buffer(std::to_string(sum)), zmq::send_flags::none);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: node <id> <port>" << std::endl;
        return 1;
    }

    node_id = std::stoi(argv[1]);
    std::string port = argv[2];
    std::string address = "tcp://*:" + port;

    std::cout << "Node " << node_id << " started on " << address << std::endl;
    process_requests(address);

    return 0;
}