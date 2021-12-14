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
    string value;
    vector<string> pairs;

    void parse(string data) {
      value = data;
      for (int i = 0; i < data.size() - 1; i++) {
        string s;
        s += data[i];
        s += data[i+1];
        pairs.push_back(s);
      }
    }

    string to_string() {
      return value; 
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

class ElementTracker {
  public:
    map<char, long> elements;

    void initialize(string value) {
      for (int i = 0; i < value.size(); i++) {
        increment(value[i]);
      }
    }

    void increment(char element) {
      map<char, long>::iterator iter = elements.find(element);
      if (iter == elements.end()) {
        elements[element] = 1;
      } else {
        elements[element] += 1;
      }
    }
};

class RuleSet {
  public:
    vector<PairInsertionRule> rules;

    void add_rule(PairInsertionRule rule) {
      rules.push_back(rule);
    }

    void process_pair(string pair, ostream *os, ElementTracker *tracker) {
      for (vector<PairInsertionRule>::iterator iter = rules.begin(); iter != rules.end(); iter++) {
        if (iter->pair == pair) {
          //cout << "Found pair " << pair << endl;
          tracker->increment(iter->insertion[0]);
          *os << pair[0];
          *os << iter->insertion;
          return;
        }
      }
    }

    PolymerTemplate step(PolymerTemplate pt, ElementTracker *tracker) {
      vector<string> pairs = pt.pairs;
      stringbuf buffer;
      ostream os (&buffer);

      for (vector<string>::iterator iter = pairs.begin(); iter != pairs.end(); iter++) {
        //cout << "Processing pair " << *iter << endl;
        process_pair(*iter, &os, tracker);
        
        if (next(iter) == pairs.end()) {
          //cout << "End of pairs" << endl;
          string s = *iter;
          os << s[1];
        }
      }

      PolymerTemplate new_pt = PolymerTemplate();
      new_pt.parse(buffer.str());
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

  // parse data
  PolymerTemplate pt;
  pt.parse(data[0]);

  cout << "Polymer template:" <<  pt.to_string() << endl;

  RuleSet rules;
  for (int i = 2; i < data.size(); i++) {
    PairInsertionRule rule;
    rule.parse(data[i]);
    rules.add_rule(rule);
  }

  //cout << "Pair insertion rules:" << endl;
  //cout << rules.to_string() << endl;

  ElementTracker tracker;
  tracker.initialize(pt.to_string());

  for (int i = 0; i < steps; i++) {
    pt = rules.step(pt, &tracker);
    //cout << "After step " << (i + 1) << ": " << pt.to_string() << endl;
  }

  long min = 0;
  long max = 0;
  cout << "Element occurences:" << endl;
  for (map<char, long>::iterator iter = tracker.elements.begin(); iter != tracker.elements.end(); iter++) {
    if (iter->second > max) max = iter->second;
    if (iter->second < min || min == 0) min = iter->second;
    cout << iter->first << ":" << iter->second << endl;
  }

  cout << "Result: " << max - min << endl;

  cout << "Done" << endl;
}
