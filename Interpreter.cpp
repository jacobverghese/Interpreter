#include "Interpreter.h"

void printVector(vector<string> v) {
    for (int i = 0; i < v.size(); i++) {
        //cout << i << "-" << v.at(i) << "-" << endl;
    }
    // std::cout << std::endl;
}

void printStack(stack<string> s) {
    while (!s.empty()) {
        // std::cout << s.top() << " ";
        s.pop();
    }
    // std::cout << std::endl;
}

void printStack(stack<double> s) {
    while (!s.empty()) {
        // std::cout << s.top() << " ";
        s.pop();
    }
    //std::cout << std::endl;
}


bool Interpreter::precedence(stack<string> *ops) {
    if (!ops->empty()) {
        char x = ops->top()[0];
        return x == '*' || x == '/';
    }
    return false;
}

bool isOperator(string s) {
    return s == "+" || s == "-" || s == "*" || s == "/";
}

double Interpreter::evaluateInfix(vector<string> v) {
    //std::cout << "In postfix" << std::endl;
    printVector(v);
    //std::cout << "--------lol---------" << std::endl;
    stack<string> *operators = new stack<string>();
    stack<double> *doubles = new stack<double>();
    vector<string> *postfix = new vector<string>();

    v.push_back(")");
    v.insert(v.begin(), "(");

    for (; !v.empty();) {
        string element = v[0];
        if (isalpha(element.front())) {
            postfix->push_back(to_string(variableMap.at(element)));
        } else if (isNumber(element)) {
            postfix->push_back(element);
        } else if (element == "(") {
            operators->push(element);
        } else if (isOperator(element)) {
            for (; precedence(operators);) {
                postfix->push_back(operators->top());
                operators->pop();
            }
            operators->push(element);
        } else {
            while (operators->top() != "(") {
                postfix->push_back(operators->top());
                operators->pop();
            }

            operators->pop();
        }
        v.erase(v.begin());
    }
    // std::cout << "postfix : ";
    printVector(*postfix);
    while (!postfix->empty()) {
        if (isOperator(postfix->at(0))) {
            char x = postfix->front().front();
            double first = doubles->top();
            doubles->pop();
            double second = doubles->top();
            doubles->pop();
            double result;
            switch (x) {
                case '+':
                    result = second + first;
                    break;
                case '-':
                    result = second - first;
                    break;
                case '*':
                    result = second * first;
                    break;
                case '/':
                    result = second / first;
                    break;
            }
            doubles->push(result);
        } else {
            doubles->push(stod(postfix->at(0)));
        }

        //std::cout << "doubles : ";
        printStack(*doubles);
        postfix->erase(postfix->begin());
    }
    //  std::cout << "-------------------------" << std::endl;
    if (doubles->size() == 1) return doubles->top();
    else return NULL;
}

void Interpreter::interpretScript(ifstream &inputFile, ofstream &outputFile) {

    string lineFromFile;

    int lineNumber = 0;
    //std::cout << "Interpret script";
    while (getline(inputFile, lineFromFile)) {
        lineNumber++;
        interpretLine(lineFromFile, outputFile, inputFile, true);
    }

}

pair<int, int> findParentheses(string s) {
    int opening, closing;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '(') opening = i;
        else if (s[i] == ')') closing = i;
    }
    return make_pair(opening, closing);
};

std::string getFileLine(ifstream& inputFile){
    string x;
    std::getline(inputFile, x);
    return x;
}


