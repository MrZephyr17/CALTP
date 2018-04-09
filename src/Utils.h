#pragma once
#include "Location.h"
#include <cmath>
#include <ctime>
#include <string>

#define PI acos(-1)
#define MAX_INCENTIVE 50.0
#define WINDOW_HEIGHT 2160
#define WINDOW_WIDTH 3840
#define XCONST 2.5 
#define YCONST -1.0
#define YB -100.0

const double MAX_LAT = 41.20324;
const double MIN_LAT = 41.17303;
const double MAX_LON = -8.555458;
const double MIN_LON = -8.622682;

double calcWeight(Location *origin, Location *dest);

double degreesToRadians(double degrees);

double distance(Location *origin, Location *dest);

double velocity(Location *origin, Location *dest);

int verifyInput(int low, int high);

bool isNumber(std::string input);

void waitConfirm();

double timeDiff(clock_t begin, clock_t end);

std::string getTime(double seconds);

double getIncentive(double seconds);

int convertLongitudeToX(double longitude);

int convertLatitudeToY(double latitude);