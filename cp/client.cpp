#include "lib/inc/utils.hpp"
#include "lib/inc/GameManager.hpp"

int main(int argc, char** argv) {

    std::string player_name;
    std::string schema_filename;

    std::cout << "Enter your player name: " << std::endl;
    std::cin >> player_name;

    std::cout << "Enter your field schema filename: " << std::endl;
    std::cin >> schema_filename;

    GameManager manager(schema_filename.c_str(), player_name);

    int pipe[2] = {std::stoi(argv[1]), std::stoi(argv[2])};
    role_t role = (role_t)std::stoi(argv[3]);

    write(pipe[WRITE], READY_MSG, 2);

    // system("clear");
    while(true) {
        manager.print();

        std::cout << ((role == ROLE_ATTACK) ? "You are attacker now!" : "You are defending now!") << std::endl;

        if(role == ROLE_ATTACK) {
            std::cout << "Enter coordinates to shot: " << std::endl;
            int x, y;
            std::cin >> x >> y;
            std::string msg = "";
            msg += std::to_string(x);
            msg += std::to_string(y);
            write(pipe[WRITE], msg.c_str(), 2);

            char* r = new char[2];
            read(pipe[READ], r, 2);
            std::string sr {r};

            if(sr == HIT_REPLY_MSG) {
                manager.set_shot_result(x, y, SHOT_HIT);
                std::cout << "Yeah! Damaged!" << std::endl;
            }
            else if(sr == MISS_REPLY_MSG) {
                manager.set_shot_result(x, y, SHOT_MISS);
                std::cout << "Ooops, missed!" << std::endl;
                role = ROLE_DEFEND;
            }
            else if(sr == CAPITULATION_MSG) {
                std::cout << "CONGRATULATIONS! YOU WON!" << std::endl;
                reload_statistics(player_name,1);
                break;
            }

            delete[] r;
        }

        else {
            std::cout << "Your opponent is shooting now! Wait...";
            char* buf = new char[2];
            read(pipe[READ], buf, 2);
            std::string smsg {buf};
            size_t x = std::stoul(smsg.substr(0, 1),nullptr,10);
            size_t y = std::stoul(smsg.substr(2, 1),nullptr,10);
            shot_t res = manager.recieve_attack(x, y);

            if(res == SHOT_HIT) {
                if(manager.check_defeat()) {
                    std::cout << "YOU LOSE! ALL YOUR SHIPS ARE DROWN :(" << std::endl;
                    reload_statistics(player_name,0);
                    write(pipe[WRITE], CAPITULATION_MSG, 2);
                    break;
                }
                write(pipe[WRITE], HIT_REPLY_MSG, 2);
                std::cout << "Hit! Your ship got damage!" << std::endl; 
            }
            else {
                write(pipe[WRITE], MISS_REPLY_MSG, 2);
                std::cout << "Opponent missed! It's your turn!" << std::endl;
                role = ROLE_ATTACK;
            }
            delete buf;
        }
    }

    close(pipe[READ]);
    close(pipe[WRITE]);
    return EXIT_SUCCESS;
}
