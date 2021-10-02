#include "helperFuncs.hpp"

/*
* By using the popen pipe, reads in a command's output and returns it as a string
*/
string readCmd(string cmd) {
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

/*
* Splits a string using ' ' delimiter into vector of strings
*/
vector<string> stringToArgs(string s) {
    stringstream ss(s);
    vector<string> args;
    string line;
    while (getline(ss, line, ' ')) {
        args.push_back(line);
    }
    return args;
}

/*
* Populates a char * array from exactly what args looks like for execvp
*/
void argsToCharC(vector<string> args, char ** argv1) {
    for (int i = 0; i < args.size(); i++) {
        argv1[i] = strdup(args[i].c_str());
    }
    argv1[args.size()] = NULL;
}
