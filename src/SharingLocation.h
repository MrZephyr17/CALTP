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
	bool depositBike();
	bool liftBike();
	bool isAvailable(bool rent) const;
	std::string getColor() const;
	int getSlots() const;
	int getMaxCapacity() const;
};
