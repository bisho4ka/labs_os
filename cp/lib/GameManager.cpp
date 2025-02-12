#include "inc/GameManager.hpp"

GameManager::GameManager(const char* filename, std::string name) {
    this->name = name;
    int fd = open(filename, O_RDONLY);

    if(fd == -1)
        throw std::runtime_error("GameManager() cannot open file");

    for(size_t i {0}; i < FIELD_SIZE; ++i) {
        for(size_t j {0}; j < FIELD_SIZE; ++j) {
            char c {'\n'};
            while(c == '\n')
                read(fd, &c, 1);
            switch (c) {
                case '#':
                    self_field[i][j] = CELL_ALIVE;
                    break;
                default:
                    self_field[i][j] = CELL_EMPTY;
                    break;
            }
        }
    }
    close(fd);
}

void GameManager::set_shot_result(size_t x, size_t y, shot_t res) {
    enemy_field[y][x] = res;
} 

shot_t GameManager::recieve_attack(size_t x, size_t y) {
    if(self_field[y][x] == CELL_ALIVE) {
        self_field[y][x] = CELL_DESTROYED;
        cells_alive--;
        return SHOT_HIT;
    }
    return SHOT_MISS;
}

void GameManager::print_self(std::ostream& out) {
    out << "   ";
    for(size_t i {0}; i < FIELD_SIZE; ++i) {
        out << "  " << char('A' + i);
    }
    out << std::endl;

    for(size_t i {0}; i < FIELD_SIZE; ++i) {
        out << i << ' ';
        for(size_t j {0}; j < FIELD_SIZE; ++j) {
            switch (self_field[i][j]) {
                case CELL_ALIVE:
                    out << "[#]";
                    break;
                case CELL_DESTROYED:
                    out << "[X]";
                    break;
                case CELL_EMPTY:
                    out << "[ ]";
                    break;
            }
        }
        out << '\n';
    }
    out << std::endl;
}

void GameManager::print_enemy(std::ostream& out) {
    out << "   ";
    for(size_t i {0}; i < FIELD_SIZE; ++i) {
        out << "  " << char('A' + i);
    }
    out << std::endl;

    for(size_t i {0}; i < FIELD_SIZE; ++i) {
        out << i + 1 << ' ';
        for(size_t j {0}; j < FIELD_SIZE; ++j) {
            switch (enemy_field[i][j]) {
                case SHOT_NO:
                    out << "[ ]";
                    break;
                case SHOT_MISS:
                    out << "[O]";
                    break;
                case SHOT_HIT:
                    out << "[X]";
                    break;
            }
        }
        out << '\n';
    }
    out << std::endl;
}

void GameManager::print(std::ostream& out) {
    out << "YOUR BATTLESHIPS:\n";
    print_self(out);
    out << "\nENEMIES RADAR:\n";
    print_enemy(out);
}

bool GameManager::check_defeat() { return (cells_alive == 0); }
