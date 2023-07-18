Flight Scanner Microservices - README

Authors:
Maor Halevi - 207691080
Dor Edelman - 315315812
Ofek Smadja - 207809260
Yanai Paran -

About:
Flight Scanner Microservices is a command-line tool written in C++ that utilizes container-based development to provide a menu-based interface
for retrieving and analyzing real-time flight data using the OpenSky API. The application consists of two microservices running in separate Docker containers,
communicating with each other via a named pipe.

Functionality:
1. Print Arriving Flights:
   - Prints a list of all flights that are currently arriving at a specified airport.
   - The list includes the flight number, departure airport, scheduled and estimated arrival times.
   - User input: Airport ICAO code(s) separated by space(' ').

2. Print Flights by Time:
   - Prints a list of all flights of specified airport(s) in the database, sorted by their estimated time of arrival/departure.
   - The list includes the flight number, departure and arrival airports, scheduled and estimated arrival/departure times.
   - User input: Airport ICAO code(s) separated by space(' ').

3. Print Flights by Aircraft:
   - Prints a list of all flights of specified aircraft(s) in the database.
   - The list includes the flight number, departure and arrival airports, scheduled and estimated arrival/departure times.
   - User input: Aircraft ICAO24 code(s) separated by space(' ').

4. Fetch Airports Data:
   - Retrieves flight data from the OpenSky API for a list of specified airports and updates the local database.
   - User input: List of Airport ICAO codes separated by space(' ').

5. Zip Flight Database:
   - Zips the "flightsDB" directory, which contains the local flight database.
   - User input: None.

6. Exit Program:
   - Gracefully exits the program, cleaning up resources and terminating both containers.
   - User input: None.

Prerequisites:
1. Docker: Ensure Docker is installed on your system to run the microservices in containers.
2. Internet Connectivity: The program requires internet connectivity to access the OpenSky API.

Getting Started:
1. Ensure you have Docker installed and running.
2. Open a terminal in the project directory.

Running the Microservices:
To start the microservices using the provided bash script, run the following command:

./setup.sh
