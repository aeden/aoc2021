#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

vector<string> read_data(string filename) {
  ifstream infile;
  infile.open(filename);
  vector<string> data;

  if (infile.is_open()) {
    string line;
    while(getline(infile, line)) {
      data.push_back(line);
    }
    return data;
  } else {
    cout << "Failed to read data" << endl;
    return data;
  }
}

void parse_ints(string row, vector<int> *numbers, char separator) {
    stringstream tokenizer(row);
    string token;
    while (getline(tokenizer, token, separator)) {
      numbers->push_back(stoi(token));
    }
}

void print_ints(vector<int> *numbers, char separator) {
  for (vector<int>::iterator iter = numbers->begin(); iter != numbers->end(); iter++) {
    cout << *iter;
    if (next(iter) != numbers->end()) cout << separator;
  }
  cout << endl;
}

void print_strings(vector<string> *strings, string separator) {
  for (vector<string>::iterator iter = strings->begin(); iter != strings->end(); iter++) {
    cout << *iter;
    if (next(iter) != strings->end()) cout << separator;
  }
  cout << endl;
}
