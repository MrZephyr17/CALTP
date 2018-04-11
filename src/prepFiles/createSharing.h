#pragma once

#include <vector>
#include <string>
#include <set>

/** @defgroup createsharing createsharing
 * @{
 *
 * Various functions and constants to support the creation of sharing locations.
 */

#define MIN_LOT 5  /**< @brief Minimum lotation */
#define MAX_LOT 20 /**< @brief Maximum lotation */

/**
 * @brief Create a Sharing File
 * 
 * @param file the name of the file
 * @param info information for the sharing locations
 */
void createSharingFile(std::string file, const std::vector<std::string> &info);

/**
 * @brief Returns a random number in the given interval
 * 
 * @param min the lower limit
 * @param max the higher limit
 * @return the random number
 */
int randBetween(int min, int max);

/**
 * @brief Create information for a given number of sharing locations
 * 
 * @param ids the ids on the nodes file
 * @param lineNumbers the corresponding line numbers on the nodes file
 * @return information for the sharing locations
 */
std::vector<std::string> createSharingInfo(const std::vector<std::string> &ids, const std::set<int> &lineNumbers);

/**
 * @brief Generates random line IDs
 * 
 * @param number the number of numbers to be generated
 * @param numLines the upper limit
 * @return random line ids
 */
std::set<int> generateRandomLineIDs(unsigned int number, int numLines);

/** @} end of createsharing */