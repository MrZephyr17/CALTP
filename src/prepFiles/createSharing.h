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
 * @brief Create a Sharing File object
 * 
 * @param file 
 * @param info 
 */
void createSharingFile(std::string file, const std::vector<std::string> &info);

/**
 * @brief 
 * 
 * @param min 
 * @param max 
 * @return int 
 */
int randBetween(int min, int max);

/**
 * @brief Create a Sharing Info object
 * 
 * @param ids 
 * @param lineNumbers 
 * @return std::vector<std::string> 
 */
std::vector<std::string> createSharingInfo(const std::vector<std::string> &ids, const std::set<int> &lineNumbers);

/**
 * @brief 
 * 
 * @param number 
 * @param numLines 
 * @return std::set<int> 
 */
std::set<int> generateRandomLineIDs(unsigned int number, int numLines);

/** @} end of createsharing */