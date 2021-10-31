#include <queue>
#include <iostream>
#include <string>
#include <pthread.h>
#include <chrono>
#include <stdio.h>
#include <iomanip>
#include <vector>
#include <map>

#include "tands.c"
#include "writer.cpp"
using namespace std;

// global mutex var for accessing the queue
pthread_mutex_t qMutex = PTHREAD_MUTEX_INITIALIZER;
// global mutex for consumers checking on the empty queue cond
pthread_mutex_t qEmptyCondMutex = PTHREAD_MUTEX_INITIALIZER;

// global pthread conditions for when the queue is empty
pthread_cond_t qEmptyCond  = PTHREAD_COND_INITIALIZER;
// global pthread conditions for when the queue is full
pthread_cond_t qFullCond  = PTHREAD_COND_INITIALIZER;

// global var for signalling producer reached EOF 
// and no more work is going to be assigned
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
