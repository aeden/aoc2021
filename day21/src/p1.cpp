#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/io.h"

#include <limits>
#include <algorithm>

using namespace std;

const int BOARD_SPACES = 10;
const int WIN_SCORE = 1000;

class Die {
  private:
    int value = 0;
    int sides = 100;

  public:
    int roll() {
      if (value == sides) {
        value = 0;
      }
      value += 1;
      return value;
    }
};

class Player {
  public:
    int player_number;
    int position;
    int score = 0;

    void move_forward(int spaces) {
      int next_position = position + spaces;
      if (next_position > BOARD_SPACES) {
        position = next_position % BOARD_SPACES;
      } else {
        position = next_position;
      }
      score += position;
    }

    string to_string() {
      return "Player " + std::to_string(player_number) + ": " + std::to_string(position);
    }
};

int check_winners(vector<Player> *players, vector<Player> *winners) {
  int winners_count = 0;
  for (vector<Player>::iterator iter = players->begin(); iter != players->end(); iter++) {
    if (iter->score >= WIN_SCORE) {
      winners->push_back(*iter);
      winners_count++;
    }
  }
  return winners_count;
}

class Loser {
  public:
    bool operator()(Player rhs) {
      return rhs.score < WIN_SCORE;
    }
};

int main(int argc, char** argv) {
  string filename;

  if (argc != 2) {
    cout << "Usage: " << argv[0] << " datafile" << endl;
    return 1;
  } else {
    filename = argv[1];
  }

  vector<string> data = read_data(filename);
  cout << "Loaded " << data.size() << " data lines" << endl;

  vector<Player> players;
  for (int i = 0; i < data.size(); i++) {
    Player p;
    p.player_number = stoi(data[i].substr(7, 1));
    p.position = stoi(data[i].substr(data[i].size() - 1));
    players.push_back(p);
    cout << p.to_string() << endl;
  }

  int total_rolls = 0;
  Die d;
  vector<Player> winners;
  int turn = 0;
  int max_turns = numeric_limits<int>::max();
  while (check_winners(&players, &winners) == 0 && turn < max_turns) {
    turn++;
    for (vector<Player>::iterator iter = players.begin(); iter != players.end(); iter++) {
      int r1 = d.roll();
      int r2 = d.roll();
      int r3 = d.roll();
      total_rolls += 3;
      int total = r1 + r2 + r3;
      cout << "player " << iter->player_number << " total roll (" << r1 << " + " << r2 << " + " << r3 << "): " << total << endl;
      iter->move_forward(total > BOARD_SPACES ? total % BOARD_SPACES : total);
      cout << "player " << iter->player_number << " score after turn " << turn << ": " << iter->score << endl;

      if (iter->score >= WIN_SCORE) {
        break;
      }
    }
  }

  vector<Player>::iterator loser = find_if(players.begin(), players.end(), Loser());

  cout << "Total rolls: " << total_rolls << endl;
  cout << "Result: " << total_rolls * loser->score << endl;

  cout << "Done" << endl;
  
  return 0;
}
