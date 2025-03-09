#include "tworker.h"
#include "common.h"
#include <iostream>
#include <chrono>
#include <pthread.h>
#include <sstream>

pthread_mutex_t g_print_mutex = PTHREAD_MUTEX_INITIALIZER;

TWorkerNode::TWorkerNode(int id, int parent_id, const std::string& endpoint)
: id_(id), parent_id_(parent_id), endpoint_(endpoint) {}

bool TWorkerNode::Init() {
    if (!messaging_.InitWorker("tcp://127.0.0.1:5555", id_)) {
        std::cerr << "Worker " << id_ << ": Failed to init messaging\n";
        return false;
    }
    return true;
}

void* TWorkerNode::HeartbeatThreadStatic(void* arg) {
    TWorkerNode* self = static_cast<TWorkerNode*>(arg);
    while (self->running_) {
        self->SendHeartbeat();
        usleep(2000 * 1000); // 2000 ms = 2 s
    }
    return NULL;
}

void TWorkerNode::Run() {
    bool hb_started = false;
    pthread_t hb_thread;

    while (running_) {
        std::string msg;
        if (!messaging_.RecvFromController(msg)) {
            usleep(100 * 1000); // 100 ms задержка
            continue;
        }

        if (msg == "HB_START") {
            if (!hb_started) {
                hb_started = true;
                pthread_create(&hb_thread, NULL, &HeartbeatThreadStatic, this);
                pthread_detach(hb_thread);
            }
            continue;
        }

        if (msg == "QUIT") {
            // Завершаем работу узла
            running_ = false;
            pthread_mutex_lock(&g_print_mutex);
            std::cerr << id_ << " quitting...\n";
            pthread_mutex_unlock(&g_print_mutex);
            break;
        }

        if (msg.rfind("EXEC", 0) == 0) {
            size_t pos1 = msg.find('\n');
            size_t pos2 = msg.find('\n', pos1 + 1);
            if (pos1 == std::string::npos || pos2 == std::string::npos) {
                messaging_.SendToController("Error:" + std::to_string(id_) + ": Invalid EXEC format");
                continue;
            }

            std::string n_str = msg.substr(pos1 + 1, pos2 - (pos1 + 1));
            std::string numbers = msg.substr(pos2 + 1);

            std::istringstream iss(n_str);
            int n;
            iss >> n;

            std::istringstream iss_numbers(numbers);
            int sum = 0;
            for (int i = 0; i < n; ++i) {
                int num;
                if (!(iss_numbers >> num)) {
                    messaging_.SendToController("Error:" + std::to_string(id_) + ": Invalid argument");
                    return;
                }
                sum += num;
            }

            std::string result = "Ok:" + std::to_string(id_) + ": " + std::to_string(sum);
            messaging_.SendToController(result);
        } else {
            messaging_.SendToController("Error:" + std::to_string(id_) + ": Unknown command");
        }
    }
}

void TWorkerNode::SendHeartbeat() {
    messaging_.SendToController("HB");
}