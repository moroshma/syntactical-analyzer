
#include "handler.hpp"
#include <utility>

int dataClass::inputBaseType() {
  int ret = SUCCESS;
  FILE *fs = fopen("type", "r");
  char *line = nullptr;
  size_t len = 0;
  if (nullptr != fs) {
    while ((getline(&line, &len, fs)) != -1) {
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

int dataClass::findType(const string &file) {
    int ret = SUCCESS;

    FILE *fs = fopen(file.c_str(), "r");
    char *line = nullptr;
    size_t len = 0;

    if (nullptr == fs) {
        print_error("", 0, OPEN_FILE_ERROR);
        ret = OPEN_FILE_ERROR;
    } else {

        bool flag = false;

        while ((getline(&line, &len, fs)) != -1) {
            if (line[strlen(line) - 1] == '\n') {
                line[strlen(line) - 1] = '\0';
            }
            bool while_trigger = false;
            string s = line;
            s = addSpaceInStr(s);

            string delimiterSpace = " ";
            string delimiterBracket = "{"/*}*/;
            size_t pos;
            string token;




            while (newType.empty() &&
                   ((pos = s.find(delimiterSpace)) != string::npos ||
                    (pos = s.find(delimiterBracket)) != string::npos)) {
                token = s.substr(0, pos);

                if (!flag && "class" == token) {
                    flag = true;
                } else if (flag && " " != token) {
                    this->newType = token;
                    this->baseTypeSet.insert(newType);
                    flag = false;
                    while_trigger = true;
                    cout <<"find type: " << token << endl;

                }
                s.erase(0, pos + delimiterSpace.length());
            }

            if (!newType.empty() && !while_trigger) {
                size_t countConst = checkConst(line, this->newType);
                if (countConst ) {
                    checkValid(line, countConst);
                }
            }
            this->current_line++;
        }

        free(line);
    }
    return ret;
}

int dataClass::valid_brackets(const string &line, size_t* num_str) {
    FILE * fs = fopen(line.c_str(), "r");
    int ret;
    if (!fs) {
    ret = OPEN_FILE_ERROR;
    } else {
        ret = is_valid_bracket(fs, num_str);
        fclose(fs);
    }

    if (true == ret){
        ret = SUCCESS;
    } else if (false == ret){
        ret = INVALID_BRACETS_VISION;
    }

    return ret;

}

bool dataClass::is_valid_bracket(FILE *fs, size_t * line) {
    std::stack<char> stack;

    for (int bracket = fgetc(fs); bracket != EOF; bracket = fgetc(fs)) {

        if (bracket == '(' /*)*/|| bracket == '[' /*]*/|| bracket == '{' /*}*/|| bracket /*(*/== ')' || bracket == /*[*/']'  || bracket ==/*{*/ '}' ) {
            if (bracket == '('/*)*/ || bracket == '[' /*]*/|| bracket == '{'/*}*/) {
                stack.push(bracket);
            } else {
                if (stack.empty()) {
                    return false;
                }
                char top = stack.top();
                if ((bracket ==/*(*/ ')' && top == '(' /*)*/) || (bracket ==/*[*/ ']' && top == '['/*]*/) ||
                    (bracket ==/*{*/ '}' && top == '{'/*}*/)) {
                    stack.pop();
                } else {
                    return false;
                }
            }
        }
        if (bracket == '\n') {
            (*line)++;
        }
    }
    return stack.empty();
}


int main(int argc, char **argv) {
  dataClass dc;
  if (argc != 2) {
    dc.print_error("", 0, OPEN_FILE_ERROR);

  } else {
    size_t line = 0;
     if(dc.valid_brackets(argv[1], &line) != SUCCESS) {
         dc.print_error("", line, INVALID_BRACETS_VISION);
     } else {
         dc.findType(argv[1]);
     }
  }

  return 0;
}








