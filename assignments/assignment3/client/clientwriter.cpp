#include <string>
#include <chrono>
#include <map>
#include <string>
#include <vector>

using namespace std;

/**
 * @brief a writer for appending log files for a client
 * 
 */
class clientwriter {
 private:
    // name of output file
    string outputFileName;
    // port number
    int portNum;
    // server address
    string serverAddr;

 public:
    /**
     * @brief Construct a new writer object using an outputfile name
     * 
     * This constructor will both set the outputfile name and 
     * initialize the file access semaphore
     * 
     * @param outputFileName the filename for appending our logs
     */
    clientwriter(string outputFileName, string serverAddr, int portNum) {
        // get the outputfile name
        this->outputFileName = outputFileName;
        this->serverAddr = serverAddr;
        this->portNum = portNum;

        // write our header to the file
        writeHeaderToFile();
    }

    ~clientwriter(){}

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

        // print our header info into output logs
        fprintf(pFile, "Using port %d\n", portNum);
        fprintf(pFile, "Using server address %s\n", serverAddr.c_str());
        fprintf(pFile, "Host %s\n", outputFileName.c_str());

        // close
        fclose(pFile);
    }

    /**
     * @brief Writes jobs to a outputfile that a client sends to a server, and what it recieves
     * 
     * @param event the event name as a string
     * @param n     the value of the input command
     */
    void writeToFile(string event, int n) {
        // initialize our file for output
        FILE * pFile;
        // get unix time in milliseconds
        double epochTimeMilli = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
        // append to output file
        pFile = fopen(outputFileName.c_str(),"a");
        // log to our file if we're sleeping or sending work
        if (event.at(0) != 'S') {
            string eventName = event.at(0) == 'T' ? "Send" : "Recv";
            fprintf(pFile, "%.2f: %s (%c%3d)\n", epochTimeMilli/1000, eventName.c_str(), event.at(0), n);
        }
        else {
            fprintf(pFile, "Sleep %d units\n", n);
        }
        // close and release
        fclose(pFile);
    }

    /**
     * @brief takes a the number of transactions and writes it out to output file
     * 
     * @param n the number of transactions done
     */
    void appendSummary(int n) {
        // setup file for appending
        FILE * pFile;
        pFile = fopen(outputFileName.c_str(),"a");
        // print the number of sent transactions
        fprintf(pFile, "Sent %d transactions", n);
        // close file
        fclose(pFile);
    }
};
