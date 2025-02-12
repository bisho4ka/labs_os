#pragma once

#include <string>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

enum {
    READ,
    WRITE,
};

pid_t create_child();

void create_pipe(int fd[2]);

void bind_fd(int old, int n);

int open_file(const char* fname);

void reload_statistics(std::string player_name, bool result);

void get_statistics(std::string player_name);
