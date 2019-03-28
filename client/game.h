#ifndef _GAME_H_
#define _GAME_H_

using namespace std;

struct buildings{
  int x, y;
  int team;
  bool selected;
  bool exists;
};

void drawBuilding(char type, int pop, int teamColor, int xPos, int yPos); // draws buildings on tft display

void makeBuilding(); // parses message from server to construct buildings

void updateGame(); // updates new game screen

void playerTurn(); // player makes their move

#endif
