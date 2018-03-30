#pragma once
#include "Location.h"
#include <cmath>

const double PI = acos(-1);

double calcWeight(Location* origin, Location* dest);

double degreesToRadians(double degrees);

double distance(Location* origin, Location* dest);

double velocity(Location* origin, Location* dest);