#include "containerComunication.h"
#define WAIT_FOR_OPCODE -2
#define SHUTDOWN 6
int printMenu();
int printInstructionsAndGetInput(vector<string>& params);
void getInput(vector<string>& params);

int main ()
{
    const char* instructionPipe = "/tmp/flights_pipe/instructionPipe";
    const char* resultPipe = "/tmp/flights_pipe/resultPipe";
    int outfd, infd;
    try
    {
        int outfd = open(instructionPipe, O_WRONLY);
        int infd = open(resultPipe, O_RDONLY);

        if(outfd == -1 || infd == -1) //error while opening the pipe
        {
            perror("pipe");
            throw runtime_error("");
        }

        vector<string> params;
        int opCode = WAIT_FOR_OPCODE;
        // Write to the named pipe
        while (true)
        {
            while (opCode == WAIT_FOR_OPCODE)
            {
                opCode = printInstructionsAndGetInput(params);
            }
            passInstructionsToChild(opCode, params, outfd);
            if (opCode == SHUTDOWN)
            {
                break;
            }
            collectAndPrintResults(infd);
            opCode = WAIT_FOR_OPCODE;
        }
    }
    catch(const exception& e)
    {
        perror("");
        cout << e.what() << endl;
    }

    //needs to handel container logic shut down

    close(outfd);
    close(infd);
    return 0;
}

int printMenu()
{
    const char* message =   "Which method would you like to run?\n"
                            "1.Print arrivels of an airport.\n"
                            "2.Print flights by time\n"
                            "3.Print flights by aircraft.\n"
                            "4.Fetch airport data.\n"
                            "5.Zip the database files\n"
                            "6.Shutdown.\n";
    ssize_t bytes_written = write(STDOUT_FILENO, message, strlen(message));

    if (bytes_written == -1)
    {// Error occurred while writing
        perror("Error writing");
        throw runtime_error("");
    }

    char buffer[1024]; // Buffer to store the read data
    ssize_t bytesRead;

    // Read data from standard input into the buffer
    bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer));
    if (bytesRead == -1)
    {// Error occurred while reading
        perror("Error reading from input");
        throw runtime_error("");
    }
    buffer[bytesRead] = '\0';

    // Convert the read data to an integer
    int number = atoi(buffer);
    return number;
}

int printInstructionsAndGetInput(vector<string>& params)
{
    int choice = printMenu();
    switch(choice)
    {
        case 1:
        case 2:
            cout << "Please enter the ICAO of needed airports." << endl;
            getInput(params);
            break;
        case 3:
            cout << "Please enter the aircraft names of needed aircrafts." << endl;
            getInput(params);
            break;
        case 4:
            cout << "Please enter the ICAO of needed airports." << endl;
            getInput(params);
            break;
        case 5:
            cout << "Zipping the DB files." << endl;
            break;
        case 6:
            cout << "Shutting Down." << endl;
            break;
        default:
            write(STDOUT_FILENO, "Invalid opcode - please enter again.\n", 38);
            return WAIT_FOR_OPCODE;
    }
    return choice;
}

void getInput(vector<string>& params)
{
    string input;
    getline(cin, input);

    istringstream iss(input);
    string token;
    while (getline(iss, token, ' '))
    {
        params.push_back(token);
    }
}