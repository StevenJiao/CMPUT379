#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <fcntl.h>
#include "helperFuncs.hpp"
using namespace std; 

class Shell379 {
    public:
    pid_t mainPid;
    struct rusage myUsage;
    
    Shell379();
    ~Shell379();
    void run();
};