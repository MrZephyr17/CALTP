#include "SharingLocation.h"
#include <stdexcept>

using namespace std;

SharingLocation::SharingLocation(int id, double latitude, double longitude, double altitude,
	int maxCapacity, int slots) : Location(id, latitude, longitude, altitude)
{
	if (maxCapacity <= 0 || slots < 0 || slots > maxCapacity)
		throw invalid_argument("One of the arguments is invalid, check again!");

	this->maxCapacity = maxCapacity;
	this->slots = slots;
	this->color = "RED";
	this->visited = false;
}

bool SharingLocation::depositBike(int number)
{
	if (number > slots)
		return false;

	slots -= number;

	return true;
}

bool SharingLocation::liftBike(int number)
{
	if (number > maxCapacity - slots)
		return false;

	slots += number;
	return true;
}

bool SharingLocation::isAvailable() const
{
	return slots > 0;
}

string SharingLocation::getColor() const
{
	return color;
}

int SharingLocation::getSlots() const
{
	return slots;
}

int SharingLocation::getMaxCapacity() const
{
	return maxCapacity;
}
