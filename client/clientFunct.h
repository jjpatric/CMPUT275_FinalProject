#ifndef _CLIENT_FUNCT_H_
#define _CLIENT_FUNCT_H_

using namespace std;

struct xy_pos { // useful struct
  int16_t x, y;
  xy_pos(int16_t x1 = 0, int16_t y1 = 0) : x(x1), y(y1) {}
};

struct shared_vars {
  bool select_pushed, action_pushed, move_pushed;
  bool readBuildings;

  // very simple finite state machine:
  // STATE2: Main Menu
  // STATE3: Map Selection
  // STATE4: Game play
  // STATE5: End Screen
  enum {STATE2, STATE3, STATE4, STATE5} curr_mode;



};

char updateMainMenu(); // updates main menu returns which mode was selected

char updateMapMenu(); // updates map menu returns which map was selected

void drawBuilding(char type, int pop, int textBackground, int xPos, int yPos); // draws buildings on tft display

void makeBuilding(); // parses message from server to construct buildings

void drawMainMenu(); // draws main menu

void drawMapMenu(); // draws map menu

void process_line(); // determines what to do based on what has been read into buffer

void process_input(); // reads which buttons are pushed

void read_line(); // reads characters from serial port and stores them in buffer when it reads'\n'

void updateGame(); // updates new game screen

void setup(); // performs handshake with server and sets up background

#endif
