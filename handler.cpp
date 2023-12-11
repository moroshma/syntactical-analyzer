//
// Created by moroshma on 11.11.23.
//
#include "handler.h"

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

  if (NULL == fs) {
    print_error("", 0, OPEN_FILE_ERROR);
    ret = OPEN_FILE_ERROR;
  } else {

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

      string new_var = trim(s.substr(0, end_init), " ,){");

      if (new_var.empty()) {
        print_error(line, this->current_line, INVALID_VARIBLE_INIT);
        break;
      } else {
        name_var.insert(new_var);
      }
      s.erase(0, end_init + 1);
    }
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

void dataClass::check_out_class(string line) {

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

      string new_var = trim(s.substr(0, end_init), " ,){");

      if (new_var.empty()) {
        print_error(line, this->current_line, INVALID_VARIBLE_INIT);
        break;
      } else {
        name_var.insert(new_var);
      }
      s.erase(0, end_init + 1);
    }
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

void dataClass::print_error(string line, size_t current_line, error err) {
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
    cout << "Undefined  error (╯°益°)╯彡┻━┻ LINE: " << current_line + 1
         << " string: " << line << endl;
    break;
  case MANY_CONST:
    cout << "more than 1 constructors (╯°益°)╯彡┻━┻ LINE: " << current_line + 1
         << " string: " << line << endl;
    break;
  case INITIALIZATION_LIST_ERROR:
    cout << "more than 1 constructors (•_•) LINE: " << current_line + 1
         << " string: " << line << endl;
    break;
  case OPEN_FILE_ERROR:
    cout << "can't open file ʕ•͡-•ʔ LINE: " << current_line + 1
         << " string: " << line << endl;
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

int main(int argc, char **argv) {

  dataClass dc;

  if (argc != 2) {
    dc.print_error("", 0, OPEN_FILE_ERROR);
  } else {
    dc.findType(0, argv[1]);
  }

  return 0;
}
