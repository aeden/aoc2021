#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/io.h"

#include <deque>
#include <cassert>

using namespace std;

class Element {
  public:
    virtual string to_string() {
      cout << "Element::to_string()" << endl;
      return "-";
    }

    virtual void print() {
      cout << "Element::print()" << endl;
    }
};

class Number: public Element {
  public:
    int value;

    Number(int v) {
      this->value = v;
    }

    void print() override {
      cout << "Number::print()" << endl;
    }

    string to_string() override {
      cout << "Number::to_string()" << endl;
      //cout << "Value is: " << value;
      return "";
    }
};

class Pair: public Element {
  public:
    Element* first;
    Element* second;

    string to_string() override {
      return "[" + first->to_string() + "," + second->to_string() + "]";
    }
    
    void print() override {
      cout << "Pair::print()" << endl;
      cout << first->to_string() << endl;
    }
};


void parse(Pair *p, string rest, vector<Element*> *s) {
  cout << "Rest: " << rest << endl; 
  char c = rest[0];
  if (c == '[') {
    cout << "Found opening bracket" << endl;
    Pair next_pair;
    parse(&next_pair, rest.substr(1), s);
  } else if (c == ']') {
    cout << "Found closing bracket" << endl;
    Element* e = s->back();
    cout << "Setting " << e->to_string() << " as second element" << endl;
    p->second = e;
    s->pop_back();
  } else if (c == ',') {
    cout << "Found pair separator char" << endl;
    Element* e = s->back();
    cout << "Setting " << e->to_string() << " as first element" << endl;
    p->first = e;
    s->pop_back();
    parse(p, rest.substr(1), s);
  } else {
    int v = c - '0';
    cout << "Found literal value " << v << endl;
    Number n (v);
    cout << "Pushing literal value " << n.to_string() << " on to stack" << endl;
    s->push_back(&n);
    parse(p, rest.substr(1), s);
  }

}

void parse(Pair *p, string rest) {
  vector<Element*> s;
  parse(p, rest, &s);
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

  Pair p;
  parse(&p, data[0].substr(1));
  cout << "Completed parsing line 1" << endl;

  assert(p.first != NULL);

  cout << "Trying to print p" << endl;
  p.print();
  // cout << p.to_string() << endl;

  cout << "Done" << endl;
}

