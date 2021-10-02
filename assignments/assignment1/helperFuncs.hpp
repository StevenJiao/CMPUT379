#include <string>
#include <string.h> // strdup complains without this older C header...
#include <vector>
#include <array>
#include <sstream>
using namespace std;

string readCmd(string cmd);
vector<string> stringToArgs(string s);
void argsToCharC(vector<string> args, char ** argv1);