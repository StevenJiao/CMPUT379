#include <unistd.h>
#include <iostream>
#include <sstream>
using namespace std; 
int main(int argc, char const *argv[])
{
    if (argc > 1) {
        stringstream strValue;
        strValue << argv[1];

        unsigned int intValue;
        strValue >> intValue;
        sleep(intValue);
    }
    else {
        sleep(10);
    }
    return 0;
}
