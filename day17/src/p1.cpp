#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/io.h"

#include <sstream>

using namespace std;

class Point {
  public:
    double x;
    double y;

    Point() {
      x = 0;
      y = 0;
    }

    Point(double x, double y) {
      this->x = x;
      this->y = y;
    }

    string to_string() {
      return std::to_string(int(x)) + ":" + std::to_string(int(y));
    }
};

class Velocity {
  public:
    int x;
    int y;

    Velocity() {}

    Velocity(int x, int y) {
      this->x = x;
      this->y = y;
    }

    void parse(string s) {
      int pos = s.find(",");
      x = stoi(s.substr(0, pos));
      y = stoi(s.substr(pos + 1));
    }

    string to_string() {
      return std::to_string(x) + "," + std::to_string(y);
    }
};

class Probe {
  public:
    Point location;
    Velocity velocity;

    Probe() {
      this->location = Point();
      this->velocity = Velocity();
    }

    void step() {
      location.x += velocity.x;
      location.y += velocity.y;

      if (velocity.x > 0) {
        velocity.x -= 1;
      } else if (velocity.x < 0) {
        velocity.x += 1;
      }

      velocity.y -= 1;
    }

    string to_string() {
      stringbuf buffer;
      ostream os (&buffer); 

      os << location.x << "," << location.y << " (" << velocity.x << "," << velocity.y << ")"; 

      return buffer.str();
    }
};

const int TEXT_LENGTH=13;

class Target {
  public:
  int x_start;
  int x_end;
  int y_start;
  int y_end;

  void parse(string line) {
    string xydata = line.substr(TEXT_LENGTH);
    int pos = xydata.find(", ");
    string xpart = xydata.substr(2, pos - 2);
    string ypart = xydata.substr(pos + 4);
    int xsep = xpart.find("..");
    x_start = stoi(xpart.substr(0, xsep));
    x_end = stoi(xpart.substr(xsep + 2));

    int ysep = ypart.find("..");
    y_start = stoi(ypart.substr(0, ysep));
    y_end = stoi(ypart.substr(ysep + 2));
  }

  bool in_bounds(Point p) {
    bool in_x = p.x >= x_start && p.x <= x_end;
    bool in_y = p.y >= y_start && p.y <= y_end;
    return in_x && in_y;
  }

  bool beyond_bounds(Point p) {
    return beyond_x(p) || beyond_y(p);
  }

  bool beyond_x(Point p) {
    return p.x > x_end;
  }

  bool beyond_y(Point p) {
    return p.y < y_start;
  }

  string to_string() {
    stringbuf buffer;
    ostream os (&buffer); 

    os << x_start << ".." << x_end << ", " << y_start << ".." << y_end;

    return buffer.str();
  }
};

class Result {
  public:
    int max_height = 0;
    bool hit;
    bool beyond_x;
    bool beyond_y;

    bool beyond() {
      return beyond_x || beyond_y;
    }
};

void print_debug_info(Result *r, Probe *p) {
  if (r->hit) {
    cout << "Probe hit (maxh=" << r->max_height << "): " << p->to_string() << endl;
  } else if (r->beyond()) {
    cout << "Probe went beyond (x:" << r->beyond_x << ",y:" << r->beyond_y << "): " << p->to_string() << endl;
  }

  cout << "Max height: " << r->max_height << endl;

}

Result fire_probe(Probe *p, Target t) {
  Result r;
  do {
    p->step();
    if (p->location.y > r.max_height) r.max_height = p->location.y;
    r.hit = t.in_bounds(p->location);
    r.beyond_x = t.beyond_x(p->location);
    r.beyond_y = t.beyond_y(p->location);
  } while (!r.hit && !r.beyond());

  return r;
}

int main (int argc, char** argv) {
  string filename;
  Velocity initial_velocity;
  bool use_initial_velocity = false;

  if (argc != 2 && argc != 3) {
    cout << "Usage: " << argv[0] << " datafile velocity" << endl;
    return 1;
  } else {
    filename = argv[1];
    if (argc == 3) {
      use_initial_velocity = true;
      initial_velocity.parse(argv[2]);
    }
  }

  vector<string> data = read_data(filename);
  cout << "Loaded " << data.size() << " data lines" << endl;

  Target t;
  t.parse(data[0]);
  cout << "Target range: " << t.to_string() << endl;

  Probe p;
  if (use_initial_velocity) {
    cout << "Fire probe with initial velocity given: " << initial_velocity.to_string() << endl;
    p.velocity = initial_velocity;

    cout << "Probe:" << p.to_string() << endl;

    Result r = fire_probe(&p, t);

    print_debug_info(&r, &p);
  } else {
    cout << "Fire probe until max height determined" << endl;

    int x = 1;
    int max_height = 0;
    Result r;

    while (x < t.x_end) {
      //cout << "X = " << x << endl;
      int y = 1;
      do {
        //cout << "Y = " << y << endl;
        p.location = Point(0, 0);
        p.velocity = Velocity(x, y);
        // cout << "Firing probe with velocity " << p.velocity.to_string() << endl;
        r = fire_probe(&p, t);
      
        if (r.hit && r.max_height > max_height) max_height = r.max_height;
        
        y++;

      } while (!r.beyond_x && y < abs(t.y_start));

      x += 1;
    }

    cout << "Max Height: " << max_height << endl;
    
  }

  cout << "Done" << endl;

  return 0;
}
