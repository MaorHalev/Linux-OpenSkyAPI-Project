#include "utility.cpp"

void printFlightsByAircraft(DB& DB,const string aircraft);
void printFlight(flight& flight);

int main (int argc, char* argv[])
{
    DB DB;
    LoadDB(DB);

    for(int i = 1; i < argc; i++)
    {
        printFlightsByAircraft(DB,argv[i]);
    }

    return 0;
}

void printFlightsByAircraft(DB& DB,const string aircraft)
{
    for(int i = 0; i < DB.arrAirports.size(); i++)
    {
        for(int j = 0; j < DB.arrAirports[i].arrFlights.size(); j++)
        {
            flight flight = DB.arrAirports[i].arrFlights[j];
            if(flight.icao24 == aircraft)
            {
                printFlight(flight);
            }
        }
        for(int j = 0; j < DB.arrAirports[i].depFlights.size(); j++)
        {
            flight flight = DB.arrAirports[i].depFlights[j];
            if(flight.icao24 == aircraft)
            {
                printFlight(flight);
            }
        }
    }
}

void printFlight(flight& flight)
{
    cout << flight.icao24 << " departed from " << flight.arriveFrom
    << " at " << flight.formatedDepTime << " arrived in "  << flight.departureTo 
    << " at " << flight.formatedArrivalTime << endl;
}