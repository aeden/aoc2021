#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lib/io.h"

#include <sstream>
#include <bitset>
#include <climits>

using namespace std;

const int LITERAL_TYPE_ID = 4;
const int HEADER_SIZE = 6;
const int BIT_LENGTH_FIELD_SIZE = 15;
const int SUBPACKET_COUNT_FIELD_SIZE = 11;

class Data {
  public:
    vector<bool> bits;

    void push_back(bitset<4> bs) {
      //cout << "push back bitset " << bs << endl;
      bits.push_back(bs[3]);
      bits.push_back(bs[2]);
      bits.push_back(bs[1]);
      bits.push_back(bs[0]);
      //cout << "after push back: " << to_string() << endl;
    }

    string to_string() {
      stringbuf buffer;
      ostream os (&buffer);
      for (vector<bool>::iterator iter = bits.begin(); iter != bits.end(); iter++) {
        os << *iter;
      }
      return buffer.str();
    }

    int to_int() {
      return stoi(to_string(), 0, 2);
    }
};

class Packet {
  public:
    int version;
    int type_id;

    bool is_literal() {
      return type_id == LITERAL_TYPE_ID;
    }

    bool is_operator() {
      return type_id != LITERAL_TYPE_ID;
    }
};

class Literal : public Packet {
  public:
    int value;

    Literal() {
      this->type_id = LITERAL_TYPE_ID;
    }
};

class Operator : public Packet {
  public:
    Operator(int type_id) {
      this->type_id = type_id;
    }
};

void hex_to_bin(char c, Data *bin) {
  //cout << "hex_to_bin: " << c << endl;
  switch (c) {
    case '0': bin->push_back(bitset<4>("0000")); break;
    case '1': bin->push_back(bitset<4>("0001")); break;
    case '2': bin->push_back(bitset<4>("0010")); break;
    case '3': bin->push_back(bitset<4>("0011")); break;
    case '4': bin->push_back(bitset<4>("0100")); break;
    case '5': bin->push_back(bitset<4>("0101")); break;
    case '6': bin->push_back(bitset<4>("0110")); break;
    case '7': bin->push_back(bitset<4>("0111")); break;
    case '8': bin->push_back(bitset<4>("1000")); break;
    case '9': bin->push_back(bitset<4>("1001")); break;
    case 'A': bin->push_back(bitset<4>("1010")); break;
    case 'B': bin->push_back(bitset<4>("1011")); break;
    case 'C': bin->push_back(bitset<4>("1100")); break;
    case 'D': bin->push_back(bitset<4>("1101")); break;
    case 'E': bin->push_back(bitset<4>("1110")); break;
    case 'F': bin->push_back(bitset<4>("1111")); break;
  };
}

void hex_string_to_bin(string s, Data *bin) {
  for (int i = 0; i < s.size(); i++) {
    hex_to_bin(s[i], bin);
  }
}

int parse_literal(Data *bin, int pos, Literal *p) {
  Data literal_data;
  bool packets_remain = false;
  do {
    packets_remain = bin->bits[pos+=1];

    bitset<4> part;
    part.set(3, bin->bits[pos+=1]);
    part.set(2, bin->bits[pos+=1]);
    part.set(1, bin->bits[pos+=1]);
    part.set(0, bin->bits[pos+=1]);
    literal_data.push_back(part);
  } while (packets_remain);

  p->value = literal_data.to_int();

  return pos;
}

int parse_packet(Data *bin, int pos, vector<Packet> *packets) {
  bitset<3> version;
  version.set(2, bin->bits[pos]);
  version.set(1, bin->bits[pos += 1]);
  version.set(0, bin->bits[pos += 1]);

  bitset<3> type_id;
  type_id.set(2, bin->bits[pos += 1]);
  type_id.set(1, bin->bits[pos += 1]);
  type_id.set(0, bin->bits[pos += 1]);

  if (int(type_id.to_ulong()) == LITERAL_TYPE_ID) {
    cout << "Parse literal packet (version=" << version.to_ulong() << ", type_id=" << type_id.to_ulong() << ")" << endl;
    Literal l;
    //cout << "pos before parse_literal: " << pos << endl;
    pos = parse_literal(bin, pos, &l); 
    //cout << "pos after parse_literal: " << pos << endl;
    l.version = int(version.to_ulong());
    l.type_id = int(type_id.to_ulong());
    packets->push_back(l);
  } else {
    cout << "Parse operator packet (version=" << version.to_ulong() << ", type_id=" << type_id.to_ulong() << ")" << endl;
    Operator p (int(type_id.to_ulong()));
    p.version = int(version.to_ulong());
    Data operator_data;
    bool length_type_id = bin->bits[pos+=1];
    if (length_type_id) {
      cout << "Operator uses subpacket count" << endl;
      Data count_data;
      pos += 1;

      for (int i = 0; i < SUBPACKET_COUNT_FIELD_SIZE; i++) {
        count_data.bits.push_back(bin->bits[pos+i]);
      }
      pos += SUBPACKET_COUNT_FIELD_SIZE;
      cout << "Parsing " << count_data.to_int() << " subpackets" << endl;
      for (int i = 0; i < count_data.to_int(); i++) {
        pos = parse_packet(bin, pos, packets);
        pos += 1;
      }
      cout << "Done parsing " << count_data.to_int() << " subpackets" << endl;
    } else {
      cout << "Operator uses bit length" << endl;
      Data length_data;
      pos+=1;

      for (int i = 0; i < BIT_LENGTH_FIELD_SIZE; i++) {
        length_data.bits.push_back(bin->bits[pos+i]);
      }
      pos += BIT_LENGTH_FIELD_SIZE;
      int bits_parsed = 0;
      //cout << "Subpackets length: " << length_data.to_int() << endl;
      while(bits_parsed < length_data.to_int()) {
        int start_pos = pos;
        cout << "parse_packet call in bit length operator" << endl;
        pos = parse_packet(bin, pos, packets);
        pos += 1;
        bits_parsed = pos - start_pos;
        //cout << "Bits parsed: " << bits_parsed << endl;
      }
      cout << "Subpackets length reached" << endl;
    }
    packets->push_back(p);
  }

  return pos;
}

void parse(Data *bin, vector<Packet> *packets) {
  cout << "Parse data" << endl;
  parse_packet(bin, 0, packets);
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

  string hex_string = data[0];
  cout << "Hex string: " << hex_string << endl;

  // convert from hex to a vector of bits
  Data bin;
  hex_string_to_bin(hex_string, &bin);

  cout << "Binary representation:" << endl;
  cout << bin.to_string() << endl;

  vector<Packet> packets;
  parse(&bin, &packets);
  cout << "Parsed " << packets.size() << " packets" << endl;

  int sum = 0;
  for (vector<Packet>::iterator iter = packets.begin(); iter != packets.end(); iter++) {
    sum += iter->version;
  }
  cout << "Sum of version numbers: " << sum << endl;

  /*for (int i = 0; i < hex_string.size(); i++) {*/
    /*char c = hex_string[i];*/
    /*bitset<sizeof(char) * CHAR_BIT> binary(c);*/
    /*cout << "Letter: " << c << "\t";*/
    /*cout << "Hex: " << hex << (int)c << "\t";*/
    /*cout << "Binary: " << binary << endl;*/
  /*}*/

  cout << "Done" << endl;

  return 0;
}
