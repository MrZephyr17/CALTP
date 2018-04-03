#include "Utils.h"

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