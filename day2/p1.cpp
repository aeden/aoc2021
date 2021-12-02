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

const string FORWARD = "forward";
const string UP = "up";
const string DOWN = "down";

int main(int argc, char** argv) {
  string filename;
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " datafile" << endl;
    return 1;
  } else {
    filename = argv[1];
  }

  vector<string> data = read_data(filename);
  cout << "Loaded " << data.size() << " data lines" << endl;

  int horizontal_position = 0;
  int depth = 0;

  for (int i = 0; i < data.size(); i++) {
    vector<string> tokens;
    string row = data[i];
    stringstream tokenizer(row);
    string token;
    while (tokenizer >> token) {
      tokens.push_back(token);
    }
    string command = tokens[0];
    int value = stoi(tokens[1]);

    if (command.compare(FORWARD) == 0) {
      horizontal_position += value; 
    } else if (command.compare(UP)) {
      depth += value;
    } else if (command.compare(DOWN)) {
      depth -= value;
    }
  }

  cout << "Horizontal position: " << horizontal_position << endl;
  cout << "Depth: " << depth << endl;
  cout << "Result: " << horizontal_position * depth << endl;

  cout << "Done" << endl;
  return 0;
}
