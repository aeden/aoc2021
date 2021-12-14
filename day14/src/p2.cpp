
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/io.h"

#include <sstream>
#include <map>

using namespace std;

const string INSERTION_DELIMITER = " -> ";

class PolymerTemplate {
  public:
    map<string, long> pair_map;
    char last_char;

    void parse(string data) {
      last_char = data[data.size() - 1];

      for (int i = 0; i < data.size() - 1; i++) {
        string s;
        s += data[i];
        s += data[i+1];

        map<string, long>::iterator iter = pair_map.find(s);
        if (iter == pair_map.end()) {
          pair_map[s] = 1;
        } else {
          pair_map[s] += 1;
        }
      }
    }

    string value() {
      stringbuf buffer;
      ostream os (&buffer);
      for (map<string, long>::iterator iter = pair_map.begin(); iter != pair_map.end(); iter++) {
        for (int i = 0; i < iter->second; i++) {
          string s = iter->first;
          os << s[0];
        }
      }
      os << last_char;
      return buffer.str();
    }

    string to_string() {
      stringbuf buffer;
      ostream os (&buffer);
      for (map<string, long>::iterator iter = pair_map.begin(); iter != pair_map.end(); iter++) {
        os << iter->first << ":" << iter->second;
        if (next(iter) != pair_map.end()) os << endl;
      } 
      return buffer.str();
    }
};

class PairInsertionRule {
  public:
    string pair;
    string insertion;

    void parse(string data) {
      int pos = data.find(INSERTION_DELIMITER); 
      pair = data.substr(0, 2);
      insertion = data.substr(pos + INSERTION_DELIMITER.size()); 
    }

    string to_string() {
      return pair + INSERTION_DELIMITER + insertion;
    }

};

class RuleSet {
  public:
    vector<PairInsertionRule> rules;

    void add_rule(PairInsertionRule rule) {
      rules.push_back(rule);
    }

    PolymerTemplate step(PolymerTemplate pt) {
      map<string, long> pair_map = pt.pair_map;
      map<string, long> new_pair_map;
      stringbuf buffer;
      ostream os (&buffer);

      for (vector<PairInsertionRule>::iterator iter = rules.begin(); iter != rules.end(); iter++) {
        string s = iter->pair;
        map<string, long>::iterator f = pair_map.find(s);
        if (f != pair_map.end()) {
          string k1;
          k1 += s[0];
          k1 += iter->insertion[0];

          string k2;
          k2 += iter->insertion[0];
          k2 += s[1];

          map<string, long>::iterator f2 = new_pair_map.find(k1);
          if (f2 == new_pair_map.end()) {
            new_pair_map[k1] = f->second;
          } else {
            new_pair_map[k1] += f->second;
          }

          f2 = new_pair_map.find(k2);
          if (f2 == new_pair_map.end()) {
            new_pair_map[k2] = f->second;
          } else {
            new_pair_map[k2] += f->second;
          }
        }
      }

      PolymerTemplate new_pt = PolymerTemplate();
      new_pt.pair_map = new_pair_map;
      new_pt.last_char = pt.last_char;
      
      return new_pt;
    }

    string to_string() {
      stringbuf buffer;
      ostream os (&buffer);

      for (vector<PairInsertionRule>::iterator iter = rules.begin(); iter != rules.end(); iter++) {
        os << iter->to_string();
        if (next(iter) != rules.end()) os << endl;
      }

      return buffer.str();
    }
};

int main (int argc, char** argv) {
  string filename;
  int steps;
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " datafile steps" << endl;
    return 1;
  } else {
    filename = argv[1];
    steps = stoi(argv[2]);
  }

  vector<string> data = read_data(filename);
  cout << "Loaded " << data.size() << " data lines" << endl;

  PolymerTemplate pt;
  pt.parse(data[0]);

  cout << "Polymer template:" << endl;
  cout << pt.to_string() << endl;

  RuleSet rules;
  for (int i = 2; i < data.size(); i++) {
    PairInsertionRule rule;
    rule.parse(data[i]);
    rules.add_rule(rule);
  }

  //cout << "Pair insertion rules:" << endl;
  //cout << rules.to_string() << endl;

  for (int i = 0; i < steps; i++) {
    cout << "Step " << i + 1 << endl;
    pt = rules.step(pt);
    //cout << "After step " << (i + 1) << " (len: " << pt.value().size() << "): " << pt.value() << endl;
    //cout << pt.to_string() << endl;
  }

  map<char, long> occurrences;
  for (map<string, long>::iterator iter = pt.pair_map.begin(); iter != pt.pair_map.end(); iter++) {
    string pair = iter->first;
    long pair_count = iter->second;
    
    map<char, long>::iterator f;

    f = occurrences.find(pair[0]);
    if (f == occurrences.end()) {
      occurrences[pair[0]] = pair_count;
    } else {
      occurrences[pair[0]] += pair_count;
    }
  }
  occurrences[pt.last_char] += 1;

  long min = 0;
  long max = 0;
  cout << "Occurences: " << endl;
  for (map<char, long>::iterator iter = occurrences.begin(); iter != occurrences.end(); iter++) {
    cout << iter->first << ":" << iter->second << endl;

    if (iter->second > max) max = iter->second;
    if (iter->second < min || min == 0) min = iter->second;
  }

  cout << "Result: " << max - min << endl;

  cout << "Done" << endl;
}
