#include "Utils.h"
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>

using namespace std;

double calcWeight(Location *origin, Location *dest)
{
	double dist = distance(origin, dest);
	double v = velocity(origin, dest);

	return dist / v;
}

double degreesToRadians(double degrees)
{
	return degrees * PI / 180;
}

double distance(Location *origin, Location *dest)
{
	const int earthRadiusKm = 6371;

	double dLat = degreesToRadians(origin->getLatitudecoord() - dest->getLatitudecoord());
	double dLon = degreesToRadians(dest->getLongitudecoord() - origin->getLongitudecoord());

	double lat1 = degreesToRadians(dest->getLatitudecoord());
	double lat2 = degreesToRadians(origin->getLatitudecoord());

	double a = sin(dLat / 2) * sin(dLat / 2) +
			   sin(dLon / 2) * sin(dLon / 2) * cos(lat1) * cos(lat2);

	double c = 2 * atan2(sqrt(a), sqrt(1 - a));

	double alt = dest->getAltitudecoord() - origin->getAltitudecoord();

	return sqrt(pow(earthRadiusKm * c * 1000, 2) + alt * alt);
}

double velocity(Location *origin, Location *dest)
{
	double alt = dest->getAltitudecoord() - origin->getAltitudecoord();

	if (alt >= 0)
	{
		double angle = asin(alt / distance(origin, dest));

		if (angle > PI / 4)
			return 5.0 / 6;
		else
			return (10 / 3.0 * cos(2 * angle) + 5 / 6.0);
	}
	else
	{
		double angle = asin(-alt / distance(origin, dest));

		if (angle > PI / 4)
			return 55 * 1000 / 3600.0;
		else
			return 100 / 9.0 * sin(2 * angle) + 25 / 6.0;
	}
}

int verifyInput(int low, int high)
{
	int inputValue;
	string input;
	bool validInput = false;

	while (!validInput)
	{
		cout << " - Choose an option (" << low << "-" << high << "): ";
		getline(cin, input);

		if (input.size() < 2 && isdigit(input[0]))
		{
			inputValue = stoi(input);
			if (inputValue >= low && inputValue <= high)
				validInput = true;
			else
				cout << " - Invalid input! Try again..." << endl << endl;
		}
		else
			cout << " - Invalid input! Try again..." << endl << endl;
	}

	return inputValue;
}

bool isNumber(string input)
{
	if (input == "")
		return false;
	else
	{
		for (auto &it : input)
			if (!isdigit(it))
				return false;
	}

	return true;
}

void waitConfirm()
{
	string input;
	cout << endl << " - Press any key to continue...";
	getline(cin, input);
}

double timeDiff(clock_t begin, clock_t end)
{
	return (double)(end - begin) / CLOCKS_PER_SEC;
}

string getTime(double time)
{
	int seconds = (int)time;
	int hours = seconds / 3600;
	int minutes = (seconds % 3600) / 60;
	int sec = seconds % 60;
	char formatted[9];

	sprintf_s(formatted, "%02d:%02d:%02d", hours, minutes, sec);

	return string(formatted);
}

double getIncentive(double seconds)
{
	double incentive = sqrt(seconds);

	return min(incentive, MAX_INCENTIVE);
}

int convertLongitudeToX(double longitude)
{
	return (int)floor(((longitude - MIN_LON) * (WINDOW_HEIGHT)) / (MAX_LON - MIN_LON));
}

int convertLatitudeToY(double latitude)
{
	return (int)floor(((latitude - MIN_LAT) * (WINDOW_WIDTH)) / (MAX_LAT - MIN_LAT));
}
