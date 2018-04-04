#pragma once

#include <vector>
#include <string>

std::string getCommand(std::string locations);

std::vector<double> parseResult(std::string file);

std::vector<std::string> split(std::string str);

std::vector<double> getElevations(const std::vector<std::string> &splited);

std::string parseLine(std::string line);

std::string parseLocations(const std::vector<std::string> &lines);