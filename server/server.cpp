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


int main() {
  
  handshake();

  WDigraph graph;
  unordered_map<int, Point> points;

  // build the graph
  //readGraph("edmonton-roads-2.0.1.txt", graph, points);
  //cout << "Waiting for a request..." << endl;
  return 0;
}
