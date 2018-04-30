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
			return (5 / 3.0 * cos(2 * angle) + 5 / 6.0);
	}
	else
	{
		double angle = asin(-alt / distance(origin, dest));

		if (angle > PI / 4)
			return 55 * 10 / 36.0;
		else
			return (100 / 9.0 * sin(2 * angle) + 5 / 2.0);
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
				cout << " - Invalid input! Try again..." << endl
					 << endl;
		}
		else
			cout << " - Invalid input! Try again..." << endl
				 << endl;
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
	cout << endl
		 << " - Press any key to continue...";
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

void getNames(string &street1, string &street2)
{
	cout << "Name of street 1: ";
	getline(cin, street1);
	cout << "Name of street 2: ";
	getline(cin, street2);
}

int editDistance(string pattern, string text)
{
	int pSize = pattern.size();
	int tSize = text.size();
	int oldValue, newValue;
	vector<int> D(tSize + 1);

	for (int j = 0; j <= tSize; j++)
		D.at(j) = j;

	for (int i = 1; i <= pSize; i++)
	{
		oldValue = D.at(0);
		D.at(0) = i;

		for (int j = 1; j <= tSize; j++)
		{
			if (pattern.at(i) == text.at(j))
				newValue = oldValue;
			else
				newValue = 1 + min(oldValue, D.at(j), D.at(j - 1));

			oldValue = D.at(j);
			D.at(j) = newValue;
		}
	}

	return D.at(tSize);
}

vector<int> ComputePrefixFunction(string pattern)
{
	int m = pattern.size();
	vector<int> pi(m);
	pi.at(1) = 0;
	int k = 0;

	for (int q = 2; q <= m; q++)
	{
		while (k > 0 && pattern.at(k + 1) != pattern.at(q))
			k = pi.at(k);

		if (pattern.at(k + 1) = pattern.at(q))
			k++;

		pi.at(q) = k;
	}

	return pi;
}

void KMPMatcher(string text, string pattern)
{
	int n = pattern.size();
	int m = text.size();
	vector<int> pi = ComputePrefixFunction(pattern);
	int q = 0;

	for (int i = 1; i < n; i++)
	{
		while (q > 0 && pattern.at(q + 1) != text.at(i))
			q = pi.at(q);

		if (pattern.at(q + 1) == text.at(i))
			q++;

		if (q == m)
		{
			cout << "Pattern occurs with shift " << i - m << endl;
			q = pi.at(q);
		}
	}
}
