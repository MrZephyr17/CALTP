#pragma once
#include "Location.h"

/** @defgroup sharinglocation sharinglocation
 * @{
 *
 * Struct and class to support the representation of a bycicle sharing location in the program
 */

/**
 * @brief Represents a sharing location on the map
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
	 * @param id this location's id
	 * @param latitude this location's latitude
	 * @param longitude this location's longitude
	 * @param altitude this location's altitude
	 * @param maxCapacity this location's maximum capacity
	 * @param slots this location's number of slots
	 */
	SharingLocation(int id, double latitude, double longitude, double altitude,
									int maxCapacity, int slots);
	/**
	 * @brief Deposits a bike into this sharing location.
	 * 
	 * @return true if successful, false otherwise
	 */
	bool depositBike();
	/**
	 * @brief Lifts a bike from this sharing location.
	 * 
	 * @return true if successful, false otherwise
	 */
	bool liftBike();
	/**
	 * @brief Checks if this locations is available to rent or deposit a bike
	 * 
	 * @param rent true if the objective is to rent, false if it's to deposit
	 * @return true if is available, false otherwise
	 */
	bool isAvailable(bool rent) const;
	/**
	 * @brief Get the color on the GraphViewer
	 * 
	 * @return the color 
	 */
	std::string getColor() const;
	/**
	 * @brief Get the number of slots
	 * 
	 * @return the number of slots 
	 */
	int getSlots() const;
	/**
	 * @brief Get the Max Capacity 
	 * 
	 * @return the max capacity 
	 */
	int getMaxCapacity() const;
};

/** @} end of sharinglocation */