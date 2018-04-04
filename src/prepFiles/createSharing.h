#pragma once

#include <vector>
#include <string>
#include <set>

#define MIN_LOT 5
#define MAX_LOT 20

void createSharingFile(std::string file, const std::vector<std::string> &info);

int randBetween(int min, int max);

std::vector<std::string> createSharingInfo(const std::vector<std::string> &ids, const std::set<int> &lineNumbers);

std::set<int> generateRandomLineIDs(unsigned int number, int numLines);