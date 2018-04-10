#pragma once
#include "Location.h"

/** @defgroup sharinglocation sharinglocation
 * @{
 *
 * Struct and class to support the representation of a bycicle sharing location in the program
 */

/**
 * @brief Represents a sharing location on the map
 * 
 */
class SharingLocation : public Location
{
private:
	int maxCapacity;
	int slots;

public:
	/**
 * @brief Construct a new Sharing Location object
 * 
 * @param id 
 * @param latitude 
 * @param itude 
 * @param altitude 
 * @param maxCapacity 
 * @param slots 
 */
	SharingLocation(int id, double latitude, double longitude, double altitude,
									int maxCapacity, int slots);
	/**
	 * @brief 
	 * 
	 * @return true 
	 * @return false 
	 */
	bool depositBike();
	/**
	 * @brief 
	 * 
	 * @return true 
	 * @return false 
	 */
	bool liftBike();
	/**
	 * @brief 
	 * 
	 * @param rent 
	 * @return true 
	 * @return false 
	 */
	bool isAvailable(bool rent) const;
	/**
	 * @brief Get the Color object
	 * 
	 * @return std::string 
	 */
	std::string getColor() const;
	/**
	 * @brief Get the Slots object
	 * 
	 * @return int 
	 */
	int getSlots() const;
	/**
	 * @brief Get the Max Capacity object
	 * 
	 * @return int 
	 */
	int getMaxCapacity() const;
};

/** @} end of sharinglocation */