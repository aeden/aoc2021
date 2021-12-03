#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>

using namespace std;

const int SAMPLE_BITS= 5;
const int DATA_BITS = 12;

const int BIT_LENGTH = DATA_BITS;

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

  vector<bitset<BIT_LENGTH> > bitsets;

  for (int i = 0; i < data.size(); i++) {
    bitset<BIT_LENGTH> bits (data[i]);
    bitsets.push_back(bits);
  }

  cout << "Processing " << bitsets.size() << " rows" << endl;

  bitset<BIT_LENGTH> gamma_bitset;
  bitset<BIT_LENGTH> epsilon_bitset;

  for (int h = 0; h < BIT_LENGTH; h++) {
    long on = 0;
    long off = 0;
    for (int i = 0; i < bitsets.size(); i++) {
      string bitstring = bitsets[i].to_string();
      if (bitstring[h] == '1') {
        on++;
      } else {
        off++;
      }
    }

    if (on > off) {
      gamma_bitset.set(h, true);
      epsilon_bitset.set(h, false);
    } else {
      gamma_bitset.set(h, false);
      epsilon_bitset.set(h, true);
    }
  }

  cout << "Gamma bitset " << gamma_bitset << endl;
  cout << "Epsilon bitset " << epsilon_bitset << endl;

  cout << "Gamma rate: " << gamma_bitset.to_ulong() << endl;
  cout << "Epsilon rate: " << epsilon_bitset.to_ulong() << endl;

  cout << "Power consumption: " << gamma_bitset.to_ulong() * epsilon_bitset.to_ulong() << endl;

  cout << "Done" << endl;
  return 0;
}
