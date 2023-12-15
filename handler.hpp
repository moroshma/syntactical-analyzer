

#ifndef SYNTACTICAL_ANALYZER_HANDLER_HPP
#define SYNTACTICAL_ANALYZER_HANDLER_HPP


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

    void check_in_class(const string& line);
    static string trim(const std::string &str, const std::string &chars);
    static size_t checkConst(string line, const string& nameClass);
    size_t current_line = 0;


public:
    int valid_brackets(const string &line, size_t* num_str);
    static bool is_valid_bracket(FILE *fs, size_t * line);
    void checkValid(const string& line, size_t countConst);
    void print_error(const string& line, size_t current_line, error err);
    int findType(const string &file);
    dataClass() { inputBaseType(); }
    static string addSpaceInStr(string str);
};


#endif // SYNTACTICAL_ANALYZER_HANDLER_HPP


