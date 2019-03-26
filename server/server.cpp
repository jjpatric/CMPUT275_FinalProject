#include <iostream>
#include <cassert>
#include <list>
#include <cstring>
#include <string>
#include <sstream>
#include <utility> //for pair
#include <stdlib.h>
#include "wdigraph.h"
#include "dijkstra.h"
#include "server_util.h"
#include "serialport.h"



SerialPort Serial("/dev/ttyACM0");

void handshake(){
  SerialPort Serial("/dev/ttyACM0");

  string curPhase = "PHASE00\n";
  string nextPhase = "PHASE01\n";
  string line;

  cout << "Server started, you might need to restart your Arduino" << endl;
  cout << "Server is in phase " << curPhase;
  cout << "Waiting for PHASE01 message from Arduino..." << endl;

  // read and ignore lines until we get the message PHASE01
  do {
    line = Serial.readline();
  } while (line != nextPhase);

  // switch to next phase
  curPhase = nextPhase;
  nextPhase = "PHASE02\n";
  cout << "Server is in phase " << curPhase;

  // read the introductory lines until get the message PHASE01
  do {
    line = Serial.readline();
  } while (line != nextPhase);
  assert(Serial.writeline("Ack\n"));

  // switch to next phase
  curPhase = nextPhase;

  cout << "Finished Handshake!" << endl;

}

void processRequest(WDigraph& graph, unordered_map<int, Point>& points, Point sPoint, Point ePoint){
  
  // get the points closest to the two input points
  int start = findClosest(sPoint, points), end = findClosest(ePoint, points);

  // run dijkstra's to compute a shortest path
  unordered_map<int, PLI> tree;
  dijkstra(graph, start, tree);

  if (tree.find(end) == tree.end()) {
      // no path
     
      assert(Serial.writeline("N 0\n"));
  }
  else {
    // read off the path by stepping back through the search tree
    list<int> path;
    while (end != start) {
      path.push_front(end);
      end = tree[end].second;
    }
    path.push_front(start);

    // output the path size
    
    assert(Serial.writeline("N "));
    assert(Serial.writeline(to_string(path.size())));
    assert(Serial.writeline(" \n"));
    string line;
    while (1){
      line = Serial.readline(1);
      if(line[0] == 'A'){ break; }
    }

    // output the path
    for (auto v : path) {
      
      assert(Serial.writeline("W "));
      assert(Serial.writeline(to_string(points[v].lat)));
      assert(Serial.writeline(" "));
      assert(Serial.writeline(to_string(points[v].lon)));
      assert(Serial.writeline("\n"));
      while (1){
        line = Serial.readline(1);
        if(line[0] == 'A'){ break; }
      }
    }

      assert(Serial.writeline("E\n"));
  }
}

pair<Point, Point> parseRequest(string line){
  string params[5];
  stringstream ss;
  ss << line;
  for (int i = 0; i < 5; i++){
    getline(ss, params[i], ' ');
  }
  Point start, end;
  long long coords[4];
  for (int i = 0; i < 4; i++){
    coords[i] = stoll(params[i+1]);
  }
  start.lat = coords[0];
  start.lon = coords[1];
  end.lat = coords[2];
  end.lon = coords[3];
  pair<Point, Point> startEndPair;
  startEndPair.first = start;
  startEndPair.second = end;

  return startEndPair;

}

int waitRequest(WDigraph& graph, unordered_map<int, Point>& points){
  string line;
  while (1){
    line = Serial.readline(1);
    if(line[0] == 'R'){ break; }
  }


  pair<Point, Point> coords = parseRequest(line);
  Point sPoint, ePoint;
  sPoint = coords.first;
  ePoint = coords.second;

  processRequest(graph, points, sPoint, ePoint);
  
  return 0;
}

int main() {
  
  handshake();

  WDigraph graph;
  unordered_map<int, Point> points;

  // build the graph
  readGraph("edmonton-roads-2.0.1.txt", graph, points);
  cout << "Waiting for a request..." << endl;
  while(1){
    waitRequest(graph, points);
  }
  return 0;
}
