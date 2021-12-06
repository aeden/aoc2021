#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/io.h"

using namespace std;

class Fish {
  public:
    int timer;

    Fish() {
      timer = 0;
    }

    Fish(int timer) {
      this->timer = timer;
    }

    // age the fish by one day
    bool age() {
      if (timer == 0) {
        timer = 6;
        return true;
      } else {
        timer -= 1;
        return false;
      }
    }
};

void print_fish(vector<Fish> *fish) {
  for (vector<Fish>::iterator iter = fish->begin(); iter != fish->end(); iter++) {
    cout << iter->timer;
    if (next(iter) != fish->end()) cout << ",";
  }
  cout << endl;
}

long sum_fish_ages(vector<Fish> *fish) {
  long sum = 0;
  for (vector<Fish>::iterator iter = fish->begin(); iter != fish->end(); iter++) {
    sum += iter->timer;
  }
  return sum;
}

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

  vector<Fish> fish;
  for (vector<int>::iterator iter = numbers.begin(); iter != numbers.end(); iter++) {
    fish.push_back(Fish(*iter));
  }

  for (int i = 0; i < days; i++) {
    vector<Fish> new_fish = vector<Fish>();
    for (vector<Fish>::iterator iter = fish.begin(); iter != fish.end(); iter++) {
      if (iter->age()) {
        new_fish.push_back(Fish(8));
      }
    }
    for (int j = 0; j < new_fish.size(); j++) {
      fish.push_back(new_fish[j]);
    }
  }

  // print_fish(&fish); 

  cout << "Number of fish: " << fish.size() << endl;

  cout << "Done." << endl;
  return 0;
}
