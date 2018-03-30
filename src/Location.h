#pragma once
#ifndef LOCATION_H
#define LOCATION_h

#include <vector>

typedef struct
{
	double latitude;
	double longitude;
	double altitude;
} Coordinate;


/*
ACRESCENTAR O QUE É PRECISO À LOCATION,

MENOS O QUE O GRAPH TEMPLATE Vertex JÁ POSSUI
*/

class Location
{
	unsigned long long id;
	Coordinate coords;	///< Location's coordinates (x,y,z)
	std::string name;
public:
	Location(unsigned long long id, double latitude, double longitude, double altitude, std::string name);
	Location(unsigned long long id);
	Location(unsigned long long id, double latitude, double longitude, double altitude);
	Location();

	unsigned long long getID() const;

	std::string getName() const;

	double getLatitudecoord() const;

	double getLongitudecoord() const;

	double getAltitudecoord() const;

	Coordinate getCoords() const;

	bool operator==(const Location &n2) const;

	void setID(unsigned long long newID);

	virtual bool isAvailable() const;
};

#endif 