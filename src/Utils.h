#pragma once
#include <cmath>
#include <ctime>
#include <string>
#include <map>
#include "Location.h"

/** @defgroup utils utils
 * @{
 *
 * Various functions and constants to support various features of the program. 
 */

#define PI acos(-1)        /**< @brief PI constant */
#define MAX_INCENTIVE 50.0 /**< @brief Maximum incentive given to the user */
#define WINDOW_HEIGHT 2160 /**< @brief GraphViewer's window height */
#define WINDOW_WIDTH 3840  /**< @brief GraphViewer's window width */
#define XCONST 2.5         /**< @brief X constant for GraphViewer's nodes*/
#define YCONST -1.0        /**< @brief Y constant for GraphViewer's nodes*/
#define YB -100.0          /**< @brief Y constant for GraphViewer's nodes*/
#define MAX_DISTANCE 5     /**< @brief Max edit distance */

const double MAX_LAT = 41.20324;  /**< @brief Max latitude for current file*/
const double MIN_LAT = 41.17303;  /**< @brief Min latitude for current file*/
const double MAX_LON = -8.555458; /**< @brief Max longitude for current file*/
const double MIN_LON = -8.622682; /**< @brief Min longitude for current file*/

/** @name EditDistanceHelper */
/**@{
*
* Struct to ease the approximate search algorithm, holding found streets along with their edit distance towards
* a given name.
*/

struct EditDistanceHelper
{
	std::string street;
	int editDistance;
	EditDistanceHelper(std::string street, int editDistance);
	bool operator<(const EditDistanceHelper& rhs) const;
};

/** @} end of EditDistanceHelper*/



/**
 * @brief Receives two nodes and calculates the weight for the edge that connects them.
 * 
 * Takes into account distance and medium velocity, and returns an estimate for the time
 * required to travel from origin to dest.
 * 
 * @param origin The origin location
 * @param dest The destination location
 * @return the weight between the two locations 
 */
double calcWeight(Location *origin, Location *dest);

/**
 * @brief Convertes degrees to radians.
 * 
 * @param degrees angle in degrees
 * @return its respective value in radians 
 */
double degreesToRadians(double degrees);

/**
 * @brief Returns distance between two locations.
 * 
 * Returns an estimate for the distance between the two given locations, taking into
 * account geographical distance and street inclination.
 * 
 * @param origin The origin location 
 * @param dest The destination location
 * @return the distance between the two locations. 
 */
double distance(Location *origin, Location *dest);

/**
 * @brief Returns medium velocity in a route between origin and dest.
 * 
 * Returns an estimate for the medium velocity that a cyclist would have
 * when traveling between the given locations.
 * 
 * @param origin The origin location
 * @param dest The destination location
 * @return an estimate for the medium velocity in this route 
 */
double velocity(Location *origin, Location *dest);

/**
 * @brief Waits for the user to give a valid numeric input in the request range.
 * 
 * @param low the lower limit
 * @param high the higher limit
 * @return the input given by the user
 */
int verifyInput(int low, int high);

/**
 * @brief Checks if a given string is a number.
 * 
 * @param input the input string
 * @return true if input is a number, false otherwise
 */
bool isNumber(std::string input);

/**
 * @brief Waits for the user to confirm before advancing with the program.
 */
void waitConfirm();

/**
 * @brief Returns the period between two time instants.
 * 
 * @param begin the first instant
 * @param end the last instant
 * @return the interval, in seconds, between the given limits
 */
double timeDiff(clock_t begin, clock_t end);

/**
 * @brief Get a formatted string representing a time interval.
 * 
 * @param seconds time in seconds 
 * @return the input in hours, minutes and seconds. 
 */
std::string getTime(double seconds);

/**
 * @brief Returns the incentive given to the user regarding a given travel time.
 * 
 * @param seconds total travel time between two locations
 * @return the incentive (0-50) for the given time 
 */
double getIncentive(double seconds);

/**
 * @brief Convertes a longitude value to an X coordinate, fitted for the GraphViewer
 * 
 * @param longitude The longitude
 * @return the converted value
 */
int convertLongitudeToX(double longitude);

/**
 * @brief Convertes a latitude value to an X coordinate, fitted for the GraphViewer
 * 
 * @param latitude The latitude
 * @return the converted value
 */
int convertLatitudeToY(double latitude);

/**
 * @brief Gets street names for sharing location finder
 * 
 * @param street1 The first street
 * @param street2 The second street
 */
void getNames(std::string &street1, std::string &street2);

/**
* @brief Checks if name already exists on streets
*
* @param streets Contains a collection of street names along with their edit distance towards a given pattern
* @param name The name of the street
*/
bool exists(const std::multimap<int, std::string> &streets, const std::string &name);

/** @} end of utils */