#include "tands.hpp"
#include <queue>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char const *argv[]) {
    queue<string> myQ;

    // get the input either from commandline or file
    string line;
    while (getline(cin, line)) {
        // add to our queue
        myQ.push(line);
    }

    // get the number of threads and output file number if given
    int numThreads = *argv[1];
    int maxQSize = 2*numThreads;
    string outputFileNum = argc == 3 ? argv[2] : "";
    string outputFileName = "prodcon." + outputFileNum + ".log";

    // spawn our threads
    cout << outputFileName << endl;
    return 0;
}
