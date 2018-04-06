#pragma once
#include "Location.h"

class SharingLocation : public Location
{
private:
	int maxCapacity;
	int slots;
public:
	SharingLocation(int id, double latitude, double longitude, double altitude,
		int maxCapacity, int slots);
	bool depositBike(int number = 1);
	bool liftBike(int number = 1);
	bool isAvailable() const;
	std::string getColor() const;
	int getSlots() const;
	int getMaxCapacity() const;
};

