#include <queue>
#include <iostream>
#include <string>
#include <pthread.h>
#include <semaphore.h>
#include <chrono>
#include <stdio.h>
#include <iomanip>
#include <vector>
#include <map>

#include "tands.c"
#include "writer.cpp"
using namespace std;

// global mutex var for accessing the queue
sem_t qMutex;

bool done = false;

// global queue for work
queue<int> myQ;

// global output file for logs
string outputFileName;

// global numThreads
int numThreads;

// array of events to be used for appending to log file
static string events[] = {
    "Ask", 
    "Receive", 
    "Work",
    "Sleep",
    "Complete",
    "End"
};

// global summary struct counter 
summary s;

// our global writer
writer w;
