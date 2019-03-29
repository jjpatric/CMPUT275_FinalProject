//--------------------------------------------
// Name: Zack Rodgers
// ID: 1554405
//
// Partner Name: Joshua Patrick
// CMPUT 275, Winter 2019
//
// Final Project
//--------------------------------------------

#include <iostream>
#include <cassert>
#include <list>
#include <cstring>
#include <string>
#include <sstream>
#include <utility> //for pair
#include <stdlib.h>
#include "wdigraph.h"
#include "server_util.h"
#include "serialport.h"
#include "map.h"

extern Unit::Unit(int SB, int TB, pair<int, int> SP, pair<int, int> TP, int CO, int TD):
startBuilding(SB), targetBuilding(TB), startPos(SP), targetPos(TP), control(CO), travDist(TD);
extern list<Unit> units;
extern int numUnits;

WDigraph dists;
unordered_map<int, Building> buildings;

SerialPort Serial("/dev/ttyACM0");

// performs a handshake between the server and the client
void handshake(){
  SerialPort Serial("/dev/ttyACM0");

  string curState = "STATE0\n";
  string nextState = "STATE1\n";
  string line;

  cout << "Server started, you might need to restart your Arduino" << endl;
  cout << "Server is in state " << curState;
  cout << "Waiting for STATE1 message from Arduino..." << endl;

  // read and ignore lines until we get the message STATE1
  do {
    line = Serial.readline();
  } while (line != nextState);

  assert(Serial.writeline("Ack\n"));

  // switch to next phase
  curState = nextState;
  nextState = "STATE2\n";
  cout << "Server is in state " << curState;

  // read the introductory lines until get the message STATE1
  do {
    line = Serial.readline();
  } while (line != nextState);
  
  // switch to next state
  curState = nextState;

  cout << "Finished Handshake!" << endl;

}

void sendBuildings(unordered_map<int, Building> buildings, int n){ // O(n) time
    
    for(int i = 0; i < n; i++){ 
      assert(Serial.writeline("B "));
      string s(1,buildings[i].type);
      assert(Serial.writeline(s));
      assert(Serial.writeline(" "));
      assert(Serial.writeline(to_string(buildings[i].units)));
      assert(Serial.writeline(" "));
      assert(Serial.writeline(to_string(buildings[i].control)));
      assert(Serial.writeline(" "));
      assert(Serial.writeline(to_string(buildings[i].x)));
      assert(Serial.writeline(" "));
      assert(Serial.writeline(to_string(buildings[i].y)));
      assert(Serial.writeline(" \n"));
      
      string line;
      while (1){
        line = Serial.readline(1000);
        if(line[0] == 'A'){ break; }
      }
    }

    assert(Serial.writeline("E\n"));
}

void sendUnits(list<Unit> units, int n){ // O(n) time
    
    for(list<Unit>::iterator unitIt = units.begin(); unitIt != units.end(); unitIt++){ 
      Unit thisUnit = *unitIt;
      assert(Serial.writeline("U "));
      assert(Serial.writeline(to_string(thisUnit.strength)));
      assert(Serial.writeline(" "));
      assert(Serial.writeline(to_string(thisUnit.control)));
      assert(Serial.writeline(" "));
      assert(Serial.writeline(to_string(thisUnit.currentPos.first)));
      assert(Serial.writeline(" "));
      assert(Serial.writeline(to_string(thisUnit.currentPos.second)));
      assert(Serial.writeline(" \n"));
      
      string line;
      while (1){
        line = Serial.readline(1000);
        if(line[0] == 'A'){ break; }
      }
    }

    assert(Serial.writeline("E\n"));
}



int main() {
  string line;
  string filename;
  char gameMode;
  int numBuildings = 0;
  
  handshake();

  /*
    STATE2: Main Menu
    STATE3: Level Select
    STATE4: Gameplay
    STATE5: End Screen
  */
  enum {STATE2, STATE3, STATE4, STATE5} curr_mode = STATE2;

  while(curr_mode == STATE2){
    line = Serial.readline(1000);
    if(line[0] == 'M'){
        gameMode = 'M';
        curr_mode = STATE3;
        assert(Serial.writeline("C\n"));
    }
    else if(line[0] == 'C'){
        gameMode = 'C';
        curr_mode = STATE3;
        assert(Serial.writeline("C\n"));
    }
  }

  while(curr_mode == STATE3){
    
    while(1){
        line = Serial.readline(1000);
        if(line[0] == '1'){ 
            filename = "map1.txt";
            assert(Serial.writeline("D\n"));
            break;
        }
        else if(line[0] == '2'){ 
            filename = "map2.txt";
            assert(Serial.writeline("D\n"));
            break;
        }
        else if(line[0] == '3'){
            filename = "map3.txt";
            assert(Serial.writeline("D\n"));
            break;
        }
        else if(line[0] == '4'){
            filename = "mapCust.txt";
            assert(Serial.writeline("D\n"));
            break;
        }
    }
    curr_mode = STATE4;
  }

  cout << "Loading map: " << filename << endl;

  // setup for STATE4
  readBuildings(filename, buildings, numBuildings);
  buildGraph(numBuildings, buildings, dists);
  sendBuildings(buildings, numBuildings);
  list<int> selBuilds;
  int moveToBuild;

  while(curr_mode == STATE4){
    while(1){
        line = Serial.readline(1000);
        if(line[0] == 'S'){
            selBuilds.push_back(atoi(&line[2]));
            assert(Serial.writeline("F\n"));
        }
        else if(line[0] == 'T'){
            moveToBuild = atoi(&line[2]);
            assert(Serial.writeline("F\n"));
        }
        else if(line[0] == 'E'){
            assert(Serial.writeline("F\n"));
            break;
        }
    }

    updateGame(selBuilds, moveToBuild, buildings, numBuildings);
    sendBuildings(buildings, numBuildings);
    sendUnits(units, numUnits);
  }
  return 0;
}
