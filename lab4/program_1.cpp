#include <iostream>
#include <dlfcn.h>
#include <cstring>

using PrimeCountFunc = int(*)(int, int);
using PiFunc = float(*)(int);

int main() {
    std::cout << "Динамическая загрузка библиотек\n";

    const char* pathToLib1 = std::getenv("PATH_TO_LIBRARY_1");
    if (!pathToLib1) {
        std::cerr << "Переменная PATH_TO_LIBRARY_1 не задана" << std::endl;
        return 1;
    }

    const char* pathToLib2 = std::getenv("PATH_TO_LIBRARY_2");
    if (!pathToLib2) {
        std::cerr << "Переменная PATH_TO_LIBRARY_2 не задана" << std::endl;
        return 1;
    }

    const char* lib_paths[] = {pathToLib1, pathToLib2};

    int current_lib = 0;

    std::cout << "\nКакую библиотеку вы хотели бы загрузить?\n";
    std::cout << "1 - library_1.so (Наивный поиск простых чисел, Ряд Лейбница)\n";
    std::cout << "2 - library_2.so (Поиск простых чисел при помощи алгоритма решето Эратосфена, Формула Валлиса)\n";

    std::cin >> current_lib;

    if (current_lib != 1 && current_lib != 2) {
        std::cout << "Неверная команда\n";
        return 1;
    }

    current_lib--;

    void* handle = dlopen(lib_paths[current_lib], RTLD_LAZY);

    if (!handle) {
        std::cerr << "Ошибка загрузки библиотеки: " << dlerror() << "\n";
        return 1;
    }

    PrimeCountFunc PrimeCount = reinterpret_cast<PrimeCountFunc>(dlsym(handle, "PrimeCount"));
    PiFunc Pi = reinterpret_cast<PiFunc>(dlsym(handle, "Pi"));

    char* error;
    if ((error = dlerror()) != nullptr) {
        std::cerr << "Ошибка: " << error << "\n";
        dlclose(handle);
        return 1;
    }

    while (true) {
        std::cout << "\n---------------------------------------------------------------\n";
        std::cout << "Библиотека: " << lib_paths[current_lib] << "\n";
        std::cout << "0 - Следующая библиотека\n";
        std::cout << "1 A B - Вычислить количество простых чисел в диапазоне [A,B]\n";
        std::cout << "2 K - Вычислить число Пи при заданной длине ряда K\n";
        std::cout << "3 - Выход";
        std::cout << "\n---------------------------------------------------------------\n";
        
        int command;
        std::cin >> command;

        if (command == 0) {
            dlclose(handle);
            current_lib = 1 - current_lib;
            handle = dlopen(lib_paths[current_lib], RTLD_LAZY);
            if (!handle) {
                std::cerr << "Ошибка загрузки библиотеки: " << dlerror() << "\n";
                return 1;
            }
            PrimeCount = reinterpret_cast<PrimeCountFunc>(dlsym(handle, "PrimeCount"));
            Pi = reinterpret_cast<PiFunc>(dlsym(handle, "Pi"));
        } else if (command == 1) {
            int A, B;
            std::cin >> A >> B;
            int result = PrimeCount(A, B);
            std::cout << "личество простых чисел в диапазоне [A,B]: " << result << "\n";
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

    dlclose(handle);
    return 0;
}