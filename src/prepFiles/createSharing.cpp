#include "createSharing.h"
#include <fstream>

using namespace std;

void createSharingFile(string file, const vector<string> &info)
{
    ofstream write(file);
    int size = info.size();

    for (int i = 0; i < size; i++)
    {
        write << info.at(i);

        if (i < size - 1)
            write << endl;
    }

    write.close();
}

int randBetween(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

vector<string> createSharingInfo(const vector<string> &ids, const set<int> &lineNumbers)
{
    vector<string> info;

    for (auto it = lineNumbers.begin(); it != lineNumbers.end(); it++)
    {
        int lotmax = randBetween(MIN_LOT, MAX_LOT);
        info.push_back(ids.at(*it) + ";" + to_string(lotmax) + ";" + to_string(randBetween(0, lotmax)));
    }

    return info;
}

set<int> generateRandomLineIDs(unsigned int number, int numLines)
{
    set<int> ids;

    while (ids.size() < number)
        ids.insert(randBetween(0, numLines));

    return ids;
}