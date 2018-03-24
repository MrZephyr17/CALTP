#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

string parseLine(string line)
{
    std::string parsed = "{\"latitude\":";
    double lat, lon;
    istringstream str(line);
    str >> lat >> lon;

    parsed += to_string(lat) + ",\"longitude\":" + to_string(lon) + "}";

    return parsed;
}

string parseLocations(string file)
{
    std::string parsed = "{\"locations\":[";
    vector<string> lines;
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

string parseSingleResult(vector<string> result)
{
    return result.at(0) + " " + result.at(1) + " " + result.at(2);
}

void saveResults(const vector<vector<string>> &results, string file)
{
    ofstream save;

    save.open(file);

    for (int i = 0; i < results.size(); i++)
        save << parseSingleResult(results.at(i)) << endl;

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

vector<vector<string>> getCoords(const vector<string> &splited)
{
    vector<string> single;
    vector<vector<string>> results;
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
            single = {to_string(lat), to_string(lon), to_string(ele)};
            results.push_back(single);
            lat = max;
            ele = max;
            lon = max;
        }
    }

    return results;
}

vector<vector<string>> parseResult(string file)
{
    string res;
    ifstream read;
    read.open(file);
    getline(read, res);
    read.close();

    vector<string> splited = split(res);
    vector<vector<string>> coords = getCoords(splited);

    return coords;
}

int main()
{
    int fd = open("curl_result.txt", O_WRONLY | O_CREAT | O_TRUNC);
    dup2(fd, STDOUT_FILENO);

    string cmd = "curl -X POST \\
  https://api.open-elevation.com/api/v1/lookup \\
  -H \'Accept: application/json\' \\
  -H \'Content-Type: application/json\' \\
  -d \'";

    cmd += parseLocations("locations.txt");
    cmd += "\'";

    system(cmd.c_str());

    vector<vector<string>> result = parseResult("curl_result.txt");
    saveResults(result, "results.txt");
    return 0;
}