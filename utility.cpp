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
}

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


void printAllFlightsByTime(DB& DB, vector<string>& airports)
{
    for(int i = 0; i < airports.size(); i++)
    {
        printFlightsByTime(DB,airports[i]);//go on all airports given
    }
}

void printFlightsByTime(DB& DB,const string& airportName)
{
    airport* airport = getAirport(DB,airportName);

    if(airport == NULL)
    {
        cout << "airport "+airportName+" not found\n";
        return;
    }

    // Sort the vector using the compare function
    sort(airport->arrFlights.begin(), airport->arrFlights.end(), compareArrivals);
    sort(airport->depFlights.begin(), airport->depFlights.end(), compareDepartures);
    
     // Define iterators for the two vectors
    auto it1 = airport->arrFlights.begin();
    auto it2 = airport->depFlights.begin();

    // Print the two vectors in ascending order of time to relevant airport
    while (it1 != airport->arrFlights.end() && it2 != airport->depFlights.end()) 
    {
        if (it1->arrivalTimeEpoch <= it2->depTimeEpoch) 
        {
            cout << "flight #" << it1->callSign << " arriving from " << it1->arriveFrom << ", at " << it1->formatedArrivalTime << endl;
            ++it1;
        }
        else 
        {
            cout << "flight #" << it2->callSign << " departing to " << it2->departureTo << ", at " << it2->formatedDepTime <<endl;
            ++it2;
        }
    }
    while (it1 != airport->arrFlights.end()) 
    {
        cout << "flight #" << it1->callSign << " arriving from " << it1->arriveFrom << ", at " << it1->formatedArrivalTime << endl;
        ++it1;
    }
    while (it2 != airport->depFlights.end()) 
    {
        cout << "flight #" << it2->callSign << " departing to " << it2->departureTo << ", at " << it2->formatedDepTime  <<endl;
        ++it2;
    }
        cout << endl;

}

bool compareArrivals(const flight& a, const flight& b) 
{
    return a.arrivalTimeEpoch < b.arrivalTimeEpoch;
}

bool compareDepartures(const flight& a, const flight& b) 
{
    return a.depTimeEpoch < b.depTimeEpoch;
}



void printAllFlightsByAircraft(DB& DB, vector<string>& aircrafts)
{
    for(int i = 0; i < aircrafts.size(); i++)//for all aircrafts given
    {
        printFlightsByAircraft(DB,aircrafts[i]);//print flighs of that aircraft
    }
}

void printFlightsByAircraft(DB& DB,const string& aircraft)
{
    for(int i = 0; i < DB.arrAirports.size(); i++)//go on all airports in the database
    {
        for(int j = 0; j < DB.arrAirports[i].arrFlights.size(); j++)//check if aircraft is in arrival flights
        {
            flight flight = DB.arrAirports[i].arrFlights[j];
            if(flight.icao24 == aircraft)
            {
                printFlight(flight);//print the flight
            }
        }
        for(int j = 0; j < DB.arrAirports[i].depFlights.size(); j++)//check if aircraft is in departure flights
        {
            flight flight = DB.arrAirports[i].depFlights[j];
            if(flight.icao24 == aircraft)
            {
                printFlight(flight);//print flights
            }
        }
    }
        cout << endl;

}

void printFlight(flight& flight)
{//print flight of the aircraft
    cout << flight.icao24 << " departed from " << flight.arriveFrom
    << " at " << flight.formatedDepTime << " arrived in "  << flight.departureTo 
    << " at " << flight.formatedArrivalTime << endl;
}

void printAllArrivels(DB& DB,vector<string>& params)
{
    for(int i = 0; i < params.size(); i++)//for each airport, print all arrivals
    {
        printArrivals(DB,params[i]);
    }
}

void printArrivals(DB& DB,const string& airportName)
{
    airport* airport = getAirport(DB,airportName);//get airport by name
    if(airport == NULL)
    {
        cout << "airport "+airportName+" not found\n";
        return;
    }
    for(int i = 0; i < airport->arrFlights.size(); i++)//go on all the aiports flights and print them
    {
        cout << "flight #" << airport->arrFlights[i].callSign << " arriving from " << airport->arrFlights[i].arriveFrom <<
         ", tookoff at " << airport->arrFlights[i].formatedDepTime << " landed at " << airport->arrFlights[i].formatedArrivalTime <<endl;
    }
    cout << endl;

}

