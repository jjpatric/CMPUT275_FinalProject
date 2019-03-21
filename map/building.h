#ifndef _BUILDING_H_
#define _BUILDING_H_

// represents one building on the map
struct Building {
	// building type (Barracks(B):10, Outpost(P):25, Stronghold(H):50)
	char type;
	int maxVal = 5; // default value
	
	// number of units initially contained in the building
	int units;
	
	// keeps track of who owns the building (Neutral:0, Player One:1, Player Two:2)
	int control;
	
	// x and y coordinates of the building (0 <= x <= 320, 0<= y <= 240)
	int x;
	int y;
	
	//updates maxVal based on the type of building
	void updateMax();
};


void Building::updateMax() {
	if (type == 'B') maxVal = 10;
	else if (type == 'P') maxVal = 25;
	else if (type == 'H') maxVal = 50;
}

#endif