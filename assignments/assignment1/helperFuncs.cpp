#include "helperFuncs.hpp"

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
    // make sure last spot is NULL for exec
    argv1[args.size()] = NULL;
}
