#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <array>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <unistd.h>
#include <sys/resource.h>
#include <boost/algorithm/string/trim.hpp>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std; 

int LINE_LENGTH = 100; // Max # of characters in an input line
int MAX_ARGS = 7; // Max number of arguments to a command
int MAX_LENGTH = 20; // Max # of characters in an argument
int MAX_PT_ENTRIES = 32 ;// Max entries in the Process Table

pid_t mainPid;
struct rusage myUsage;

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
        cout << "SHELL379: ";
        string initialInput;
        getline(cin, initialInput);
        cout << endl;
        stringstream ss(initialInput);
        vector<string> args;
        string line;
        while (getline(ss, line, ' ')) {
            args.push_back(line);
        }

        if (args[0].compare("exit") == 0) {
            cout << "Exiting" << endl;
            exit(0);
        }
        else if (args[0].compare("jobs") == 0) {
            string cmd = "ps -o pid,s,times:3=SEC,command | grep -v bash | grep -v defunct | grep -v " + to_string(mainPid) + " | grep -v grep | grep -v ps";
            string output_lines_from_ps = cmdExec(cmd.c_str());

            cout << "Running processes: " << endl;

            stringstream ss(output_lines_from_ps);
            string to;
            int counter = 0;
            while(getline(ss,to,'\n')) {
                if (counter == 0) {
                    cout << " #  " << to << endl;
                }
                else {
                    string space = (counter - 1 < 10) ? " " : "";
                    cout << space << (counter - 1) << ": " << to << endl;
                }
                counter++;
            }

            cmd = "ps -o pid,cmd --no-headers | grep -v bash | grep -v defunct | grep -v " + to_string(mainPid) + " | grep -v grep | grep -v ps | wc -l";
            output_lines_from_ps = cmdExec(cmd.c_str());
            boost::algorithm::trim(output_lines_from_ps);
            printf("Processes =%10s active\n", output_lines_from_ps.c_str());

            getrusage(RUSAGE_SELF, &myUsage);
            cout << "Completed Processes:" << endl;
            printf("User Time =%10ld seconds\n", myUsage.ru_utime.tv_sec);
            printf("Sys  Time =%10ld seconds\n", myUsage.ru_stime.tv_sec);
        }
        else if (args[0].compare("kill") == 0) {
            if (args[1].length() == 0) {
                cout << "invalid args for command" << endl;
                continue;
            }
            string result = cmdExec(initialInput.c_str());
            boost::algorithm::trim(result);
            cout << result;
        }
        else if (args[0].compare("resume") == 0) {
            if (args[1].length() == 0) {
                cout << "invalid args for command" << endl;
                continue;
            }
            string cmd = "kill -CONT " + args[1];
            string result = cmdExec(cmd.c_str());
            boost::algorithm::trim(result);
            cout << result;
        }
        else if (args[0].compare("sleep") == 0) {
            if (args[1].length() == 0) {
                cout << "invalid args for command" << endl;
                continue;
            }
            string result = cmdExec(initialInput.c_str());
            boost::algorithm::trim(result);
            cout << result;
        }
        else if (args[0].compare("suspend") == 0) {
            if (args[1].length() == 0) {
                cout << "invalid args for command" << endl;
                continue;
            }
            string cmd = "kill -STOP " + args[1];
            string result = cmdExec(cmd.c_str());
            boost::algorithm::trim(result);
            cout << result;
        }
        else if (args[0].compare("wait") == 0) {
            if (args[1].length() == 0) {
                cout << "invalid args for command" << endl;
                continue;
            }
            int i;
            pid_t p = stoi(args[1]);
            cout << "Start Waiting" << endl;
            waitpid(p , &i, WUNTRACED);
            cout << "Done Waiting" << endl;
        }
        else {
            string result = cmdExec(initialInput.c_str());
            boost::algorithm::trim(result);
            cout << result;
        }
        cout << endl;
    }

    return 0;
}