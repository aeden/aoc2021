#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <list>

using namespace std;

vector<string> read_data(string filename) {
  ifstream infile;
  infile.open(filename);
  vector<string> data;

  if (infile.is_open()) {
    string line;
    while(getline(infile, line)) {
      data.push_back(line);
    }
    return data;
  } else {
    cout << "Failed to read data" << endl;
    return data;
  }
}

const int WIDTH = 5;
const int HEIGHT = 5;

class Number {
  public:
    int value;
    bool marked;

    Number() {
      value = 0;
      marked = false;
    }

    Number(string value_string) {
      value = stoi(value_string);
      marked = false;
    }

    string to_string() {
      return std::to_string(value);
    }
};

class Board {
  public:
    int id;
    Number numbers[WIDTH][HEIGHT];
    bool is_setup;
    int winning_number;

    Board() {
      id = 0;
      is_setup = false;
    }

    void add_row(int row_num, string row) {
      char* chars = &row[0];
      for (int i = 0; i < 5; i++) {
        string v;
        char v1 = chars[(i*3)];
        char v2 = chars[(i*3)+1];

        if (v1 != ' ')
          v += v1;
        v += v2;

        numbers[row_num][i] = Number(v);
      }

      is_setup = true;
    }

    void mark_board(int number) {
      for (int i = 0; i < sizeof(numbers) / sizeof(numbers[0]); i++) {
        for (int j = 0; j < sizeof(numbers[i]) / sizeof(numbers[i][0]); j++) {
          if (numbers[i][j].value == number) {
            //cout << "marked " << i << "," << j << "on board" << endl;
            numbers[i][j].marked = true;
          }
        }
      }
    }

    bool is_winner() {
      int marked_in_column[5] = {0};
      for (int i = 0; i < sizeof(numbers) / sizeof(numbers[0]); i++) {
        int marked_in_row = 0;
        for (int j = 0; j < sizeof(numbers[i]) / sizeof(numbers[i][0]); j++) {
          if (numbers[i][j].marked) {
            marked_in_row += 1;
            marked_in_column[j] += 1;
          }
        }
        if (marked_in_row == 5) {
          return true;
        }
      }

      for (int i = 0; i < sizeof(marked_in_column) / sizeof(marked_in_column[0]); i++) {
        if (marked_in_column[i] == 5) {
          return true;
        }
      }

      return false;
    }

    int score() {
      int score = 0;
      for (int i = 0; i < sizeof(numbers) / sizeof(numbers[0]); i++) {
        for (int j = 0; j < sizeof(numbers[i]) / sizeof(numbers[i][0]); j++) {
          if (!numbers[i][j].marked) {
            score += numbers[i][j].value;
          }
        }
      }
      cout << score << " * " << winning_number << endl;
      return score * winning_number;
    }

    string to_string() {
      stringbuf buffer;
      ostream os (&buffer);
      for (int i = 0; i < sizeof(numbers) / sizeof(numbers[0]); i++) {
        for (int j = 0; j < sizeof(numbers[i]) / sizeof(numbers[i][0]); j++) {
          os << numbers[i][j].value << " "; 
        }
        os << endl;
      }
      return buffer.str();
    }

    bool check_is_setup() {
      return is_setup;
    }
};

void print_numbers(list<int> *numbers) {
  cout << "numbers list: ";
  for (list<int>::iterator it = numbers->begin(); it != numbers->end(); it++) {
    std::cout << *it << " ";
  }
  cout << endl;
}

void parse_numbers_list(string row, list<int> *numbers) {
  stringstream tokenizer(row);
  string token;
  while (getline(tokenizer, token, ',')) {
    numbers->push_back(stoi(token));
  }

  print_numbers(numbers); 
}

void parse_data(vector<string> data, vector<Board> *boards, list<int> *numbers) {
  Board current_board;
  int current_row;
  for (int i = 0; i < data.size(); i++) {
    if (i == 0) {
      parse_numbers_list(data[i], numbers);
    } else if (data[i].compare("") == 0) {
      if (current_board.check_is_setup()) {
        // cout << "built board with id " << current_board.id << endl;
        boards->push_back(current_board);
      }
      int next_id = current_board.id + 1;
      current_board = Board();
      current_board.id = next_id;
      current_row = 0;
    } else {
      current_board.add_row(current_row, data[i]);
      current_row += 1;
    }
  }
  // cout << "built board with id " << current_board.id << endl;
  boards->push_back(current_board);
}

void print_boards(vector<Board> boards) {
  for (int i = 0; i < boards.size(); i++) {
    cout << boards[i].to_string() << endl;
  }
}

void mark_boards(int number_drawn, vector<Board> *boards) {
  // cout << "number drawn: " << number_drawn << endl;
  for (vector<Board>::iterator it = boards->begin(); it != boards->end(); it++) {
    it->mark_board(number_drawn);
  }
}

vector<Board*> find_winners(vector<Board> *boards) {
  vector<Board*> winners;
  for (vector<Board>::iterator it = boards->begin(); it != boards->end(); it++) {
    if (it->is_winner()) {
      // cout << "found winning board " << it->id << endl;
      winners.push_back(&*it);
    }
  }
  return winners;
}

bool element_in_winners(vector<Board*> winners, Board *winner) {
  for (int i = 0; i < winners.size(); i++) {
    if (winners[i]->id == winner->id) {
      return true;
    }
  }
  return false;
}

Board* draw_numbers(vector<Board> *boards, list<int> *numbers) {
  vector<Board*> winners;
  for (list<int>::iterator it = numbers->begin(); it != numbers->end(); it++) {
    mark_boards(*it, boards);
    vector<Board*> round_winners = find_winners(boards);
    for (int i = 0; i < round_winners.size(); i++) {
      Board *winner = round_winners[i];
      if (!element_in_winners(winners, winner)) {
        // cout << "pushing " << winner->id << " into winners vector with winning number " << *it << endl;
        winner->winning_number = *it;
        winners.push_back(winner);
        if (winners.size() == boards->size()) {
          return winner;
        }
      }
    }
  }
  // cout << "winners has " << winners.size() << " elements" << endl;
  return winners.back();
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
  cout << "Loaded " << data.size() << " data lines" << endl;

  vector<Board> boards;
  list<int> numbers;

  parse_data(data, &boards, &numbers);

  //cout << "Built " << boards.size() << " boards" << endl;
  // print_boards(boards);
  
  Board *winner = draw_numbers(&boards, &numbers);
  if (winner == NULL) {
    cout << "No winner found" << endl;
  } else {
    cout << "The last winner is:" << endl;
    cout << winner->to_string() << endl;
    cout << "With a score of: " << winner->score() << endl;
  }

  cout << "Done" << endl;
  return 0;
}
