#include "processComunication.h"
#define WAIT_FOR_OPCODE -2
int printMenu();
int printInstructionsAndGetInput(vector<string>& params);
void getInput(vector<string>& params);

pid_t pid;   //declare global var from process communication.
int main ()
{
    int infd[2] = {0, 0};
    int outfd[2] = {0, 0};
    vector<string> params;
    int opCode = WAIT_FOR_OPCODE;
    try
    {
        // Create the pipes
        if (pipe(infd) == -1 || pipe(outfd) == -1)
        {
            perror("pipe");
            throw runtime_error("");
        }
        // Fork a child process
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            throw runtime_error("fork error");
        }
        if (pid == 0)
        { // Child process
            DB db;
            unzipDB();
            LoadDB(db);
            signal(SIGUSR1, handleChildSIGUSR1AndSIGINT);
            signal(SIGINT, handleChildSIGUSR1AndSIGINT);

            // Redirect cout to the ostringstream
            ostringstream oss;
            cout.rdbuf(oss.rdbuf());

            close(infd[WRITE_END]); 	// Child does not write to stdin
            close(outfd[READ_END]); 	// Child does not read from stdout

            while (true)
            {
                opCode = getInstructionFromParent(infd, params);
                executeParentCommand(opCode, params, db);
                string outputString = oss.str();  // Get the string from the ostringstream
                outputString += DELIMITER;
                write(outfd[WRITE_END], outputString.c_str(), outputString.size()); // Write the child outputString to the pipe
                oss.str("");
            }
        }
        else
        {// Parent process
            signal(SIGINT, handleSIGINTParent);
            close(infd[READ_END]); 	    // Parent does not read from stdin
            close(outfd[WRITE_END]);	// Parent does not write to stdout
            // Write to parent-to-child pipe
            while (true)
            {
                while(opCode == WAIT_FOR_OPCODE)
                {
                    opCode = printInstructionsAndGetInput(params);
                }
                passInstructionsToChild(opCode, params, infd,outfd);
                collectAndPrintResults(outfd);
                opCode = WAIT_FOR_OPCODE;
            }

        }
    }
    catch(const exception& e)
    {
        cout << e.what() << endl;
    }
    //we will get to this lock only if an exception has occurs - dealing with termination handing.
    if (pid == 0)
    {// Send SIGINT to the parent process
        kill(getppid(), SIGINT);
    }
    else
    {// Send SIGINT to the child process
        kill(pid, SIGINT);
    }
    cleanup(infd,outfd);
    return 0;
}

int printMenu()
{
    const char* message =   "Which method would you like to run?\n"
                            "1.Print arrivels of an airport.\n"
                            "2.Print flights by time\n"
                            "3.Print flights by aircraft.\n"
                            "4.Update the database.\n"
                            "5.Zip the database files\n"
                            "6.Print child proccess PID.\n"
                            "7.Exit.\n";
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
            cout << "Updating the existing airports in the DB." << endl;
            break;
        case 5:
            cout << "Zipping the DB files." << endl;
            break;
        case 6:
            break;
        case 7:
            cout << "Gracefull exiting." << endl;
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