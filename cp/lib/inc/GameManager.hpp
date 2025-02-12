#pragma once

#include <sys/types.h>
#include <iostream>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>

#define FIELD_SIZE 10

#define HIT_REPLY_MSG "_h"
#define MISS_REPLY_MSG "_m"

#define CAPITULATION_MSG "_c"
#define READY_MSG "_r"
#define STOP_MSG "_s"

enum role_t {
    ROLE_DEFEND,
    ROLE_ATTACK,
};

enum shot_t {
    SHOT_NO,
    SHOT_MISS,
    SHOT_HIT,
};

enum cell_t {
    CELL_EMPTY,
    CELL_ALIVE,
    CELL_DESTROYED,
};

class GameManager final {
    public:
        GameManager(const char* filename, std::string name);
        ~GameManager() = default;

        void set_shot_result(size_t x, size_t y, shot_t res);
        shot_t recieve_attack(size_t x, size_t y);

        void print(std::ostream& out = std::cout);
        bool check_defeat();

    private:
        std::string name;
        int cells_alive {20};

        cell_t self_field[FIELD_SIZE][FIELD_SIZE] {CELL_EMPTY};
        shot_t enemy_field[FIELD_SIZE][FIELD_SIZE] {SHOT_NO};

        void print_self(std::ostream& out);
        void print_enemy(std::ostream& out);
};
