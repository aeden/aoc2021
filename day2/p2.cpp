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

class Command {
  public:
    string label;
    int value;

    Command(string label, int value) {
      this->label = label;
      this->value = value;
    }

    void execute(int *horizontal_position, int *depth, int *aim) {
      if (label.compare(FORWARD) == 0) {
        *horizontal_position += value; 
        *depth += *aim * value;
      } else if (label.compare(UP)) {
        *aim += value;
      } else if (label.compare(DOWN)) {
        *aim -= value;
      }
    }
};

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
  int aim = 0;

  for (int i = 0; i < data.size(); i++) {
    vector<string> tokens;
    string row = data[i];
    stringstream tokenizer(row);
    string token;
    while (tokenizer >> token) {
      tokens.push_back(token);
    }

    Command command(tokens[0], stoi(tokens[1]));
    command.execute(&horizontal_position, &depth, &aim);
  }

  cout << "Horizontal position: " << horizontal_position << endl;
  cout << "Depth: " << depth << endl;
  cout << "Result: " << horizontal_position * depth << endl;

  cout << "Done" << endl;
  return 0;
}
