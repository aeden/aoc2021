#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/io.h"

#include <sstream>
#include <cctype>
#include <algorithm>
#include <map>

using namespace std;

string EDGE_DELIMITER = "-";
string START_NAME = "start";
string END_NAME = "end";

class IsUpper {
  public:
    bool operator()(char c) {
      return isupper(c);
    }
};

// A cave
class Node {
  public:
    string name;

    Node() { }

    Node(string name) {
      this->name = name;
    }

    bool is_start() {
      return name == START_NAME;
    }

    bool is_end() {
      return name == END_NAME;
    }

    bool is_big_cave() {
      return !is_start() && !is_end() && all_of(begin(name), end(name), IsUpper());
    }

    bool is_small_cave() {
      return !is_start() && !is_end() && !all_of(begin(name), end(name), IsUpper());
    }

    bool at_most_once() {
      return is_small_cave();
    }

    bool operator==(Node n) {
      return n.name == name;
    }

    string to_string() {
      stringbuf buffer;
      ostream os (&buffer);

      os << name ;
      if (is_start()) os << " (start)";
      if (is_end()) os << " (end)";
      if (is_big_cave()) os << " (big)";
      if (is_small_cave()) os << " (small)";

      return buffer.str();
    }
};

class Edge {
  public:
    Node start;
    Node end;

    string to_string() {
      return start.to_string() + EDGE_DELIMITER + end.to_string();
    }
};

class Path {
  public:
    vector <Node> nodes;

    string to_string() {
      stringbuf buffer;
      ostream os (&buffer);

      for (vector<Node>::iterator iter = nodes.begin(); iter != nodes.end(); iter++) {
        Node node = *iter;
        os << node.name;
        if (next(iter) != nodes.end()) os << ",";
      }

      return buffer.str();
    }
};

class Visited {
  public:
    Node next;
    vector<Node> visited;

    Visited(Node node, vector<Node> *visited) {
      this->next = node;
      vector<Node> visited_copy (visited->size());
      copy(visited->begin(), visited->end(), visited_copy.begin());;
      this->visited = visited_copy;
    }

    bool operator()(Node n) {
      if (n == next) {
        if (n.is_big_cave()) {
          return false;
        } else if (n.is_small_cave()) {
          // if any cave is visited twice, and n is visited return true
          // if no cave is visited twice, and n is visited return false
          // if n is never visited return false

          map<string, int> small_cave_map;
          for (vector<Node>::iterator iter = visited.begin(); iter != visited.end(); iter++) {
            if (iter->is_small_cave()) {
              if (small_cave_map.find(iter->name) == small_cave_map.end()) {
                small_cave_map[iter->name] = 1;
              } else {
                small_cave_map[iter->name] += 1;
              }
            }
          }

          vector<string> visited_twice;
          vector<string> visited_once;

          for (map<string, int>::iterator iter = small_cave_map.begin(); iter != small_cave_map.end(); iter++) {
            string name = iter->first;
            int value = iter->second;

            if (value == 2) {
              visited_twice.push_back(name); 
            } else if (value == 1) {
              visited_once.push_back(name);
            }
          }

          if (visited_twice.size() != 0) {
            return true;
          } else {
            return false;
          }
        } else {
          return true;
        }
      }
      return false;
    }
};

class Graph {
  public:
    vector<Edge> edges;
    Node start_node;

    Graph(vector<string> lines) {
      string delimiter = EDGE_DELIMITER;
      for (vector<string>::iterator iter = lines.begin(); iter != lines.end(); iter++) {
        string s = *iter;
        int pos = s.find(delimiter);
        string start_name = iter->substr(0, pos);
        string end_name = iter->substr(pos + 1);
        Edge edge;
        edge.start = Node(start_name);
        edge.end = Node(end_name);
        edges.push_back(edge); 

        if (edge.start.name == START_NAME) {
          start_node = edge.start;
        }

      }
    }

    void build_path(vector<Path> *paths, vector<Node> *visited) {
      vector <Node> path_nodes;
      for (vector<Node>::iterator iter = visited->begin(); iter != visited->end(); iter++) {
        path_nodes.push_back(*iter);
      }
      Path p;
      p.nodes = path_nodes;
      paths->push_back(p);
    }

    void traverse(Node node, vector<Path> *paths) {
      // for each start edge, visit its destination
      for (vector<Edge>::iterator iter = edges.begin(); iter != edges.end(); iter++) {
        if (iter->start == node) {
          vector<Node> visited;
          visited.push_back(node);
          visit(iter->end, paths, &visited);
          visited.pop_back();
        } else if(iter->end == node) {
          vector<Node> visited;
          visited.push_back(node);
          visit(iter->start, paths, &visited);
          visited.pop_back();
        }
      }
    }
    
    void visit(Node node, vector<Path> *paths, vector<Node> *visited) {
      // cout << "visiting node " << node.to_string() << endl;
      visited->push_back(node);
      // find connected edges
      for (vector<Edge>::iterator iter = edges.begin(); iter != edges.end(); iter++) {
        if (iter->start == node) {
          // cout << "found edge with start node: " << iter->to_string() << endl;
          Node next = iter->end;
          // cout << "check node: " << next.to_string() << endl;
          if (next.is_end()) {
            visited->push_back(next);
            // cout << "found end, build path from visited" << endl;
            build_path(paths, visited);
            visited->pop_back();
          } else if (find_if(visited->begin(), visited->end(), Visited(next, visited)) == visited->end()) {
            // cout << "moving to next node " << next.to_string() << endl;
            visit(next, paths, visited);
            visited->pop_back();
          } else {
            // cout << "already visited " << next.to_string() << endl;
          }
        } else if (iter->end == node) {
          //cout << "found edge withend node: " << iter->to_string() << endl;
          Node next = iter->start;
          //cout << "check node: " << next.to_string() << endl;
          if (next.is_end()) {
            visited->push_back(next);
            //cout << "found end, build path from visited" << endl;
            build_path(paths, visited);
            visited->pop_back();
          } else if (find_if(visited->begin(), visited->end(), Visited(next, visited)) == visited->end()) {
            //cout << "moving to next node " << next.to_string() << endl;
            visit(next, paths, visited);
            visited->pop_back();
          } else {
            //cout << "already visited " << next.to_string() << endl;
          } 
        }
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

  cout << "Graph:" << endl;
  cout << graph.to_string() << endl;


  vector<Path> paths;
  graph.traverse(graph.start_node, &paths);

  /*cout << "Paths:" << endl;*/
  /*for (vector<Path>::iterator iter = paths.begin(); iter != paths.end(); iter++) {*/
    /*cout << iter->to_string() << endl;*/
  /*}*/

  cout << "Total number of paths: " << paths.size() << endl;

  cout << "Done" << endl;
}
