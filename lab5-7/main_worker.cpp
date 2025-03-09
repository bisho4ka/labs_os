#include "tworker.h"
#include <iostream>

int main(int argc, char** argv) {
    // Проверка аргументов командной строки
    if (argc < 4) {
        std::cerr << "Usage: worker <id> <parent_id> <endpoint>\n";
        return 1;
    }

    // Парсинг аргументов
    int id = std::stoi(argv[1]);       // ID узла
    int parent_id = std::stoi(argv[2]); // ID родительского узла
    std::string endpoint = argv[3];    // Endpoint для связи

    // Создание рабочего узла
    TWorkerNode worker(id, parent_id, endpoint);

    // Инициализация рабочего узла
    if (!worker.Init()) {
        std::cerr << "Error: Failed to initialize worker.\n";
        return 1;
    }

    // Запуск основного цикла рабочего узла
    worker.Run();

    return 0;
}