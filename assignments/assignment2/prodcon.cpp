#include "tands.hpp"
#include <queue>
#include <iostream>
#include <string>
#include <pthread.h>
#include <semaphore.h>
#include <chrono>
#include <stdio.h>
#include <iomanip>
using namespace std;

// global mutex var for accessing the queue
sem_t qMutex;
// global mutex var for file writing
sem_t fileMutex;

// global queue for work
queue<int> myQ;
// global done for producer
bool done = false;

// global output file for logs
string outputFileName;

// global start time of execution
auto start = chrono::steady_clock::now();

static string events[] = {
    "Ask", 
    "Receive", 
    "Work",
    "Sleep",
    "Complete",
    "End"
};

/**
 * @brief Threadsafe writing to output file as formatted in assignment 2
 * 
 * @param event 
 * @param tid 
 * @param QSize 
 * @param n 
 */
void writeToFile(string event, pthread_t tid, int QSize, int n) {
    FILE * pFile;
    sem_wait(&fileMutex);
    auto diff = chrono::steady_clock::now() - start;
    double time = chrono::duration <double> (diff).count();
    pFile = fopen(outputFileName.c_str(),"a");
    if (n < 0 || QSize < 0) {
        if (n < 0 && QSize < 0) {
            fprintf(pFile, "%.3f ID= %-6lu %-10s\n", time, tid, event.c_str());
        }
        else if (n < 0) {
            fprintf(pFile, "%.3f ID= %lu Q= %d %-10s\n", time, tid, QSize, event.c_str());
        }
        else {
            fprintf(pFile, "%.3f ID= %-6lu %-10s %d\n", time, tid, event.c_str(), n);
        }
    }
    else {
        fprintf(pFile, "%.3f ID= %lu Q= %d %-10s %d\n", time, tid, QSize, event.c_str(), n);
    }
    fclose(pFile);
    sem_post(&fileMutex);
}

void *getWork(void *input) {
    pthread_t tid = (pthread_t) input;

    // first ask for work
    writeToFile(events[0], tid, myQ.size(), -1);

    while (!done) {
        // grab work if there is any
        sem_wait(&qMutex);
        if (myQ.size() == 0 && !done) { sem_post(&qMutex); continue;}
        if (done) { sem_post(&qMutex); break; }
        int n = myQ.back();
        int size = myQ.size();
        myQ.pop();
        sem_post(&qMutex);

        // thread recieves work, and performs work
        writeToFile(events[1], tid, myQ.size(), n);
        Trans(n);

        // thread completes work
        writeToFile(events[4], tid, -1, n);

        // thread to ask for work again
        writeToFile(events[0], tid, myQ.size(), -1);
    }
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
    // get the number of threads and output file number if given
    int numThreads = stoi(argv[1]);
    int maxQSize = 2*numThreads;
    string outputFileNum = argc == 3 ? argv[2] : "";
    outputFileName = "prodcon." + outputFileNum + ".log";

    // initialize our binary queue access mutex
    sem_init(&qMutex, 0, 1);
    // initialize our binary output file mutex
    sem_init(&fileMutex, 0, 1);

    // initialize and spawn our threads
    pthread_t thread_id[numThreads + 1];
    for (int i = 1; i < numThreads + 1; i++) {
        pthread_create(&thread_id[i], NULL, getWork, (void *) i);
    }

    // get the input either from commandline or file and assign work
    string line;
    while (getline(cin, line)) {
        int n = atoi(line.substr(1, string::npos).c_str());
        if (line.at(0) == 'T') {
            // add thread work to our queue
            myQ.push(n);
            writeToFile(events[2], 0, myQ.size(), n);
        }
        else {
            // producer sleep command
            writeToFile(events[3], 0, -1, n);
            Sleep(n);
        }
    }
    done = true;
    writeToFile(events[5], 0, -1, -1);
    return 0;
}
