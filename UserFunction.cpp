#include "UserFunction.h"
#include "Interpreter.h"
void printVector1(vector<string> v) {
    for (int i = 0; i < v.size(); i++) {
       // cout << i << "-" << v.at(i) << "-" << endl;
    }
    std::cout << std::endl;
}

void UserFunction::call(vector<double> parameters, ofstream& outputFile){
    ifstream f;
    Interpreter *i = new Interpreter();
    i->functionMap = globalMap;
    //std::cout << "After map assignment" << std::endl;
    int j = 0;
    while(j < parameters.size()){
        i->variableMap[arguments[j]] = parameters[j];
        j++;
    }
    printVector1(functionDefinition);
    for(string s : functionDefinition){
       // std::cout << s << std::endl;
        i->interpretLine(s, outputFile, f, true);
        if(getLineType(s) == RETURN) break;
    }
    thisReturn = i->returnValue;
}

void UserFunction::tostring(){
    for(string s : functionDefinition){
       // std::cout << s << std::endl;
    }
}

UserFunction::UserFunction(map<string, UserFunction> thisMap){
        globalMap = thisMap;
}

