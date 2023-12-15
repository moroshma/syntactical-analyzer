//
// Created by moroshma on 11.11.23.
//
#include "handler.hpp"


size_t dataClass::checkConst(string line, const string &nameClass) {
    string s = std::move(line);

    string delimiterSpace = " ";
    string delimiterBracket = "\n";
    string delimiterCBracket = "(";

    size_t pos;
    string token;
    size_t countConst = 0;

    while ((pos = s.find(delimiterSpace)) != string::npos ||
           (pos = s.find(delimiterBracket)) != string::npos ||
           (pos = s.find(delimiterCBracket)) != string::npos) {

        token = s.substr(0, pos);
        if (nameClass == token) {
            countConst++;
        }
        s.erase(0, pos + delimiterSpace.length());
    }
    return countConst;
}

string dataClass::addSpaceInStr(string str) {
    string newSt;
    for (size_t i = 0; i < str.length(); i++) {
        if ('\n' == str[i] || '(' == str[i] || ')' == str[i] || '{' == str[i] ||
            '}' == str[i]) {
            newSt += " ";
            newSt += str[i];
            newSt += " ";
        } else {
            newSt += str[i];
        }
    }
    return newSt;
}

void dataClass::checkValid(const string &line, size_t countConst) {

    size_t findet_open_bracket = line.find('(');
    size_t findet_vision_bracket = line.find("::");

    if (string::npos == findet_open_bracket &&
        string::npos == findet_vision_bracket) {
        print_error(line, this->current_line, INVALID_BRACETS_VISION);
    } else {
        if (countConst != 1 && line.find("(") >  line.find(this -> newType)) {
            print_error(line, this->current_line, MANY_CONST);
        } else {
            check_in_class(line);
        }
    }
}

void dataClass::check_in_class(const string &line) {
    size_t pos;
    string token;
    string s = line.substr(line.find('(') + 1);

    set<string> name_var;

    bool trigger_args = false;

    while (trim(s," ") != "" && trim(s," ").find(':') != 0 && trim(s," ").find(';') != 0){

        pos = min(s.find(')'), s.find(','));

        if (pos != string::npos) {
            s = trim(s, " ");
            size_t space_index = s.find(' ');
            if (space_index!= string::npos && baseTypeSet.count(s.substr(0, space_index))){
                s.erase(0,  s.find(' '));
            } else {
                print_error(line, this->current_line, INVALID_VARIBLE_INIT);
                return;
            }

            size_t end_init = min(s.find(")"), s.find(","));
            string new_var = trim(s.substr(0, end_init), " ,){");

            if (new_var.empty()) {
                print_error(line, this->current_line, INVALID_VARIBLE_INIT);
                break;
            } else {
                name_var.insert(new_var);
                trigger_args = true;
            }
            s.erase(0, end_init + 1);
        }
    }


    auto find_close_bracket =  s.find(')');
    if (!trigger_args && find_close_bracket == string::npos ) {
        print_error(line, this->current_line, INVALID_BRACETS_VISION);
        return;
    } else if (!trigger_args && !baseTypeSet.count(s.substr(0, s.find(' '))) ) {
        print_error(line, this->current_line, INVALID_VARIBLE_INIT);
        return;
    }


    size_t find_list_init = s.find(':');
    if (find_list_init != string::npos) {
        s.erase(0, find_list_init + 1);
        while (!s.empty()) {

            size_t start_brackets = s.find('{');
            size_t end_brackets = s.find('}');

            if (start_brackets != string::npos && end_brackets != string::npos) {
                string st_cut_bracket = trim(s.substr(0, start_brackets), " ");
                if (start_brackets > end_brackets || st_cut_bracket.empty()) {
                    print_error(line, current_line, INITIALIZATION_LIST_ERROR);
                    break;
                } else {
                    string type =
                            s.substr(start_brackets + 1, end_brackets - start_brackets - 1);
                    if (!name_var.count(trim(type, " "))) {
                        print_error(line, current_line, INITIALIZATION_LIST_ERROR);
                        break;
                    }
                }
                s.erase(0, end_brackets + 1);
            } else {
                break;
            }
        }
    }
}

void dataClass::print_error(const string &line, size_t current_line,
                            error err) {
    switch (err) {
        case INVALID_BRACETS_VISION:
            cout << "In line: " << current_line + 1
                 << " error in quotes or scope operator line: *ฅ^•ﻌ•^ฅ*" << endl;
            break;
        case INVALID_VARIBLE_INIT:
            cout << "INVALID VARIABLE INIT (☞ ͡° ͜ʖ ͡°)☞ line: " << line << endl;
            break;
        case MALLOC_ERROR:
            cout << "Memory error ＼(￣▽￣)／" << endl;
            break;
        case UNDEFIND_ERROR:
            cout << "Undefined  error ¯\\_(ツ)_/¯ LINE: " << current_line + 1
                 << " string: " << line << endl;
            break;
        case MANY_CONST:
            cout << "more than 1 constructors in 1 line (╯°益°)╯彡┻━┻ LINE: " << current_line + 1
                 << " string: " << line << endl;
            break;
        case INITIALIZATION_LIST_ERROR:
            cout << "INITIALIZATION LIST ERROR (•_•) LINE: " << current_line + 1
                 << " string: " << line << endl;
            break;
        case OPEN_FILE_ERROR:
            cout << "can't open file ʕ•͡-•ʔ " << endl;
            break;
        case INVALID_DEFULT_INITIAL:
            cout << "INVALID DEFAULT INITIAL (◣ _ ◢) LINE: " << current_line + 1
                 << " string: " << line << endl;
            break;

        default:
            cout << "Undefined  error (╯°益°)╯彡┻━┻ LINE: " << current_line + 1
                 << " string: " << line << endl;
            break;
    }
}

string dataClass::trim(const string &str, const string &chars) {
    size_t first = str.find_first_not_of(chars);
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(chars);
    return str.substr(first, (last - first + 1));
}


