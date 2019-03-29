#ifndef _CLIENT_FUNCT_H_
#define _CLIENT_FUNCT_H_

using namespace std;

struct xy_pos { // useful struct
  int16_t x, y;
  xy_pos(int16_t x1 = 0, int16_t y1 = 0) : x(x1), y(y1) {}
};

struct shared_vars {
  bool select_pushed, action_pushed, move_pushed;
  bool readBuildings, readUnits;
  bool readData;

  // very simple finite state machine:
  // STATE2: Main Menu
  // STATE3: Map Selection
  // STATE4: Game play
  // STATE5: End Screen
  enum {STATE2, STATE3, STATE4, STATE5} curr_mode;

};

void drawMainMenu(); // draws main menu

char updateMainMenu(); // updates main menu returns which mode was selected

void drawMapMenu(); // draws map menu

char updateMapMenu(); // updates map menu returns which map was selected

void process_line(); // determines what to do based on what has been read into buffer

void read_line(); // reads characters from serial port and stores them in buffer when it reads'\n'

void process_input(); // reads which buttons are pushed

void setup(); // performs handshake with server and sets up background

#endif
