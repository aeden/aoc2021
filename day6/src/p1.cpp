#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/io.h"

using namespace std;

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

  cout << "Done." << endl;
  return 0;
}
