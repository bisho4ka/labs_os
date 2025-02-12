#include "inc/utils.hpp"
#include <vector>
#include <fstream>
using namespace std;


pid_t create_child() {
    pid_t pid = fork();
    if(pid == -1)
        throw std::runtime_error("Failed fork()");
    return pid;  
}

void create_pipe(int fd[2]) {
    if(pipe(fd) == -1)
        throw std::runtime_error("Failed pipe()");
}

void bind_fd(int old, int n) {
    if(dup2(old, n) == -1)
        throw std::runtime_error("Failed dup2()");
}

int open_file(const char* fname) {
    int fd = open(fname, O_RDWR);
    if(fd == -1)
        throw std::runtime_error("Failed open()");
    return fd;
}

struct PlayerData {
    string Playername;
    int win_num;
    int lose_num;
};


void reload_statistics(string player_name, bool result){
    string filename = "statistics.txt";

    vector<PlayerData> Player_data_list;

    ifstream file_in(filename);
    if (!file_in.is_open()) {
        throw std::runtime_error("Failed open()");
    }

    bool found_Player = false;
    PlayerData Player_data;

    while (file_in >> Player_data.Playername >> Player_data.win_num >> Player_data.lose_num) {
        if (Player_data.Playername == player_name) {
            found_Player = true;
            break;
        }
        Player_data_list.push_back(Player_data);
    }

    file_in.close();

    if (!found_Player) {
        Player_data.win_num = 0;
        Player_data.lose_num = 0;
        Player_data_list.push_back(Player_data);
    }

    if (result) {
        Player_data_list.back().win_num += 1;
    } else {
        Player_data_list.back().lose_num += 1;
    }

    ofstream file_out(filename);
    if (!file_out.is_open()) {
        throw std::runtime_error("Failed open()");
    }

    for (const auto& data : Player_data_list) {
        file_out << data.Playername << " " << data.win_num << " " << data.lose_num << endl;
    }

    file_out.close();
}

void get_statistics(std::string player_name){
    string filename = "statistics.txt";

    ifstream file_in(filename);
    if (!file_in.is_open()) {
        throw std::runtime_error("Failed open()");
    }

    bool found_Player = false;
    PlayerData Player_data;

    while (file_in >> Player_data.Playername >> Player_data.win_num >> Player_data.lose_num) {
        if (Player_data.Playername == player_name) {
            found_Player = true;
            cout << endl << player_name << " win: " << Player_data.win_num << " lose: " << Player_data.win_num << endl;
            break;
        }
    }

    file_in.close();

    if (!found_Player) {
        cout << "You have not played yet!" << endl;
    }
}
