Program was made by:
Maor Halevi - 207691080
Dor Edelman - 315315812

About:
This program is a command-line tool written in C++ that uses the OpenSky API to retrieve and process information about flights in real-time.
It provides a menu-based interface with multiple options for retrieving and analyzing flight data.
The program consists of two processes: the parent process, which interacts with the user, and the child process,
which executes the chosen options and returns the output to the parent process.

Functionality:
1. Print Arriving Flights:
   - Prints a list of all flights that are currently arriving at a specified airport.
   - The list includes the flight number, departure airport, scheduled and estimated arrival times.
   - User input: Airport ICAO code(s) seperated by space(' ').

2. Print Flights by Time:
   - Prints a list of all flights of specified airport(s) in the database, sorted by their estimated time of arrival/departure.
   - The list includes the flight number, departure and arrival airports, scheduled and estimated arrival/departure times.
   - User input: Airport ICAO code(s) seperated by space(' ').

3. Print Flights by Aircraft:
   - Prints a list of all flights of specified aircraft(s) in the database.
   - The list includes the flight number, departure and arrival airports, scheduled and estimated arrival/departure times.
   - User input: Aircraft ICAO24 code(s) seperated by space(' ').

4. Update Flight Database:
   - Retrieves flight data from the OpenSky API and updates the local database.
   - No user input is required for this option.

5. Zip Flight Database:
   - Zips the "flightsDB" directory, which contains the local flight database.
   - User input: None.

6. Print Child Process ID:
   - Prints the process ID (PID) of the child process.
   - User input: None.

7. Exit Program:
   - Gracefully exits the program, cleaning up resources and terminating the child process.
   - User input: None.

Prerequisites:
1. A Linux environment: The program is designed to run on a Linux operating system.
2. C++ compiler: You will need a C++ compiler to compile the source code.
3. Required libraries: Ensure that you have the necessary libraries installed, including libzip , curl, and pkg-config.
4. Internet connectivity: The program requires internet connectivity to access the OpenSky API.

Getting Started:
These instructions will help you get the program up and running on your local machine for development and testing purposes.

1. Install the necessary development tools and dependencies:
   - On Ubuntu or Debian, run: `sudo apt-get install build-essential`
   - On CentOS or Red Hat, run: `sudo yum groupinstall "Development Tools"`
   - Install any additional dependencies required by your project, such as libraries or headers.

2. Install the necessary libraries:
   - Install libcurl on Ubuntu or Debian: `sudo apt-get install libcurl4-openssl-dev`
   - Install libcurl on CentOS or Red Hat: `sudo yum install libcurl-devel`
   - Install libzip using : 'sudo apt install libzip-dev'
   - Install pkg-config : 'sudo apt-get install pkg-config'

3. Copy the project files to your computer.

4. Compile the program using CMake:
   - Create a build directory: `mkdir build && cd build`
   - Generate build files: `cmake ..`
   - Build the program: `make`

5. Set the library path:
   - Open terminal in the directory containing the files and run the command: `export LD_LIBRARY_PATH=.`

Running the programs:

1. Open terminal in the directory containing the files and run the command : "./lastFlightsScript.sh <airport icao>"
- get data from openskyAPI - can have multiple icao codes seperated by a space.

2. Run the program: `./openSkyAPI.out`

3. Follow the on-screen menu and enter the desired option number.

4. Provide any required input as prompted by the selected option.

5. View the output displayed by the program.

Cleaning Up:
To remove the compiled executable and object files,
(while in the directory containing the files) run the following command in the terminal: `cd build && cmake --build . --target clean`

