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

double Location::getLatitudecoord() const
{
	return this->coords.latitude;
}

double Location::getLongitudecoord() const
{
	return this->coords.longitude;
}

Coordinate Location::getCoords() const
{
	return this->coords;
}

bool Location::operator==(const Location &n2) const
{
	return
		this->coords.latitude == n2.coords.latitude
		&&
		this->coords.longitude == n2.coords.longitude;
}

void Location::setID(int newID)
{
	this->id = newID;
}