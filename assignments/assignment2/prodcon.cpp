#include "tands.hpp"
#include <queue>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char const *argv[]) {
    queue<string> myQ;
    int test;
    string line;
    while (getline(cin, line)) {
        myQ.push(line);
    }
    cout << argv[1] << endl;
    cout << "Number of args: " << argc << endl;
    return 0;
}
