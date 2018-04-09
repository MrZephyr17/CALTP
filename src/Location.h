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
ACRESCENTAR O QUE � PRECISO � LOCATION,

MENOS O QUE O GRAPH TEMPLATE Vertex J� POSSUI
*/

class Location
{
  protected:
	int id;
	Coordinate coords; ///< Location's coordinates (x,y,z)
	std::string color;
	bool visited;

  public:
	Location(int id);
	Location(int id, double latitude, double longitude, double altitude);
	Location();

	int getID() const;

	double getLatitudecoord() const;

	double getLongitudecoord() const;

	double getAltitudecoord() const;

	Coordinate getCoords() const;

	bool operator==(const Location &n2) const;

	void setID(int newID);

	virtual bool isAvailable(bool rent) const;

	virtual std::string getColor() const;

	bool getVisited() const;

	void setVisited(bool vis);

	void setColor(std::string color);
};

#endif