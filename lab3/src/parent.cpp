#include "../include/parent.h"

void Parent(std::istream& fileName) {
    int shmFd = shm_open("/shared_memory_file", O_CREAT | O_RDWR, 0666);
    if (shmFd == -1) {
        throw std::runtime_error("Ошибка shm_open");
    }

     if (ftruncate(shmFd, sizeof(SharedData)) == -1) {
        shm_unlink("/shared_memory_file");
        throw std::runtime_error("Ошибка ftruncate");
    }

    // Отображаем файл в память
    SharedData* shared = (SharedData*)mmap(nullptr, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
    if (shared == MAP_FAILED) {
        close(shmFd);
        shm_unlink("/shared_memory_file");

        throw std::runtime_error("Ошибка mmap");
    }

    close(shmFd);

    sem_init(&shared->sem_parent, 1, 0);
    sem_init(&shared->sem_child, 1, 0);

    pid_t pid = fork();

    if (pid < 0) {
        throw std::runtime_error("Ошибка fork");
    }

    if (pid == 0) {
        // Дочерний процесс

        const char* pathToChild = getenv("PATH_TO_CHILD");
        if (!pathToChild) {
            throw std::runtime_error("Ошибка PATH_TO_CHILD");
        }
        execlp(pathToChild, pathToChild, nullptr);

        throw std::runtime_error("Ошибка exec");
    } else {
        // Родительский процесс

        // Ввод данных
        std::cout << "Введите имя файла:\n";
        fileName.getline(shared->name, sizeof(shared->name));

        sem_post(&shared->sem_child);

        float number;
        std::cout << "Введите числа: ";
        while (fileName >> number) {
            shared->finished = false;
            shared->number = number;
            
            sem_post(&shared->sem_child);
            sem_wait(&shared->sem_parent);
        }

        shared->finished = true;
        sem_post(&shared->sem_child);

        // Очищаем поток ввода
        fileName.clear();
        fileName.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        sem_wait(&shared->sem_parent);

        // Читаем результат
        std::cout << "Данные от дочернего процесса: " << shared->div << std::endl;

        // Уничтожаем семафоры
        sem_destroy(&shared->sem_parent);
        sem_destroy(&shared->sem_child);

        // Удаляем отображение памяти
        munmap(shared, sizeof(SharedData));

        // Удаляем файл
        shm_unlink("/shared_memory_file");

        // Ждем завершения дочернего процесса
        wait(nullptr);
    }
}