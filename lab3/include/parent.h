#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <limits>



using namespace std;

struct SharedData {
    sem_t sem_parent;
    sem_t sem_child; 
    char name[256];
    float number;
    float div;
    bool finished;
};

void Parent(std::istream& fileName);