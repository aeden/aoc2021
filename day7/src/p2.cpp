#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/io.h"

#include <set>
#include <map>

using namespace std;

class Crab {
  public:
    int starting_position;

    Crab(int p) {
      starting_position = p;
    }

    // calculate the cost for the crab to reach position p
    int cost(int p) {
      int distance = abs(p - starting_position);
      int c = 0;
      for (int i = 0; i < distance; i++) {
        c += i + 1;
      }
      return c;
    }
};

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

  vector<int> starting_positions;
  parse_ints(data[0], &starting_positions, ',');
  print_ints(&starting_positions, ',');;

  vector<Crab> crabs;
  set<int> positions;

  for (vector<int>::iterator iter = starting_positions.begin(); iter != starting_positions.end(); iter++) {
    crabs.push_back(Crab(*iter));
  }

  int min_position = *min_element(starting_positions.begin(), starting_positions.end());
  int max_position = *max_element(starting_positions.begin(), starting_positions.end());

  for (int i = min_position; i < max_position + 1; i++) {
    positions.insert(i);
  }

  map<int,int> cost_map;
  for (set<int>::iterator iter = positions.begin(); iter != positions.end(); iter++) {
    int p = *iter;
    // cout << p;
    // if (next(iter) != positions.end()) cout << ",";

    for (vector<Crab>::iterator crab_iter = crabs.begin(); crab_iter != crabs.end(); crab_iter++) {
      int cost = crab_iter -> cost(p);
      if (cost_map.find(p) == cost_map.end()) {
        cost_map[p] = cost;
      } else {
        cost_map[p] += cost;
      }
    }
  }
  // cout << endl;

  int lowest_cost = 0;
  int lowest_cost_position = 0;
  // cout << "Costs:" << endl;
  for (map<int,int>::iterator iter = cost_map.begin(); iter != cost_map.end(); iter++) {
    int p = iter->first;
    int cost = iter->second;
    if (lowest_cost == 0 || cost < lowest_cost) {
      lowest_cost = cost;
      lowest_cost_position = p;
    }
    // cout << p << "=" << cost << endl;
  }

  cout << "Lowest cost " << lowest_cost << " is at position " << lowest_cost_position << endl;

  cout << "Done" << endl;
}
