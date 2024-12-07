#include "../include/parent.h"

int main() {
    int shmFd = shm_open("/shared_memory_file", O_RDWR, 0666);
    if (shmFd == -1) {
        throw std::runtime_error("Ошибка shm_open");
    }

    // Отображаем файл в память
    SharedData* shared = (SharedData*)mmap(nullptr, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
    if (shared == MAP_FAILED) {
        close(shmFd);
        shm_unlink("/shared_memory_file");

        throw std::runtime_error("Ошибка mmap");
    }
    close(shmFd);

    // Ждем сигнал от родительского процесса
    sem_wait(&shared->sem_child);

    // Читаем данные
    char fileName[256];
    strcpy(fileName, shared->name);

    float div;
    bool isFirst = true;

    while (true) {
        // Ждем число
        sem_wait(&shared->sem_child);

        if (shared->finished) {
            break;
        }

        if (isFirst) {
            div  = shared->number;
            isFirst = false;
        }
        else if (shared->number == 0) {
            throw std::invalid_argument("Ошибка при делении на 0");
        }
        else
            div /= shared->number;

        // Можно продолжать
        sem_post(&shared->sem_parent);
    }

    // Записываем частное в файл
    {
        std::ofstream file(fileName);
        if (!file.is_open()) {
            throw std::runtime_error("Ошибка при открытии файла");
        }

        file << "Частное: " << div << '\n';
    }

    // Записываем результат в общую память
    shared->div = div;

    // Сигнализируем родительскому процессу о завершении
    sem_post(&shared->sem_parent);

    // Удаляем отображение памяти
    munmap(shared, sizeof(SharedData));

    return 0;
}