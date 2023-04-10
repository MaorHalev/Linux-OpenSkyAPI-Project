#ifndef _UTILITYS_OS_LIB_
#define _UTILITYS_OS_LIB_

using namespace std;
#include <iostream>
#include <ctime>
#include <filesystem>
#include <string>
#include <list>
#include <fstream>
#include <vector>
#include <algorithm>

vector<string> splitFlightCsvStr(const string &flightStr);
class flight
{
public:
    flight(string &flightStr)
    {
        vector<string> flightParts = splitFlightCsvStr(flightStr);
        this->flightStr = flightStr;
        this->icao24 = flightParts[0];
        this->callSing = flightParts[5];
        this->depTimeEpoch = stoi(flightParts[1]);
        this->arrivalTimeEpoch = stoi(flightParts[3]);
        this->arriveFrom = flightParts[2];
        this->departureTo = flightParts[4];

        time_t epoch = arrivalTimeEpoch;
        struct tm* timeinfo = localtime(&epoch);
        strftime(formatedArrivalTime, 80,"%c", timeinfo);
        epoch = depTimeEpoch;
        timeinfo = localtime(&epoch);
        strftime(formatedDepTime, 80,"%c", timeinfo);
    }
    string flightStr, arriveFrom, departureTo;
    string icao24, callSing;
    char formatedArrivalTime[80],formatedDepTime[80];
    int depTimeEpoch, arrivalTimeEpoch;
};

class airport
{
public:
    airport(string airportName)
    {
        this->airportName = airportName;
    };
    void getFile(string path, bool isArrivals)
    {
        string line;
        ifstream infile(path);
        if (infile)
        {
            getline(infile, line); // skip first line - description.
            if (isArrivals)
            {
                while (getline(infile, line))
                {
                    arrFlights.push_back(line);
                }
            }
            else
            {
                while (getline(infile, line))
                {
                    depFlights.push_back(line);
                }
            }
        }
    }
    vector<flight> arrFlights;
    vector<flight> depFlights;
    string airportName;
};

class DB
{
public:
    vector<airport> arrAirports;
};

void LoadDB(DB &db)
{
    filesystem::path directoryPath = filesystem::current_path() / "flightsDB";

    for (const auto &dirEntry : std::filesystem::directory_iterator(directoryPath))
    {
        if (dirEntry.is_directory())
        {
            airport airport(dirEntry.path().stem().string());

            for (const auto &file_entry : std::filesystem::directory_iterator(dirEntry.path()))
            {
                if (file_entry.is_regular_file() && file_entry.path().extension() == ".arv")
                {
                    airport.getFile(file_entry.path().string(), true);
                }
                if (file_entry.is_regular_file() && file_entry.path().extension() == ".dpt")
                {
                    airport.getFile(file_entry.path().string(), false);
                }
            }

            db.arrAirports.push_back(airport);
        }
    }
}

airport *getAirport(DB &db, string airportName)
{
    for (airport &airport : db.arrAirports)
    {
        if (airport.airportName == airportName)
        {
            return &airport;
        }
    }
    return nullptr;
}

vector<string> splitFlightCsvStr(const string &flightStr)
{
    vector<string> fields;
    stringstream ss(flightStr);
    string field;
    while (getline(ss, field, ','))
    {
        fields.push_back(field);
    }
    return fields;
}

#endif