void Interpreter::interpretLine(string lineFromFile, ofstream &outputFile, ifstream &inputFile, bool _eval) {
    if (_eval) {
        LINE_TYPE lineType = getLineType(lineFromFile);
        vector<string> *tokens = new vector<string>();
        vector<string> quotes = tokenize(lineFromFile, "\"");
        vector<string> splitByEqual = tokenize(lineFromFile, "=");

        while (!lineFromFile.empty()) {
            tokens->push_back(getNextSymbol(lineFromFile));
        }

        printVector(*tokens);
        printVector(quotes);
        printVector(splitByEqual);
        //std::cout << "Outside switch";
        //std::cout << "Line type : " << (lineType) << std::endl;

        for (auto it = functionMap.begin(); it != functionMap.end(); it++) {
            //std::cout << it->first << std::endl;
            it->second.tostring();
        }

        //std::cout << std::endl;
        switch (lineType) {
            case DEFINE_VAR: {
                if (isFunction(splitByEqual[1])) {
                    // std::cout << "Passed isFunction";
                    vector<string> parts;
                    vector<double> parameters;
                    while (!splitByEqual[1].empty()) {
                        parts.push_back(getNextSymbol(splitByEqual[1]));
                    }
                    //std::cout << "Before print parts" << std::endl;
                    printVector(parts);

                    int index = 2;
                    while (index < parts.size() - 1) {
                        parameters.push_back(variableMap.at(parts.at(index)));
                        index += 2;
                    }

                    //std::cout << "Hey !" << std::endl;
                    UserFunction function = functionMap.at(parts.front());
                    function.call(parameters, outputFile);
                    double functionReturn = function.thisReturn;

                    //std::cout << "Return value :" << functionReturn << std::endl;
                    string x = tokens->at(0);

                    if (x == "var") {
                        variableMap.insert(make_pair(tokens->at(1), functionReturn));
                    } else {
                        variableMap[x] = functionReturn;
                    }
                } else {
                    // std::cout << "1";
                    string name = tokens->at(1);
                    tokens->erase(tokens->begin(), tokens->begin() + 3);
                    double value = evaluateInfix(*tokens);
                    variableMap.insert(make_pair(name, value));
                }
            }
                break;

            case USER_DEFINED: {
                if (splitByEqual.size() == 2 && isFunction(splitByEqual[1])) {
                    vector<string> parts;
                    vector<double> parameters;
                    while (!splitByEqual[1].empty()) {
                        parts.push_back(getNextSymbol(splitByEqual[1]));
                    }
                    //std::cout << "Before print parts" << std::endl;
                    printVector(parts);

                    int x = 2;
                    while (x < parts.size() - 1) {
                        parameters.push_back(variableMap.at(parts.at(x)));
                        x++;
                        x++;
                    }

                    //std::cout << "Hey !" << std::endl;
                    UserFunction function = functionMap.at(parts.front());
                    function.call(parameters, outputFile);
                    double returnValue = function.thisReturn;
                    variableMap[tokens->at(0)] = returnValue;
                }
                else if (splitByEqual.size() == 1) {
                    vector<string> parts;
                    vector<double> parameters;
                    while (!splitByEqual[0].empty()) {
                        parts.push_back(getNextSymbol(splitByEqual[0]));
                    }
                    // std::cout << "Before print parts" << std::endl;
                    printVector(parts);

                    int x = 2;
                    while (x < parts.size() - 1) {
                        parameters.push_back(variableMap.at(parts.at(x)));
                        x++;
                        x++;
                    }

                    // std::cout << "Hey !" << std::endl;
                    UserFunction function = functionMap.at(parts.front());
                    function.call(parameters, outputFile);
                } else {
                    // std::cout << "2";
                    string name = tokens->at(0);
                    tokens->erase(tokens->begin(), tokens->begin() + 2);
                    double result = evaluateInfix(*tokens);
                    variableMap[name] = result;
                }
            }
                break;
            case DOC_WRITE: {
                //std::cout << "3";
                quotes.size() > 1 ? outputFile << quotes.at(1) : outputFile << variableMap.at(tokens->at(2));
            }
                break;

            case FUNCTION_DEF: {
                int ifCounter = 0;
                UserFunction function(functionMap);
                int closing;
                for (int i = 0; i < tokens->size(); i++) { if (tokens->at(i) == ")") closing = i; }
                for (int i = 3; i < closing; i = i + 2) { function.arguments.push_back(tokens->at(i)); }
                printVector(function.arguments);
                string functionLine;

                while (getline(inputFile, functionLine)) {

                    LINE_TYPE type = getLineType(functionLine);
                    if (type == IF) {
                        ifCounter = ifCounter + 1;
                    }
                    if (type == END_BLOCK) {
                        if (ifCounter == 0) break;
                        else ifCounter = ifCounter - 1;
                    }
                    //if(getLineType(functionLine) == END_BLOCK) break;
                    //   std::cout << ifCounter << " " << functionLine << std::endl;
                    function.functionDefinition.push_back(functionLine);
                }
                functionMap.insert(make_pair(tokens->at(1), function));
            }
                break;

            case RETURN: {
                returnValue = variableMap.at(tokens->at(1));
            }
                break;

            case BLANK_LINE: {
                break;
            }

            case END_BLOCK: {
                if (!conditionStack.empty()) {
                    conditionStack.pop();
                }
                break;
            }
        }
    }
}
