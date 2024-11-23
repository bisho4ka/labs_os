#include "parent.h"

void Parent(std::istream& stream) {
    int pipe1[2], pipe2[2];
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Ошибка pipe");
        exit(1);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Ошибка fork");
        exit(1);
    }
    else if (pid == 0) {
        close(pipe1[1]);
        close(pipe2[0]);

        if (dup2(pipe1[0], 0) == -1 || dup2(pipe2[1], 1) == -1) {
            perror("Ошибка dup2");
            exit(1);
        }
        close(pipe1[0]);  
        close(pipe2[1]); 

        const char* pathToChild = std::getenv("PATH_TO_CHILD");
        if (pathToChild == nullptr) {
            perror("Ошибка PATH_TO_CHILD");
            exit(1);
        }

        execlp(pathToChild, pathToChild, nullptr);
        perror("Ошибка execlp");
        exit(1);
    }
    else {
        close(pipe1[0]);
        close(pipe2[1]);
        
        char name[100];
        stream >> name;

        if (write(pipe1[1], &name, 100) == -1) {
            perror("Ошибка write");
            close(pipe1[1]);
            exit(1);
        }

        float number;
        std::cout << "Введите числа: ";
        while (stream >> number) {
            if (write(pipe1[1], &number, sizeof(number)) == -1) {
                perror("Ошибка write");
                close(pipe1[1]);
                exit(1);
            }
        }

        close(pipe1[1]);
        wait(nullptr);

        char buffer[100];
        size_t bytes = read(pipe2[0], buffer, sizeof(buffer));
        if (bytes > 0) 
            std::cout << "Данные от дочернего процесса: " << buffer;
        else {
            perror("Ошибка read");
            exit(1);
        }
        close(pipe2[0]);
    }
}