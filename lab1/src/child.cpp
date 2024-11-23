#include "parent.h"

int main() {
    char name[100];
    read(0, name, 100);

    float number, result;
    bool isFirst = true;
    
    while (read(0, &number, sizeof(number)) > 0) {
        if (isFirst) {
            result  = number;
            isFirst = false;
        }
        else if (number == 0) {
            perror("Ошибка при делении на 0");
            exit(1);
        }
        else
            result /= number;
    }

    std::ofstream file(name);
    if (!file.is_open()) {
        perror("Ошибка при открытии файла");
        exit(1);
    }

    if (isFirst) {
        perror("Ошибка: пустая строка");
        exit(1);
    }

    file << "Частное: " << result << std::endl;
    std::cout << "Частное: " << result << std::endl;

    return 0;
}