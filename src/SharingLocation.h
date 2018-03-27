#pragma once
#include "Location.h"

class SharingLocation : public Location
{
private:
	int maxCapacity;
	int slots;
public:
	SharingLocation(int id, double latitude, double longitude, double altitude, 
		std::string name, int maxCapacity, int slots);
	~SharingLocation();
	bool depositBike(int number = 1);
	bool liftBike(int number = 1);
};

