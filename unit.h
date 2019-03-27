#ifndef _UNIT_H_
#define _UNIT_H_

#include <utility>

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
	
	// startBuilding.units, startBuilding.control
	int strength, control;
	
	//** ABOVE ARE CONSTRUCTOR PARAMETERS **
	
	// calculated in the constructor for the unit's position
	int turnsLeft;
	pair<int, int> moveFactors;
	pair<int, int> currentPos;
	
public:
	// Unit constructor: parameters are in the order in which they appear in private.
	// Constructor also calculates the values for turns, moveFactors, and currentPos.
	Unit(int SB, int TB, pair<int, int> SP, pair<int, int> TP, int ST, int CO);
	
	// update the unit's current position using its moveFactors
	// also decrease the turn count
	void moveUnit();
	
	// Checks if the unit has reached its destination. If so,
    // call damageCalc() and delete the unit	
	void checkUnit();
	
	// add or subtract the unit's strength from targetbuilding.units
	// if targetbuilding.units falls to or below 0, change targetbuilding.control to other player.
	void damageCalc();
};
#endif