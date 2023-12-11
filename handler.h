//
// Created by moroshma on 11.11.23.
//

#ifndef SYNTACTICAL_ANALYZER_HANDLER_H
#define SYNTACTICAL_ANALYZER_HANDLER_H


#include <algorithm>
#include <bits/stdc++.h>
#include <cstring>
#include <iostream>
#include <set>
#include <string>

enum error {
    SUCCESS,
    MALLOC_ERROR,
    INVALID_BRACETS_VISION,
    UNDEFIND_ERROR,
    INVALID_DEFULT_INITIAL,
    INVALID_VARIBLE_INIT,
    MANY_CONST,
    INITIALIZATION_LIST_ERROR,
    OPEN_FILE_ERROR
};

using namespace std;
class dataClass {
private:

    set<string> baseTypeSet;
    string newType;
    int inputBaseType();

    void check_in_class(string line);
    void check_out_class(string line);

    string trim(const std::string &str, const std::string &chars);
    size_t checkConst(string line, string nameClass);
    void checkValid(string line, size_t countConst);

    size_t starLineType = 0;
    size_t endLineType = 0;
    size_t countConstructor = 0;
    size_t current_line = 0;
    long int count_breakets = 0;

public:

    void print_error(string line, size_t current_line, error err);
    int findType(size_t start, string file);
    dataClass() { inputBaseType(); }
    string addSpaceInStr(string str);
};


#endif // SYNTACTICAL_ANALYZER_HANDLER_H
