//--------------------------------------------
// Name: Zack Rodgers
// ID: 1554405
//
// Partner Name: Joshua Patrick
// CMPUT 275, Winter 2019
//
// Final Project: Civilization Wars
//--------------------------------------------

#ifndef _GAME_H_
#define _GAME_H_

using namespace std;

struct buildings{ // store simplified building data
  int x, y;
  int team;
  bool selected;
};

struct units{ // store simplified unit data
  int x, y;
  int team;
};

void drawBuilding(char type, int pop, int teamColor, int xPos, int yPos); // draws buildings on tft display

void makeBuilding(); // parses message from server to construct buildings

void drawUnit(int str, int teamColor, int xPos, int yPos); // draws units on tft display

void makeUnit(); // parses message rom server to construct units

void updateGame(); // updates new game screen

void playerTurn(int turn); // player makes their move

void resetTown(); // reset all building data

void sendData(); // sends all data from playerTurn over to server

#endif
