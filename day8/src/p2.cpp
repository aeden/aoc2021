#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/io.h"

#include <sstream>
#include <map>

using namespace std;

const string zero = "abcefg";
const string one = "cf";
const string two = "acdeg";
const string three = "acdfg";
const string four = "bcdf";
const string five = "abdfg";
const string six = "abdefg";
const string seven = "acf";
const string eight = "abcdefg";
const string nine = "abcdfg";

struct compare_length {
    bool operator()(const std::string &l, const std::string &r) const {
        return l.size()<r.size();
    }
};

class Entry {
  public:
    vector<string> signal_pattern;
    vector<string> output_value;

    map<int, string> number_to_pattern_map;
    map<string, int> signal_pattern_map;

    Entry() { }

    Entry(string raw_signal_pattern, string raw_output_value) {
      stringstream signal_patterns_tokenizer(raw_signal_pattern);
      string token;
      while (getline(signal_patterns_tokenizer, token, ' ')) {
        signal_pattern.push_back(token);
      }

      stringstream output_value_tokenizer(raw_output_value);
      while (getline(output_value_tokenizer, token, ' ')) {
        output_value.push_back(token);
      }
    }

    void build_maps() {
      // calculate based on the number of times each letter appears
      //
      // 0 = 0 1 2   4 5 6     6
      // 1 =     2     5       2 *
      // 2 = 0   2 3 4   6     5
      // 3 = 0   2 3   5 6     5
      // 4 =   1 2 3   5       4 *
      // 5 = 0 1   3   5 6     5
      // 6 = 0 1   3 4 5 6     6
      // 7 = 0   2     5       3 *
      // 8 = 0 1 2 3 4 5 6     7 *
      // 9 = 0 1 2 3   5 6     6

      char wire_map[7];
      print_strings(&signal_pattern, " ");
      vector<string> zero_six_or_nine;
      vector<string> two_three_or_five;
      for (vector<string>::iterator iter = signal_pattern.begin(); iter != signal_pattern.end(); iter++) {
        string s = *iter;
        int str_len = iter->length();
        if (str_len == one.length()) { // 2 chars
          number_to_pattern_map[1] = s;
        } else if (str_len == seven.length()) { // 3 chars
          number_to_pattern_map[7] = s;
        } else if (str_len == four.length()) { // 4 chars
          number_to_pattern_map[4] = s;
        } else if (str_len == eight.length()) { // 7 chars
          number_to_pattern_map[8] = s;
        } else if (str_len == zero.length()) { // 6 chars
          zero_six_or_nine.push_back(s);
        } else if (str_len == two.length()) { // 5 chars
          two_three_or_five.push_back(s);
        }
      }

      vector<char> d71 = diff_string(number_to_pattern_map[7], number_to_pattern_map[1]);
      vector<char> d47 = diff_string(number_to_pattern_map[4], number_to_pattern_map[7]);
      vector<char> d84 = diff_string(number_to_pattern_map[8], number_to_pattern_map[4]);

      wire_map[0] = d71[0];

      for (int i = 0; i < 3; i++) {
        string p = zero_six_or_nine[i];
        string s = vec_to_string(diff_string(number_to_pattern_map[8], p));
        cout << "diff 8 and " << p << ": " << s << endl;

        string s1 = vec_to_string(diff_string(s, number_to_pattern_map[1]));
        cout << "diff " << s << " and 1: " << s1 << endl;

        if (s1.length() == 0) {
          number_to_pattern_map[6] = p;
          wire_map[5] = number_to_pattern_map[1][number_to_pattern_map[1].find_first_not_of(s)];
          wire_map[2] = number_to_pattern_map[1][number_to_pattern_map[1].find_first_not_of(s1)];
        } else { 
          string s2 = vec_to_string(diff_string(vec_to_string(d84), p));
          // cout << "diff 8+4 and " << p << ": " << s2 << endl;
          if (s2.length() == 1) {
            number_to_pattern_map[9] = p;
            // cout << "set pos 4 to " << s2[0] << endl;
            wire_map[4] = s2[0];
          } else {
            number_to_pattern_map[0] = p;
          }
        }
      }

      for (int i = 0; i < 3; i++) {
        string p = two_three_or_five[i];
        string s = vec_to_string(diff_string(number_to_pattern_map[8], p));
        cout << "diff 8 and " << p << ": " << s << endl;

        string s1 = vec_to_string(diff_string(s, number_to_pattern_map[1]));
        cout << "diff " << s << " and 1: " << s1 << endl;

        if (s1.length() == 2) {
           number_to_pattern_map[3] = p;

           string s2 = string(1, wire_map[5]);
           int pos = s.find_first_not_of(s2);
           //cout << "char: " << s[pos] << endl;
           wire_map[1] = s[pos];
        } else {
          string s2 = vec_to_string(diff_string(vec_to_string(d84), p));
          cout << "diff 8+4 and " << p << ": " << s2 << endl;

          if (s2.length() == 1) {
            number_to_pattern_map[5] = p;
          } else {
            number_to_pattern_map[2] = p;
          }
        }
      }

      for (int i = 0; i <= 9; i++) {
        cout << i << ":" << number_to_pattern_map[i] << endl;
        sort(number_to_pattern_map[i].begin(), number_to_pattern_map[i].end());
        signal_pattern_map[number_to_pattern_map[i]] = i;
      }
    }

    int get_output_value() {
      string output_string;
      for (vector<string>::iterator iter = output_value.begin(); iter != output_value.end(); iter++) {
        string k = *iter;
        sort(k.begin(), k.end());
        int v = signal_pattern_map[k];
        //cout << "KV: " << k << "," << v << endl;
        output_string += std::to_string(v);
      }
      //cout << "Output string: " << output_string << endl;
      return stoi(output_string);
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
      for (vector<string>::iterator iter = signal_pattern.begin(); iter != signal_pattern.end(); iter++) {
        os << *iter << " ";
      }
      os << " | ";
      for (vector<string>::iterator iter = output_value.begin(); iter != output_value.end(); iter++) {
        os << *iter;
        if (next(iter) != output_value.end()) os << " ";
      }
      return buffer.str();
    }

    string vec_to_string(vector<char> v) {
      stringbuf buffer;
      ostream os (&buffer);
      for (vector<char>::iterator iter = v.begin(); iter != v.end(); iter++) {
        os << *iter;
      }
      return buffer.str();
    }

    vector<char> diff_string(string s1, string s2) {
      vector<char> diff;
      for (int i = 0; i < s1.size(); i++) {
        if (s2.find(s1[i]) == string::npos) {
          diff.push_back(s1[i]);
        }
      }
      return diff;
    } 
};

vector<Entry> parse_data(vector<string> lines) {
  vector<Entry> entries;
  string delimiter = " | ";
  for (vector<string>::iterator iter = lines.begin(); iter != lines.end(); iter++) {
    string s = *iter;
    cout << "parsing: " << s << endl;
    int pos = s.find(delimiter);
    Entry entry (iter->substr(0, pos), iter->substr(pos + 3));
    entry.build_maps();
    entries.push_back(entry);
  }
  return entries;
}

int main (int argc, char** argv) {
  string filename;
  int days;
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
    int v = iter->get_output_value();
    cout << "Output value: " << v << endl;
    count += v;
  }

  cout << "Count: " << count << endl;

  cout << "Done" << endl;
}
