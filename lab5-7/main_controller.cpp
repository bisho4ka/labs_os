#include "tcontroller.h"
#include <iostream>

int main() {
    TControllerNode controller;

    // Инициализация контроллера с endpoint "tcp://127.0.0.1:5555"
    if (!controller.Init("tcp://127.0.0.1:5555")) {
        std::cerr << "Error: Failed to initialize controller.\n";
        return 1;
    }

    // Запуск основного цикла контроллера
    controller.Run();

    return 0;
}