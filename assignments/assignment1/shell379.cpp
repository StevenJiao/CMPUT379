#include "shell379.hpp"

Shell379::Shell379() {
    mainPid = getpid();
    cout << "Welcome to SHELL379 - Syjiao" << endl;
}

Shell379::~Shell379() {}

/*
* The main loop for running Shell379
*/
void Shell379::run() {
    // keep looking for inputs after executing commands forever until exit
    while (true) {
        cout << "SHELL379: ";

        // prompt for our input
        string initialInput;
        getline(cin, initialInput);

        // split input by spaces into vector
        vector<string> args = stringToArgs(initialInput);

        // parse for input and output files
        string outputFileName;
        string inputFileName;
        for (int i = 0; i < args.size(); i++) {
            string s = args.at(i);
            if (s.at(0) == '<') {
                inputFileName = s.substr(1);
            }
            else if (s.at(0) == '>') {
                outputFileName = s.substr(1);
            }
        }

        // flag for ampersand
        bool hasAmpersand = initialInput.find("&") == string::npos ? false : true;
        
        if (args[0].compare("exit") == 0) { // exit command
            cout << endl;
            
            // cleanup child processes
            killAllChildProcesses(mainPid);

            // show our usage
            showUsage(args[0]);
            cout << endl;

            exit(0);
        }
        else if (args[0].compare("jobs") == 0) { // jobs command
            cout << endl;
            // show current jobs and job number
            showFormattedJobs();
            // show current usage
            showUsage(args[0]);
            cout << endl;
        }
        else if (args[0].compare("kill") == 0) { // kill command
            // argument check then send kill signal
            if (!hasValidArgs(args)) continue;
            int childPid = stoi(args[1]);
            kill(childPid, SIGKILL);

            // 'wait' for the child to collect usage info
            int i;
            waitpid(childPid, &i, 0);
        }
        else if (args[0].compare("resume") == 0) { // resume command
            // error check then send resume signal
            if (!hasValidArgs(args)) continue;
            kill(stoi(args[1]), SIGCONT);
        }
        else if (args[0].compare("sleep") == 0) { // sleep command
            // error check then sleep
            if (!hasValidArgs(args)) continue;
            sleep(stoi(args[1]));
        }
        else if (args[0].compare("suspend") == 0) { // suspend command
            // error check and send stop signal
            if (!hasValidArgs(args)) continue;
            kill(stoi(args[1]), SIGSTOP);
        }
        else if (args[0].compare("wait") == 0) { // wait command
            // error check then try and wait for child process
            if (!hasValidArgs(args)) continue;
            int i;
            waitpid(stoi(args[1]), &i, 0);
        }
        else { // execute Unix commands
            // open input and output files and get their fids
            int inputFid = -1, outputFid = -1;
            if (inputFileName.size() > 0) {
                inputFid = open(inputFileName.c_str(), O_RDONLY, S_IRUSR | S_IWUSR);
            }
            if (outputFileName.size() > 0) {
                outputFid = open(outputFileName.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
            }

            // fork here for child to execvp
            pid_t rc = fork();

            if (rc < 0) { // fork failed
                fprintf(stderr, "fork failed\n");
                exit(1);
            }
            else if (rc == 0) { // child
                // format our command from vector<string> to char**
                char *argv1[args.size()];
                argsToCharC(args, argv1);

                // set stdin and stdout to our input and output files 
                if (inputFid != -1) {
                    dup2(inputFid, STDIN_FILENO);
                }
                if (outputFid != -1) {
                    dup2(outputFid, STDOUT_FILENO);
                }

                // execute our command
                if (execvp(argv1[0], argv1) < 0) {
                    perror("SHELL379");
                    _exit(0);
                }
            }
            else { // parent
                // if there is no ampersand then wait for our child process
                if (!hasAmpersand) {
                    int j;
                    waitpid(rc, &j, 0);
                }

                // close our input and output files
                if (inputFid != -1) {
                    close(inputFid);
                }
                if (outputFid != -1) {
                    close(outputFid);
                }
            }
        }
    }
}

/*
* Validation for arguments required for certain Shell379 commands
*/
bool Shell379::hasValidArgs(vector<string> args) {
    if (args.size() > 1 && args[1].size() > 0) {
        return true;
    }
    cout << "invalid args for command" << endl;
    return false;
}

/*
* Shows all child processes as formatted in the assignments requirements
*/
void Shell379::showFormattedJobs() {
    // call ps and format columns to assignment requirments
    // only show pids whose parent pid is mainPid
    // grep exclude unecessary info
    string cmd = "ps -o pid,s,times:3=SEC,command --ppid " + to_string(mainPid) + " | grep -v sh | grep -v defunct";
    string output_lines_from_ps = readCmd(cmd);
    cout << "Running processes: " << endl;

    stringstream ss(output_lines_from_ps);
    string to;
    int counter = 0;
    string header;
    // ugly formatting stuff
    while(getline(ss,to,'\n')) {
        // set the header at first count
        if (counter == 0) {
            header = " #  " + to + "\n";
        }
        else {
            // if we've reached 2nd count, print the header first
            if (counter == 1) {
                cout << header;
            }
            // set spacing if nums > 10 to make less ugly
            string space = (counter - 1 < 10) ? " " : "";
            cout << space << (counter - 1) << ": " << to << endl;
        }
        // count up processes
        counter++;
    }

    // counter is 0-starting counted so actual number of processes is counter--;
    printf("Processes =%10d active\n", (counter - 1));
}

/*
* Shows sys and user usage of cpu time as formatted by either using jobs or exit
*/
void Shell379::showUsage(string cmd) {
    // get usage
    getrusage(RUSAGE_CHILDREN, &myUsage);
    // header depends on if command is JOBS or EXIT
    string header = cmd.compare("jobs") == 0 ? "Completed processes:" : "Resources used:";
    cout << header << endl;
    // print seconds info for user and sys time usage
    printf("User Time =%10ld seconds\n", myUsage.ru_utime.tv_sec);
    printf("Sys  Time =%10ld seconds\n", myUsage.ru_stime.tv_sec);
}

/* 
* Kills all child processes of a parent using its pid
*/
void Shell379::killAllChildProcesses(int parentPid) {
    // get child process ids
    string cmd = "pgrep -P " + to_string(parentPid);
    string output_lines_from_ps = readCmd(cmd.c_str());
    stringstream ss(output_lines_from_ps);
    string childId;
    // process each line of child pids
    while (getline(ss, childId, '\n')) {
        int childIdNum = stoi(childId);
        // send kill signal
        kill(childIdNum, SIGKILL);
        // 'wait' to collect usage info
        int i;
        waitpid(childIdNum, &i, 0);
    }
}

/*
* By using the popen pipe, reads in a command's output and returns it as a string
*/
string Shell379::readCmd(string cmd) {
    FILE * p; 
    array<char, 128> buffer;
    string result;
    // pipe our command as read
    if ((p = popen(cmd.c_str(), "r")) == NULL) {
        perror( "Couldn't open pipe\n");
    }
    else {
        // read the data into result
        while (fgets(buffer.data(), buffer.size(), p) != nullptr) {
            result += buffer.data();
        }
        pclose(p);
    }
    return result;
}
