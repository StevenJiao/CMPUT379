#include "prodcon.hpp"

/**
 * @brief The main thread worker function.
 * This function will continually run on the queue until nothing is left
 * and the queue itself is empty
 * 
 * @param input the void* of the thread's ID
 * @return void* 
 */
void *getWork(void *input) {
    // get the thread id from input
    pthread_t tid = (pthread_t) input;

    // first ask for work
    w.writeToFile(events[0], tid, -1, -1);
    s.s["Ask"]++;

    // keep looping until producer is EOF and queue is empty
    while (!(done && myQ.size() == 0)) {
        // lock queue state lookup/work pop so it won't change
        pthread_mutex_lock(&qMutex);
        // check if the queue size is 0
        while (myQ.size() == 0) {
            // if producer reached EOF/finished then we exit
            if (done) {
                pthread_mutex_unlock(&qMutex);
                pthread_exit(NULL);
                return 0;
            }
            // if producer not done yet we wait for the signal
            else {
                pthread_cond_wait(&qEmptyCond, &qMutex);
            }
        }
        // get the next job
        int n = myQ.back();
        // get the queue size at the time of grabbing the job
        int size = myQ.size();
        // pop our selection out of queue
        myQ.pop();
        // send signal we're not full anymore to producer if its waiting 
        pthread_cond_signal(&qFullCond);
        pthread_mutex_unlock(&qMutex);

        // log work and perform it
        w.writeToFile(events[1], tid, myQ.size(), n);
        s.s["Receive"]++;
        Trans(n); // "work"
        // increase job counter of this consumer after finishing
        s.tJobs[tid - 1]++;
        // log thread completes work
        w.writeToFile(events[4], tid, -1, n);
        s.s["Complete"]++;

        // thread to ask for work again
        w.writeToFile(events[0], tid, -1, -1);
        s.s["Ask"]++;
    }
    // exit and return to make sure we get out
    pthread_exit(NULL);
    return NULL;
}

int main(int argc, char const *argv[]) {
    // catch errors where invalid inputs are given
    if (argc == 1 || argc > 3) {
        cout << "Invalid number of arguments provided to prodcon." << endl;
        return 0; 
    }
    else if (stoi(argv[1]) <= 0) {
        cout << "Invalid number of threads chosen." << endl;
        return 0;
    }
    else if (argc == 3 && stoi(argv[2]) < 0) {
        cout << "Invalid outputfile number chosen." << endl;
        return 0;
    }

    // get the number of threads and output file number if given
    numThreads = stoi(argv[1]);
    // max queue size is 2 times number of threads
    int maxQSize = 2*numThreads;
    // construct output file name as per assignment spec
    string outputFileNum = argc == 3 ? argv[2] : "";
    outputFileName = outputFileNum.size() > 0 ? "prodcon." + outputFileNum + ".log" : "prodcon.log";
    // set the outputfile name for our writer
    w.setOutputFile(outputFileName);

    // initialize and spawn our consumers/threads
    pthread_t thread_id[numThreads + 1];
    for (int i = 1; i < numThreads + 1; i++) {
        // initialize thread job count for summary
        s.tJobs.push_back(0);
        // create our thread and get it to do work
        pthread_create(&thread_id[i], NULL, getWork, (void *)(pthread_t) i);
    }

    // producer: get the input either from commandline or file and assign work
    string line;
    while (getline(cin, line)) {
        // parse the numerical value 
        int n = atoi(line.substr(1, string::npos).c_str());
        
        // check if its a thread task or producer sleep
        if (line.at(0) == 'T') {
            // lock queue while putting work in
            pthread_mutex_lock(&qMutex);
            // if queue is full, wait on condition from consumers
            while (myQ.size() == maxQSize) {
                pthread_cond_wait(&qFullCond, &qMutex);
            }
            // push thread work onto the queue
            myQ.push(n);
            // grab current size of our queue for log
            int qSize = myQ.size();
            // send signal to currently waiting consumers that work is ready
            pthread_cond_broadcast(&qEmptyCond);
            pthread_mutex_unlock(&qMutex);

            // log work given
            w.writeToFile(events[2], 0, qSize, n);
            s.s["Work"]++;
        }
        else {
            // producer sleep command; log first to make sense
            w.writeToFile(events[3], 0, -1, n);
            Sleep(n);
            s.s["Sleep"]++;
        }
    }
    // our flag for signalling the producer is done/reached EOF
    done = true;
    // log end/EOF 
    w.writeToFile(events[5], 0, -1, -1);

    // wait for all threads to finish
    for (int i = 1; i < numThreads + 1; i++) {
        // send the broadcast to straggler consumers that just made it to wait cond
        pthread_cond_broadcast(&qEmptyCond);
        pthread_join(thread_id[i], NULL);
    }
    
    // append our summary
    w.appendSummary(s);

    // return main
    return 0;
}
