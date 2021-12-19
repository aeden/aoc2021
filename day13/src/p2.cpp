#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/io.h"

#include <sstream>
#include <cmath>
#include <algorithm>
#include <cassert>

using namespace std;

const string POINT_DELIMITER=",";
const string FOLD_DELIMITER="=";
const short UP = 0;
const short LEFT = 1;

class Point {
  public:
    double x;
    double y;

    Point() { }

    Point(double x, double y) {
      this->x = x;
      this->y = y;
    }

    string to_string() {
      return std::to_string((int)x) + POINT_DELIMITER + std::to_string((int)y);
    }

    bool operator==(Point p) {
      return (int)p.x == (int)x && (int)p.y == (int)y;
    }

    double distance_from(Point p) {
      return sqrt(pow(x - p.x, 2) + pow(y - p.y, 2));
    }
};

class PointSorter {
  public:
    bool operator()(Point lhs, Point rhs) {
      return lhs.distance_from(Point(0, 0)) < rhs.distance_from(Point(0, 0)); 
    }
};

class ArePointsEqual {
  public:
    bool operator()(Point lhs, Point rhs) {
      return lhs == rhs;
    }
};

class Fold {
  public:
    short direction;
    int line_number;

    string to_string() {
      string line = "x";
      if (direction == UP) line = "y";
      return "fold along " + line + FOLD_DELIMITER + std::to_string(line_number); 
    }
};

class Paper {
  public:
    vector<Point> dots;
    vector<Fold> folds;
    int width;
    int height;

    void fold_once() {
      fold_once(0);
    }

    void fold_once(int i) {
      Fold f = folds[i];
      if (f.direction == UP) {
        fold_up(f.line_number);
        height = f.line_number;
      } else {
        fold_left(f.line_number);
        width = f.line_number;
      }
      merge_overlapping();
    }

    void fold_all() {
      for (int i = 0; i < folds.size(); i++) {
        fold_once(i); 
      }
    }

    void fold_up(int line_number) {
      for (vector<Point>::iterator iter = dots.begin(); iter != dots.end(); iter++) {
        if (iter->y > line_number) {
          int delta = (iter->y - line_number) * 2;
          iter->y = iter->y - delta;
        }
      }
    }

    void fold_left(int line_number) {
      for (vector<Point>::iterator iter = dots.begin(); iter != dots.end(); iter++) {
        if (iter->x > line_number) {
          int delta = (iter->x - line_number) * 2;
          iter->x = iter->x - delta;
        }
      }
    }

    void merge_overlapping() {
      sort(dots.begin(), dots.end(), PointSorter());
      vector<Point>::iterator iter = unique(dots.begin(), dots.end(), ArePointsEqual());
      dots.resize(distance(dots.begin(), iter));
    }

    string to_string() {
      stringbuf buffer;
      ostream os (&buffer); 

      for (vector<Point>::iterator iter = dots.begin(); iter != dots.end(); iter++) {
        os << iter->to_string() << endl;
      }
      
      os << endl;

      for (vector<Fold>::iterator iter = folds.begin(); iter != folds.end(); iter++) {
        os << iter->to_string();
        if (next(iter) != folds.end()) os << endl;
      }

      return buffer.str();
    }

    string draw() {
      stringbuf buffer;
      ostream os (&buffer);

      for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
          vector<Point>::iterator iter  = find(dots.begin(), dots.end(), Point(x, y));
          if (iter == dots.end()) {
            os << ".";
          } else {
            os << "#";
          }
        }
        os << endl;
      }
      
      return buffer.str();
    }

    void parse_data(vector<string> data) {
      bool points_loaded = false;
      width = 0;
      height = 0;
      for (int i = 0; i < data.size(); i++) {
        string line = data[i];
        if (line == "") {
          points_loaded = true;
        } else if(points_loaded) {
          // fold instruction
          Fold f;
          int pos = line.find(FOLD_DELIMITER);
          string x_or_y = line.substr(pos-1, 1);
          string line_number = line.substr(pos + 1);

          assert(x_or_y == "x" || x_or_y == "y");

          if (x_or_y == "x") {
            f.direction = LEFT;
          } else {
            f.direction = UP;
          }

          f.line_number = stoi(line_number);
          folds.push_back(f);
        } else {
          // dot coordinates
          Point p;
          int pos = line.find(POINT_DELIMITER);
          string x = line.substr(0, pos);
          string y = line.substr(pos + 1);
          p.x = stoi(x);
          p.y = stoi(y);
          if (p.x > width) width = p.x;
          if (p.y > height) height = p.y;
          dots.push_back(p);
        }
      }
      sort(dots.begin(), dots.end(), PointSorter());
    }
};

int main (int argc, char** argv) {
  string filename;
  int max_folds = -1;
  if (argc != 2 && argc != 3) {
    cout << "Usage: " << argv[0] << " datafile [max_folds]" << endl;
    return 1;
  } else {
    filename = argv[1];
    if (argc == 3)
      max_folds = stoi(argv[2]);
  }

  vector<string> data = read_data(filename);
  cout << "Loaded " << data.size() << " data lines" << endl;

  Paper paper;
  paper.parse_data(data);

  //cout << "Start:" << endl;
  //cout << paper.draw() << endl;

  if (max_folds == -1) max_folds = paper.folds.size();

  for (int i = 0; i < max_folds; i++) {
    paper.fold_once(i);
  }

  cout << "After fold " << max_folds << endl;
  cout << paper.draw() << endl;

  cout << "Visible: " << paper.dots.size() << endl;

  cout << "Done" << endl;
}
