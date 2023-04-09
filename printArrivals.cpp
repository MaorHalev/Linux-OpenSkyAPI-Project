#include "utility.cpp"

void printArrivals(DB& DB,char* airport_name);

/*int main (int argc, char* argv[])
{
    DB DB;
    LoadDB(DB);

    for(int i = 1; i < argc; i++)
    {
        printArrivals(DB,argv[i]);
    }

    return 0;
}*/

void printArrivals(DB& DB,char* airport_name)
{
    string airportName(airport_name);
    airport* airport = getAirport(DB,airportName);

    for(int i = 0; i < airport->arrFlights.size(); i++)
    {
        cout << airport->arrFlights[i].flightStr;
    }
}