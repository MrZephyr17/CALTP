#include "Location.h"

using namespace std;

Location::Location(int id, double latitude, double longitude, double altitude,
	std::string name)
{
	this->id = id;
	this->coords.latitude = latitude;
	this->coords.longitude = longitude;
	this->coords.altitude = altitude;
	this->name = name;
}

Location::Location(int id)
{
	this->id = id;
}

Location::Location(int id, double latitude, double longitude, double altitude)
{
	this->id = id;
	this->coords.latitude = latitude;
	this->coords.longitude = longitude;
	this->coords.altitude = altitude;
}

Location::Location()
{
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

double Location::getLatitudecoord() const
{
	return this->coords.latitude;
}

double Location::getLongitudecoord() const
{
	return this->coords.longitude;
}

double Location::getAltitudecoord() const
{
	return coords.altitude;
}

Coordinate Location::getCoords() const
{
	return this->coords;
}

bool Location::operator==(const Location &n2) const
{
	return id == n2.id;
}

void Location::setID(int newID)
{
	this->id = newID;
}