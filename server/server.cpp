//--------------------------------------------
// Name: Zack Rodgers
// ID: 1554405
//
// Partner Name: Joshua Patrick
// CMPUT 275, Winter 2019
//
// Final Project: Civilization Wars
//--------------------------------------------

#include <iostream>
#include <cassert>
#include <list>
#include <cstring>
#include <string>
#include <utility> //for pair
#include <stdlib.h>
#include "wdigraph.h"
#include "map.h"

WDigraph dists;
unordered_map<int, Building> buildings;

SerialPort Serial("/dev/ttyACM0");

// performs a handshake between the server and the client based on handshake shown on eclass in assignment 2
void handshake(){
    string curState = "STATE0\n";
    string nextState = "STATE1\n";
    string line;

    cout << "Server started, you might need to restart your Arduino" << endl;
    cout << "Server is in state " << curState;
    cout << "Waiting for STATE1 message from Arduino..." << endl;

    // read and ignore lines until we get the message STATE1
    do {
        line = Serial.readline(1000);
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

// send over most current building data
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
        while (1){ // wait for clients acknowledgement before sending more
            line = Serial.readline(1000);
        if(line[0] == 'A'){ break; }
        }
    }

    assert(Serial.writeline("E\n")); // send end of transmission signal
}


int main() {
    string line;
    string filename;
    int numBuildings = 0;

    handshake();

    /*
    STATE2: Main Menu
    STATE3: Level Select
    STATE4: Gameplay
    STATE5: End Screen
    */
    enum {STATE2, STATE3, STATE4, STATE5} curr_mode = STATE2; // finite state machine

    while(curr_mode == STATE2){ // Main menu
        line = Serial.readline(1000);
        if(line[0] == 'M'){ // wait for server to select mode
            curr_mode = STATE3;
            assert(Serial.writeline("C\n")); // acknowledge client
        }
    }

    while(curr_mode == STATE3){ // Level Selection menu

        while(1){ // wait for selection from Client, model based on eclass code for assignment 2
            line = Serial.readline(1000);
            if(line[0] == '1'){ 
                filename = "map1.txt";
                assert(Serial.writeline("D\n")); // acknowledge client
                break;
            }
            else if(line[0] == '2'){ 
                filename = "map2.txt";
                assert(Serial.writeline("D\n")); // acknowledge client
                break;
            }
            else if(line[0] == '3'){
                filename = "map3.txt";
                assert(Serial.writeline("D\n")); // acknowledge client
                break;
            }
            else if(line[0] == '4'){
                filename = "mapCust.txt";
                assert(Serial.writeline("D\n")); // acknowledge client
                break;
            }
        }
        curr_mode = STATE4;
    }

    cout << "Loading map: " << filename << endl; // let user know where we are

    // setup for STATE4
    list<int> selBuilds; // list storing all selected buildings index
    int moveToBuild = 0; // store building index player chose to move to
    readBuildings(filename, buildings, numBuildings); // read initial building data from text file
    buildGraph(numBuildings, buildings, dists); // build weighted digraph out of building data
    updateGame(selBuilds, moveToBuild, buildings, numBuildings); // update game one time to draw all initial data
    sendBuildings(buildings, numBuildings); // send over initial building data
    int turn = 1; // player 1 starts

    while(curr_mode == STATE4){ // gameplay
        cout << "Player " << turn << "'s Turn to select ally building(s)" << endl;
        while(1){ // wait for players selections of buldings
            line = Serial.readline(1000);
            if(line[0] == 'S'){ // add selected buildings to list
                selBuilds.push_back(atoi(&line[2]));
                assert(Serial.writeline("F\n"));
            }
            else if(line[0] == 'N'){ // let server know what part of player turn it is
                cout << "Player " << turn << "'s Turn to target a building" << endl;
                assert(Serial.writeline("F\n"));
            }
            else if(line[0] == 'T'){ // save building that player wants to move to
                moveToBuild = atoi(&line[2]);
                assert(Serial.writeline("F\n"));
            }
            else if(line[0] == 'E'){ // end of players transmission
                assert(Serial.writeline("F\n"));
                break;
            }
        }

        updateGame(selBuilds, moveToBuild, buildings, numBuildings); // update with new data and send unit data to client
        sendBuildings(buildings, numBuildings); // send new building data to client
        if(turn == 1) turn = 2; // switch players turn
        else turn = 1;
    }
    return 0;
}
