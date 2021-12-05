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
