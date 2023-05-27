#include "childProcessLogic.h"

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
    for(int i = 0; i < aircrafts.size(); i++)//for all aircraft given
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

void printAllArrivals(DB& DB, vector<string>& params)
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
    for(int i = 0; i < airport->arrFlights.size(); i++)//go on all the airports flights and print them
    {
        cout << "flight #" << airport->arrFlights[i].callSign << " arriving from " << airport->arrFlights[i].arriveFrom <<
             ", tookoff at " << airport->arrFlights[i].formatedDepTime << " landed at " << airport->arrFlights[i].formatedArrivalTime <<endl;
    }
    cout << endl;

}