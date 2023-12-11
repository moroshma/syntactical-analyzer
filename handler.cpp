//
// Created by moroshma on 11.11.23.
//
#include "handler.h"
#include <algorithm>
#include <bits/stdc++.h>
#include <cstring>
#include <iostream>
#include <set>
#include <string>

using namespace std;

class dataClass {
private:
  enum error {
    MALLOC_ERROR,
    INVALID_BRACETS_VISION,
    UNDEFIND_ERROR,
    INVALID_DEFULT_INITIAL,
    INVALID_VARIBLE_INIT,
    MANY_CONST
  };
  set<string> baseTypeSet;
  string newType;
  int inputBaseType();

  void check_in_class(string line);
  void check_out_class(string line);
  void print_error(string line, size_t current_line, error err);
  string trim(const std::string &str, const std::string &chars);
  size_t checkConst(string line, string nameClass);
  void checkValid(string line, size_t countConst);

  size_t starLineType = 0;
  size_t endLineType = 0;
  size_t countConstructor = 0;
  size_t current_line = 0;
  long int count_breakets = 0;

public:
  int findType(size_t start, string file);
  dataClass() { inputBaseType(); }
  string addSpaceInStr(string str);
};

int dataClass::inputBaseType() {
  int ret = SUCCESS;
  FILE *fs = fopen("type", "r");
  char *line = NULL;
  ssize_t read;
  size_t len = 0;
  if (NULL != fs) {
    while ((read = getline(&line, &len, fs)) != -1) {
      size_t lenLine = strlen(line);
      if (line[lenLine - 1] == '\n') {
        line[lenLine - 1] = '\0';
      }
      this->baseTypeSet.insert(line);
    }
    free(line);
  } else {
    ret = OPEN_FILE_ERROR;
  }
  return ret;
}

int dataClass::findType(size_t start, string file) {
  int ret = SUCCESS;

  FILE *fs = fopen(file.c_str(), "r");
  char *line = NULL;
  ssize_t read;
  size_t len = 0;

  if (NULL != fs) {
    bool flag = false;
    size_t countBrecket = 0;
    bool findet_end_type = false;
    while ((read = getline(&line, &len, fs)) != -1) {
      if (line[strlen(line) - 1] == '\n') {
        line[strlen(line) - 1] = '\0';
      }

      string s = line;
      s = addSpaceInStr(s);
      string delimiterSpace = " ";
      string delimiterBracket = "{";
      size_t pos = 0;
      string token;

      this->count_breakets += count(s.begin(), s.end(), '{');
      this->count_breakets -= count(s.begin(), s.end(), '}');

      while (!newType.length() &&
             ((pos = s.find(delimiterSpace)) != string::npos ||
              (pos = s.find(delimiterBracket)) != string::npos)) {
        token = s.substr(0, pos);

        if (!flag && "class" == token) {
          flag = true;
        } else if (flag && " " != token) {
          this->newType = token;
          this->starLineType = this->current_line;
          this->baseTypeSet.insert(newType);
          flag = false;
          cout << token << endl;
        }
        s.erase(0, pos + delimiterSpace.length());
      }

      if (!newType.empty() && !findet_end_type) {
        this->endLineType = this->current_line;
        if (!count_breakets) {
          findet_end_type = true;
        }
      }

      if (!newType.empty()) {
        size_t countConst = checkConst(line, this->newType);
        if (countConst) {
          checkValid(line, countConst);
        }
      }
      this->current_line++;
    }

    free(line);

  } else {
    ret = OPEN_FILE_ERROR;
  }
  return ret;
}

size_t dataClass::checkConst(string line, string nameClass) {
  string s = line;

  string delimiterSpace = " ";
  string delimiterBracket = "\n";
  string delimiterCBracket = "(";

  size_t pos = 0;
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
  string newSt = "";
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

void dataClass::checkValid(string line, size_t countConst) {

  size_t findet_open_bracket = line.find("(");
  size_t findet_vision_bracket = line.find("::");

  if (string::npos == findet_open_bracket &&
      string::npos == findet_vision_bracket) {

    print_error(line, this->current_line, INVALID_BRACETS_VISION);
  } else if (current_line > endLineType) {

    check_out_class(line);

  } else {
    if (countConst != 1) {
      print_error(line, this->current_line, MANY_CONST);
    } else {
      check_in_class(line);
    }
  }
}

void dataClass::check_in_class(string line) {

  size_t pos = 0;
  string token;
  string s = line.substr(line.find("(") + 1);

  set<string> name_var;

  for (auto ik = baseTypeSet.begin(); ik != baseTypeSet.end(); ++ik) {
    string delim = *ik;
    pos = s.find(delim);
    if (pos != string::npos) {
      token = s.substr(0, pos);
      s.erase(0, pos + delim.length());
      size_t end_init = min(s.find(")"), s.find(","));
      string new_var = trim(s, " ,)");
      if (new_var.empty()) {
        print_error(line, this->current_line, INVALID_VARIBLE_INIT);
        break;
      } else {
        name_var.insert(new_var);
      }
      s.erase(end_init + 1);
    }
  }
  if (s.at(0) == '(') {
    s.erase(1);
  }

  size_t find_list_init = s.find(":");
  if (find_list_init) {

  }

}

void dataClass::check_out_class(string line) {}

void dataClass::print_error(string line, size_t current_line,
                            dataClass::error err) {
  switch (err) {
  case INVALID_BRACETS_VISION:
    cout << "In line: " << current_line + 1
         << " error in quotes or scope operator line: " << line << endl;
    break;
  case INVALID_VARIBLE_INIT:
    cout << "INVALID VARIABLE INIT ＼(￣▽￣)／ line: " << line << endl;
    break;

  case MALLOC_ERROR:
    cout << "Memory error ＼(￣▽￣)／" << endl;
    break;
  case UNDEFIND_ERROR:
    cout << "Undefined  error (╯°益°)╯彡┻━┻ LINE: " << current_line + 1
         << "string: " << line << endl;
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

int main() {
  int ret = SUCCESS;
  dataClass dc;

  string st = "int sa) {";

  auto find = st.find("int");

  dc.findType(0, "test");

  return ret;
}
