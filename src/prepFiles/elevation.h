#pragma once

#include <vector>
#include <string>

/** @defgroup elevation elevation
 * @{
 *
 * Various functions to support the request for elevation data and their results' parsing.
 */

/**
 * @brief Get the Command object
 * 
 * @param locations 
 * @return std::string 
 */
std::string getCommand(std::string locations);

/**
 * @brief 
 * 
 * @param file 
 * @param what 
 * @return std::vector<double> 
 */
std::vector<double> parseResult(std::string file, int what);

/**
 * @brief 
 * 
 * @param str 
 * @return std::vector<std::string> 
 */
std::vector<std::string> split(std::string str);

/**
 * @brief Get the Elevations object
 * 
 * @param splited 
 * @return std::vector<double> 
 */
std::vector<double> getElevations(const std::vector<std::string> &splited);

/**
 * @brief 
 * 
 * @param line 
 * @return std::string 
 */
std::string parseLine(std::string line);

/**
 * @brief 
 * 
 * @param lines 
 * @param begin 
 * @param end 
 * @return std::string 
 */
std::string parseLocations(const std::vector<std::string> &lines, int begin, int end);

/** @} end of elevation */