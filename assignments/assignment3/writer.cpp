#include <string>
#include <pthread.h>
#include <semaphore.h>
#include <chrono>
#include <map>
#include <string>
#include <vector>

using namespace std;

// struct definition for building our summary report
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

// global start time of execution
auto start = chrono::steady_clock::now();;

/**
 * @brief a threadsafe writer for appending log files and a summary file into output
 * 
 */
class writer {
 private:
    // mutex var for file writing
    sem_t fileMutex;
    // name of output file
    string outputFileName;

 public:
    /**
     * @brief Construct a new writer object
     * 
     * will just initialize the file access semaphore
     */
    writer() {
        // initialize our binary output file mutex
        sem_init(&fileMutex, 0, 1);
    }

    /**
     * @brief Construct a new writer object using an outputfile name
     * 
     * This constructor will both set the outputfile name and 
     * initialize the file access semaphore
     * 
     * @param outputFileName the filename for appending our logs
     */
    writer(string outputFileName) {
        // initialize our binary output file mutex
        sem_init(&fileMutex, 0, 1);
        // get the outputfile name
        this->outputFileName = outputFileName;
    }

    ~writer(){}

    /**
     * @brief Set the Output File name 
     * 
     * @param outputFileName the output file name
     */
    void setOutputFile(string outputFileName) {
        this->outputFileName = outputFileName;
    }

    /**
     * @brief Threadsafe writing to output file as formatted in assignment 2
     * 
     * @param event the event name as a string
     * @param tid   the thread id 
     * @param QSize the queue size
     * @param n     the value of the input command
     */
    void writeToFile(string event, pthread_t tid, int QSize, int n) {
        // initialize our file for output
        FILE * pFile;
        sem_wait(&fileMutex);
        // get our time in here so it appears chronological in logs
        auto unix_timestamp = std::chrono::seconds(std::time(NULL));
        double time = chrono::seconds
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
     * @brief takes a summary struct and writes all the summary out to the output file
     * 
     * NOTE: This is assumed to take place after threads are dead and producer finishes
     * 
     * @param s summary struct that contains the job counters
     */
    void appendSummary(summary s) {
        // // get total time and transactions per second
        // auto diff = chrono::steady_clock::now() - start;
        // double time = chrono::duration <double> (diff).count();
        // // calculate the amount of work done per second
        // double tPerS = (double) s.s["Work"] / time;
        // // setup file for appending
        // FILE * pFile;
        // pFile = fopen(outputFileName.c_str(),"a");
        // fprintf(pFile, "Summary:\n");
        // // print the number and counter of our summary struct
        // for (const auto& kv : s.s) {
        //     fprintf(pFile, "\t%-15s %d\n", kv.first.c_str(), kv.second);
        // }
        // // print out all threads job counters with their number (index)
        // for (int i = 0; i < s.tJobs.size(); i++) {
        //     fprintf(pFile, "\tThread  %-7d %d\n", (i + 1), s.tJobs[i]);
        // }
        // // print the transactions per second
        // fprintf(pFile, "Transactions per second: %.2f", tPerS);
        // fclose(pFile);
    }
};
