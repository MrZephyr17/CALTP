#include <iostream>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include "elevation.h"
#include "createSharing.h"

using namespace std;

#define CURL_RESULT "curl_result.txt"

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

void saveResults(const vector<string> &originalLines, const vector<double> &elevations, string file, const set<int> &sharingLines)
{
    ofstream save(file);

    int size = originalLines.size();

    for (int i = 0; i < size; i++)
    {
        save << originalLines.at(i) << ";" << elevations.at(i);

        if (sharingLines.find(i) != sharingLines.end())
            save << ";true" << endl;
        else
            save << ";false" << endl;
    }

    save.close();
}

int usage(char *argv[])
{
    cout << "Usage: " << argv[0] << " <nodes file> <output file> <sharingLocations file> <number of sharingLocations>" << endl;
    return 1;
}

int main(int argc, char *argv[])
{
    if (argc != 5)
        return usage(argv);

    srand(time(NULL));

    int fd = open(CURL_RESULT, O_WRONLY | O_CREAT | O_TRUNC);
    int save_stdout = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);

    vector<string> ids;
    vector<string> lines;
    string locations;
    vector<double> elevations;
    vector<string> sharingInfo;
    set<int> lineNumbers;
    int linesSize;

    lines = readNodes(argv[1], ids);
    linesSize = lines.size();
    locations = parseLocations(lines);

    system(getCommand(locations).c_str());
    dup2(save_stdout,STDOUT_FILENO);
    close(save_stdout);
    close(fd);

    elevations = parseResult(CURL_RESULT);
    lineNumbers = generateRandomLineIDs(atoi(argv[4]), linesSize);
    sharingInfo = createSharingInfo(ids, lineNumbers);
    createSharingFile(argv[3], sharingInfo);
    saveResults(lines, elevations, argv[2], lineNumbers);

    cout << endl << endl << "Successfully added elevation and sharing location information to the specified files!" << endl;

    return 0;
}