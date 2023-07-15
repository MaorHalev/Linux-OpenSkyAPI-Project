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
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <zip.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>

const int BUFFER_SIZE = 10000;

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
void rerunScript(vector<string>& params);

void zipDB();
static bool is_dir(const string& dir);
static void walk_directory(const string& startdir, const string& inputdir, zip_t *zipper);
static void zip_directory(const string& inputdir, const string& output_filename);
void unzipDB();

#endif