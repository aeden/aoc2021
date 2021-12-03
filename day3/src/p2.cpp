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

void print_bitsets(vector<bitset<BIT_LENGTH> > bitsets) {
  for (int i = 0; i < bitsets.size(); i++) {
    cout << bitsets[i];
    if (i < bitsets.size() - 1) {
      cout << ",";
    }
  }
  cout << endl;
}

vector<bitset<BIT_LENGTH> > strings_to_bitsets(vector<string> data) {
  vector<bitset<BIT_LENGTH> > bitsets;

  for (int i = 0; i < data.size(); i++) {
    bitset<BIT_LENGTH> bits (data[i]);
    bitsets.push_back(bits);
  }

  return bitsets;
}

vector<bitset<BIT_LENGTH> > oxygen_generator_rating(int column_number, vector<bitset<BIT_LENGTH> > bitsets) {
  vector<bitset<BIT_LENGTH> > on;
  vector<bitset<BIT_LENGTH> > off;
  for (int i = 0; i < bitsets.size(); i++) {
    string bitstring = bitsets[i].to_string();
    if (bitstring[column_number] == '1') {
      on.push_back(bitsets[i]);
    } else {
      off.push_back(bitsets[i]);
    }
  }

  if (on.size() > off.size()) {
    return on; 
  } else if (on.size() == off.size()) {
    return on;
  } else {
    return off;
  }
}

vector<bitset<BIT_LENGTH> > co2_scrubber_rating(int column_number, vector<bitset<BIT_LENGTH> > bitsets) {
  vector<bitset<BIT_LENGTH> > on;
  vector<bitset<BIT_LENGTH> > off;
  for (int i = 0; i < bitsets.size(); i++) {
    string bitstring = bitsets[i].to_string();
    if (bitstring[column_number] == '1') {
      on.push_back(bitsets[i]);
    } else {
      off.push_back(bitsets[i]);
    }
  }

  if (on.size() > off.size()) {
    return off;
  } else if (on.size() == off.size()) {
    return off;
  } else {
    return on;
  }
}

int calculate_oxygen_generator_rating(vector<string> data) {
  vector<bitset<BIT_LENGTH> > bitsets = strings_to_bitsets(data); 
  cout << "Processing " << bitsets.size() << " rows for oxygen generator rating" << endl;

  for (int h = 0; h < BIT_LENGTH; h++) {
    bitsets = oxygen_generator_rating(h, bitsets);
  }

  return bitsets[0].to_ulong();
}

int calculate_co2_scrubber_rating(vector<string> data) {
  vector<bitset<BIT_LENGTH> > bitsets = strings_to_bitsets(data); 
  cout << "Processing " << bitsets.size() << " rows for CO2 scrubber rating" << endl;

  for (int h = 0; h < BIT_LENGTH; h++) {
    bitsets = co2_scrubber_rating(h, bitsets);
  }

  return bitsets[0].to_ulong();
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

  int oxygen_generator_rating = calculate_oxygen_generator_rating(data);
  int co2_scrubber_rating = calculate_co2_scrubber_rating(data);

  cout << "Oxygen generator rating: " << oxygen_generator_rating << endl;
  cout << "CO2 scrubber rating: " << co2_scrubber_rating << endl;

  cout << "Life support rating: " << oxygen_generator_rating * co2_scrubber_rating << endl; 

  cout << "Done" << endl;
  return 0;
}


