#include "Location.h"



Location::Location(double latitude, double longitude, double altitude,
	std::string name)
{
	this->latitude = latitude;
	this->longitude = longitude;
	this->altitude = altitude;
	this->name = name;
}


Location::~Location()
{
}
