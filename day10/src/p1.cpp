#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/io.h"

#include <map>

using namespace std;

const int INCOMPLETE = 1;
const int CORRUPTED = 2;

struct parse_result {
  int status;
  char last_char;
};

parse_result parse_line(string line) {
  parse_result res;
  deque<char> s;
  for (int i = 0; i < line.size(); i++) { 
    char c = line[i];
    res.last_char = c;
    if (c== '(' || c == '[' || c == '{' || c == '<') {
      s.push_back(c);
    } else if (c == ')') {
      if (s.back() == '(') {
        s.pop_back();
      } else {
        res.status = CORRUPTED;
        return res;
      }
    } else if (c == ']') {
      if (s.back() == '[') {
        s.pop_back();
      } else {
        res.status = CORRUPTED;
        return res;
      }
    } else if (c == '}') {
      if (s.back() == '{') {
        s.pop_back();
      } else {
        res.status = CORRUPTED;
        return res;
      } 
    }else if (c == '>') {
      if (s.back() == '<') {
        s.pop_back();
      } else {
        res.status = CORRUPTED;
        return res;
      }
    }
  }

  if(!s.empty()) {
    res.status = INCOMPLETE;
    return res;
  }

  cout << endl;

  return res;
}

int main (int argc, char** argv) {
  string filename;
  int days;
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " datafile" << endl;
    return 1;
  } else {
    filename = argv[1];
  }

  vector<string> data = read_data(filename);
  cout << "Loaded " << data.size() << " data lines" << endl;

  map<char, int> score_table;
  score_table[')'] = 3;
  score_table[']'] = 57;
  score_table['}'] = 1197;
  score_table['>'] = 25137;

  int sum = 0;
  for (int i = 0; i < data.size(); i++) {
    parse_result res = parse_line(data[i]);
    if (res.status == CORRUPTED) {
      sum += score_table[res.last_char];
    }
  }
  cout << "Result: " << sum << endl;

  cout << "Done" << endl;
}
