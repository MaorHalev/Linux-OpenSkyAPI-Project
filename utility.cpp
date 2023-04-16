#include "utility.h"

flight::flight(string &flightStr)
{
    vector<string> flightParts = splitFlightCsvStr(flightStr);//parse the csv
    this->flightStr = flightStr;
    icao24 = flightParts[0];
    callSign = flightParts[5];
    depTimeEpoch = stoi(flightParts[1]);
    arrivalTimeEpoch = stoi(flightParts[3]);
    arriveFrom = flightParts[2];
    departureTo = flightParts[4];
    time_t epoch = arrivalTimeEpoch;
    struct tm* timeinfo = localtime(&epoch);
    strftime(formatedArrivalTime, 80,"%c", timeinfo);//get formatted time (bonus)
    epoch = depTimeEpoch;
    timeinfo = localtime(&epoch);
    strftime(formatedDepTime, 80,"%c", timeinfo);
}


airport::airport(string airportName)
{
    this->airportName = airportName;
};
void airport::getFile(string path, bool isArrivals)
{
    string line;
    ifstream infile(path);
    if (infile)//while not end of file
    {
        getline(infile, line); // skip first line - description.
        if (isArrivals)//if arrival flight
        {
            while (getline(infile, line))
            {
                arrFlights.push_back(line);
            }
        }
        else//departure flight
        {
            while (getline(infile, line))
            {
                depFlights.push_back(line);
            }
        }
    }
}

void LoadDB(DB &db)
{
    filesystem::path directoryPath = filesystem::current_path() / "flightsDB";//get into the db folder

    for (const auto &dirEntry : std::filesystem::directory_iterator(directoryPath))//for each folder of airport
    {
        if (dirEntry.is_directory())
        {
            airport airport(dirEntry.path().stem().string());//create airport with the folder name

            for (const auto &file_entry : std::filesystem::directory_iterator(dirEntry.path()))//get into the arrivals and departure files and get the flights
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
{//finds airport in the database
    for (airport &airport : db.arrAirports)
    {
        if (airport.airportName == airportName)
        {
            return &airport;
        }
    }
    return nullptr;//if not found return nullptr
}

vector<string> splitFlightCsvStr(const string &flightStr)
{//parses from csv to flight fields
    vector<string> fields;
    stringstream ss(flightStr);
    string field;
    while (getline(ss, field, ','))
    {
        fields.push_back(field);
    }
    return fields;
}

void rerunScript(DB& database)
{
    //first we give the base name of the script
    string script = "./lastFlightsScript.sh";

    for(int i = 0; i < database.arrAirports.size(); i++)//concatenate inputs to string
    {
        script +=  " "  + database.arrAirports[i].airportName;
    }
    system(script.c_str());//run script
}

