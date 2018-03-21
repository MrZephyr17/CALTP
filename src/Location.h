#pragma once
#include <string>

class Location
{
	double latitude;
	double longitude;
	double altitude;
	std::string name;
public:
	Location(double latitude, double longitude, double altitude, std::string name);
	~Location();
};

