#include <iostream>
#include "include/functions.h"

int main() {
    std::cout << "Статическая линковка\n";

    while (true) {
        std::cout << "\n---------------------------------------------------------------\n";
        std::cout << "1 A B - Вычислить количество простых чисел в диапазоне [A,B]\n";
        std::cout << "2 K - Вычислить число Пи при заданной длине ряда K\n";
        std::cout << "3 - Выход";
        std::cout << "\n---------------------------------------------------------------\n";

        int command;
        std::cin >> command;

        if (command == 1) {
            int A, B;
            std::cin >> A >> B;
            int result = PrimeCount(A, B);
            std::cout << "Количество простых чисел в диапазоне [A,B]: " << result << "\n";
        } else if (command == 2) {
            int K;
            std::cin >> K;
            float result = Pi(K);
            std::cout << "Число Пи: " << result << "\n";
        } else if (command == 3) {
            break;
        } else {
            std::cout << "Такого пункта меню не существует\n";
        }
    }

    return 0;
}