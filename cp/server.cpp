#include <sys/wait.h>

#include "lib/inc/GameManager.hpp"
#include "lib/inc/utils.hpp"

#define CLIENT1_IN "in_0"
#define CLIENT1_OUT "out_0"

#define CLIENT2_IN "in_1"
#define CLIENT2_OUT "out_1"

int main(void) {
    std::cout << "=== BATTLESHIPS GAME SERVER ===" << std::endl;
    std::cout << "PID [" << getpid() << "]" << std::endl;

    bool is_running = true;

    pid_t clients[2] = {-1};
    role_t roles[2] = {ROLE_DEFEND, ROLE_ATTACK};
    int input_fd[2] = {0, 0};

    int pipe1_send[2];
    create_pipe(pipe1_send);
    int pipe1_recv[2];
    create_pipe(pipe1_recv);

    int pipe2_send[2];
    create_pipe(pipe2_send);
    int pipe2_recv[2];
    create_pipe(pipe2_recv);

    clients[0] = create_child();
    if(clients[0] == 0) {
        close(pipe1_recv[READ]);
        close(pipe1_send[WRITE]);

        close(pipe2_recv[READ]);
        close(pipe2_recv[WRITE]);
        close(pipe2_send[READ]);
        close(pipe2_send[WRITE]);

        int fd[2];
        create_pipe(fd);
        input_fd[0] = fd[WRITE];

        bind_fd(fd[READ], STDIN_FILENO);
        std::cout << "[INFO]: Use fd " << input_fd[0] << " for input PID " << getpid() << std::endl;
        bind_fd(open_file(CLIENT1_OUT), STDOUT_FILENO);
        
        const char* pr = std::to_string(pipe1_send[READ]).c_str();
        const char* pw = std::to_string(pipe1_recv[WRITE]).c_str();

        execl("./client", "./client", pr, pw, "0", NULL);
    } 
    else {
        clients[1] = create_child();
        if(clients[1] == 0) {
            close(pipe2_recv[READ]);
            close(pipe2_send[WRITE]);
            
            close(pipe1_recv[READ]);
            close(pipe1_recv[WRITE]);
            close(pipe1_send[READ]);
            close(pipe1_send[WRITE]);

            int fd[2];
            create_pipe(fd);
            input_fd[1] = fd[WRITE];

            bind_fd(fd[READ], STDIN_FILENO);
            std::cout << "[INFO]: Use fd " << input_fd[1] << " for input PID " << getpid() << std::endl;
            bind_fd(open_file(CLIENT2_OUT), STDOUT_FILENO);

            const char* pr = std::to_string(pipe2_send[READ]).c_str();
            const char* pw = std::to_string(pipe2_recv[WRITE]).c_str();

            execl("./client", "./client", pr, pw, "1", NULL);
        }
        else {
            close(pipe1_send[READ]);
            close(pipe1_recv[WRITE]);
            close(pipe2_send[READ]);
            close(pipe2_recv[WRITE]);
        }
    }
    
    std::cout << "[INFO]: Created clients with PIDs " << clients[0] << " and " << clients[1] << std::endl;

    char* buf = new char[2];
    read(pipe1_recv[READ], buf, 2);
    if(buf[1] == 'r') 
        std::cout << "[INFO]: Client 1 is ready -- OK" << std::endl;
    
    read(pipe2_recv[READ], buf, 2);
    if(buf[1] == 'r') 
        std::cout << "[INFO]: Client 2 is ready -- OK" << std::endl;
    delete[] buf;

    while(true) {
        char* buf = new char[2];
        
        if(roles[0] == ROLE_ATTACK) {
            read(pipe1_recv[READ], buf, 2);
            write(pipe2_send[WRITE], buf, 2);
            read(pipe2_recv[READ], buf, 2);

            if(buf[1] == 'h') {
                std::cout << "Client 1 hit" << std::endl;
            }
            else if(buf[1] == 'm') {
                std::cout << "Client 1 missed" << std::endl;
                roles[0] = ROLE_DEFEND;
                roles[1] = ROLE_ATTACK;
            }
            else if(buf[1] == 'c') {
                std::cout << "Client 1 won" << std::endl;
                is_running = false;
            }

            write(pipe1_send[WRITE], buf, 2);
        }

        else {
            read(pipe2_recv[READ], buf, 2);
            write(pipe1_send[WRITE], buf, 2);
            read(pipe1_recv[READ], buf, 2);

            if(buf[1] == 'h') {
                std::cout << "Client 2 hit" << std::endl;
            }
            else if(buf[1] == 'm') {
                std::cout << "Client 2 missed" << std::endl;
                roles[1] = ROLE_DEFEND;
                roles[0] = ROLE_ATTACK;
            }
            else if(buf[1] == 'c') {
                std::cout << "Client 2 won" << std::endl;
                is_running = false;
            }

            write(pipe2_send[WRITE], buf, 2);
        }

        delete[] buf;

        if(!is_running)
            break;
    }

    waitpid(clients[0], NULL, 0);
    waitpid(clients[1], NULL, 0);

    close(pipe1_recv[READ]);
    close(pipe1_send[WRITE]);
    close(pipe2_recv[READ]);
    close(pipe2_send[WRITE]);

    return EXIT_SUCCESS;
}