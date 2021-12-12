#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/io.h"

#include <sstream>

using namespace std;

string EDGE_DELIMITER = "-";

class Vertices {
  public:
    string name;

    Vertices() { }

    Vertices(string name) {
      this->name = name;
    }

    string to_string() {
      return name;
    }
};

class Edge {
  public:
    Vertices start;
    Vertices end;

    string to_string() {
      return start.to_string() + EDGE_DELIMITER + end.to_string();
    }
};

class Graph {
  public:
    vector<Edge> edges;

    Graph(vector<string> lines) {
      string delimiter = EDGE_DELIMITER;
      for (vector<string>::iterator iter = lines.begin(); iter != lines.end(); iter++) {
        string s = *iter;
        int pos = s.find(delimiter);
        string start_name = iter->substr(0, pos);
        string end_name = iter->substr(pos + 1);
        Edge edge;
        edge.start = Vertices(start_name);
        edge.end = Vertices(end_name);
        edges.push_back(edge); 
      }
    }

    string to_string() {
      stringbuf buffer;
      ostream os (&buffer);

      for (vector<Edge>::iterator iter = edges.begin(); iter != edges.end(); iter++) {
        Edge edge = *iter;
        os << edge.to_string();
        if (next(iter) != edges.end()) os << endl;
      }

      return buffer.str();
    }
};


int main (int argc, char** argv) {
  string filename;
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " datafile steps" << endl;
    return 1;
  } else {
    filename = argv[1];
  }

  vector<string> data = read_data(filename);
  cout << "Loaded " << data.size() << " data lines" << endl;

  Graph graph (data);

  cout << "Graph: " << endl;
  cout << graph.to_string() << endl;

  cout << "Done" << endl;
}
