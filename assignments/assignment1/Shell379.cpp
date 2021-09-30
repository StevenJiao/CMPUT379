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
#include <sys/signal.h>
using namespace std; 

int LINE_LENGTH = 100; // Max # of characters in an input line
int MAX_ARGS = 7; // Max number of arguments to a command
int MAX_LENGTH = 20; // Max # of characters in an argument
int MAX_PT_ENTRIES = 32 ;// Max entries in the Process Table

pid_t mainPid;
struct rusage myUsage;
int status;

// std::string cmdExec(const char* cmd) {
//     std::array<char, 128> buffer;
//     std::string result;
//     std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
//     if (!pipe) {
//         throw std::runtime_error("popen() failed!");
//     }
//     while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
//         result += buffer.data();
//     }
//     return result;
// }

string cmdExec(string cmd) {
    FILE * p; 
    array<char, 128> buffer;
    string result;
    if ((p = popen(cmd.c_str(), "r")) == NULL) {
        perror( "Couldn't open pipe\n");
    }
    else {
        while (fgets(buffer.data(), buffer.size(), p) != nullptr) {
            result += buffer.data();
        }
        pclose(p);
    }
    return result;
}

vector<string> stringToArgs(string s) {
    stringstream ss(s);
    vector<string> args;
    string line;
    while (getline(ss, line, ' ')) {
        args.push_back(line);
    }
    return args;
}

void argsToCharC(vector<string> args, char ** argv1) {
    for (int i = 0; i < args.size(); i++) {
        argv1[i] = strdup(args[i].c_str());
    }
    argv1[args.size()] = NULL;
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
        vector<string> args = stringToArgs(initialInput);
        bool hasAmpersand = initialInput.find("&") == string::npos ? false : true;

        if (args[0].compare("exit") == 0) {
            getrusage(RUSAGE_SELF, &myUsage);
            cout << "Resources used: " << endl;
            printf("User Time =%10ld seconds\n", myUsage.ru_utime.tv_sec);
            printf("Sys  Time =%10ld seconds\n", myUsage.ru_stime.tv_sec);

            string cmd = "pgrep -P " + to_string(mainPid);
            string output_lines_from_ps = cmdExec(cmd.c_str());
            cout << output_lines_from_ps << " " << mainPid << endl;
            exit(0);
        }
        else if (args[0].compare("jobs") == 0) {
            string cmd = "ps -o pid,s,times:3=SEC,command --ppid " + to_string(mainPid) + " | grep -v sh";
            string output_lines_from_ps = cmdExec(cmd);
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

            printf("Processes =%10d active\n", (counter - 1));

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
            kill(stoi(args[1]), SIGKILL);
        }
        else if (args[0].compare("resume") == 0) {
            if (args[1].length() == 0) {
                cout << "invalid args for command" << endl;
                continue;
            }
            kill(stoi(args[1]), SIGCONT);
        }
        else if (args[0].compare("sleep") == 0) {
            if (args[1].length() == 0) {
                cout << "invalid args for command" << endl;
                continue;
            }
            sleep(stoi(args[1]));
        }
        else if (args[0].compare("suspend") == 0) {
            if (args[1].length() == 0) {
                cout << "invalid args for command" << endl;
                continue;
            }
            kill(stoi(args[1]), SIGSTOP);
        }
        else if (args[0].compare("wait") == 0) {
            if (args[1].length() == 0) {
                cout << "invalid args for command" << endl;
                continue;
            }
            int i;
            pid_t p = stoi(args[1]);
            cout << "Start Waiting for " << p << endl;
            waitpid(p, &i, 0);
            cout << "Done Waiting" << endl;
        }
        else {
            pid_t rc = fork();

            if (rc < 0) { // fork failed; exit
                fprintf(stderr, "fork failed\n");
                exit(1);
            }
            else if (rc == 0) { // child goes off to execute its process and finishes whenever
                char *argv1[args.size()];
                argsToCharC(args, argv1);
                if (execvp(argv1[0], argv1) < 0) {
                    perror("SHELL379");
                }
                _exit(0);
            }
            else { // parent
                if (!hasAmpersand) {
                    int j;
                    wait(&j);
                }
            }
        }
        cout << endl;
    }

    return 0;
}