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
};

#endif /* EXCEPTIONS_H */

