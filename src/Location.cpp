#include "Location.h"

using namespace std;

Location::Location(double latitude, double longitude, double altitude,
	std::string name)
{
	this->coords.latitude = latitude;
	this->coords.longitude = longitude;
	this->altitude = altitude;
	this->name = name;
}


Location::~Location()
{
}


int Location::getID() const
{
	return this->id;
}

string Location::getName() const
{
	return this->name;
}

int Location::getXcoord() const
{
	return this->coords.x;
}

int Location::getYcoord() const
{
	return this->coords.y;
}

Coordinate Location::getCoords() const
{
	return this->coords;
}

bool Location::operator==(const Location &n2) const
{
	return
		this->coords.x == n2.coords.x
		&&
		this->coords.y == n2.coords.y;
}

void Location::setID(int newID)
{
	this->id = newID;
}