#include <set>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

vector<string> readNodes(string file, vector<string> &ids)
{
    ifstream read(file);
    string line;
    vector<string> lines;

    if (!read.is_open())
    {
        cerr << "File not found! Exiting..." << endl;
        exit(1);
    }

    while (true)
    {
        getline(read, line);
        ids.push_back(line.substr(0, line.find(";")));

        if (!read.eof())
            lines.push_back(line.substr(0, line.size() - 1));
        else
            break;
    }

    read.close();
    return lines;
}

int randBetween(int min, int max)
{
    return rand()%(max-min + 1) + min;

}

set<int> generateRandomLineIDs(unsigned int number, int numLines)
{
    set<int> ids;

    while (ids.size() < number)
        ids.insert(randBetween(0, numLines));

    return ids;
}

void putSharing(string file, const vector<string> &lines, set<int> sharingLines)
{
    ofstream write(file);

    if (!write.is_open())
    {
        cerr << "File not found! Exiting..." << endl;
        exit(1);
    }

    int size = lines.size();

    for (int i = 0; i < size; i++)
    {
        write << lines.at(i);

        if (sharingLines.find(i) != sharingLines.end())
            write << ";true" << endl;
        else
            write << ";false" << endl;
    }

    write.close();
}

vector<string> createSharingInfo(vector<string> ids, set<int> lineNumbers, int minLot, int maxLot)
{
    vector<string> info;

    for(auto it = lineNumbers.begin();it!=lineNumbers.end();it++)
    {
        int lotmax = randBetween(minLot, maxLot);
        info.push_back(ids.at(*it) + ";" + to_string(lotmax) + ";" + to_string(randBetween(0, lotmax)) + "\n");
    }

    return info;
}

void createSharingFile(string file, const vector<string>& info)
{
    ofstream write(file);

    for(auto it : info)
        write << it;

    write.close();
}

int usage(char* argv[])
{
    cout << "Usage: " << argv[0] << " < number of sharing locations >  < min lotation >  < max lotation > " << endl;
    return 1;
}

int main(int argc, char* argv[])
{
    if(argc == 1)
        return usage(argv);

    srand(time(NULL));

    vector<string> ids;
    vector<string> nodes;
    vector<string> sharingInfo;
    set<int> lineNumbers;
    int nodesSize;

    nodes = readNodes("nodes.txt",ids);
    nodesSize = nodes.size();

    lineNumbers = generateRandomLineIDs(atoi(argv[1]), nodesSize);

    putSharing("nodes.txt",nodes, lineNumbers);
    sharingInfo = createSharingInfo(ids, lineNumbers, atoi(argv[2]), atoi(argv[3]));
    createSharingFile("sharingLocations.txt", sharingInfo);

    return 0;
}