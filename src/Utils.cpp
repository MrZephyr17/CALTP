#include "Utils.h"
#include <string>
#include <iostream>

using namespace std;

double calcWeight(Location* origin, Location* dest)
{
	double dist = distance(origin, dest);
	double v = velocity(origin, dest);

	return dist*v;
}


double degreesToRadians(double degrees)
{
	return degrees * PI / 180;
}

double distance(Location *origin, Location* dest)
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

	return sqrt(pow(earthRadiusKm * c*1000, 2) + alt * alt);	
}

double velocity(Location* origin, Location* dest)
{
	double alt = dest->getAltitudecoord() - origin->getAltitudecoord();

	if (alt >= 0)
	{
		double angle = asin(alt / distance(origin, dest));
		
		if (angle > PI / 4)
		{
			return 5.0/6;
		}
		else
		{
			return (10/3.0 * cos(2 * angle) + 5/6.0);
		}
	}
	else
	{
		double angle = asin(-alt / distance(origin, dest));

		if (angle > PI / 4)
		{
			return 55 * 1000 / 3600.0;
		}
		else
		{
			return 100/9.0 * sin(2 * angle) + 25/6.0;
		}
	}
}

int verifyInput(int low, int high)
{
	int inputValue;
	string input;
	bool validInput = false;

	while (!validInput)
	{
		cout << "Choose an option (" << low << "-" << high << "): ";

		getline(cin, input);

		if (isdigit(input[0]))
		{
			//Apenas aceita como input numeros entre o limite inferior e superior (inclusive)
			if (stoi(input) >= low && stoi(input) <= high && input.size()<2)
				validInput = true;
			else
				cout << "Invalid input! Try again..." << endl;
		}
		//Qualquer outro input e recusado, e e novamente pedido ao utilizador que introduza um número.
		else
			cout << "Invalid input! Try again..." << endl;
	}

	inputValue = stoi(input);

	return inputValue;
}

bool isNumber(string input)
{
	bool isNumber;

	if (input == "")
		return false;
	else
	{
		for (int i = 0; i < input.size(); i++)
			if (isdigit(input[i]))
				isNumber = true;
			else
			{
				isNumber = false;
				break;
			}
	}
	return isNumber;

}