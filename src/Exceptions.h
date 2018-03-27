#pragma once

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>

class LocationNotFound
{
public:
	std::string location;
	LocationNotFound(std::string location) {
		this->location = location;
	}
	std::string message()
	{
		return "Location Not Found: The location " + location + " doesn't exist.\n";
	}

};

#endif /* EXCEPTIONS_H */

