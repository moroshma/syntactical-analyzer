//
// Created by moroshma on 11.11.23.
//
#include "handler.h"
#include <cstring>
#include <iostream>
#include <set>
#include <string>
// TODO Создать map с зарезервированными типами по типу char* : bool
// TODO Создать map с новыми типами по типу char* : bool
// TODO Создать map с переменными, которые были созданы
using namespace std;
class dataClass {
private:
  enum error {
    MALLOC_ERROR,
    INVALID_BRACETS_VISION,
    UNDEFIND_ERROR,
    INVALID_DEFULT_INITIAL
  };
  set<string> baseTypeSet;
  string newType;
  int inputBaseType();

  void checkValid(string line);

  bool check_in_class(string line);
  bool check_out_class(string line);
  void  print_error(string line, size_t current_line, error err);
  size_t checkConst(string line, string nameClass);

  size_t starLineType = 0;
  size_t endLineType = 0;
  size_t countConstructor = 0;
  size_t current_line = 0;

public:
  int findType(size_t start, string file);
  dataClass() { inputBaseType(); }
  string addSpaceInStr(string str);
};

int dataClass::inputBaseType() {
  int ret = SUCCESS;
  FILE *fs = fopen("test", "r");
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
    size_t countLine = 0;

    while ((read = getline(&line, &len, fs)) != -1) {
      countLine++;
      if (line[strlen(line) - 1] == '\n') {
        line[strlen(line) - 1] = '\0';
      }
      string s = line;
      s = addSpaceInStr(s);
      string delimiterSpace = " ";
      string delimiterBracket = "{";
      size_t pos = 0;
      string token;

      while (!newType.length() &&
             ((pos = s.find(delimiterSpace)) != string::npos ||
              (pos = s.find(delimiterBracket)) != string::npos)) {
        token = s.substr(0, pos);
        if (!flag && "class" == token) {
          flag = true;
        } else if (flag && " " != token) {
          this->newType = token;
          this->starLineType = countLine;
          flag = false;
          cout << token << endl;
        }
        s.erase(0, pos + delimiterSpace.length());
      }

      if (newType.length()) {
        size_t countConst = checkConst(line, this->newType);
        if (countConst) {
            checkValid(line);
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

void dataClass::checkValid(string line) {

    size_t findet_open_bracket = line.find("(");
    size_t findet_vision_bracket = line.find("::");

    if (string::npos == findet_open_bracket && string::npos == findet_vision_bracket) {
        print_error(line, this ->current_line, INVALID_BRACETS_VISION);
    } else if (current_line > endLineType) {
        check_out_class(line);
    } else {
        check_in_class(line);
    }
  }


bool dataClass::check_in_class(string line) { return false; }

bool dataClass::check_out_class(string line) { return false; }

void dataClass::print_error(string line, size_t current_line, dataClass::error err) {
    switch (err) {
        case INVALID_BRACETS_VISION: {
            cout << "In line: " << current_line + 1 << "error in quotes or scope operator line: " << line << endl;
            break;
        }
        case MALLOC_ERROR:
            cout << "Memory error ＼(￣▽￣)／" << endl;
            break;
        case UNDEFIND_ERROR:
            cout << "Undefined  error (╯°益°)╯彡┻━┻ LINE: "<<current_line + 1 <<  "string: " << line << endl;
            break;

    }

}

int main() {
  int ret = SUCCESS;
  dataClass dc;

  dc.findType(0, "test");

  return ret;
}