#ifndef _UTILITYS_OS_LIB_
#define _UTILITYS_OS_LIB_

using namespace std;
#include <iostream>
#include <filesystem>
#include <string>
#include <list>
#include <fstream>
#include <vector>
#include <algorithm>

string getColomFromFlight(string& flight, const char* colom);

class flight
{
public:
    flight(string& flightStr)
    {
        this->flightStr = flightStr;
        this->icao24 = getColomFromFlight(flightStr, "icao24");
        this->callSing = getColomFromFlight(flightStr, "callSing");
        this->depTime = stoi(getColomFromFlight(flightStr, "depTime"));
        this->arrivalTime = stoi(getColomFromFlight(flightStr, "arrivalTime"));
        this->arriveFrom = getColomFromFlight(flightStr, "arriveFrom");
        this->departureTo = getColomFromFlight(flightStr, "departureTo");
    }
    string flightStr, arriveFrom, departureTo;
    string icao24, callSing;
    int depTime, arrivalTime;
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
            getline(infile, line);   //skip first line - description.
            if(isArrivals)
            {
                while(getline(infile, line))
                {
                    arrFlights.push_back(line);
                }
            }
            else
            {
                while(getline(infile, line))
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


void LoadDB(DB& db)
{
    filesystem::path directoryPath = filesystem::current_path() / "flightsDB";

    for (const auto& dirEntry : std::filesystem::directory_iterator(directoryPath))
    {
        if (dirEntry.is_directory()) 
        {
            airport airport(dirEntry.path().stem().string());

            for (const auto& file_entry : std::filesystem::directory_iterator(dirEntry.path())) 
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

airport* getAirport(DB& db, string airportName)
{
    for(airport& airport : db.arrAirports)
    {
        if (airport.airportName == airportName)
        {
            return &airport;
        }
    }
    return nullptr;
}

string getColomFromFlight(string& flight, const char* colom)
{
    size_t colon_pos_1, colon_pos_2;
    if (colom == "icao24")
    {
        colon_pos_1 = 0;
        colon_pos_2 = flight.find(',',1);    // index not occurence.
    }
    else if(colom == "arrivalTime")
    {
        colon_pos_1 = flight.find(',',3);
        colon_pos_2 = flight.find(',',4);
    }
    else if (colom == "depTime")
    {
        colon_pos_1 = flight.find(',',1);
        colon_pos_2 = flight.find(',',2);
    }
    else if (colom == "callSing")
    {
        colon_pos_1 = flight.find(',',5);
        colon_pos_2 = flight.find(',',6);
    }
    else if (colom == "arriveFrom")
    {
        colon_pos_1 = flight.find(',',2);
        colon_pos_2 = flight.find(',',3);
    }
    else if (colom == "departureTo")
    {
        colon_pos_1 = flight.find(',',4);
        colon_pos_2 = flight.find(',',5);
    }
    if(colon_pos_1 == 0)
        return flight.substr(colon_pos_1, colon_pos_2-colon_pos_1-1); // Parse the column from the SVC row string
    return flight.substr(colon_pos_1+1, colon_pos_2-colon_pos_1-1);

}


#endif 
