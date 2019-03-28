#ifndef _UNIT_H_
#define _UNIT_H_

#include <utility> // for pair

extern WDigraph dists;
extern unordered_map<int, Building> buildings;

/*  
Defines a unit that moves from one building to another.
Its path is defined by the coordinates of its start and end buildings.
Once a unit has reached its destination, it adds or subtracts its strength from
*/
class Unit {
private:
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
	
public:
	// Unit constructor: parameters are in the order in which they appear in private.
	// Constructor also calculates the values for turns, moveFactors, and currentPos.
	Unit(int SB, int TB, pair<int, int> SP, pair<int, int> TP, int CO, int TD);
	
	// update the unit's current position using its moveFactors
	// also decrease the turn count
	void moveUnit();
	
	// Checks if the unit has reached its destination. If so,
    // call damageCalc() and delete the unit	
	void checkUnit();
	
	// add or subtract the unit's strength from buildings[targetBuilding].units
	// if buildings[targetBuilding].units falls to or below 0, change buildings[targetBuilding].control to other player.
	void damageCalc();

	// initializes a new units strength
	void init();
};

Unit::Unit(int SB, int TB, pair<int, int> SP, pair<int, int> TP, int CO, int TD):
startBuilding(SB), targetBuilding(TB), startPos(SP), targetPos(TP), control(CO), travDist(TD){
	init();
	if(strength == 0) // delete the unit
	turns = travDist / 100;
	if(travDist % 100 > 0) turns++;
	currentPos = startPos;
	moveFactors.first = abs(startPos.first - targetPos.first) / turns;
	moveFactors.second = abs(startPos.first - targetPos.first) / turns;
}


void Unit::moveUnit(){
	currentPos.first += moveFactors.first;
	currentPos.second += moveFactors.second;
	turns--;
}

void Unit::checkUnit(){
	if(turns == 0){
		damageCalc();
		//delete UNIT object
	}
}

void Unit::damageCalc(){
	if(buildings[targetBuilding].control == control) buildings[targetBuilding].units += strength;
	
	if(buildings[targetBuilding].control != control){
		buildings[targetBuilding].units -= strength;

		if(buildings[targetBuilding].units <= 0){
			buildings[targetBuilding].control = control;
			buildings[targetBuilding].units = abs(buildings[targetBuilding].units);
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