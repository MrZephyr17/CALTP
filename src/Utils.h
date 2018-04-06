#pragma once
#include "Location.h"
#include <cmath>

const double PI = acos(-1);

double calcWeight(Location* origin, Location* dest);

double degreesToRadians(double degrees);

double distance(Location* origin, Location* dest);

double velocity(Location* origin, Location* dest);

int verifyInput(int low, int high);

bool isNumber(std::string input);