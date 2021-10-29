#include "prodcon.hpp"

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
    w.writeToFile(events[0], tid, -1, -1);
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
        w.writeToFile(events[1], tid, myQ.size(), n);
        s.s["Receive"]++;
        Trans(n); // "work"
        s.tJobs[tid - 1]++;
        // thread completes work
        w.writeToFile(events[4], tid, -1, n);
        s.s["Complete"]++;

        // thread to ask for work again
        w.writeToFile(events[0], tid, -1, -1);
        s.s["Ask"]++;
    }
    // exit and return
    pthread_exit(NULL);
    return NULL;
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

    w.setOutputFile(outputFileName);

    // initialize and spawn our threads
    pthread_t thread_id[numThreads + 1];
    for (int i = 1; i < numThreads + 1; i++) {
        // initialize thread job count for summary
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
            w.writeToFile(events[2], 0, myQ.size(), n);
        }
        else {
            // producer sleep command
            w.writeToFile(events[3], 0, -1, n);
            Sleep(n);
            s.s["Sleep"]++;
        }
    }
    done = true;
    w.writeToFile(events[5], 0, -1, -1);

    // wait for threads to finish
    for (int i = 1; i < numThreads + 1; i++) {
        pthread_join(thread_id[i], NULL);
    }
    
    // append our summary
    w.appendSummary(s);

    // return main
    return 0;
}
