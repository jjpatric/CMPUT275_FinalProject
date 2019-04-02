//-----------------------------------
// Name: Zack Rodgers
// ID: 1554405
//
// Partner Name: Joshua Patrick
// CMPUT 275, Winter 2019
//
// Final Project: Civilization Wars
//-----------------------------------

#ifndef _MAP_H_
#define _MAP_H_

#include <string>
#include <list>
#include <unordered_map>
#include "wdigraph.h"
#include "serialport.h"

using namespace std;

// represents one building on the map
struct Building {
    // building type (Barracks(B):10, Outpost(P):25, Stronghold(H):50)
    char type;
    int maxVal = 5; // default value
    int incVal = 0; // value that units increase each turn
    
    // number of units initially contained in the building
    int units;
    
    // keeps track of who owns the building (Neutral:0, Player One:1, Player Two:2)
    int control;
    
    // x and y coordinates of the building (0 <= x <= 320, 0 <= y <= 240)
    int x;
    int y;
    
    //updates maxVal based on the type of building
    void updateMax();
};


// reads in a text file containing the building data for the map.
// stores each Building object in an unordered map.
void readBuildings(string filename, unordered_map<int, Building>& buildings, int& n);

// calculates the distance between two coordinates, rounded to a whole number
long long distance(int x1, int y1, int x2, int y2);


// stores the distances between each building for every building
void buildGraph(int n, unordered_map<int, Building> buildings, WDigraph& dists);

// updates all game data values on servers side and send over new unit data to client
void updateGame(list<int>& selBuilds, int moveToBuild, unordered_map<int, Building>& buildings, int numBuildings);

#endif