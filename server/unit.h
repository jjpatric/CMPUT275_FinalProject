//-----------------------------------
// Name: Zack Rodgers
// ID: 1554405
//
// Partner Name: Joshua Patrick
// CMPUT 275, Winter 2019
//
// Final Project: Civilization Wars
//-----------------------------------

#ifndef _UNIT_H_
#define _UNIT_H_

#include <utility> // for pair
#include <list>
#include <iostream>

using namespace std;

extern WDigraph dists;
extern unordered_map<int, Building> buildings;


/*  
Defines a unit that moves from one building to another.
Its path is defined by the coordinates of its start and end buildings.
Once a unit has reached its destination, it adds or subtracts its strength from
*/
class Unit {
public:
	// indices of the unit's start building and the target building in the buildings
	// unordered map.
	int startBuilding, targetBuilding;
	
	// building coordinates
	pair<int, int> startPos, targetPos;
	
	// buildings[startBuilding].units, buildings[startBuilding].control
	int control;

	// distance between start and target building
	int travDist;
	
	//** ABOVE ARE CONSTRUCTOR PARAMETERS **
	
	// calculated in the constructor for the unit's position
	int turns;
	int strength;
	pair<int, int> moveFactors;
	pair<int, int> currentPos;
	

	// units iterator position within list public so server can alter it
	list<Unit>::iterator myIt;

	// Unit constructor: parameters are in the order in which they appear in private.
	// Constructor also calculates the values for turns, moveFactors, strength, and currentPos.
	Unit(int SB, int TB, pair<int, int> SP, pair<int, int> TP, int CO, int TD);
	
	// update the unit's current position using its moveFactors
	// also decrease the turn count
	void moveUnit();
	
	// Checks if the unit has reached its destination. If so,
    // return false if unit is to be destroyed
	bool checkUnit();
	
	// add or subtract the unit's strength from buildings[targetBuilding].units
	// if buildings[targetBuilding].units falls to or below 0, change buildings[targetBuilding].control to other player.
	void damageCalc();

	// initializes a new units strength
	void init();

};

Unit::Unit(int SB, int TB, pair<int, int> SP, pair<int, int> TP, int CO, int TD):
startBuilding(SB), targetBuilding(TB), startPos(SP), targetPos(TP), control(CO), travDist(TD){
	init(); // initialise starting data
	turns = travDist / 100; // calculate number of turns required to reach target building
	if(travDist % 100 > 0) turns++; // add extra turn if distance is not divisible by 100 exactly
	currentPos = startPos; // set current position to start position

	moveFactors.first = (targetPos.first - startPos.first) / turns; // calculate move factors
	moveFactors.second = (targetPos.second - startPos.second) / turns; // used to calculate postion after each turn
}


void Unit::moveUnit(){ // updates current position and turns remaining
	currentPos.first += moveFactors.first;
	currentPos.second += moveFactors.second;
	turns--;
}


bool Unit::checkUnit(){ // check if unit has reached its target
	if(turns <= 0){
		damageCalc(); // do damage to target building
		return false; // destroy this unit
	}
	return true; // unit is still alive
}


void Unit::damageCalc(){ // do damage to target building
	if(buildings[targetBuilding].control == control) buildings[targetBuilding].units += strength; // if it is your own building increase health

	if(buildings[targetBuilding].control != control){ // if it is enemy building 
		buildings[targetBuilding].units -= strength; // do damage
		if(buildings[targetBuilding].units <= 0){ // if enemy buildings health is below 0
			if(buildings[targetBuilding].control == 0){ // if it was a neutral building fill up to max population
				buildings[targetBuilding].units = buildings[targetBuilding].maxVal;
				buildings[targetBuilding].control = control;
			}
			else if(buildings[targetBuilding].control > 0){ // if it was other players building just fill with number of units left over
				buildings[targetBuilding].control = control;
				buildings[targetBuilding].units = abs(buildings[targetBuilding].units);
			}
		}
	}
}

// called in constructor
void Unit::init(){ // take half of units from selected building
	if(buildings[startBuilding].units > 1){
		strength = buildings[startBuilding].units / 2;
		buildings[startBuilding].units -= strength;
	}
}


#endif