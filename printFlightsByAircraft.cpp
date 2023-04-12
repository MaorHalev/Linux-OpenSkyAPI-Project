#include "utility.cpp"

void printFlightsByAircraft(DB& DB,const string& aircraft);
void printFlight(flight& flight);

int main (int argc, char* argv[])
{
    DB DB;
    LoadDB(DB);

    for(int i = 1; i < argc; i++)//for all aircrafts given
    {
        printFlightsByAircraft(DB,argv[i]);//print flighs of that aircraft
    }

    return 0;
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
}

void printFlight(flight& flight)//print flight of the aircraft
{
    cout << flight.icao24 << " departed from " << flight.arriveFrom
    << " at " << flight.formatedDepTime << " arrived in "  << flight.departureTo 
    << " at " << flight.formatedArrivalTime << endl;
}