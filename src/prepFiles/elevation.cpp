#include "elevation.h"
#include <limits>
#include <fstream>
#include <sstream>

using namespace std;

string getCommand(string locations)
{
    return "curl -X POST \\
  https://api.open-elevation.com/api/v1/lookup \\
  -H \'Accept: application/json\' \\
  -H \'Content-Type: application/json\' \\
  -d \'" + locations + "\'";
}

vector<double> parseResult(string file)
{
    string res;
    ifstream read(file);

    if(!read.is_open())
        exit(1);

    getline(read, res);

    read.close();

    return getElevations(split(res));
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
    int size = splited.size();
    for (int i = 0; i < size; i++)
    {
        if (splited.at(i).size() != 0 && (isdigit(splited.at(i).at(0)) || splited.at(i).at(0) == '-'))
        {
            if (lat == max)
                lat = stod(splited.at(i));
            else if (ele == max)
                ele = stod(splited.at(i));
            else
                lon = stod(splited.at(i));
        }

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

string parseLocations(const vector<string> &lines)
{
    string parsed = "{\"locations\":[";
    int size = lines.size();

    for (int i = 0; i < size; i++)
    {
        if (i > 0)
            parsed += ",";
        parsed += parseLine(lines.at(i));
    }

    parsed += "]}";
    return parsed;
}