#include "include/utils.h"
#include "include/GameManager.h"

int main(int argc, char** argv) {
    if (argc < 5) {
        std::cerr << "Usage: ./client <pipe_read> <pipe_write> <role> <game_name>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string player_name;
    std::string schema_filename;
    std::string game_name = argv[4];

    std::cout << "Enter your player name: ";
    std::cin >> player_name;

    std::cout << "Enter your field schema filename: ";
    std::cin >> schema_filename;

    GameManager manager(schema_filename.c_str(), player_name);

    int pipe_read = std::stoi(argv[1]);
    int pipe_write = std::stoi(argv[2]);
    role_t role = (role_t)std::stoi(argv[3]);

    write(pipe_write, READY_MSG, 2);

    while (true) {
        manager.print();

        if (role == ROLE_ATTACK) {
            std::cout << "You are attacker now!" << std::endl;
            std::cout << "Enter coordinates to shot: ";
            int x, y;
            std::cin >> x >> y;
            std::string msg = std::to_string(x) + std::to_string(y);
            write(pipe_write, msg.c_str(), 2);

            char r[2];
            read(pipe_read, r, 2);
            std::string sr{r};

            if (sr == HIT_REPLY_MSG) {
                manager.set_shot_result(x, y, SHOT_HIT);
                std::cout << "Yeah! Damaged!" << std::endl;
            } else if (sr == MISS_REPLY_MSG) {
                manager.set_shot_result(x, y, SHOT_MISS);
                std::cout << "Ooops, missed!" << std::endl;
                role = ROLE_DEFEND;
            } else if (sr == CAPITULATION_MSG) {
                std::cout << "CONGRATULATIONS! YOU WON!" << std::endl;
                reload_statistics(player_name, 1);
                break;
            }
        } else {
            std::cout << "Your opponent is shooting now! Wait..." << std::endl;
            char buf[2];
            read(pipe_read, buf, 2);
            std::string smsg{buf};
            size_t x = std::stoul(smsg.substr(0, 1));
            size_t y = std::stoul(smsg.substr(1, 1));
            shot_t res = manager.recieve_attack(x, y);

            if (res == SHOT_HIT) {
                if (manager.check_defeat()) {
                    std::cout << "YOU LOSE! ALL YOUR SHIPS ARE DROWN :(" << std::endl;
                    reload_statistics(player_name, 0);
                    write(pipe_write, CAPITULATION_MSG, 2);
                    break;
                }
                write(pipe_write, HIT_REPLY_MSG, 2);
                std::cout << "Hit! Your ship got damage!" << std::endl;
            } else {
                write(pipe_write, MISS_REPLY_MSG, 2);
                std::cout << "Opponent missed! It's your turn!" << std::endl;
                role = ROLE_ATTACK;
            }
        }
    }

    close(pipe_read);
    close(pipe_write);
    return EXIT_SUCCESS;
}