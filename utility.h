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

class flight
{
public:
    flight(string &flightStr);
    string flightStr, arriveFrom, departureTo;
    string icao24, callSign;
    char formatedArrivalTime[80],formatedDepTime[80];
    int depTimeEpoch, arrivalTimeEpoch;
};

class airport
{
public:
    airport(string airportName);
    void getFile(string path, bool isArrivals);
    vector<flight> arrFlights;
    vector<flight> depFlights;
    string airportName;
};

class DB
{
public:
    vector<airport> arrAirports;
};

void LoadDB(DB &db);

airport *getAirport(DB &db, string airportName);

vector<string> splitFlightCsvStr(const string &flightStr);

void rerunScript(DB& database);

#endif