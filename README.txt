Program was made by:
Maor Halevi - 207691080
Dor Edelman - 315315812

About:
This project is a set of command-line tools written in C++ that use the OpenSky API to retrieve and process information about
flights in real-time. It consists of four individual programs and a bash script that gathers the data from the OpenSky API:

1.updateDB: Retrieves flight data from the OpenSky API updates the local database.
This program is typically run periodically to keep the database up-to-date with the latest flight information.
Does not take input and will only update airports which are already in the database.

2.printArrivals: Prints a list of all flights that are currently arriving at a specified airport.
The list includes the flight number, departure airport, scheduled and estimated arrival times.

3.printFlightsByTime: Prints a list of all flights of an airport/s that are was given as an input - and are in the database,
and sorts by their estimated time of arrival/departure.
for each airport the list will be standalone, it will not mix the flights of diffrent airports.
The list includes the flight number, departure and arrival airports, scheduled and estimated arrival/departure times.

4.printFlightsByAircraft: Prints a list of all flights of an aircraft/s that are was given as an input - and are in the database.
The list includes the flight number, departure and arrival airports, scheduled and estimated arrival/departure times.

Each program can be executed independently, connected to the local database.

A bash script is provided to automate the retrieval of data from the OpenSky API and store it in the local database.
This project is designed for Linux environments.

Getting Started :
These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

Prerequisites:
1.A Linux environment: The program and shared library can be compiled and executed on a Linux operating system.
    Ensure that you have a Linux distribution installed on your machine.
2.A C++ compiler: You will need a C++ compiler to compile the source code into an executable file.
    You can use a variety of C++ compilers such as GCC, Clang, or Visual C++.
3.The required libraries: Ensure that you have installed the necessary libraries required by the program and shared library.
    This may include libraries such as libcurl, OpenSSL, and others.
4.Internet connectivity: Must as the program is using an API from URL.

Installing computer
1.Install the necessary development tools and dependencies:

    On Ubuntu or Debian, run: sudo apt-get install build-essential
    On CentOS or Red Hat, run: sudo yum groupinstall "Development Tools"
    Install any additional dependencies required by your project, such as libraries or headers.

2.Install the necessary libraries for making HTTP requests:

    This program is using libcurl. You can install it on Ubuntu or Debian with: sudo apt-get install libcurl4-openssl-dev
    On CentOS or Red Hat, run: sudo yum install libcurl-devel

3.Copy the project files to the new computer (on the same directory)

4.Open terminal in the directory containing the files and run the command : "make".

5.Open terminal in the directory containing the files and run the command :"export LD_LIBRARY_PATH=."

After steps 1-4 have been completed - you will have 4 .out files and 1 .so file.


Running the programs:

Open terminal in the directory containing the files and run the command : "./lastFlightsScript.sh <airport icao>"
- get data from openskyAPI - can have multiple icao codes seperated by a space.

1.updateDB: to run this program simply type : the command : "./updateDB.out" - no input needed.
2.printArrivals: to run this program simply type : the command : "./printArrivals.out <airport icao>" 
- can have multiple icao codes seperated by a space.
3.printFlightsByTime: to run this program simply type : the command : "./printFlightsByTime.out <airport icao>" 
- can have multiple icao codes seperated by a space.
4.printFlightsByAircraft: to run this program simply type : the command : "./printFlightsByAircraft.out <aircraft icao24>" 
- can have multiple aircraft icao24 codes seperated by a space.

comments:
*if you would like to remove outfiles , go to the terminal in directory containing the files and run the command : "make clean".