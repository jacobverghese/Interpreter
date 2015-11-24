#ifndef UNTITLED_INTERPRETER_H
#define UNTITLED_INTERPRETER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <stack>

#include "Parser.h"
#include "UserFunction.h"

using namespace std;

class Interpreter {

public:

    map<string, double> variableMap;
    map<string, UserFunction> functionMap;

    bool isFunction(string s) {
        vector<string> list;
        while(!s.empty()){
            list.push_back(getNextSymbol(s));
        }
        return functionMap.count(list[0]) == 1;
    }

    stack<bool> conditionStack;
    void interpretScript(ifstream &, ofstream &);

    void interpretLine(string, ofstream &, ifstream &, bool);

    double evaluateInfix(vector<string>);

    bool precedence(stack<string> *);

    double returnValue;

};

#endif //UNTITLED_INTERPRETER_H
