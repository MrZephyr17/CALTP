#pragma once
#ifndef LOCATION_H
#define LOCATION_h

#include <vector>

typedef struct
{
	double latitude;
	double longitude;
}Coordinate;


/*
ACRESCENTAR O QUE É PRECISO À LOCATION,

MENOS O QUE O GRAPH TEMPLATE Vertex JÁ POSSUI
*/

class Location
{
	int id;
	Coordinate coords;	///< Location's coordinates (x,y)
	double altitude;
	std::string name;
public:
	Location(double latitude, double longitude, double altitude, std::string name);
	~Location(); 

	int getID() const;

	std::string getName() const;

	double getLatitudecoord() const;

	double getLongitudecoord() const;

	Coordinate getCoords() const;

	bool operator==(const Location &n2) const;

	void setID(int newID);
};

#endif 