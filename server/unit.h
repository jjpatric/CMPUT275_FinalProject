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
	// Constructor also calculates the values for turns, moveFactors, and currentPos.
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
	init();
	turns = travDist / 100;
	if(travDist % 100 > 0) turns++;
	currentPos = startPos;

	moveFactors.first = (targetPos.first - startPos.first) / turns;
	moveFactors.second = (targetPos.second - startPos.second) / turns;
}


void Unit::moveUnit(){
	currentPos.first += moveFactors.first;
	currentPos.second += moveFactors.second;
	turns--;
}

bool Unit::checkUnit(){
	if(turns <= 0){
		damageCalc();
		return false;
	}
	return true;
}

void Unit::damageCalc(){
	if(buildings[targetBuilding].control == control) buildings[targetBuilding].units += strength;

	if(buildings[targetBuilding].control != control){
		buildings[targetBuilding].units -= strength;
		if(buildings[targetBuilding].units <= 0){
			if(buildings[targetBuilding].control == 0){
				if(buildings[targetBuilding].type == 'B') buildings[targetBuilding].units = 10;
				if(buildings[targetBuilding].type == 'P') buildings[targetBuilding].units = 25;
				if(buildings[targetBuilding].type == 'H') buildings[targetBuilding].units = 50;
				buildings[targetBuilding].control = control;
			}
			else if(buildings[targetBuilding].control > 0){
				buildings[targetBuilding].control = control;
				buildings[targetBuilding].units = abs(buildings[targetBuilding].units);
			}
		}
	}

}

// called in constructor
void Unit::init(){
	if(buildings[startBuilding].units > 1){
		strength = buildings[startBuilding].units / 2;
		buildings[startBuilding].units -= strength;
	}

}


#endif