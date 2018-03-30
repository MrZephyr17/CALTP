#pragma once
#include "Location.h"

class SharingLocation : public Location
{
private:
	int maxCapacity;
	int slots;
public:
	SharingLocation(unsigned long long id, double latitude, double longitude, double altitude,
		std::string name, int maxCapacity, int slots);
	bool depositBike(int number = 1);
	bool liftBike(int number = 1);
	bool isAvailable() const;
};

