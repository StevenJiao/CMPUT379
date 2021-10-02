#include <string>
#include <string.h> // strdup complains without this older C header...
#include <vector>
#include <array>
#include <sstream>
#include <sys/types.h>
#include <signal.h>
using namespace std;

vector<string> stringToArgs(string s);
void argsToCharC(vector<string> args, char ** argv1);
