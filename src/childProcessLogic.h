#ifndef OPENSKYAPI_CHILDPROCESSLOGIC_H
#define OPENSKYAPI_CHILDPROCESSLOGIC_H
#include "utility.h"

void printArrivals(DB& DB,const string& airportName);
void printAllArrivals(DB& DB, vector<string>& params);

void printFlightsByAircraft(DB& DB,const string& aircraft);
void printFlight(flight& flight);
void printAllFlightsByAircraft(DB& DB, vector<string>& aircrafts);

void printFlightsByTime(DB& DB,const string& airport_name);
bool compareArrivals(const flight& a, const flight& b) ;
bool compareDepartures(const flight& a, const flight& b) ;
void printAllFlightsByTime(DB& DB, vector<string>& airports);


#endif //OPENSKYAPI_CHILDPROCESSLOGIC_H
