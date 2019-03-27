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


SerialPort Serial("/dev/ttyACM0");

// performs a TCP handshake between the server and the client
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

void sendBuildings(WDigraph& dists, unordered_map<int, Building> buildings, int n){
    
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

int main() {
  
  handshake();

  int n = 0;
  string filename = "test.txt";
  WDigraph dists;
  unordered_map<int, Building> buildings;
  
  readBuildings(filename, buildings, n);
  buildGraph(n, buildings, dists);
  sendBuildings(dists, buildings, n);

  return 0;
}
