#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/io.h"

#include <sstream>
#include <cassert>

using namespace std;

class Heightmap {
  public:
    vector<vector<int> > risk_values;

    Heightmap() { }
    
    Heightmap(vector<string> data) {
      for (vector<string>::iterator iter = data.begin(); iter != data.end(); iter++) {
        vector<int> row;
        string line = *iter;
        for (string::iterator line_iter = line.begin(); line_iter != line.end(); line_iter++) {
          char v = *line_iter;
          row.push_back(v - '0');
        }
        risk_values.push_back(row);
      }
    }

    vector<int> find_low_points() {
      vector<int> low_points;
      for (int i = 0; i < risk_values.size(); i++) {
        for (int j = 0; j < risk_values[i].size(); j++) {
          vector<int> neighbors;
          if (j - 1 >= 0) {
            neighbors.push_back(risk_values[i][j - 1]);
          }
          if (j + 1 < risk_values[i].size()) {
            neighbors.push_back(risk_values[i][j + 1]);
          }
          if (i - 1 >= 0) {
            neighbors.push_back(risk_values[i-1][j]);
          }
          if (i + 1 < risk_values.size()) {
            neighbors.push_back(risk_values[i+1][j]);
          }
          
          // print_ints(&neighbors, ',');

          bool is_low_point = true;
          for (int k = 0; k < neighbors.size(); k++) {
            if (risk_values[i][j] >= neighbors[k]) {
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

    string to_string() {
      stringbuf buffer;
      ostream os (&buffer);
      for (vector<vector<int> >::iterator iter = risk_values.begin(); iter != risk_values.end(); iter++) {
        for (vector<int>::iterator viter = iter->begin(); viter != iter->end(); viter++) {
          os << *viter;
        }
        os << endl;
      }
      return buffer.str();
    }
};

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

  Heightmap heightmap (data);
  //cout << "Heightmap: " << endl;
  //cout << heightmap.to_string() << endl;

  vector<int> low_points = heightmap.find_low_points();
  // print_ints(&low_points, ',');

  int sum = 0;
  for (int i = 0; i < low_points.size(); i++) {
    sum += 1 + low_points[i];
  }
  cout << "Sum of lowpoints: " << sum << endl;

  cout << "Done" << endl;
}
