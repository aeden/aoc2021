#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "lib/io.h"

using namespace std;

const string SEGMENT_DELIMITER = " -> ";
const string COORDINATE_DELIMITER = ",";

class Coordinates {
  public:
    int x;
    int y;

    Coordinates() {
      x = 0;
      y = 0;
    }

    Coordinates(int x, int y) {
      this->x = x;
      this->y = y;
    } 

    bool operator== ( Coordinates const& rhs) const {
      return rhs.x == x && rhs.y == y;
    }
    
    bool operator< ( const Coordinates& rhs) const {
      return (y < rhs.y || (y == rhs.y && x < rhs.x));
    }

    string to_string() {
      return std::to_string(x) + COORDINATE_DELIMITER + std::to_string(y);
    }
};

void print_coordinates(vector<Coordinates> coordinates) {
  for (int i = 0; i < coordinates.size(); i++) {
    cout << coordinates[i].to_string() << endl;
  }
}

class Line {
  public:
    Coordinates start;
    Coordinates end;

    Line(string data) {
      int pos;

      pos = data.find(SEGMENT_DELIMITER);
      string start_str = data.substr(0, pos);
      data.erase(0, pos + SEGMENT_DELIMITER.length());
      string end_str = data;

      pos = start_str.find(COORDINATE_DELIMITER);
      string start_x = start_str.substr(0, pos);
      start_str.erase(0, pos + COORDINATE_DELIMITER.length());
      string start_y = start_str;
      start = Coordinates(stoi(start_x), stoi(start_y));

      pos = end_str.find(COORDINATE_DELIMITER);
      string end_x = end_str.substr(0, pos);
      end_str.erase(0, pos + COORDINATE_DELIMITER.length());
      string end_y = end_str;
      end = Coordinates(stoi(end_x), stoi(end_y));
    }

    bool is_diagonal() {
      return !(start.x == end.x || start.y == end.y);
    }

    // Returns all points covered by the line.
    vector<Coordinates> points() {
      vector <Coordinates> points;
      if (start.x == end.x) {
        int x = start.x;
        if (start.y > end.y) {
          for (int i = end.y; i < start.y + 1; i++) {
            points.push_back(Coordinates(x, i));
          }
        } else {
          for (int i = start.y; i < end.y + 1; i++) {
            points.push_back(Coordinates(x, i));
          }
        }
      } else {
        int y = start.y;
        if (start.x > end.x) {
          for (int i = end.x; i < start.x + 1; i++) {
            points.push_back(Coordinates(i, y));
          } 
        } else {
          for (int i = start.x; i < end.x + 1; i++) {
            points.push_back(Coordinates(i, y));
          }
        }
      }
      return points;
    }

    string to_string() {
      return start.to_string() + SEGMENT_DELIMITER + end.to_string();
    }
};

class Diagram {
  public:
    vector<Line> lines;
    Coordinates top_left;
    Coordinates bottom_right;

    Diagram(vector<string> data) {
      for (int i = 0; i < data.size(); i++) {
        Line line = Line(data[i]); 
        lines.push_back(line);
        
        vector<Coordinates> points = line.points();
        for (int j = 0; j < points.size(); j++) {
          if (points[j].x < top_left.x) top_left.x = points[j].x;
          if (points[j].x > bottom_right.x) bottom_right.x  = points[j].x;
          if (points[j].y < top_left.x) top_left.y = points[j].y;
          if (points[j].y > bottom_right.y) bottom_right.y = points[j].y;
        } 
      }
    }

    vector<Coordinates> covered_points() {
      vector<Coordinates> covered_points;
      for (int i = 0; i < lines.size(); i++) {
        if (lines[i].is_diagonal()) continue;
        vector<Coordinates> points = lines[i].points();
        for (int j = 0; j < points.size(); j++) {
          covered_points.push_back(points[j]);
        }
      }
      return covered_points;
    }
    
    int covered_count(Coordinates point) {
      int count = 0;
      vector<Coordinates> points = covered_points();
      for (int i = 0; i < points.size(); i++) {
        if (points[i] == point) {
          count += 1;
        }
      }
      return count;
    }

    vector<Coordinates> overlapping_points() {
      map<Coordinates,int> overlap;
      vector<Coordinates> overlapping;
      vector<Coordinates> points = covered_points();
      for (vector<Coordinates>::iterator it = points.begin(); it != points.end(); it++) {
        Coordinates p = *it;
        if (overlap.find(p) == overlap.end()) {
          overlap[p] = 1;
        } else {
          overlap[p] = overlap[p] + 1;
        }
      }

      for (map<Coordinates,int>::iterator iter = overlap.begin(); iter != overlap.end(); iter++) {
        Coordinates p = iter->first;
        int count = iter->second;
        if (count >= 2) {
          overlapping.push_back(p);
        }
      }

      return overlapping;
    }

    string to_string() {
      stringbuf buffer;
      ostream os (&buffer);
      
      for (int i = top_left.y; i <= bottom_right.y; i++) {
        for (int j = top_left.x; j <= bottom_right.x; j++) {
          Coordinates current = Coordinates(j, i);
          int count = covered_count(current);
          if (count == 0) {
            os << ".";
          } else {
            os << count;
          }
        }
        os << endl;
      }

      return buffer.str();
    }

    string to_data_string() {
      stringbuf buffer;
      ostream os (&buffer);
      for (int i = 0; i < lines.size(); i++) {
        os << lines[i].to_string();
        if (i < lines.size() - 1) os << endl;
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

  Diagram diagram = Diagram(data);
  
  cout << "Top left: " << diagram.top_left.to_string() << endl;
  cout << "Bottom right: " << diagram.bottom_right.to_string() << endl;

  cout << "Number of overlapping points: " << diagram.overlapping_points().size() << endl;

  cout << "Done" << endl;
}
