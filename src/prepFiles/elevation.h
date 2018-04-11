#pragma once

#include <vector>
#include <string>

/** @defgroup elevation elevation
 * @{
 *
 * Various functions to support the request for elevation data and their results' parsing.
 */

/**
 * @brief Returns a command to be sent to the system, returning information about the elevation of the locations
 * 
 * @param locations a string containing information about the locations
 * @return a command ready to be called using system() 
 */
std::string getCommand(std::string locations);

/**
 * @brief Parses the result of the cURL command, returning their elevation
 * 
 * @param file the file contaning the result
 * @param what the line where the result is at
 * @return a vector containing elevation results
 */
std::vector<double> parseResult(std::string file, int what);

/**
 * @brief Splits a line of results, making it easier to retrieve elevation data
 * 
 * @param str line of results to be split
 * @return the splitted result
 */
std::vector<std::string> split(std::string str);

/**
 * @brief Returns elevation information
 * 
 * @param splited splitted information from the cURL command
 * @return elevations
 */
std::vector<double> getElevations(const std::vector<std::string> &splited);

/**
 * @brief Parses a line from the entry file to further build the command
 * 
 * @param line a line from the entry file
 * @return parsed result 
 */
std::string parseLine(std::string line);

/**
 * @brief Parses information on entry file to be sent to the cURL command
 * 
 * @param lines lines from the entry file
 * @param begin the beginning line
 * @param end the ending line
 * @return parsed information 
 */
std::string parseLocations(const std::vector<std::string> &lines, int begin, int end);

/** @} end of elevation */