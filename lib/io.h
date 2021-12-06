#ifndef MYFILE_H_
#define MYFILE_H_

#include <string>
#include <vector>

using namespace std;

vector<string> read_data(string filename);
void parse_ints(string row, vector<int> *numbers, char separator);
void print_ints(vector<int> *numbers, char separator);

#endif
