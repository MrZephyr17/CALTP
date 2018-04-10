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
   * @param id 
   */
	Location(int id);

	/**
	 * @brief Construct a new Location object
	 * 
	 * @param id 
	 * @param latitude 
	 * @param itude 
	 * @param altitude 
	 */
	Location(int id, double latitude, double longitude, double altitude);

	/**
	 * @brief Construct a new Location object
	 * 
	 */
	Location();

	/**
 * @brief 
 * 
 * @return int 
 */
	int getID() const;

	/**
	 * @brief Get the Latitudecoord object
	 * 
	 * @return double 
	 */
	double getLatitudecoord() const;

	/**
	 * @brief Get the Longitudecoord object
	 * 
	 * @return double 
	 */
	double getLongitudecoord() const;
	/**
	 * @brief Get the Altitudecoord object
	 * 
	 * @return double 
	 */
	double getAltitudecoord() const;

	/**
	 * @brief Get the Coords object
	 * 
	 * @return Coordinate 
	 */
	Coordinate getCoords() const;

	/**
	 * @brief 
	 * 
	 * @param n2 
	 * @return true 
	 * @return false 
	 */
	bool operator==(const Location &n2) const;

	/**
	 * @brief 
	 * 
	 * @param newID 
	 */
	void setID(int newID);

	/**
	 * @brief 
	 * 
	 * @param rent 
	 * @return true 
	 * @return false 
	 */
	virtual bool isAvailable(bool rent) const;

	/**
	 * @brief Get the Color object
	 * 
	 * @return std::string 
	 */
	virtual std::string getColor() const;

	/**
	 * @brief Get the Visited object
	 * 
	 * @return true 
	 * @return false 
	 */
	bool getVisited() const;

	/**
	 * @brief Set the Visited object
	 * 
	 * @param vis 
	 */
	void setVisited(bool vis);

	/**
	 * @brief Set the Color object
	 * 
	 * @param color 
	 */
	void setColor(std::string color);
};

/** @} end of location */

#endif
