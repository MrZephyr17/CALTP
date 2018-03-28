#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include <algorithm>
#include <vector>

using namespace std;

string parseLine(string line)
{
    std::string parsed = "{\"latitude\":";
    double lat, lon, x, y;
    long id;
    char ign;
    istringstream str(line);
    str >> id >> ign >> lat >> ign >> lon >> ign >> x >> ign >> y;

    parsed += to_string(lat) + ",\"longitude\":" + to_string(lon) + "}";

    return parsed;
}

string parseLocations(string file, vector<string> &lines)
{
    std::string parsed = "{\"locations\":[";
    string line;
    ifstream read;
    read.open(file);

    if (!read.is_open())
    {
        cerr << "File not found!" << endl;
        exit(1);
    }

    while (!read.eof())
    {
        getline(read, line);
        lines.push_back(line);
    }

    read.close();

    for (int i = 0; i < lines.size(); i++)
    {
        if (i > 0)
            parsed += ",";
        parsed += parseLine(lines.at(i));
    }

    parsed += "]}";

    return parsed;
}

void saveResults(const vector<string> originalLines, vector<double> elevations,  string file)
{
    ofstream save;

    save.open(file);

    for (int i = 0; i < originalLines.size()-1; i++)
        save << originalLines.at(i) << ";" << elevations.at(i) << "\r\n";

    save.close();
}

vector<string> split(string str)
{
    vector<string> splited;
    string delim = " ,}";
    int pos1 = 0;
    int pos2 = 0;

    while ((pos2 = str.find_first_of(delim, pos1)) != string::npos)
    {
        splited.push_back(str.substr(pos1, pos2 - pos1));
        pos1 = pos2 + 1;
    }

    return splited;
}

vector<double> getElevations(const vector<string> &splited)
{
    vector<double> results;
    double max = std::numeric_limits<double>::max();
    double lat = max, lon = max, ele = max;

    for (int i = 0; i < splited.size(); i++)
    {
        if (splited.at(i).size() != 0 && (isdigit(splited.at(i).at(0)) || splited.at(i).at(0) == '-'))
            if (lat == max)
                lat = stod(splited.at(i));
            else if (ele == max)
                ele = stod(splited.at(i));
            else
                lon = stod(splited.at(i));

        if (lat != max && ele != max && lon != max)
        {
            results.push_back(ele);
            lat = max;
            ele = max;
            lon = max;
        }
    }

    return results;
}

vector<double> parseResult(string file)
{
    string res;
    ifstream read;
    read.open(file);
    getline(read, res);
    read.close();

    vector<string> splited = split(res);
    vector<double> elevations = getElevations(splited);

    return elevations;
}

string getCommand(string locations)
{
    string cmd = "curl -X POST \\
  https://api.open-elevation.com/api/v1/lookup \\
  -H \'Accept: application/json\' \\
  -H \'Content-Type: application/json\' \\
  -d \'";

    cmd += locations + "\'";

    return cmd;
}

int main()
{
    int fd = open("curl_result.txt", O_WRONLY | O_CREAT | O_TRUNC);
    dup2(fd, STDOUT_FILENO);

    vector<string> lines;
    string locations = parseLocations("locations.txt", lines);

    system(getCommand(locations).c_str());

    vector<double> result = parseResult("curl_result.txt");
    saveResults(lines, result, "results.txt");
    return 0;
}