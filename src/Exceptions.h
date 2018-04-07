#pragma once

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>

class LocationNotFound
{
  public:
	int location;
	LocationNotFound(int location)
	{
		this->location = location;
	}
	std::string message()
	{
		return "\n\nLocation Not Found: The location " + to_string(location) + " doesn't exist.\n\n";
	}
};

#endif /* EXCEPTIONS_H */
