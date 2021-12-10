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
  vector<char> completion;
  long line_score;
};

vector<char> complete_line(deque<char> dq) {
  vector<char> completion;
  while(dq.size() > 0) {
    char c = dq.back();
    if (c == '[') {
      completion.push_back(']');
    } else if (c == '(') {
      completion.push_back(')');
    } else if (c == '{') {
      completion.push_back('}');
    } else if (c == '<') {
      completion.push_back('>');
    }
    dq.pop_back();
  }

  return completion;
}

void print_deque(deque<char> dq) {
  for (int i = 0; i < dq.size(); i++) {
    cout << dq[i];
  }
  cout << endl;
}

string char_vector_to_string(vector<char> v) {
  string s;
  for (int i = 0; i < v.size(); i++) {
    s += v[i];
  }
  return s;
}

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
    res.completion = complete_line(s);
    return res;
  }

  cout << endl;

  return res;
}

long score_line(vector<char> completion, map<char, int> score_table) {
  long line_score = 0;
  for (int i = 0; i < completion.size(); i++) {
    line_score *= 5;
    line_score += score_table[completion[i]];
  }
  return line_score;
}

class ScoreSort {
  public:
    bool operator()(parse_result lhs, parse_result rhs) {
      return lhs.line_score < rhs.line_score;
    }
};

int main (int argc, char** argv) {
  string filename;
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " datafile" << endl;
    return 1;
  } else {
    filename = argv[1];
  }

  vector<string> data = read_data(filename);
  cout << "Loaded " << data.size() << " data lines" << endl;

  map<char, int> score_table;
  score_table[')'] = 1;
  score_table[']'] = 2;
  score_table['}'] = 3;
  score_table['>'] = 4;

  vector<parse_result> incompletes;
  for (int i = 0; i < data.size(); i++) {
    parse_result res = parse_line(data[i]);
    if (res.status == INCOMPLETE) {
      res.line_score = score_line(res.completion, score_table);
      incompletes.push_back(res);
    }
  }
  
  sort(incompletes.begin(), incompletes.end(), ScoreSort());
  for (int i = 0; i < incompletes.size(); i++) {
    cout << char_vector_to_string(incompletes[i].completion) << ": " << incompletes[i].line_score << endl;
  }

  long score = incompletes[incompletes.size() / 2].line_score;

  cout << "Result: " << score << endl;

  cout << "Done" << endl;
}

