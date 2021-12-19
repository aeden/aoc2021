#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
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
    vector<int> measurements;
    vector<int> triples;
    int increased = 0;
    
    while(getline(infile, line)) {
      int value = stoi(line);
      measurements.push_back(value);
    }

    for (int i = 0; i < measurements.size(); i++) {
      if (i < measurements.size() - 2) {
        triples.push_back(measurements[i] + measurements[i+1] + measurements[i+2]);
      }
    }

    int previousValue = std::numeric_limits<int>::quiet_NaN();
    for (int i = 0; i < triples.size(); i++) {
      if (previousValue == std::numeric_limits<int>::quiet_NaN()) {
        // do nothing
      } else {
        if (triples[i] > previousValue) {
          increased += 1;
        }
      }
      previousValue = triples[i];
    }

    cout << "Increased " << increased << " times" << endl;
  } else {
    cout << "Cannot open file: " << filename << endl;
  }

  cout << "Done" << endl;
}
