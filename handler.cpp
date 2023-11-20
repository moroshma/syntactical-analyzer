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
  set<string> baseTypeSet;
  string newType;
  int inputBaseType();
  size_t checkConst(string line, string nameClass);

  size_t starLineType = 0;
  size_t endLineType = 0;
  size_t countConstructor = 0;



public:
  int findType(size_t start, string file);

  dataClass() { inputBaseType(); }

  // Имя, все переменные доступные на данный момент, хэш сумма конструктора.
  // Добавить функцию для считывания данных из файла
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

      string delimiterSpace = " ";
      string delimiterBracket = "{";

      size_t pos = 0;
      string token;

      while (!newType.length() && ((pos = s.find(delimiterSpace)) != string::npos ||
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
        s.erase(0, pos + delimiterSpace.length() );
      }
        if (newType.length()) {
            size_t countConst = checkConst(line, this -> newType);
            cout << countConst << endl;

        }
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
                                 (pos = s.find(delimiterCBracket)) != string::npos   ) {

        token = s.substr(0, pos);
        if (nameClass == token) {
            countConst++;
        }


        s.erase(0, pos + delimiterSpace.length() );
    }

    return countConst;
}

int main() {
  int ret = SUCCESS;
  dataClass dc;

  dc.findType(0, "test");

  return ret;
}