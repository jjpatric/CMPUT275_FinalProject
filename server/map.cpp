//-----------------------------------
// Name: Zack Rodgers
// ID: 1554405
//
// Partner Name: Joshua Patrick
// CMPUT 275, Fall 2019
//
// Final Project: Civilization Wars
//-----------------------------------

#include "map.h"
#include "unit.h"

#include <fstream>
#include <cstring>
#include <sstream>
#include <math.h>
#include <iostream>
#include <cassert>
#include <string>
#include <stdlib.h>

using namespace std;

extern SerialPort Serial;

void Building::updateMax() {
    if (type == 'B') maxVal = 10;
    else if (type == 'P') maxVal = 25;
    else if (type == 'H') maxVal = 50;
}

// reads in a text file containing the building data for the map.
// stores each Building object in an unordered map.
void readBuildings(string filename, unordered_map<int, Building>& buildings, int& n) {
    string line;
    string data[5];
    Building b;
    ifstream bData(filename);
    int index = 0;
    
    if(bData.is_open()) {
        
        // assume first line of text is always N n, then store n
        getline(bData, line);
        stringstream s;
        s << line;
        getline(s, data[0], ' ');
        getline(s, data[1]);
        n = stoi(data[1]);
        
        // parse a line by space, then convert and store the parameters in Building
        // then store the Building object in buildings.  runs in O(5n) time
        while(getline(bData, line)) {
            stringstream ss;
            ss << line;
            for(int i = 0; i < 5; i++) {
                getline(ss, data[i], ' ');
            }
            
            b.type = data[0][0];
            b.updateMax();
            b.units = stoi(data[1]);
            b.control = stoi(data[2]);
            b.x = stoi(data[3]);
            b.y = stoi(data[4]);
            buildings[index] = b;
            index++;
        }
    }
    bData.close();
}


// calculates the distance between two coordinates, rounded to a whole number
long long distance(int x1, int y1, int x2, int y2) {
    int xdiff = abs(x1-x2);
    int ydiff = abs(y1-y2);
    double d = (sqrt(xdiff*xdiff + ydiff*ydiff));
    long long dist = d;
    return dist;
}


// stores the distances between each building for every building
void buildGraph(int n, unordered_map<int, Building> buildings, WDigraph& dists) {
    for(int i = 0; i < n; i++) {
        dists.addVertex(i);
    }
    // BigO notation is O((n * (n - 1)) / 2)
    int cost, x1,y1,x2,y2;
    for (int i = 0; i < n-1; i++) {
        x1 = buildings[i].x;
        y1 = buildings[i].y;
        for(int j =i+1; j < n; j++) {
            x2 = buildings[j].x;
            y2 = buildings[j].y;
            cost = distance(x1, y1, x2, y2);
            dists.addEdge(i, j, cost);
            dists.addEdge(j, i, cost);
        }
    }
}

list<Unit> units;

void updateGame(list<int>& selBuilds, int moveToBuild, unordered_map<int, Building>& buildings, int numBuildings){

    while(selBuilds.size()){ // create new units
        if(selBuilds.back() == moveToBuild){}
        else{
            pair<int, int> xyStart (buildings[selBuilds.back()].x, buildings[selBuilds.back()].y);
            pair<int, int> xyTarget (buildings[moveToBuild].x, buildings[moveToBuild].y);
            int travDist = sqrt(pow(xyTarget.first - xyStart.first, 2) + pow(xyTarget.second - xyStart.second, 2));

            Unit unit(selBuilds.back(), moveToBuild, xyStart, xyTarget, buildings[selBuilds.back()].control, travDist);
            if(unit.strength > 0){ // does not create a unit if its strength would be 0
                units.push_back(unit);
            }
        }
        selBuilds.pop_back();
    }

    for(list<Unit>::iterator unitIt = units.begin(); unitIt != units.end(); unitIt++){ // update Unit position and delete if needed
        Unit& thisUnit = *unitIt;
        thisUnit.moveUnit();
        if(!thisUnit.checkUnit()){
            unitIt = units.erase(unitIt);
            if(unitIt != units.begin() && units.size() > 0) --unitIt; // fix iterator if needed
        }
        else{
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
    }
    assert(Serial.writeline("E\n"));

    for (int i = 0; i < numBuildings; i++)
    {
        if(buildings[i].type == 'B'){
            if(buildings[i].control){ // if a player controls the building increase units every turn
                if(buildings[i].units <= 10){
                    buildings[i].units += 1;
                    if(buildings[i].units > 10) buildings[i].units = 10;
                }
            }
            if(buildings[i].units > 10){
                buildings[i].units -= 2;
                if(buildings[i].units < 10) buildings[i].units = 10;
            }
        }
        else if(buildings[i].type == 'P'){
            if(buildings[i].control){ // if a player controls the building increase units every turn
                if(buildings[i].units <= 25){
                    buildings[i].units += 2;
                    if(buildings[i].units > 25) buildings[i].units = 25;
                }
            }
            if(buildings[i].units > 25){
                buildings[i].units -= 2;
                if(buildings[i].units < 25) buildings[i].units = 25;
            }
        }
        else if(buildings[i].type == 'H'){
            if(buildings[i].control){ // if a player controls the building increase units every turn
                if(buildings[i].units <= 50){
                    buildings[i].units += 3;
                    if(buildings[i].units > 50) buildings[i].units = 50;
                }
            }
            if(buildings[i].units > 50){
                buildings[i].units -= 2;
                if(buildings[i].units < 50) buildings[i].units = 50;
            }
        }
    }
}
