#include "utility.cpp"

int main ()
{
    DB DB;
    LoadDB(DB);

    string airportName, script = "./lastFlightsScript.sh";

    for(int i = 0; i < DB.arrAirports.size(); i++)
    {
        script +=  " "  + DB.arrAirports[i].airportName;
    }
    system(script.c_str());

    return 0;
}