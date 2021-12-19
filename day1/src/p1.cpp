#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <limits>

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
    string line;
    int increased = 0;
    int previousValue = std::numeric_limits<int>::quiet_NaN();
    while(getline(infile, line)) {
      int value = stoi(line);
      if (previousValue == std::numeric_limits<int>::quiet_NaN()) {
        // do nothing
      } else {
        if (value > previousValue) {
          increased += 1;
        }
      }
      previousValue = value;
    }

    cout << "Increased " << increased << " times" << endl;
  } else {
    cout << "Cannot open file: " << filename << endl;
  }

  cout << "Done" << endl;
  return 0;
}
