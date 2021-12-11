#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/io.h"

#include <sstream>
#include <algorithm>

using namespace std;

class Point {
  public:
    int x;
    int y;

    Point() { }

    Point(int x, int y) {
      this->x = x;
      this->y = y;
    }

    string to_string() {
      return std::to_string(x) + "," + std::to_string(y);
    }

    bool operator==(Point p) {
      return p.x == x && p.y == y;
    }

    bool is_valid() {
      return x >= 0 && x <= 9 && y >= 0 && y <= 9;
    }
};

class Octopus {
  public:
    Point location;
    int energy;

    Octopus(int energy, Point location) {
      this->energy = energy;
      this->location = location;
    }

    string to_string() {
      stringbuf buffer;
      ostream os (&buffer);
      os << location.to_string() << ":" << std::to_string(energy);
      return buffer.str();
    }

    bool operator==(Octopus other) {
      return this->location == other.location;
    }
};

class Flashed {
  public:
    vector<Octopus> *flashed;

    Flashed(vector<Octopus> *flashed) {
      this->flashed = flashed;
    }

    bool operator()(Octopus o) {
      vector<Octopus>::iterator iter = find(flashed->begin(), flashed->end(), o);
      return iter != flashed->end();
    }
};

class Cave {
  public:
    vector<vector<Octopus> > octopuses;
    int flash_counter;

    Cave() {
      this->flash_counter = 0;
    }

    void flash(Octopus o, vector<Octopus> *neighbors, vector<Octopus> *flashed) {
      cout << "flash " << o.to_string() << endl;
      flashed->push_back(o);
      for (vector<Octopus>::iterator iter = neighbors->begin(); iter != neighbors->end(); iter++) {
        iter->energy += 1;
        if (iter->energy > 9) {
          cout << "additional flash needed for " << iter->to_string() << endl;
          vector<Octopus> next_neighbors = find_neighbors(*iter, flashed);          
          flash(*iter, &next_neighbors, flashed);
        }
      }
    }

    int step(int n) {
      cout << "Step " << n << endl;
      int flashes = 0;
      for (int i = 0; i < octopuses.size(); i++) {
        for (int j = 0; j < octopuses[i].size(); j++) {
          octopuses[i][j].energy += 1;
        }
      }

      cout << "Flashing" << endl;

      vector<Octopus> flashed;
      for (int i = 0; i < octopuses.size(); i++) {
        for (int j = 0; j < octopuses[i].size(); j++) {
          if (octopuses[i][j].energy > 9) {
            vector<Octopus> neighbors = find_neighbors(octopuses[i][j], &flashed);
            flash(octopuses[i][j], &neighbors, &flashed); 
          }
        }
      }

      for (int i = 0; i < flashed.size(); i++) {
        Point p = flashed[i].location;
        octopuses[p.y][p.x].energy = 0;
      }

      return flashes;
    }

    void print_octopuses(vector<Octopus> *octopuses) {
      for (vector<Octopus>::iterator iter = octopuses->begin(); iter != octopuses->end(); iter++) {
        cout << iter->to_string();
        if (next(iter) != octopuses->end()) cout << " | ";
      }
      cout << endl;
    }

    void configure(vector<string> data) {
      for (int y = 0; y < data.size(); y++) {
        vector<Octopus> line_octopuses;
        for (int x = 0; x < data[y].size(); x++) {
          int v = data[y][x] - '0';
          Point p = Point(x, y);
          Octopus o = Octopus(v, p);
          line_octopuses.push_back(o);
        }
        octopuses.push_back(line_octopuses);
      }
    }

    vector<Octopus> find_neighbors(Octopus o, vector<Octopus> *flashed) {
      vector<Octopus> neighbors; 
      Point l = o.location;

      Point left = Point(l.x - 1, l.y);
      if (left.is_valid()) {
        Octopus n = octopuses[left.y][left.x];
        if (!Flashed(flashed)(n)) neighbors.push_back(n);
      }

      Point right = Point(l.x + 1, l.y);
      if (right.is_valid()) {
        Octopus n = octopuses[right.y][right.x];
        if (!Flashed(flashed)(n)) neighbors.push_back(n);
      }

      Point up = Point(l.x, l.y - 1);
      if (up.is_valid()) {
        Octopus n = octopuses[up.y][up.x];
        if (!Flashed(flashed)(n)) neighbors.push_back(n);
      }

      Point down = Point(l.x, l.y + 1);
      if (down.is_valid()) {
        Octopus n = octopuses[down.y][down.x];
        if (!Flashed(flashed)(n)) neighbors.push_back(n);
      }

      Point left_up = Point(l.x - 1, l.y - 1);
      if (left_up.is_valid()) {
        Octopus n = octopuses[left_up.y][left_up.x];
        if (!Flashed(flashed)(n)) neighbors.push_back(n);
      }

      Point right_up = Point(l.x + 1, l.y - 1);
      if (right_up.is_valid()) {
        Octopus n = octopuses[right_up.y][right_up.x];
        if (!Flashed(flashed)(n)) neighbors.push_back(n);
      }

      Point left_down = Point(l.x - 1, l.y + 1);
      if (left_down.is_valid()) {
        Octopus n = octopuses[left_down.y][left_down.x];
        if (!Flashed(flashed)(n)) neighbors.push_back(n);
      }

      Point right_down = Point(l.x + 1, l.y + 1);
      if (right_down.is_valid()) {
        Octopus n = octopuses[right_down.y][right_down.x];
        if (!Flashed(flashed)(n)) neighbors.push_back(n);
      }

      return neighbors;
    }

    string to_string() {
      stringbuf buffer;
      ostream os (&buffer);
      for (int i = 0; i < octopuses.size(); i++) {
        for (int j = 0; j < octopuses[i].size(); j++) {
          os << octopuses[i][j].energy;
        }
        os << endl;
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
  
  Cave cave; 
  
  cout << "Configuring cave" << endl;
  cave.configure(data); 
 
  int flashes = 0;
  cout << "Executing steps" << endl;
  for (int i = 1; i <= steps; i++) {
    // cout << "Step " << i << endl;
    flashes += cave.step(i);
    cout << "After step " << i << endl;
    cout << cave.to_string() << endl;
  }

  cout << "Flashes: " << flashes << endl;
  cout << "Done" << endl;

}
