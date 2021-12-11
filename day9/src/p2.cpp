#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/io.h"

#include <sstream>
#include <map>
#include <algorithm>

using namespace std;


// Represents a point on the heightmap
class Point {
  public:
    int x;
    int y;

    Point() { }

    Point(int x, int y) {
      this->x = x;
      this->y = y;
    }

    bool operator==(Point v) {
      return v.x == x && v.y == y;
    }

    string to_string() {
      return std::to_string(x) + ":" + std::to_string(y);
    }
};

// Represents a single risk value at a point on the heightmap
class RiskValue {
  public:
    int value;
    Point location;

    RiskValue() { }

    RiskValue(int value, Point location) {
      this->value = value;
      this->location = location;
    }

    bool operator==(RiskValue v) {
      return v.location == location;
    }

    string to_string() {
      return std::to_string(value) + " @ " + location.to_string();
    }
};

// A predicate used to test of the risk value should be removed from a collection because it
// is either already processed or is a value that should not be processed (i.e. a 9)
class ShouldRemove {
  public:
    vector<RiskValue> feeders;

    ShouldRemove(vector<RiskValue> *feeders) {
      this->feeders = *feeders;
    }

    bool operator()(RiskValue v) {
      vector<RiskValue>::iterator iter = std::find(feeders.begin(), feeders.end(), v);
      return v.value == 9 || iter != feeders.end();
    }
};

class Heightmap {
  public:
    vector<vector<RiskValue> > risk_values;

    Heightmap() { }

    Heightmap(vector<string> data) {
      for (int y  = 0; y < data.size(); y++) {
        vector<RiskValue> line_risk_values;
        for (int x = 0; x < data[y].size(); x++) {
          char c = data[y][x];
          int v = c - '0';
          Point p = Point(x, y);
          RiskValue rv (v, p);
          line_risk_values.push_back(rv);
        }
        risk_values.push_back(line_risk_values);
      }
    }

    // Find all valid neighbors for a given RiskValue
    vector<RiskValue> find_neighbors(RiskValue risk_value) {
      vector<RiskValue> neighbors;
      Point l = risk_value.location;
      if (l.x - 1 >= 0) {
        neighbors.push_back(risk_values[l.y][l.x - 1]);
      }
      if (l.x + 1 < risk_values[l.y].size()) {
        neighbors.push_back(risk_values[l.y][l.x + 1]);
      }
      if (l.y - 1 >= 0) {
        neighbors.push_back(risk_values[l.y - 1][l.x]);
      }
      if (l.y + 1 < risk_values.size()) {
        neighbors.push_back(risk_values[l.y + 1][l.x]);
      }
      return neighbors;
    }

    // Find all low points on the heightmap.
    vector<RiskValue> find_low_points() {
      vector<RiskValue> low_points;
      for (int i = 0; i < risk_values.size(); i++) {
        for (int j = 0; j < risk_values[i].size(); j++) {
          vector<RiskValue> neighbors = find_neighbors(risk_values[i][j]);

          bool is_low_point = true;
          for (int k = 0; k < neighbors.size(); k++) {
            if (risk_values[i][j].value >= neighbors[k].value) {
              is_low_point = false;
              break;
            }
          }

          if (is_low_point) {
            low_points.push_back(risk_values[i][j]);
          }
        }
      }
      return low_points;
    }

    // Given a RiskValue, find all of the feeders to that RiskValue. Note that 
    // the first value in the feeders collection will always be the RiskValue 
    // itself if allowed.
    void find_feeders(RiskValue risk_value, vector<RiskValue> *feeders) {
      vector<RiskValue> neighbors = find_neighbors(risk_value);

      if (!ShouldRemove(feeders)(risk_value)) {
        feeders->push_back(risk_value);
      }

      vector<RiskValue>::iterator pend = remove_if(neighbors.begin(), neighbors.end(), ShouldRemove(feeders));
      vector<RiskValue> n2 (neighbors.begin(), pend);

      for (int i = 0; i < n2.size(); i++) {
        find_feeders(n2[i], feeders);
      }
    }

    vector<vector<RiskValue> > find_basins() {
      vector<vector<RiskValue> > basins;
      vector<RiskValue> low_points = find_low_points();
      for (vector<RiskValue>::iterator iter = low_points.begin(); iter != low_points.end(); iter++) {
        vector<RiskValue> feeders;
        RiskValue low_point = *iter;
        find_feeders(low_point, &feeders);
        basins.push_back(feeders); 
      }

      return basins;
    }

    string to_string() {
      stringbuf buffer;
      ostream os (&buffer);
      for (vector<vector<RiskValue> >::iterator iter = risk_values.begin(); iter != risk_values.end(); iter++) {
        for (vector<RiskValue>::iterator viter = iter->begin(); viter != iter->end(); viter++) {
          os << viter->value;
        }
        os << endl;
      }
      return buffer.str();
    }

    static void print_risk_values(vector<RiskValue> *risk_values) {
      for (vector<RiskValue>::iterator iter = risk_values->begin(); iter != risk_values->end(); iter++) {
        cout << iter->value;
        if (next(iter) != risk_values->end()) cout << ",";
      }
      cout << endl;
    }
};


void print_basins(vector<vector<RiskValue> > *basins) {
  for (vector<vector<RiskValue> >::iterator iter = basins->begin(); iter != basins->end(); iter++) {
    vector<RiskValue> basin = *iter;
    
    for (int i = 0; i < basin.size(); i++) {
      cout << basin[i].value;
      if (i < basin.size() - 1) cout << ",";
    }
    cout << endl;
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
  // cout << "Loaded " << data.size() << " data lines" << endl;

  Heightmap heightmap (data);
  // cout << "Heightmap: " << endl;
  // cout << heightmap.to_string() << endl;

  vector<RiskValue> low_points = heightmap.find_low_points();
  // Heightmap::print_risk_values(&low_points);

  vector<vector<RiskValue> > basins = heightmap.find_basins();
  assert(low_points.size() == basins.size());

  //print_basins(&basins);

  vector<int> basin_sizes;
  for (int i = 0; i < basins.size(); i++) {
    basin_sizes.push_back(basins[i].size());
  }

  sort(basin_sizes.begin(), basin_sizes.end());
  reverse(basin_sizes.begin(), basin_sizes.end());

  int total = 1;
  for (int i = 0; i < 3; i++) {
    total *= basin_sizes[i];
  }

  cout << "Result: " << total << endl;

  cout << "Done" << endl;
}
