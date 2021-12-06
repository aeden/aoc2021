#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/io.h"

#include <map>

using namespace std;

void print_fish_map(map<int, long> *fish) {
  long sum = 0;
  for (map<int, long>::iterator iter = fish->begin(); iter != fish->end(); iter++) {
    long value = iter->second;
    sum = sum + value;
    cout << iter->first << ":" << iter->second << endl;
  }
  cout << "Total fish: " << sum << endl;
}

void print_fish_transform(map<int, long> start, map<int, long> end) {
  long sum = 0;
  for (map<int, long>::iterator iter = start.begin(); iter != start.end(); iter++) {
    int day = iter->first;
    long begin_val = iter->second;
    long end_val = end[day];
    sum = sum + end_val;
    cout << day << ":" << begin_val << "->" << end_val << endl;
  }
  cout << "Total fish: " << sum << endl;
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

  map<int,long> fish;
  for (int i = 0; i <= 8; i++) {
    fish[i] = 0;
  }

  for (int i = 0; i < numbers.size(); i++) {
    if(fish.find(numbers[i]) == fish.end()) {
      fish[numbers[i]] = 1;
    } else {
      fish[numbers[i]] += 1;
    }
  }

  for (int i = 0; i < days; i++) {
    // cout << "day " << i << endl;
    map<int,long> tomorrow;
    for (int i = 0; i <= 8; i++) {
      tomorrow[i] = 0;
    }

    for (map<int, long>::iterator iter = fish.begin(); iter != fish.end(); iter++) {
      int day = iter->first;
      long count = iter->second;

      if (day == 0) {
        tomorrow[6] = tomorrow[6] + count;
        tomorrow[8] = tomorrow[8] + count;
      } else {
        tomorrow[day - 1] = tomorrow[day - 1] + count; 
      }
    }

    // print_fish_transform(fish, tomorrow);
    fish = tomorrow; 
    
  }

  print_fish_map(&fish);

  cout << "Done." << endl;
  return 0;
} 
