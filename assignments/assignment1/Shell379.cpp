#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <array>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <unistd.h>
using namespace std; 

int LINE_LENGTH = 100; // Max # of characters in an input line
int MAX_ARGS = 7; // Max number of arguments to a command
int MAX_LENGTH = 20; // Max # of characters in an argument
int MAX_PT_ENTRIES = 32 ;// Max entries in the Process Table

pid_t mainPid;

std::string cmdExec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int main(int argc, char const *argv[]) {
    bool continueRunning = true;
    mainPid = getpid();
    cout << "Welcome to Shell379 - Syjiao" << endl;

    while (continueRunning) {
        cout << "Please enter a command" << endl;
        string initialInput;
        getline(cin, initialInput);

        stringstream ss(initialInput);
        vector<string> args;
        string line;
        while (getline(ss, line, ' ')) {
            args.push_back(line);
        }

        if (args[0].compare("exit") == 0) {
            // TODO; kill jobs
        }
        else if (args[0].compare("jobs") == 0) {
            string cmd = "ps -o pid,s,times:3=SEC,command | grep -v bash | grep -v defunct | grep -v " + to_string(mainPid) + " | grep -v grep | grep -v ps";
            string output_lines_from_ps = cmdExec(cmd.c_str());

            stringstream ss(output_lines_from_ps);
            string to;
            int counter = 0;
            while(getline(ss,to,'\n')) {
                if (counter == 0) {
                    cout << "#: " << to << endl;
                    counter++;
                }
                else {
                    cout << (counter - 1) << " " << to << endl;
                    counter++;
                }
            }

            cmd = "ps -o pid,cmd --no-headers | grep -v bash | grep -v defunct | grep -v " + to_string(mainPid) + " | grep -v grep | grep -v ps | wc -l";
            output_lines_from_ps = cmdExec(cmd.c_str());
            cout << output_lines_from_ps << endl;
        }
    }

    return 0;
}