#ifndef _MAPDATA_H_
#define _MAPDATA_H_


class Mapdata {
private:
	// building type (Barracks:10, Outpost:25, Stronghold:50)
	char type;
	
	// number of units initially contained in the building
	int units;
	int control;
	
};

#endif