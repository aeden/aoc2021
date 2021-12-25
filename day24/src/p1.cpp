#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/io.h"

#include <map>
#include <deque>

using namespace std;

const string ASSIGN_INPUT = "inp";
const string ADD = "add";
const string MULTIPLY = "mul";
const string DIVIDE = "div";
const string MODULO = "mod";
const string CHECK_EQUALITY = "eql";

void tokenize(string line, int line_number, vector<string> *tokens) {
  size_t pos = 0;
  string delimiter = " ";
  while ((pos = line.find(delimiter)) != string::npos) {
    string token = line.substr(0, pos);
    tokens->push_back(token);
    line.erase(0, pos + delimiter.length());
  }

  if (line.length() > 0) {
    tokens->push_back(line);
  }
}

void execute(vector<string> *tokens, int line_number, map<string, long> *vars, deque<int> *inputs) {
  //cout << "processing line " << line_number << endl;
  for (vector<string>::iterator iter = tokens->begin(); iter != tokens->end(); iter++) {
    //cout << "processing token: " << *iter << endl;
    if (*iter == ASSIGN_INPUT) {
      string a_var = *next(iter);
      int input = inputs->front();
      inputs->pop_front();
      (*vars)[a_var] = input;
      //cout << "assign input " << input << " to " << a_var << endl;
    } else if (*iter == ADD) {
      string a_var = *++iter;
      string b_var = *++iter;
      
      int b_var_char = b_var[0] - '0';
      int b_val = b_var_char > 9 ? (*vars)[b_var] : stoi(b_var);

      //cout << "add " << (*vars)[a_var] << " + " << b_val << " and assign to " << a_var << endl; 
      (*vars)[a_var] = (*vars)[a_var] + b_val;
    } else if (*iter == MULTIPLY) {
      string a_var = *++iter;
      string b_var = *++iter;

      int b_var_char = b_var[0] - '0';
      int b_val = b_var_char > 9 ? (*vars)[b_var] : stoi(b_var);

      //cout << "multiply " << (*vars)[a_var] << " * " << b_val << " and assign to " << a_var << endl; 
      (*vars)[a_var] = (*vars)[a_var] * b_val;
    } else if (*iter == DIVIDE) {
      string a_var = *++iter;
      string b_var = *++iter;

      int b_var_char = b_var[0] - '0';
      int b_val = b_var_char > 9 ? (*vars)[b_var] : stoi(b_var);

      //cout << "divide " << (*vars)[a_var] << " / " << b_val << " and assign to " << a_var << endl; 
      (*vars)[a_var] = (*vars)[a_var] / b_val;
    } else if (*iter == MODULO) {
      string a_var = *++iter;
      string b_var = *++iter;

      int b_var_char = b_var[0] - '0';
      int b_val = b_var_char > 9 ? (*vars)[b_var] : stoi(b_var);

      //cout << "modulo " << (*vars)[a_var] << " % " << b_val << " and assign to " << a_var << endl; 
      (*vars)[a_var] = (*vars)[a_var] % b_val;
    } else if (*iter == CHECK_EQUALITY) {
       string a_var = *++iter;
       string b_var = *++iter;
       (*vars)[a_var] = (*vars)[a_var] == (*vars)[b_var];
       //cout << "assign result of a == b (" << (*vars)[a_var] << "==" << (*vars)[b_var] << ") to " << a_var << endl;
    }
  }
  //cout << "done processing line" << endl;
}

long execute(vector<string> lines, deque<int> inputs) {
  map<string, long> vars{{"w", 0}, {"x", 0}, {"y", 0}, {"z", 0}};
  vector<vector<string> > tokens{};

  for (int i = 0; i < lines.size(); i++) {
    int line_no = i + 1;
    vector<string> line_tokens {};
    tokenize(lines[i], line_no, &line_tokens);
    tokens.push_back(line_tokens);
  }
  //cout << tokens.size() << " lines tokenized" << endl;
  
  for (int i = 0; i < tokens.size(); i++) {
    execute(&tokens[i], i + 1, &vars, &inputs); 
    //cout << "w:" << vars["w"] << ",";
    //cout << "x:" << vars["x"] << ",";
    //cout << "y:" << vars["y"] << ",";
    //cout << "z:" << vars["z"] << endl;
  }

  return vars["z"];
}

void parse_input_string(string input_string, deque<int> *inputs) {
  for (int i = 0; i < input_string.size(); i++) {
    inputs->push_back(input_string[i] - '0');
  }
}

int main(int argc, char** argv) {
  string filename;
  string input_string;
  deque<int> inputs;

  if (argc != 2 && argc != 3) {
    cout << "Usage: " << argv[0] << " datafile input" << endl;
    return 1;
  }

  bool find_value = false;
  filename = argv[1];
  if(argc == 3) {
    parse_input_string(argv[2], &inputs);
  } else if (argc == 2) {
    find_value = true; 
  }

  vector<string> data = read_data(filename);
  cout << "Loaded " << data.size() << " data lines" << endl;

  if (find_value) {
    long z = 0;
    do {
      inputs.clear();
      string current = "12345678912345";
      parse_input_string(current, &inputs);
      z = execute(data, inputs);
    } while (z != 0);
    //cout << "Max value: " <<  << endl;
  } else {
    long z = execute(data, inputs);
    cout << "Z:" << z << endl;
  }

  cout << "Done" << endl;
}
