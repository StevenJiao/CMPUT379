#include "tands.hpp"
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

// global numThreads
int numThreads;

// global start time of execution
auto start = chrono::steady_clock::now();

// array of events to be used for appending to log file
static string events[] = {
    "Ask", 
    "Receive", 
    "Work",
    "Sleep",
    "Complete",
    "End"
};

// struct for building our summary report
struct summary {
    // counters for our summary report
    map<string, int> s {
        {"Work", 0},
        {"Ask", 0},
        {"Receive", 0},
        {"Complete", 0},
        {"Sleep", 0},
    };
    // counter for num jobs assigned to each thread
    vector<int> tJobs;
};
// global summary
summary s;

/**
 * @brief Threadsafe writing to output file as formatted in assignment 2
 * 
 * @param event 
 * @param tid 
 * @param QSize 
 * @param n 
 */
void writeToFile(string event, pthread_t tid, int QSize, int n) {
    // initialize our file for output
    FILE * pFile;
    sem_wait(&fileMutex);
    // get our time in here so it appears chronological in logs
    auto diff = chrono::steady_clock::now() - start;
    double time = chrono::duration <double> (diff).count();
    // append to output file
    pFile = fopen(outputFileName.c_str(),"a");
    // if we're not given job size or Q size, format it
    if (n < 0 || QSize < 0) {
        if (n < 0 && QSize < 0) { // if not given both job size and Q size
            fprintf(pFile, "   %.3f ID= %-6lu %-10s\n", time, tid, event.c_str());
        }
        else if (n < 0) { // if we only have Q size
            fprintf(pFile, "   %.3f ID= %lu Q= %d %-10s\n", time, tid, QSize, event.c_str());
        }
        else { // if we only have job size
            fprintf(pFile, "   %.3f ID= %-6lu %-10s %d\n", time, tid, event.c_str(), n);
        }
    }
    else {
        // print all fields
        fprintf(pFile, "   %.3f ID= %lu Q= %d %-10s %d\n", time, tid, QSize, event.c_str(), n);
    }
    // close and release
    fclose(pFile);
    sem_post(&fileMutex);
}

/**
 * @brief Get the Work object
 * 
 * @param input 
 * @return void* 
 */
void *getWork(void *input) {
    // get the thread id from input
    pthread_t tid = (pthread_t) input;

    // first ask for work
    writeToFile(events[0], tid, -1, -1);
    s.s["Ask"]++;

    // keep looping if producer is still going or we have work in the queue
    while (!done || myQ.size() != 0) {
        // lock queue state lookup/work pop so it won't change
        sem_wait(&qMutex);
        // if we're fully done, post and break so we can end this thread
        if (done && myQ.size() == 0) { sem_post(&qMutex); break; }
        // if we've done all the work but producer still going, post and retry
        if (myQ.size() == 0 && !done) { sem_post(&qMutex); continue;}
        // get the next job
        int n = myQ.back();
        // get the size at the time of grabbing the job
        int size = myQ.size();
        // pop our selection out of queue
        myQ.pop();
        sem_post(&qMutex); // release

        // thread receives work, and performs work
        writeToFile(events[1], tid, myQ.size(), n);
        s.s["Receive"]++;
        Trans(n); // "work"
        s.tJobs[tid - 1]++;
        // thread completes work
        writeToFile(events[4], tid, -1, n);
        s.s["Complete"]++;

        // thread to ask for work again
        writeToFile(events[0], tid, -1, -1);
        s.s["Ask"]++;
    }
    // exit and return
    pthread_exit(NULL);
    return NULL;
}

/**
 * @brief 
 * 
 */
void appendSummary() {
    // get total time and transactions per second
    auto diff = chrono::steady_clock::now() - start;
    double time = chrono::duration <double> (diff).count();
    double tPerS = (double) s.s["Work"] / time;
    // setup file for appending
    FILE * pFile;
    pFile = fopen(outputFileName.c_str(),"a");
    fprintf(pFile, "Summary:\n");
    for (const auto& kv : s.s) {
        fprintf(pFile, "\t%-15s %d\n", kv.first.c_str(), kv.second);
    }
    for (int i = 0; i < s.tJobs.size(); i++) {
        fprintf(pFile, "\tThread  %-7d %d\n", (i + 1), s.tJobs[i]);
    }
    fprintf(pFile, "Transactions per second: %.2f", tPerS);
    fclose(pFile);
}

int main(int argc, char const *argv[]) {
    // get the number of threads and output file number if given
    numThreads = stoi(argv[1]);
    // max queue size is 2 times number of threads
    int maxQSize = 2*numThreads;
    string outputFileNum = argc == 3 ? argv[2] : "";
    outputFileName = outputFileNum.size() > 0 ? "prodcon." + outputFileNum + ".log" : "prodcon.log";

    // initialize our binary queue access mutex
    sem_init(&qMutex, 0, 1);
    // initialize our binary output file mutex
    sem_init(&fileMutex, 0, 1);

    // initialize and spawn our threads
    pthread_t thread_id[numThreads + 1];
    for (int i = 1; i < numThreads + 1; i++) {
        // initialize thread job count
        s.tJobs.push_back(0);
        // create our thread and get it to do work
        pthread_create(&thread_id[i], NULL, getWork, (void *) i);
    }

    // get the input either from commandline or file and assign work
    string line;
    while (getline(cin, line)) {
        int n = atoi(line.substr(1, string::npos).c_str());
        if (line.at(0) == 'T') {
            // wait while we've hit max capacity
            while (myQ.size() == maxQSize);
            myQ.push(n);
            s.s["Work"]++;
            writeToFile(events[2], 0, myQ.size(), n);
        }
        else {
            // producer sleep command
            s.s["Sleep"]++;
            writeToFile(events[3], 0, -1, n);
            Sleep(n);
            s.s["Sleep"]++;
        }
    }
    done = true;
    writeToFile(events[5], 0, -1, -1);

    // wait for threads to finish
    for (int i = 1; i < numThreads + 1; i++) {
        pthread_join(thread_id[i], NULL);
    }
    
    // append our summary
    appendSummary();

    // return main
    return 0;
}
