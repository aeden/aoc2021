#include <iostream>
#include <fstream>
#include <string>
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

  cout << "Done" << endl;
  return 0;
}
