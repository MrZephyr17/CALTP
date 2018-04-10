#pragma once

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>

/** @defgroup exceptions exceptions
 * @{
 *
 * Classes to represent possible exceptions in the runtime of the project.
 */

/**
 * @brief To be thrown when a given location could not be found.
 */
class LocationNotFound
{
  public:
	int location;

	/**
	 * @brief Construct a new Location Not Found object
	 * 
	 * @param location the location's id
	 */
	LocationNotFound(int location)
	{
		this->location = location;
	}

	/**
	 * @brief Message to be printed to the user
	 * 
	 * @return the message
	 */
	std::string message()
	{
		return "\n - Location Not Found: The location " + to_string(location) + " doesn't exist.\n";
	}
};

/** @} end of exceptions */

#endif /* EXCEPTIONS_H */
