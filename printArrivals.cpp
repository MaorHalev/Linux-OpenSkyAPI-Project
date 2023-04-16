#include "utility.cpp"

void printArrivals(DB& DB,const string& airportName);

int main (int argc, char* argv[])
{
    DB DB;//load db
    LoadDB(DB);

    for(int i = 1; i < argc; i++)//for each airport, print all arrivals
    {
        printArrivals(DB,argv[i]);
    }

    return 0;
}

void printArrivals(DB& DB,const string& airportName)
{
    airport* airport = getAirport(DB,airportName);//get airport by name
    for(int i = 0; i < airport->arrFlights.size(); i++)//go on all the aiports flights and print them
    {
        cout << "flight #" << airport->arrFlights[i].callSign << " arriving from " << airport->arrFlights[i].arriveFrom <<
         ", tookoff at " << airport->arrFlights[i].formatedDepTime << " landed at " << airport->arrFlights[i].formatedArrivalTime <<endl;
    }
}