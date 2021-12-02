#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char** argv) {
  string filename;
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " datafile" << endl;
    return 1;
  } else {
    filename = argv[1];
  }

  ifstream infile;
  infile.open(filename);

  if (infile.is_open()) {
    vector<string> data;
    string line;
    while(getline(infile, line)) {
      data.push_back(line);
    }
    cout << "Loaded " << data.size() << " data lines" << endl;
  }

  cout << "Done" << endl;
  return 0;
}
