#include <string>
#include <chrono>
#include <map>
#include <string>
#include <vector>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

/**
 * @brief a threadsafe writer for appending log files for a server
 * 
 */
class serverwriter {
 private:
    // mutex var for file writing
    sem_t fileMutex;
    // name of output file
    string outputFileName;
    // port number
    int portNum;
    // start time in ms of execution for summary
    double startTime;
    // end time in ms of execution for summary
    double endTime;

 public:
    /**
     * @brief Construct a new writer object using an outputfile name
     * 
     * This constructor will both set the outputfile name and 
     * initialize the file access semaphore
     * 
     * @param outputFileName    the filename for appending our logs
     * @param portNum           the port number used
     */
    serverwriter(string outputFileName, int portNum) {
        // initialize our binary output file mutex
        sem_init(&fileMutex, 0, 1);
        // get the outputfile name
        this->outputFileName = outputFileName;
        this->portNum = portNum;

        // write our header to the file
        writeHeaderToFile();
    }

    serverwriter() {
        // initialize our binary output file mutex
        sem_init(&fileMutex, 0, 1);
    }

    ~serverwriter(){}

    /**
     * @brief Set the Output File name 
     * 
     * @param outputFileName the output file name
     */
    void setOutputFile(string outputFileName) {
        this->outputFileName = outputFileName;
    }
    
    /**
     * @brief Writes the header information (port num, server address, and host name) into the output file for client
     * 
     */
    void writeHeaderToFile() {
        FILE * pFile;
        // append to output file
        pFile = fopen(outputFileName.c_str(),"a");

        // print our port number into output logs
        fprintf(pFile, "Using port %d\n", portNum);

        // close
        fclose(pFile);
    }

    /**
     * @brief Set the Start Time object
     * 
     */
    void setStartTime() {
        this->startTime = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    }

    /**
     * @brief Threadsafe writing to output file for a server to handle multiple connected clients
     * 
     * @param event         the event name as a string
     * @param n             the value of the input command
     * @param jobNum        the job number        
     * @param clientName    the client name
     */
    void writeToFile(string event, int n, int jobNum, string clientName) {
        // initialize our file for output
        FILE * pFile;
        sem_wait(&fileMutex);
        // get unix time in milliseconds
        double epochTimeMilli = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
        endTime = epochTimeMilli;
        // append to output file
        pFile = fopen(outputFileName.c_str(),"a");
        // log to our file if we're sleeping or sending work
        if (event.at(0) == 'T') {
            fprintf(pFile, "%.2f: #%3d (%c%3d) from %s\n", epochTimeMilli/1000, jobNum, event.at(0), n, clientName.c_str());
        }
        else {
            fprintf(pFile, "%.2f: #%3d (Done) from %s\n", epochTimeMilli/1000, jobNum, clientName.c_str());
        }
        
        // close and release
        fclose(pFile);
        sem_post(&fileMutex);
    }

    /**
     * @brief takes a the number of transactions and writes it out to output file
     * 
     * @param n         the number of transactions done
     * @param summary   the map of client name to number of jobs they gave
     * @param totalTime the total time it took to complete all jobs from all clients
     */
    void appendSummary(map<string, int>summary, int n) {
        // calculate total elapsed time
        double totalTimeMilli = this->endTime - this->startTime;

        // setup file for appending
        FILE * pFile;
        pFile = fopen(outputFileName.c_str(),"a");
        fprintf(pFile, "\nSUMMARY\n");

        // print each client and number of transactions
        for (const auto& kv : summary) {
            fprintf(pFile, "%4d transactions from %s\n", kv.second, kv.first.c_str());
        }

        fprintf(pFile, "%.1f transactions/sec (%d/%.2f)", (double)n/(totalTimeMilli/1000), n, totalTimeMilli/1000);
        // close file
        fclose(pFile);
    }
};
