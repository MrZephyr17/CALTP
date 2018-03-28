#include "Utils.h"

double calcWeight(Location origin, Location dest)
{
	double dist = distance(origin, dest);
	double v = velocity(origin, dest);

	return dist*v;
}


double degreesToRadians(double degrees)
{
	return degrees * PI / 180;
}

double distance(Location origin, Location dest)
{
	const int earthRadiusKm = 6371;

	double dLat = degreesToRadians(origin.getLatitudecoord() - dest.getLatitudecoord());
	double dLon = degreesToRadians(dest.getLongitudecoord() - origin.getLongitudecoord());

	double lat1 = degreesToRadians(dest.getLatitudecoord());
	double lat2 = degreesToRadians(origin.getLatitudecoord());

	double a = sin(dLat / 2) * sin(dLat / 2) +
		sin(dLon / 2) * sin(dLon / 2) * cos(lat1) * cos(lat2);

	double c = 2 * atan2(sqrt(a), sqrt(1 - a));

	return earthRadiusKm * c;

}

double velocity(Location origin, Location dest)
{
	double originEl = origin.getAltitudecoord();
	double destEl = dest.getAltitudecoord();
	double medEl = (originEl + destEl) / 2;

	//tabela para calcular velocidade media
	return 0;
}