#ifndef CSE250_FALL2015_USERFUNCTION_H
#define CSE250_FALL2015_USERFUNCTION_H

#include <vector>
#include <string>
#include <map>

class Interpreter;

using namespace std;

// This class is not required, but might be helpful in writing your interpreter
class UserFunction {
public:
    vector<string> functionDefinition;
    vector<string> arguments;
    map<string, UserFunction> globalMap;
    void call(vector<double> parameters, ofstream&);
    UserFunction(map<string, UserFunction>);
    double thisReturn = -1;
    void tostring();
};

#endif //CSE250_FALL2015_USERFUNCTION_H
