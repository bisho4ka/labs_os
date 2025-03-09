#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <map>
#include <vector>
#include <chrono>
#include <pthread.h>

struct TNodeInfo {
    int id;
    int parent_id;
    std::string endpoint;
    bool alive = true;
    bool hb_received = false;
    std::chrono::steady_clock::time_point last_heartbeat;
    std::vector<int> children; // Добавляем поле для хранения дочерних узлов
};

#endif