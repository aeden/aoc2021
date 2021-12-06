#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/io.h"

using namespace std;

int main (int argc, char** argv) {
  string filename;
  int days;
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " datafile days" << endl;
    return 1;
  } else {
    filename = argv[1];
    days = stoi(argv[2]);
  }

  vector<string> data = read_data(filename);
  cout << "Loaded " << data.size() << " data lines" << endl;

  vector<int> numbers;
  parse_ints(data[0], &numbers, ',');

  cout << "Done." << endl;
  return 0;
} 
