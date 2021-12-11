#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/io.h"

#include <sstream>

using namespace std;

const string one = "cf";
const string two = "acdeg";
const string three = "acdfg";
const string four = "bcdf";
const string five = "abdfg";
const string six = "abdefg";
const string seven = "acf";
const string eight = "abcdefg";
const string nine = "abcdfg";

class Entry {
  public:
    vector<string> signal_patterns;
    vector<string> output_value;

    Entry() { }

    Entry(string raw_signal_patterns, string raw_output_value) {
      stringstream signal_patterns_tokenizer(raw_signal_patterns);
      string token;
      while (getline(signal_patterns_tokenizer, token, ' ')) {
        signal_patterns.push_back(token);
      }

      stringstream output_value_tokenizer(raw_output_value);
      while (getline(output_value_tokenizer, token, ' ')) {
        output_value.push_back(token);
      }
    }

    int count_unique_number_segment_output_values() {
      int count = 0;
      for (vector<string>::iterator iter = output_value.begin(); iter != output_value.end(); iter++) {
        int str_len = iter->length();
        if (str_len == 2 || str_len == 3 || str_len == 4 || str_len == 7) {
          count++;
        }
      }
      return count;
    }

    string to_string() {
      stringbuf buffer;
      ostream os (&buffer);
      for (vector<string>::iterator iter = signal_patterns.begin(); iter != signal_patterns.end(); iter++) {
        os << *iter << " ";
      }
      os << "|";
      for (vector<string>::iterator iter = output_value.begin(); iter != output_value.end(); iter++) {
        os << *iter;
        if (next(iter) != output_value.end()) os << " ";
      }
      return buffer.str();
    }
};

vector<Entry> parse_data(vector<string> lines) {
  vector<Entry> entries;
  string delimiter = "|";
  for (vector<string>::iterator iter = lines.begin(); iter != lines.end(); iter++) {
    string s = *iter;
    int pos = s.find(delimiter);
    Entry entry (iter->substr(0, pos), iter->substr(pos + 1));
    entries.push_back(entry);
  }
  return entries;
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

  vector<Entry> entries = parse_data(data);

  int count = 0;
  for (vector<Entry>::iterator iter = entries.begin(); iter != entries.end(); iter++) {
    count += iter->count_unique_number_segment_output_values();
    // cout << iter->to_string()<< endl;
  }

  cout << "Count: " << count << endl;

  cout << "Done" << endl;
}
