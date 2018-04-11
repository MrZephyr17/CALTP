#pragma once
#ifndef LOCATION_H
#define LOCATION_h

#include <vector>

/** @defgroup location location
 * @{
 *
 * Struct and class to support the representation of a location in the program
 */

/** @name Coordinate */
/**@{
 *
 * Struct to store coordinates, namely latitude, longitude, and altitude
 */

typedef struct
{
	double latitude;
	double longitude;
	double altitude;
} Coordinate;

/** @} end of Coordinate*/

/**
 * @brief Represents a location on the map
 * 
 */
class Location
{
  protected:
	int id;
	Coordinate coords; ///< Location's coordinates (x,y,z)
	std::string color;
	bool visited;

  public:
	/**
   * @brief Construct a new Location object
   * 
   * @param id this location's id
   */
	Location(int id);

	/**
	 * @brief Construct a new Location object
	 * 
   	 * @param id this location's id
	 * @param latitude this location's latitude
	 * @param longitude this location's longitude
	 * @param altitude this location's altitude
	 */
	Location(int id, double latitude, double longitude, double altitude);

	/**
	 * @brief Construct a new Location object
	 */
	Location();

	/**
 	* @brief Returns this location's id
 	* 
 	* @return this location's id 
 	*/
	int getID() const;

	/**
	 * @brief Gets this location's latitude
	 * 
	 * @return this location's latitude 
	 */
	double getLatitudecoord() const;

	/**
	 * @brief Gets this location's longitude
	 * 
	 * @return this location's longitude 
	 */
	double getLongitudecoord() const;
	/**
	 * @brief Gets this location's altitude
	 * 
	 * @return this location's altitude 
	 */
	double getAltitudecoord() const;

	/**
	 * @brief Get this location's coordinates
	 * 
	 * @return this location's coordinates 
	 */
	Coordinate getCoords() const;

	/**
	 * @brief Checks if two locations represent the same location
	 * 
	 * @param n2 the location to be compared 
	 * @return true if they are equal, false otherwise 
	 */
	bool operator==(const Location &n2) const;

	/**
	 * @brief Sets this location's id
	 * 
	 * @param newID this location's new id
	 */
	void setID(int newID);

	/**
	 * @brief Checks if this location is available to rent or deposit a bike
	 * 
	 * @param rent true if the objective is to rent, false if it's to deposit
	 * @return false if it's not a sharing location, or true according to the latter's information 
	 */
	virtual bool isAvailable(bool rent) const;

	/**
	 * @brief Get this location's color on the GraphViewer
	 * 
	 * @return this location's color 
	 */
	virtual std::string getColor() const;

	/**
	 * @brief Returns if this location has been visited by the DFS algorithm.
	 * 
	 * @return true if it has been visited, false otherwise
	 */
	bool getVisited() const;

	/**
	 * @brief Sets the visited attribute.
	 * 
	 * @param vis the new value 
	 */
	void setVisited(bool vis);

	/**
	 * @brief Sets this location's color
	 * 
	 * @param color the new color 
	 */
	void setColor(std::string color);
};

/** @} end of location */

#endif
