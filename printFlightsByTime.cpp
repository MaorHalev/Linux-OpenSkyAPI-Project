#include "utility.h"

void printFlightsByTime(DB& DB,const string& airport_name);
bool compareArrivals(const flight& a, const flight& b) ;
bool compareDepartures(const flight& a, const flight& b) ;

int main (int argc, char* argv[])
{
    DB DB;
    LoadDB(DB);

    for(int i = 1; i < argc; i++)
    {
        printFlightsByTime(DB,argv[i]);//go on all airports given
    }
    return 0;
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