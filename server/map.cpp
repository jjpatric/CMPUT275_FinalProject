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
#include <fstream>
#include <sstream>
#include <math.h>
#include <iostream>

using namespace std;

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
